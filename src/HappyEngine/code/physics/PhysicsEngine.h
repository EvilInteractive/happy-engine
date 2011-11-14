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

#ifndef _HE_PHYSICS_ENGINE_H_
#define _HE_PHYSICS_ENGINE_H_
#pragma once

#include "PxPhysics.h"
#include "extensions/PxExtensionsAPI.h"
#include "PxCudaContextManager.h"
#include "ExternalError.h"

#pragma warning(disable:4244)
#include "boost/thread.hpp"
#pragma warning(default:4244)

#include "AssetContainer.h"

namespace he {
namespace px {

class HappyPhysicsAllocator : public physx::PxAllocatorCallback
{
    void* allocate(size_t size, const char*, const char*, int)
    {
        return _aligned_malloc(size, 16);
    }

    void deallocate(void* ptr)
    {
        _aligned_free(ptr);
    }
};

enum PxFilter
{
    PxFilter_CarChassi = 1 << 0,
    PxFilter_CarWheel = 1 << 1,

};

class PhysicsEngine
{
public:
	PhysicsEngine();
    virtual ~PhysicsEngine();

    void tick(float dTime);

    void startSimulation();
    void stopSimulation();

    physx::PxPhysics* getSDK() const;
    physx::PxScene* getScene() const;

    physx::PxMaterial* createMaterial(float staticFriction, float dynamicFriction, float restitution);

private:

    physx::PxPhysics* m_pPhysXSDK;
    physx::PxScene* m_pScene;

    physx::PxErrorCallback* m_pErrorCallback;
    physx::PxAllocatorCallback* m_pAllocator;

    physx::PxDefaultCpuDispatcher* m_pCpuDispatcher;
    physx::pxtask::CudaContextManager* m_pCudaContextManager;

    ct::AssetContainer<physx::PxMaterial*>* m_pMaterials;

    boost::thread m_PhysXThread;

    void createScene();

    float m_Timer;

    bool m_Simulate;

    //Disable default copy constructor and default assignment operator
    PhysicsEngine(const PhysicsEngine&);
    PhysicsEngine& operator=(const PhysicsEngine&);
};

} } //end namespace

#endif
