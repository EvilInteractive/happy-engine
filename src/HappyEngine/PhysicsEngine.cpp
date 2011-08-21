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


namespace happyengine {
namespace physics {

PhysicsEngine::PhysicsEngine(): m_pPhysXSDK(nullptr), m_pScene(nullptr), 
                            m_pCpuDispatcher(nullptr), m_pCudaContextManager(nullptr), 
                            m_pAllocator(new HappyPhysicsAllocator()), m_pErrorCallback(new error::HappyPhysicsErrorCallback()),
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
    if (m_Simulate == false)
    {
        m_Simulate = true;
        m_PhysXThread = boost::thread(boost::bind(&PhysicsEngine::loop, this));
    }
}
void PhysicsEngine::stopSimulation()
{
    if (m_Simulate == true)
    {
        m_Simulate = false;
        m_PhysXThread.join();
    }
}
void PhysicsEngine::loop()
{
    uint prevTicks(SDL_GetTicks());

    PxReal fixedStep(1.0f / 60.0f);
    while(m_Simulate)
    {
        uint curTicks(SDL_GetTicks());
        double dTime((curTicks - prevTicks) / 1000.0);
        prevTicks = curTicks;

        if (dTime < fixedStep)
            boost::this_thread::sleep(boost::posix_time::milliseconds(static_cast<int64_t>((fixedStep - dTime) * 1000)));

        m_pScene->simulate(fixedStep);
        m_pScene->fetchResults(true);
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