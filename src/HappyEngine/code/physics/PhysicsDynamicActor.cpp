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
//Created: 22/08/2011
//Updated: 14/09/2011 -added addForce/velocity and kinematic methods
#include "HappyPCH.h" 

#include "PhysicsDynamicActor.h"
#include "HappyEngine.h"
#include "HeAssert.h"
#include "PhysicsEngine.h"

#include "PhysicsBoxShape.h"
#include "PhysicsSphereShape.h"
#include "PhysicsConvexShape.h"
#include "PhysicsCapsuleShape.h"
#include "geometry/PxCapsuleGeometry.h"

namespace he {
namespace px {

PhysicsDynamicActor::PhysicsDynamicActor(const mat44& pose, const IPhysicsShape* pShape, const PhysicsMaterial& material, float mass)
{
    PHYSICS->lock();
    m_pActor = PHYSICS->getSDK()->createRigidDynamic(physx::PxTransform(pose.getPhyicsMatrix().column3.getXYZ(), 
        physx::PxQuat(physx::PxMat33(pose.getPhyicsMatrix().column0.getXYZ(), 
        pose.getPhyicsMatrix().column1.getXYZ(), 
        pose.getPhyicsMatrix().column2.getXYZ()))));
    PHYSICS->unlock();
    HE_ASSERT(m_pActor != nullptr, "Actor creation failed");

    addShape(pShape, material, mass);

    PHYSICS->lock();
    PHYSICS->getScene()->addActor(*m_pActor);
    PHYSICS->unlock();

}
PhysicsDynamicActor::PhysicsDynamicActor(const mat44& pose)
{  
    PHYSICS->lock();
    m_pActor = PHYSICS->getSDK()->createRigidDynamic(physx::PxTransform(pose.getPhyicsMatrix().column3.getXYZ(), 
                                  physx::PxQuat(physx::PxMat33(pose.getPhyicsMatrix().column0.getXYZ(), 
                                                                 pose.getPhyicsMatrix().column1.getXYZ(), 
                                                                 pose.getPhyicsMatrix().column2.getXYZ()))));
    PHYSICS->unlock();
    HE_ASSERT(m_pActor != nullptr, "Actor creation failed");

    PHYSICS->lock();
    PHYSICS->getScene()->addActor(*m_pActor);
    PHYSICS->unlock();
}
void PhysicsDynamicActor::addShape( const IPhysicsShape* pShape, const PhysicsMaterial& material, float mass, const mat44& localPose ) const
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
                *material.getInternalMaterial(), physx::PxTransform(localPose.getPhyicsMatrix()));
            break;
        }
    case PhysicsShapeType_Sphere:
        {
            const PhysicsSphereShape* pSphereShape(static_cast<const PhysicsSphereShape*>(pShape));
            pPxShape = m_pActor->createShape(
                physx::PxSphereGeometry(pSphereShape->getRadius()), 
                *material.getInternalMaterial(), physx::PxTransform(localPose.getPhyicsMatrix()));
            break;
        }
    case PhysicsShapeType_Capsule:
        {
            const PhysicsCapsuleShape* pCapsuleShape(static_cast<const PhysicsCapsuleShape*>(pShape));
            pPxShape = m_pActor->createShape(
                physx::PxCapsuleGeometry(pCapsuleShape->getRadius(), pCapsuleShape->getHeight() / 2.0f), 
                *material.getInternalMaterial(), physx::PxTransform(localPose.getPhyicsMatrix()));
            break;
        }
    case PhysicsShapeType_Convex:
        {
            const PhysicsConvexShape* pConvexShape(static_cast<const PhysicsConvexShape*>(pShape));
            pPxShape = m_pActor->createShape(
                physx::PxConvexMeshGeometry(pConvexShape->getInternalMesh(), 
                    physx::PxMeshScale(physx::PxVec3(pConvexShape->getScale().x, pConvexShape->getScale().y, pConvexShape->getScale().z), physx::PxQuat::createIdentity())),
                    *material.getInternalMaterial(), physx::PxTransform(localPose.getPhyicsMatrix()));
            break;
        }

    default: HE_ASSERT(false, "Type not supported with dynamic actors");
        break;
    }
    HE_ASSERT(pPxShape != nullptr, "Shape creation failed");

    pPxShape->userData = nullptr;

    physx::PxRigidBodyExt::setMassAndUpdateInertia(*m_pActor, m_pActor->getMass() + mass);
    //physx::PxRigidBodyExt::updateMassAndInertia(*m_pActor, 1.0f);

    physx::PxFilterData sFilter;
    sFilter.word0 = COLLISION_FLAG_OBSTACLE;
    sFilter.word1 = COLLISION_FLAG_OBSTACLE_AGAINST;

    physx::PxFilterData qFilter;
    //physx::PxSetupDrivableShapeQueryFilterData(&qFilter);

    pPxShape->setQueryFilterData(qFilter);
    pPxShape->setSimulationFilterData(sFilter);
    PHYSICS->unlock();
}

PhysicsDynamicActor::~PhysicsDynamicActor()
{
    if (PHYSICS != nullptr)
    {
        PHYSICS->lock();
        PHYSICS->getScene()->removeActor(*m_pActor);
        PHYSICS->unlock();
        m_pActor->release();
    }
}

vec3 PhysicsDynamicActor::getPosition() const
{
    return vec3(m_pActor->getGlobalPose().p);
}
mat44 PhysicsDynamicActor::getPose() const
{
    return mat44(physx::PxMat44(physx::PxMat33(m_pActor->getGlobalPose().q), m_pActor->getGlobalPose().p));
}

void PhysicsDynamicActor::setVelocity(const vec3& velocity)
{
    m_pActor->setLinearVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
}
void PhysicsDynamicActor::addVelocity(const vec3& velocity)
{
    m_pActor->addForce(physx::PxVec3(velocity.x, velocity.y, velocity.z), physx::PxForceMode::eVELOCITY_CHANGE);
}
void PhysicsDynamicActor::addForce(const vec3& force)
{
    m_pActor->addForce(physx::PxVec3(force.x, force.y, force.z), physx::PxForceMode::eFORCE);
}

void PhysicsDynamicActor::setKeyframed(bool keyframed)
{
    m_pActor->setRigidDynamicFlag(physx::PxRigidDynamicFlag::eKINEMATIC, keyframed);
}
void PhysicsDynamicActor::keyframedSetPose(const vec3& position, const vec3& axis, float angle)
{
    m_pActor->setKinematicTarget(physx::PxTransform(physx::PxVec3(position.x, position.y, position.z),
        physx::PxQuat(angle, physx::PxVec3(axis.x, axis.y, axis.z))));
}

void PhysicsDynamicActor::keyframedSetPose(const mat44& pose)
{
    m_pActor->setKinematicTarget(physx::PxTransform(pose.getPhyicsMatrix()));
}

physx::PxRigidDynamic* PhysicsDynamicActor::getInternalActor() const
{
    return m_pActor;
}

} } //end namespace