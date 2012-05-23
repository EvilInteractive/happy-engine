//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
//Updated to physx 3.2: 16/05/2012
#include "HappyPCH.h" 

#include "PhysicsEngine.h"
#include "PhysicsCarManager.h"
#include "PhysicsTrigger.h"
#include "extensions/PxVisualDebuggerExt.h"

#include "Ray.h"

namespace he {
namespace px {

PhysicsEngine::PhysicsEngine(): m_PhysXSDK(nullptr), m_Scene(nullptr), 
                            m_CpuDispatcher(nullptr), m_CudaContextManager(nullptr), 
                            m_Allocator(NEW HappyPhysicsAllocator()), m_ErrorCallback(NEW err::HappyPhysicsErrorCallback()),
                            m_Simulate(false), m_PxProfileZoneManager(nullptr),
                           /* m_pCarManager(NEW PhysicsCarManager()),*/ m_PhysXFoundation(nullptr), m_VisualDebuggerConnection(nullptr),
                           m_Timer(0.0f)
{
    bool memDebug(false);
    #if _DEBUG || DEBUG
        memDebug = true;
    #endif
    
    m_PhysXFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *m_Allocator, *m_ErrorCallback);
    HE_ASSERT(m_PhysXFoundation != nullptr, "Loading physx foundation unsuccessful");

    #if DEBUG || _DEBUG
    m_PxProfileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(m_PhysXFoundation);
    HE_ASSERT(m_PxProfileZoneManager != nullptr, "Loading physx profileZoneManager unsuccessful");
    #endif

    m_PhysXSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *m_PhysXFoundation,
                                    physx::PxTolerancesScale(), memDebug, m_PxProfileZoneManager);   
    HE_ASSERT(m_PhysXSDK != nullptr, "init of physX failed");
   
    if (!PxInitExtensions(*m_PhysXSDK))
    {
        HE_ASSERT(false, "PxInitExtensions failed!");
    }

    if (m_PhysXSDK->getPvdConnectionManager() != nullptr)
    {
        //#if _DEBUG || DEBUG
        HE_INFO("connecting to PVD");
        physx::PxVisualDebuggerConnectionFlags connectionFlags(physx::PxVisualDebuggerExt::getAllConnectionFlags());
        m_VisualDebuggerConnection = physx::PxVisualDebuggerExt::createConnection(m_PhysXSDK->getPvdConnectionManager(), 
            "localhost", 5425, 100, connectionFlags);
        if (m_VisualDebuggerConnection == nullptr)
            HE_INFO("    NOT CONNECTED!");
        else
            HE_INFO("    CONNECTED!");
        //#endif
    }

    createScene();

}
void PhysicsEngine::createScene()
{
    HE_ASSERT(m_PhysXSDK != nullptr, "m_pPhysXSDK == null");
    physx::PxSceneDesc sceneDesc(m_PhysXSDK->getTolerancesScale());

    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

    if(!sceneDesc.cpuDispatcher)
    {
        uint useThreads(he::max<uint>(1, boost::thread::hardware_concurrency() - 1));
        HE_INFO("PhysX using Cpu - %d threads", useThreads);
        m_CpuDispatcher = physx::PxDefaultCpuDispatcherCreate(useThreads);

        HE_ASSERT(m_CpuDispatcher != nullptr, "PxDefaultCpuDispatcherCreate failed!");

        sceneDesc.cpuDispatcher = m_CpuDispatcher;
    }

    if(!sceneDesc.filterShader)
        sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

    #ifdef PX_WINDOWS
    physx::pxtask::CudaContextManagerDesc cudaDesc;
    m_CudaContextManager = physx::pxtask::createCudaContextManager(*m_PhysXFoundation, cudaDesc, m_PhysXSDK->getProfileZoneManager());
    if(!sceneDesc.gpuDispatcher && m_CudaContextManager != nullptr)
    {
        HE_INFO("PhysX using Gpu - %s, %d cores @%d", m_CudaContextManager->getDeviceName(), 
                                                      m_CudaContextManager->getMultiprocessorCount(),
                                                      (int)m_CudaContextManager->getClockRate());
        sceneDesc.gpuDispatcher = m_CudaContextManager->getGpuDispatcher();
    }
    #endif

    m_Scene = m_PhysXSDK->createScene(sceneDesc);
    HE_ASSERT(m_Scene != nullptr, "createScene failed!");

#if DEBUG || _DEBUG
    m_Scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
    m_Scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
#endif

    m_Scene->setSimulationEventCallback(this);
}


PhysicsEngine::~PhysicsEngine()
{
    if (m_Simulate)
        stopSimulation();

    m_Scene->fetchResults(true);
    m_Scene->release();
    m_Materials.removeAllAssets();
    
    PxCloseExtensions();

    if (m_VisualDebuggerConnection != nullptr)
        m_VisualDebuggerConnection->release();

    if (m_CpuDispatcher != nullptr)
        m_CpuDispatcher->release();
    if (m_CudaContextManager != nullptr)
        m_CudaContextManager->release();

    if (m_PxProfileZoneManager != nullptr)
        m_PxProfileZoneManager->release();

    if (m_PhysXSDK != nullptr)
        m_PhysXSDK->release();

    if (m_PhysXFoundation != nullptr)
        m_PhysXFoundation->release();
    //delete m_pCarManager;
    delete m_Allocator;
    delete m_ErrorCallback;
}

void PhysicsEngine::startSimulation()
{
    m_Timer = 0.0f;
    m_Simulate = true;
    //m_PhysXThread = boost::thread(boost::bind(&PhysicsEngine::physXThread, this));
}
void PhysicsEngine::stopSimulation()
{
    m_Simulate = false;
    m_Scene->fetchResults(true);
    //m_PhysXThread.join();
}
void PhysicsEngine::tick(float dTime)
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    if (m_Simulate)
    {
        m_Timer += dTime;
        static const physx::PxReal fixedStep(1.0f / 60.0f);
        while (m_Timer >= fixedStep)
        {
            m_Timer -= fixedStep;
            m_Scene->fetchResults(true);
            m_Scene->simulate(fixedStep);
        }
    }
}

physx::PxPhysics* PhysicsEngine::getSDK() const
{
    return m_PhysXSDK;
}
physx::PxScene* PhysicsEngine::getScene() const
{
    return m_Scene;
}

physx::PxMaterial* PhysicsEngine::createMaterial( float staticFriction, float dynamicFriction, float restitution )
{
    std::stringstream key;
    key << staticFriction << " " << dynamicFriction << " " << restitution;
    if (m_Materials.isAssetPresent(key.str()))
    {
        return m_Materials.getAsset(key.str());
    }
    else
    {
        physx::PxMaterial* pMat(m_PhysXSDK->createMaterial(staticFriction, dynamicFriction, restitution));
        if(pMat == nullptr)
        {
            CONSOLE->addMessage("physx error: createMaterial failed", CMSG_TYPE_ERROR);
        }
        m_Materials.addAsset(key.str(), pMat);
        return pMat;
    }
}

//PhysicsCarManager* PhysicsEngine::getCarManager() const
//{
//    return m_pCarManager;
//}
//
//const px::PhysicsMaterial& PhysicsEngine::getDriveableMaterial( byte id )
//{
//    return m_pCarManager->getFrictionTable()->getMaterial(id);
//}

void PhysicsEngine::lock()
{
    m_PhysXMutex.lock();
}

void PhysicsEngine::unlock()
{
    m_PhysXMutex.unlock();
}


void PhysicsEngine::onConstraintBreak( physx::PxConstraintInfo* /*constraints*/, physx::PxU32 /*count*/ )
{

}

void PhysicsEngine::onWake( physx::PxActor** /*actors*/, physx::PxU32 /*count*/ )
{

}

void PhysicsEngine::onSleep( physx::PxActor** /*actors*/, physx::PxU32 /*count*/ )
{

}

void PhysicsEngine::onContact( const physx::PxContactPairHeader& /*pairHeader*/, const physx::PxContactPair* /*pairs*/, physx::PxU32 /*nbPairs*/ )
{

}

void PhysicsEngine::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 /*count*/)
{
    if (pairs->triggerShape->userData == nullptr)
        return;

    PhysicsTrigger* pTrigger(static_cast<PhysicsTrigger*>(pairs->triggerShape->userData));

    switch (pairs->status)
    {
    case physx::PxPairFlag::eNOTIFY_TOUCH_FOUND:
        {
            pTrigger->onTriggerEnter(pairs->otherShape);

            break;
        }

    case physx::PxPairFlag::eNOTIFY_TOUCH_LOST:
        {
            pTrigger->onTriggerLeave(pairs->otherShape);

            break;
        }
    }
}

RayCastResult PhysicsEngine::raycast( const Ray& ray ) const
{
    physx::PxRaycastHit hit;
    const physx::PxSceneQueryFlags outputFlags =    physx::PxSceneQueryFlag::eDISTANCE | 
                                                    physx::PxSceneQueryFlag::eIMPACT | 
                                                    physx::PxSceneQueryFlag::eNORMAL;

    physx::PxVec3 origin, direction;
    ray.getOrigin().toPxVec3(&origin);
    ray.getDirection().toPxVec3(&direction);
    RayCastResult result;
    if (m_Scene->raycastSingle(origin, direction, ray.getMaxDistance(), outputFlags, hit))
    {
        result.hit = true;
        result.hitDistance = hit.distance;
        result.hitPosition = vec3(hit.impact);
        result.hitNormal = vec3(hit.normal);
        result.actorHit = static_cast<IPhysicsActor*>(hit.shape->userData);
    }
    else
    {
        result.hit = false;
        result.actorHit = nullptr;
    }
    return result;
}

} } //end namespace