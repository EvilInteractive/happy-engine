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

namespace happyengine {
namespace physics {

class HappyPhysicsAllocator : public PxAllocatorCallback
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

class PhysicsEngine
{
public:
	PhysicsEngine();
    virtual ~PhysicsEngine();

    void tick(float dTime);

    void startSimulation();
    void stopSimulation();

    PxPhysics* getSDK() const;
    PxScene* getScene() const;

private:

    PxPhysics* m_pPhysXSDK;
    PxScene* m_pScene;

    PxErrorCallback* m_pErrorCallback;
    PxAllocatorCallback* m_pAllocator;

    PxDefaultCpuDispatcher* m_pCpuDispatcher;
    pxtask::CudaContextManager* m_pCudaContextManager;

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
