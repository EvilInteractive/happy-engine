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
//Created: 20/08/2011
#include "HappyPCH.h" 

#include "PhysicsStaticActor.h"
#include "HappyEngine.h"
#include "HeAssert.h"
#include "PhysicsEngine.h"

#include "PhysicsBoxShape.h"
#include "PhysicsSphereShape.h"
#include "PhysicsConvexShape.h"
#include "PhysicsConcaveShape.h"
#include "PhysicsCapsuleShape.h"
#include "geometry/PxCapsuleGeometry.h"

namespace he {
namespace px {

PhysicsStaticActor::PhysicsStaticActor(const mat44& pose, const IPhysicsShape* pShape, const PhysicsMaterial& material)
{
    PHYSICS->lock();
    m_pActor = PHYSICS->getSDK()->createRigidStatic(physx::PxTransform(pose.getPhyicsMatrix().column3.getXYZ(), 
        physx::PxQuat(physx::PxMat33(pose.getPhyicsMatrix().column0.getXYZ(), 
        pose.getPhyicsMatrix().column1.getXYZ(), 
        pose.getPhyicsMatrix().column2.getXYZ()))));
    PHYSICS->unlock();
    HE_ASSERT(m_pActor != nullptr, "Actor creation failed");

    addShape(pShape, material);

    PHYSICS->lock();
    PHYSICS->getScene()->addActor(*m_pActor);
    PHYSICS->unlock();
}
PhysicsStaticActor::PhysicsStaticActor(const mat44& pose)
{  
    PHYSICS->lock();
    m_pActor = PHYSICS->getSDK()->createRigidStatic(physx::PxTransform(pose.getPhyicsMatrix().column3.getXYZ(), 
        physx::PxQuat(physx::PxMat33(pose.getPhyicsMatrix().column0.getXYZ(), 
        pose.getPhyicsMatrix().column1.getXYZ(), 
        pose.getPhyicsMatrix().column2.getXYZ()))));
    PHYSICS->unlock();
    HE_ASSERT(m_pActor != nullptr, "Actor creation failed");

    PHYSICS->lock();
    PHYSICS->getScene()->addActor(*m_pActor);
    PHYSICS->unlock();
}
void PhysicsStaticActor::addShape( const IPhysicsShape* pShape, const PhysicsMaterial& material) const
{
    PHYSICS->lock();
    physx::PxShape* pPxShape(nullptr);
    switch (pShape->getType())
    {
    case PhysicsShapeType_Box:
        {
            const PhysicsBoxShape* pBoxShape(static_cast<const PhysicsBoxShape*>(pShape));
            pPxShape = m_pActor->createShape(
                physx::PxBoxGeometry(pBoxShape->getDimension().x / 2.0f, pBoxShape->getDimension().y / 2.0f, pBoxShape->getDimension().z / 2.0f), 
                *material.getInternalMaterial());
            break;
        }
    case PhysicsShapeType_Sphere:
        {
            const PhysicsSphereShape* pSphereShape(static_cast<const PhysicsSphereShape*>(pShape));
            pPxShape = m_pActor->createShape(
                physx::PxSphereGeometry(pSphereShape->getRadius()), 
                *material.getInternalMaterial());
            break;
        }
    case PhysicsShapeType_Capsule:
        {
            const PhysicsCapsuleShape* pCapsuleShape(static_cast<const PhysicsCapsuleShape*>(pShape));
            pPxShape = m_pActor->createShape(
                physx::PxCapsuleGeometry(pCapsuleShape->getRadius(), pCapsuleShape->getHeight() / 2.0f), 
                *material.getInternalMaterial());
            break;
        }
        break;
    case PhysicsShapeType_Convex:
        {
            const PhysicsConvexShape* pConvexShape(static_cast<const PhysicsConvexShape*>(pShape));
            pPxShape = m_pActor->createShape(
                physx::PxConvexMeshGeometry(pConvexShape->getInternalMesh(), 
                physx::PxMeshScale(physx::PxVec3(pConvexShape->getScale().x, pConvexShape->getScale().y, pConvexShape->getScale().z), physx::PxQuat::createIdentity())),
                *material.getInternalMaterial());
            break;
        }
    case PhysicsShapeType_Concave:
        {
            const PhysicsConcaveShape* pConcaveShape(static_cast<const PhysicsConcaveShape*>(pShape));
            pPxShape = m_pActor->createShape(
                physx::PxTriangleMeshGeometry(pConcaveShape->getInternalMesh(), 
                physx::PxMeshScale(physx::PxVec3(pConcaveShape->getScale().x, pConcaveShape->getScale().y, pConcaveShape->getScale().z), physx::PxQuat::createIdentity())),
                *material.getInternalMaterial());
            break;
        }

    default: 
        HE_ASSERT(false, "Not Implemented");
        break;
    }
    HE_ASSERT(pPxShape != nullptr, "Shape creation failed");

    physx::PxFilterData sFilter;
    sFilter.word0 = COLLISION_FLAG_GROUND;
    sFilter.word1 = COLLISION_FLAG_GROUND_AGAINST;
    physx::PxFilterData qFilter;
    //physx::PxSetupDrivableShapeQueryFilterData(&qFilter);

    pPxShape->setQueryFilterData(qFilter);
    pPxShape->setSimulationFilterData(sFilter);
    PHYSICS->unlock();
}

PhysicsStaticActor::~PhysicsStaticActor()
{
    if (PHYSICS != nullptr)
    {
        PHYSICS->lock();
        PHYSICS->getScene()->removeActor(*m_pActor);
        PHYSICS->unlock();
        m_pActor->release();
    }
}

vec3 PhysicsStaticActor::getPosition() const
{
    return vec3(m_pActor->getGlobalPose().p);
}
mat44 PhysicsStaticActor::getPose() const
{
    return mat44(physx::PxMat44(physx::PxMat33(m_pActor->getGlobalPose().q), m_pActor->getGlobalPose().p));
}

physx::PxRigidActor* PhysicsStaticActor::getInternalActor() const
{
    return m_pActor;
}

} } //end namespace