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
//Created: 16/12/2011
#include "HappyPCH.h" 

#include "PhysicsRevoluteJoint.h"

#include "PhysicsEngine.h"
#include "IPhysicsActor.h"

namespace he {
namespace px {

PhysicsRevoluteJoint::PhysicsRevoluteJoint(const IPhysicsActor* pActor0, const vec3& localAttach0,
                                           const IPhysicsActor* pActor1, const vec3& localAttach1)
{
    m_pJoint = physx::PxRevoluteJointCreate(*PHYSICS->getSDK(), 
        pActor0->getInternalActor(), physx::PxTransform(physx::PxVec3(localAttach0.x, localAttach0.y, localAttach0.z)),
        pActor1->getInternalActor(), physx::PxTransform(physx::PxVec3(localAttach1.x, localAttach1.y, localAttach1.z)));
}


PhysicsRevoluteJoint::~PhysicsRevoluteJoint()
{
    m_pJoint->release();
}

void PhysicsRevoluteJoint::setBreakForce( float force, float torque )
{
    m_pJoint->setBreakForce(force, torque);
}

void PhysicsRevoluteJoint::setAsHardLimit(float lower, float upper, float restitution)
{
    physx::PxJointLimitPair limit(lower, upper, 0.01f);
    limit.restitution = restitution;
    m_pJoint->setLimit(limit);
    m_pJoint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eLIMIT_ENABLED, true);
}

void PhysicsRevoluteJoint::setAsSpring( float lower, float upper, float springStrength, float springDamping )
{
    physx::PxJointLimitPair limit(lower, upper, 0.01f);
    limit.spring = springStrength;
    limit.damping = springDamping;
    m_pJoint->setLimit(limit);
    m_pJoint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eLIMIT_ENABLED, true);
}

} } //end namespace