//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyEngine.
//
//    HappyEngine is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyEngine is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.
//
//Author:  Bastian Damman
//Created: 19/08/2011

#include "PhysicsEngine.h"
#include "Assert.h"
#include "SDL.h"
#include "HappyTypes.h"
#include "boost/bind.hpp"
#include "MathConstants.h"
#include "HappyNew.h"


namespace happyengine {
namespace physics {

PhysicsEngine::PhysicsEngine(): m_pPhysXSDK(nullptr), m_pScene(nullptr), 
                            m_pCpuDispatcher(nullptr), m_pCudaContextManager(nullptr), 
                            m_pAllocator(NEW HappyPhysicsAllocator()), m_pErrorCallback(NEW error::HappyPhysicsErrorCallback()),
                            m_Simulate(false)
{
    bool memDebug(false);
    #if _DEBUG || DEBUG
        memDebug = true;
    #endif
    
    m_pPhysXSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pAllocator, *m_pErrorCallback, 
                                    PxTolerancesScale(), memDebug);
    
    ASSERT(m_pPhysXSDK != nullptr, "init of physX failed");

    
    if (!PxInitExtensions(*m_pPhysXSDK))
        ASSERT("PxInitExtensions failed!");

    createScene();
}
void PhysicsEngine::createScene()
{
    ASSERT(m_pPhysXSDK != nullptr, "m_pPhysXSDK == null");
    PxSceneDesc sceneDesc(m_pPhysXSDK->getTolerancesScale());

    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

    if(!sceneDesc.cpuDispatcher)
    {
        std::cout << "PhysX using Cpu - " << SDL_GetCPUCount() << " cores\n";
        m_pCpuDispatcher = PxDefaultCpuDispatcherCreate(SDL_GetCPUCount());

        ASSERT(m_pCpuDispatcher != nullptr, "PxDefaultCpuDispatcherCreate failed!");

        sceneDesc.cpuDispatcher = m_pCpuDispatcher;
    }

    if(!sceneDesc.filterShader)
        sceneDesc.filterShader = PxDefaultSimulationFilterShader;

    #ifdef PX_WINDOWS
    pxtask::CudaContextManagerDesc cudaDesc;
    m_pCudaContextManager = pxtask::createCudaContextManager(cudaDesc);
    if(!sceneDesc.gpuDispatcher && m_pCudaContextManager != nullptr)
    {
        std::cout << "PhysX using Gpu - " << m_pCudaContextManager->getDeviceName() << ", " << 
            m_pCudaContextManager->getMultiprocessorCount() << " cores @" << m_pCudaContextManager->getClockRate() << "\n";
        sceneDesc.gpuDispatcher = m_pCudaContextManager->getGpuDispatcher();
    }
    #endif

    m_pScene = m_pPhysXSDK->createScene(sceneDesc);
    ASSERT(m_pScene != nullptr, "createScene failed!");

    PxRigidStatic* plane = m_pPhysXSDK->createRigidStatic(PxTransform(PxVec3(0, -5, 0), PxQuat(math::piOverTwo, PxVec3(0, 0, 1))));
    ASSERT(plane != nullptr, "");
    PxShape* pShape = plane->createShape(PxPlaneGeometry(), *m_pPhysXSDK->createMaterial(0.3f, 0.6f, 0.2f) );
    ASSERT(pShape != nullptr, "");
    m_pScene->addActor(*plane);

}


PhysicsEngine::~PhysicsEngine()
{
    m_pScene->release();
    m_pCpuDispatcher->release();
    m_pCudaContextManager->release();
    m_pPhysXSDK->release();
    delete m_pAllocator;
    delete m_pErrorCallback;
}

void PhysicsEngine::startSimulation()
{
    m_Timer = 0.0f;
    m_Simulate = true;
}
void PhysicsEngine::stopSimulation()
{
    m_Simulate = false;
}
void PhysicsEngine::tick(float dTime)
{
    static const PxReal s_fixedStep(1.0f / 60.0f);

    if (m_Simulate)
    {
        m_Timer += dTime;
        if (m_Timer >= s_fixedStep)
        {
            m_Timer -= s_fixedStep;

            m_pScene->fetchResults(true);
            m_pScene->simulate(s_fixedStep);
        }
    }
}

PxPhysics* PhysicsEngine::getSDK() const
{
    return m_pPhysXSDK;
}
PxScene* PhysicsEngine::getScene() const
{
    return m_pScene;
}

} } //end namespace