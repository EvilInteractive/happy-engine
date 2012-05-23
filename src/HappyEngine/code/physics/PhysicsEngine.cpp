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

PhysicsEngine::PhysicsEngine(): m_pPhysXSDK(nullptr), m_pScene(nullptr), 
                            m_pCpuDispatcher(nullptr), m_pCudaContextManager(nullptr), 
                            m_pAllocator(NEW HappyPhysicsAllocator()), m_pErrorCallback(NEW err::HappyPhysicsErrorCallback()),
                            m_Simulate(false), m_pMaterials(NEW ct::AssetContainer<physx::PxMaterial*>()),
                           /* m_pCarManager(NEW PhysicsCarManager()),*/ m_pPhysXFoundation(nullptr), m_pVisualDebuggerConnection(nullptr),
                           m_Timer(0.0f)
{
    bool memDebug(false);
    #if _DEBUG || DEBUG
        memDebug = true;
    #endif
    
    m_pPhysXFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *m_pAllocator, *m_pErrorCallback);
    HE_ASSERT(m_pPhysXFoundation != nullptr, "Loading physx foundation unsuccessful");

    physx::PxProfileZoneManager* profileZoneManager(nullptr);
    #if DEBUG || _DEBUG
    profileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(m_pPhysXFoundation);
    HE_ASSERT(profileZoneManager != nullptr, "Loading physx profileZoneManager unsuccessful");
    #endif

    m_pPhysXSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pPhysXFoundation,
                                    physx::PxTolerancesScale(), memDebug, profileZoneManager);   
    HE_ASSERT(m_pPhysXSDK != nullptr, "init of physX failed");
   
    if (!PxInitExtensions(*m_pPhysXSDK))
    {
        HE_ASSERT(false, "PxInitExtensions failed!");
    }

    if (m_pPhysXSDK->getPvdConnectionManager() != nullptr)
    {
        //#if _DEBUG || DEBUG
        HE_INFO("connecting to PVD");
        physx::PxVisualDebuggerConnectionFlags connectionFlags(physx::PxVisualDebuggerExt::getAllConnectionFlags());
        m_pVisualDebuggerConnection = physx::PxVisualDebuggerExt::createConnection(m_pPhysXSDK->getPvdConnectionManager(), 
            "localhost", 5425, 100, connectionFlags);
        if (m_pVisualDebuggerConnection == nullptr)
            HE_INFO("    NOT CONNECTED!");
        else
            HE_INFO("    CONNECTED!");
        //#endif
    }

    createScene();

}
void PhysicsEngine::createScene()
{
    HE_ASSERT(m_pPhysXSDK != nullptr, "m_pPhysXSDK == null");
    physx::PxSceneDesc sceneDesc(m_pPhysXSDK->getTolerancesScale());

    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

    if(!sceneDesc.cpuDispatcher)
    {
        uint useThreads(he::max<uint>(1, boost::thread::hardware_concurrency() - 1));
        HE_INFO("PhysX using Cpu - %d threads", useThreads);
        m_pCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(useThreads);

        HE_ASSERT(m_pCpuDispatcher != nullptr, "PxDefaultCpuDispatcherCreate failed!");

        sceneDesc.cpuDispatcher = m_pCpuDispatcher;
    }

    if(!sceneDesc.filterShader)
        sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

    #ifdef PX_WINDOWS
    physx::pxtask::CudaContextManagerDesc cudaDesc;
    m_pCudaContextManager = physx::pxtask::createCudaContextManager(*m_pPhysXFoundation, cudaDesc, m_pPhysXSDK->getProfileZoneManager());
    if(!sceneDesc.gpuDispatcher && m_pCudaContextManager != nullptr)
    {
        HE_INFO("PhysX using Gpu - %s, %d cores @%d", m_pCudaContextManager->getDeviceName(), 
                                                      m_pCudaContextManager->getMultiprocessorCount(),
                                                      (int)m_pCudaContextManager->getClockRate());
        sceneDesc.gpuDispatcher = m_pCudaContextManager->getGpuDispatcher();
    }
    #endif

    m_pScene = m_pPhysXSDK->createScene(sceneDesc);
    HE_ASSERT(m_pScene != nullptr, "createScene failed!");

#if DEBUG || _DEBUG
    m_pScene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
    m_pScene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
#endif

    m_pScene->setSimulationEventCallback(this);
}


PhysicsEngine::~PhysicsEngine()
{
    if (m_Simulate)
        stopSimulation();

    m_pScene->fetchResults(true);
    m_pScene->release();
    delete m_pMaterials;
    
    PxCloseExtensions();

    if (m_pVisualDebuggerConnection != nullptr)
        m_pVisualDebuggerConnection->release();

    m_pCpuDispatcher->release();
    if (m_pCudaContextManager != nullptr)
        m_pCudaContextManager->release();

    m_pPhysXSDK->release();
    m_pPhysXFoundation->release();
    //delete m_pCarManager;
    delete m_pAllocator;
    delete m_pErrorCallback;
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
    m_pScene->fetchResults(true);
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
            m_pScene->fetchResults(true);
            m_pScene->simulate(fixedStep);
        }
    }
}

physx::PxPhysics* PhysicsEngine::getSDK() const
{
    return m_pPhysXSDK;
}
physx::PxScene* PhysicsEngine::getScene() const
{
    return m_pScene;
}

physx::PxMaterial* PhysicsEngine::createMaterial( float staticFriction, float dynamicFriction, float restitution )
{
    std::stringstream key;
    key << staticFriction << " " << dynamicFriction << " " << restitution;
    if (m_pMaterials->isAssetPresent(key.str()))
    {
        return m_pMaterials->getAsset(key.str());
    }
    else
    {
        physx::PxMaterial* pMat(m_pPhysXSDK->createMaterial(staticFriction, dynamicFriction, restitution));
        if(pMat == nullptr)
        {
            CONSOLE->addMessage("physx error: createMaterial failed", CMSG_TYPE_ERROR);
        }
        m_pMaterials->addAsset(key.str(), pMat);
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
    if (m_pScene->raycastSingle(origin, direction, ray.getMaxDistance(), outputFlags, hit))
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