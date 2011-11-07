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
//Created: 22/08/2011
//Updated: 14/09/2011 -added addForce/velocity and kinematic methods
#include "StdAfx.h" 

#include "PhysicsDynamicActor.h"
#include "HappyEngine.h"
#include "HeAssert.h"
#include "PhysicsEngine.h"

#include "PhysicsBoxShape.h"
#include "PhysicsSphereShape.h"
#include "PhysicsConvexShape.h"

namespace he {
namespace px {

PhysicsDynamicActor::PhysicsDynamicActor(const mat44& pose, const IPhysicsShape* pShape, const PhysicsMaterial& material, float mass)
{
    m_pActor = PHYSICS->getSDK()->createRigidDynamic(physx::PxTransform(pose.getPhyicsMatrix().column3.getXYZ(), 
        physx::PxQuat(physx::PxMat33(pose.getPhyicsMatrix().column0.getXYZ(), 
        pose.getPhyicsMatrix().column1.getXYZ(), 
        pose.getPhyicsMatrix().column2.getXYZ()))));
    ASSERT(m_pActor != nullptr, "Actor creation failed");

    addShape(pShape, material, mass);

    PHYSICS->getScene()->addActor(*m_pActor);
}
PhysicsDynamicActor::PhysicsDynamicActor(const mat44& pose)
{  
    m_pActor = PHYSICS->getSDK()->createRigidDynamic(physx::PxTransform(pose.getPhyicsMatrix().column3.getXYZ(), 
                                  physx::PxQuat(physx::PxMat33(pose.getPhyicsMatrix().column0.getXYZ(), 
                                                                 pose.getPhyicsMatrix().column1.getXYZ(), 
                                                                 pose.getPhyicsMatrix().column2.getXYZ()))));
    ASSERT(m_pActor != nullptr, "Actor creation failed");

    PHYSICS->getScene()->addActor(*m_pActor);
}
void PhysicsDynamicActor::addShape( const IPhysicsShape* pShape, const PhysicsMaterial& material, float mass ) const
{
    physx::PxShape* pPxShape(nullptr);
    switch (pShape->getType())
    {
    case PhysicsShapeType_Box:
        {
            const PhysicsBoxShape* pBoxShape(dynamic_cast<const PhysicsBoxShape*>(pShape));
            ASSERT(pBoxShape != nullptr, "IPhysicsShape type PhysicsShapeType_Box is not a PhysicsBoxShape");
            pPxShape = m_pActor->createShape(
                physx::PxBoxGeometry(pBoxShape->getDimension().x, pBoxShape->getDimension().y, pBoxShape->getDimension().z), 
                *material.getInternalMaterial());
            break;
        }
    case PhysicsShapeType_Sphere:
        {
            const PhysicsSphereShape* pSphereShape(dynamic_cast<const PhysicsSphereShape*>(pShape));
            ASSERT(pSphereShape != nullptr, "IPhysicsShape type PhysicsShapeType_Sphere is not a PhysicsSphereShape");
            pPxShape = m_pActor->createShape(
                physx::PxSphereGeometry(pSphereShape->getRadius()), 
                *material.getInternalMaterial());
            break;
        }
    case PhysicsShapeType_Capsule:
        ASSERT(false, "Not Implemented");
        break;
    case PhysicsShapeType_Convex:
        {
            const PhysicsConvexShape* pConvexShape(dynamic_cast<const PhysicsConvexShape*>(pShape));
            ASSERT(pConvexShape != nullptr, "IPhysicsShape type PhysicsShapeType_Convex is not a PhysicsConvexShape");
            pPxShape = m_pActor->createShape(
                physx::PxConvexMeshGeometry(pConvexShape->getInternalMesh(), 
                    physx::PxMeshScale(physx::PxVec3(pConvexShape->getScale().x, pConvexShape->getScale().y, pConvexShape->getScale().z), physx::PxQuat::createIdentity())),
                    *material.getInternalMaterial());
            break;
        }

    default: ASSERT(false, "Type not supported with dynamic actors");
        break;
    }
    ASSERT(pPxShape != nullptr, "Shape creation failed");

    physx::PxRigidBodyExt::setMassAndUpdateInertia(*m_pActor, m_pActor->getMass() + mass);
}


PhysicsDynamicActor::~PhysicsDynamicActor()
{
    PHYSICS->getScene()->removeActor(*m_pActor);
    m_pActor->release();
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
    m_pActor->setLinearVelocity(physx::pubfnd3::PxVec3(velocity.x, velocity.y, velocity.z));
}
void PhysicsDynamicActor::addVelocity(const vec3& velocity)
{
    m_pActor->addForce(physx::pubfnd3::PxVec3(velocity.x, velocity.y, velocity.z), physx::PxForceMode::eVELOCITY_CHANGE);
}
void PhysicsDynamicActor::addForce(const vec3& force)
{
    m_pActor->addForce(physx::pubfnd3::PxVec3(force.x, force.y, force.z), physx::PxForceMode::eFORCE);
}

void PhysicsDynamicActor::setKeyframed(bool keyframed)
{
    m_pActor->setRigidDynamicFlag(physx::PxRigidDynamicFlag::eKINEMATIC, keyframed);
}
void PhysicsDynamicActor::keyframedSetPose(const vec3& move, const vec3& axis, float angle)
{
    m_pActor->moveKinematic(physx::PxTransform(physx::PxVec3(move.x, move.y, move.z),
        physx::PxQuat(angle, physx::PxVec3(axis.x, axis.y, axis.z))));
}

} } //end namespace