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

#include "PhysicsEngine.h"

#include "PhysicsBoxShape.h"
#include "PhysicsSphereShape.h"
#include "PhysicsConvexShape.h"
#include "PhysicsCapsuleShape.h"

#include "ResourceFactory.h"
#include "PhysicsConvexMesh.h"

namespace he {
namespace px {

/* CONSTRUCTOR - DESTRUCTOR */
PhysicsTrigger::PhysicsTrigger(const mat44& pose) : m_Actor(NEW PhysicsDynamicActor(pose))
{
    m_Actor->setKeyframed(true);
}

PhysicsTrigger::~PhysicsTrigger()
{
    delete m_Actor;
}

/* GENERAL */
void PhysicsTrigger::addTriggerShape(const IPhysicsShape* shape, uint32 collisionGroup, uint32 collisionAgainstGroup, const mat44& localPose)
{
    PHYSICS->lock();
    switch (shape->getType())
    {
    case PhysicsShapeType_Box:
        {
            const PhysicsBoxShape* pBoxShape(static_cast<const PhysicsBoxShape*>(shape));
            physx::PxShape* pxShape(m_Actor->getInternalActor()->createShape(
                physx::PxBoxGeometry(pBoxShape->getDimension().x / 2.0f, pBoxShape->getDimension().y / 2.0f, pBoxShape->getDimension().z / 2.0f), 
                *PHYSICS->createMaterial(0, 0, 0), physx::PxTransform(localPose.getPhyicsMatrix())));
            addShape(pxShape, collisionGroup, collisionAgainstGroup);
            break;
        }
    case PhysicsShapeType_Sphere:
        {
            const PhysicsSphereShape* pSphereShape(static_cast<const PhysicsSphereShape*>(shape));
            physx::PxShape* pxShape(m_Actor->getInternalActor()->createShape(
                physx::PxSphereGeometry(pSphereShape->getRadius()), 
                *PHYSICS->createMaterial(0, 0, 0), physx::PxTransform(localPose.getPhyicsMatrix())));
            addShape(pxShape, collisionGroup, collisionAgainstGroup);
            break;
        }
    case PhysicsShapeType_Capsule:
        {
            const PhysicsCapsuleShape* pCapsuleShape(static_cast<const PhysicsCapsuleShape*>(shape));
            physx::PxShape* pxShape(m_Actor->getInternalActor()->createShape(
                physx::PxCapsuleGeometry(pCapsuleShape->getRadius(), pCapsuleShape->getHeight() / 2.0f), 
                *PHYSICS->createMaterial(0, 0, 0), physx::PxTransform(localPose.getPhyicsMatrix())));
            addShape(pxShape, collisionGroup, collisionAgainstGroup);
            break;
        }
    case PhysicsShapeType_Convex:
        {
            const PhysicsConvexShape* convexShape(static_cast<const PhysicsConvexShape*>(shape));
            if (convexShape->getConvexMesh() != ObjectHandle::unassigned) // load failed
            {
                const std::vector<physx::PxConvexMesh*>& meshes(
                    ResourceFactory<PhysicsConvexMesh>::getInstance()->get(
                    convexShape->getConvexMesh())->getInternalMeshes());

                std::for_each(meshes.cbegin(), meshes.cend(), [&](physx::PxConvexMesh* mesh)
                {
                    physx::PxVec3 scale;
                    convexShape->getScale().toPxVec3(&scale);

                    physx::PxShape* pxShape(m_Actor->getInternalActor()->createShape(
                        physx::PxConvexMeshGeometry(mesh, 
                        physx::PxMeshScale(scale, physx::PxQuat::createIdentity())),
                        *PHYSICS->createMaterial(0, 0, 0), physx::PxTransform(localPose.getPhyicsMatrix())));
                    addShape(pxShape, collisionGroup, collisionAgainstGroup);
                });
            }
            break;
        }
    default: HE_ASSERT(false, "Shape not supported with triggers");
        break;
    }
    PHYSICS->unlock();
}
void PhysicsTrigger::addShape( physx::PxShape* shape, uint32 collisionGroup, uint32 collisionAgainstGroup )
{
    HE_ASSERT(shape != nullptr, "Trigger shape creation failed");

    shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true); // trigger shape
    shape->userData = this;

    physx::PxFilterData filter;
    filter.word0 = collisionGroup;
    filter.word1 = collisionAgainstGroup;

    shape->setQueryFilterData(filter);
    shape->setSimulationFilterData(filter);
}


/* SETTERS */
void PhysicsTrigger::setPose(const vec3& move, const vec3& axis, float angle)
{
    m_Actor->getInternalActor()->setKinematicTarget(physx::PxTransform(physx::PxVec3(move.x, move.y, move.z),
        physx::PxQuat(angle, physx::PxVec3(axis.x, axis.y, axis.z))));
}

void PhysicsTrigger::setPose(const mat44& pose)
{
    m_Actor->getInternalActor()->setKinematicTarget(physx::PxTransform(pose.getPhyicsMatrix()));
}

/* CALLBACKS */
void PhysicsTrigger::onTriggerEnter(physx::PxShape* shape)
{
    IPhysicsActor* actor(static_cast<IPhysicsActor*>(shape->userData));
    OnTriggerEnter(actor);
}

void PhysicsTrigger::onTriggerLeave(physx::PxShape* shape)
{
    IPhysicsActor* actor(static_cast<IPhysicsActor*>(shape->userData));
    OnTriggerLeave(actor);
}


} } //end namespace