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
//Author:  Sebastiaan Sprengers
//Created: 22/01/2012

#include "HappyPCH.h" 

#include "PhysicsTrigger.h"

#include "HeAssert.h"
#include "PhysicsEngine.h"

#include "PhysicsBoxShape.h"
#include "PhysicsSphereShape.h"
#include "PhysicsConvexShape.h"
#include "PhysicsCapsuleShape.h"
#include "geometry/PxCapsuleGeometry.h"

namespace he {
namespace px {

/* CONSTRUCTOR - DESTRUCTOR */
PhysicsTrigger::PhysicsTrigger(const mat44& pose) : m_pActor(NEW PhysicsDynamicActor(pose))
{
    m_pActor->setKeyframed(true);
}

PhysicsTrigger::~PhysicsTrigger()
{
    delete m_pActor;
}

/* GENERAL */
void PhysicsTrigger::addTriggerShape(const IPhysicsShape* pShape, const mat44& localPose)
{
    PHYSICS->lock();
    physx::PxShape* pPxShape(nullptr);
    switch (pShape->getType())
    {
    case PhysicsShapeType_Box:
        {
            const PhysicsBoxShape* pBoxShape(static_cast<const PhysicsBoxShape*>(pShape));
            pPxShape = m_pActor->getInternalActor()->createShape(
                physx::PxBoxGeometry(pBoxShape->getDimension().x / 2.0f, pBoxShape->getDimension().y / 2.0f, pBoxShape->getDimension().z / 2.0f), 
                *px::PhysicsMaterial(0.0f, 0.0f, 0.0f).getInternalMaterial(), physx::PxTransform(localPose.getPhyicsMatrix()));
            break;
        }
    case PhysicsShapeType_Sphere:
        {
            const PhysicsSphereShape* pSphereShape(static_cast<const PhysicsSphereShape*>(pShape));
            pPxShape = m_pActor->getInternalActor()->createShape(
                physx::PxSphereGeometry(pSphereShape->getRadius()), 
                *px::PhysicsMaterial(0.0f, 0.0f, 0.0f).getInternalMaterial(), physx::PxTransform(localPose.getPhyicsMatrix()));
            break;
        }
    case PhysicsShapeType_Capsule:
        {
            const PhysicsCapsuleShape* pCapsuleShape(static_cast<const PhysicsCapsuleShape*>(pShape));
            pPxShape = m_pActor->getInternalActor()->createShape(
                physx::PxCapsuleGeometry(pCapsuleShape->getRadius(), pCapsuleShape->getHeight() / 2.0f), 
                *px::PhysicsMaterial(0.0f, 0.0f, 0.0f).getInternalMaterial(), physx::PxTransform(localPose.getPhyicsMatrix()));
            break;
        }
    case PhysicsShapeType_Convex:
        {
            const PhysicsConvexShape* pConvexShape(static_cast<const PhysicsConvexShape*>(pShape));
            pPxShape = m_pActor->getInternalActor()->createShape(
                physx::PxConvexMeshGeometry(pConvexShape->getInternalMesh(), 
                physx::PxMeshScale(physx::PxVec3(pConvexShape->getScale().x, pConvexShape->getScale().y, pConvexShape->getScale().z), physx::PxQuat::createIdentity())),
                *px::PhysicsMaterial(0.0f, 0.0f, 0.0f).getInternalMaterial(), physx::PxTransform(localPose.getPhyicsMatrix()));
            break;
        }

    default: HE_ASSERT(false, "Type not supported with dynamic actors");
        break;
    }
    HE_ASSERT(pPxShape != nullptr, "Trigger shape creation failed");

    pPxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true); // trigger shape
    pPxShape->userData = this;

    physx::PxFilterData sFilter;
    sFilter.word0 = COLLISION_FLAG_OBSTACLE;
    sFilter.word1 = COLLISION_FLAG_OBSTACLE_AGAINST;

    //physx::PxFilterData qFilter;
    //physx::PxSetupDrivableShapeQueryFilterData(&qFilter);

    //pPxShape->setQueryFilterData(qFilter);
    pPxShape->setSimulationFilterData(sFilter);
    PHYSICS->unlock();
}

/* SETTERS */
void PhysicsTrigger::setPose(const vec3& move, const vec3& axis, float angle)
{
    m_pActor->getInternalActor()->moveKinematic(physx::PxTransform(physx::PxVec3(move.x, move.y, move.z),
        physx::PxQuat(angle, physx::PxVec3(axis.x, axis.y, axis.z))));
}

void PhysicsTrigger::setPose(const mat44& pose)
{
    m_pActor->getInternalActor()->moveKinematic(physx::PxTransform(pose.getPhyicsMatrix()));
}

/* CALLBACKS */
void PhysicsTrigger::onTriggerEnter(physx::PxShape* /*pShape*/)
{
    m_OnTriggerEnterEvent();
}

void PhysicsTrigger::onTriggerLeave(physx::PxShape* /*pShape*/)
{
    m_OnTriggerLeaveEvent();
}

void PhysicsTrigger::addOnTriggerEnterCallBack(boost::function<void()> callback)
{
    m_OnTriggerEnterEvent += callback;
}

void PhysicsTrigger::addOnTriggerLeaveCallBack(boost::function<void()> callback)
{
    m_OnTriggerLeaveEvent += callback;
}

} } //end namespace