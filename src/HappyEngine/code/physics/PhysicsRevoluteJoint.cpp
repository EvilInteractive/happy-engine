//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
//Created: 16/12/2011
#include "HappyPCH.h" 

#include "PhysicsRevoluteJoint.h"

#include "PhysicsEngine.h"
#include "PhysicsActor.h"

#include <PxConstraint.h>
#include <extensions/PxRevoluteJoint.h>

namespace he {
namespace px {

PhysicsRevoluteJoint::PhysicsRevoluteJoint(const PhysicsActor* actor0, const vec3& localAttach0,
                                           const PhysicsActor* actor1, const vec3& localAttach1)
{
    m_Joint = physx::PxRevoluteJointCreate(*PHYSICS->getSDK(), 
        actor0->getInternalActor(), physx::PxTransform(physx::PxVec3(localAttach0.x, localAttach0.y, localAttach0.z)),
        actor1->getInternalActor(), physx::PxTransform(physx::PxVec3(localAttach1.x, localAttach1.y, localAttach1.z)));

#ifdef HE_DEBUG
    m_Joint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, true);
#endif
}


PhysicsRevoluteJoint::~PhysicsRevoluteJoint()
{
    m_Joint->release();
}

void PhysicsRevoluteJoint::setBreakForce( float force, float torque )
{
    m_Joint->setBreakForce(force, torque);
}

void PhysicsRevoluteJoint::setAsHardLimit(float lower, float upper, float restitution)
{
    physx::PxJointLimitPair limit(lower, upper, 0.01f);
    limit.restitution = restitution;
    m_Joint->setLimit(limit);
    m_Joint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eLIMIT_ENABLED, true);
}

void PhysicsRevoluteJoint::setAsSpring( float lower, float upper, float springStrength, float springDamping )
{
    physx::PxJointLimitPair limit(lower, upper, 0.01f);
    limit.spring = springStrength;
    limit.damping = springDamping;
    m_Joint->setLimit(limit);
    m_Joint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eLIMIT_ENABLED, true);
}

} } //end namespace