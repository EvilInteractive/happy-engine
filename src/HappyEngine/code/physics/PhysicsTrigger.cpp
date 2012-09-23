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
#include "PhysicsDynamicActor.h"

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
void PhysicsTrigger::addTriggerShape(const IPhysicsShape* shape, uint32 collisionGroup /*= 0xffffffff*/, 
                                     uint32 collisionAgainstGroup /*= 0xffffffff*/, const mat44& localPose)
{
    px::PhysicsMaterial mat(0, 0, 0);
    std::vector<physx::PxShape*> shapes;
    if (createShape(shapes, shape, mat, localPose))
    {
        PHYSICS->lock();
        std::for_each(shapes.cbegin(), shapes.cend(), [&](physx::PxShape* pxShape)
        {
            addShape(pxShape, collisionGroup, collisionAgainstGroup);
        });
        PHYSICS->unlock();
    }
}
void PhysicsTrigger::addShape( physx::PxShape* shape, uint32 collisionGroup, uint32 collisionAgainstGroup )
{
    HE_ASSERT(shape != nullptr, "Trigger shape creation failed");

    shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true); // trigger shape
    shape->userData = this;

    physx::PxFilterData sFilter;
    sFilter.word0 = collisionGroup;
    sFilter.word1 = collisionAgainstGroup;

    shape->setSimulationFilterData(sFilter);
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
void PhysicsTrigger::onTriggerEnter(physx::PxShape* /*pShape*/)
{
    m_OnTriggerEnterEvent();
}

void PhysicsTrigger::onTriggerLeave(physx::PxShape* /*pShape*/)
{
    m_OnTriggerLeaveEvent();
}

void PhysicsTrigger::addOnTriggerEnterCallBack(const boost::function<void()>& callback)
{
    m_OnTriggerEnterEvent += callback;
}

void PhysicsTrigger::addOnTriggerLeaveCallBack(const boost::function<void()>& callback)
{
    m_OnTriggerLeaveEvent += callback;
}

he::uint PhysicsTrigger::getCompatibleShapes() const
{
    return PhysicsShapeType_TriggerCompatible;
}

physx::PxRigidActor* PhysicsTrigger::getInternalActor() const
{
    return m_Actor->getInternalActor();
}

void PhysicsTrigger::getTranslation( vec3& translation ) const
{
    m_Actor->getTranslation(translation);
}

void PhysicsTrigger::getRotation( mat33& rotation ) const
{
    m_Actor->getRotation(rotation);
}

void PhysicsTrigger::getPose( mat44& pose ) const
{
    m_Actor->getPose(pose);
}

} } //end namespace