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
//Created: 14/12/2011
#include "HappyPCH.h" 

#include "PhysicsSphericalJoint.h"

#include "PhysicsEngine.h"
#include "PhysicsActor.h"

#include <PxConstraint.h>
#include <extensions/PxSphericalJoint.h>

namespace he {
namespace px {

PhysicsSphericalJoint::PhysicsSphericalJoint(const PhysicsActor* pActor0, const vec3& localAttach0,
                                             const PhysicsActor* pActor1, const vec3& localAttach1)
{
    m_pJoint = physx::PxSphericalJointCreate(*PHYSICS->getSDK(), 
        pActor0->getInternalActor(), physx::PxTransform(physx::PxVec3(localAttach0.x, localAttach0.y, localAttach0.z)),
        pActor1->getInternalActor(), physx::PxTransform(physx::PxVec3(localAttach1.x, localAttach1.y, localAttach1.z)));

#if DEBUG || _DEBUG
    m_pJoint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, true);
#endif
}

PhysicsSphericalJoint::PhysicsSphericalJoint( const PhysicsActor* pActor0, const mat44& localAttach0, const PhysicsActor* pActor1, const mat44& localAttach1 )
{
    m_pJoint = physx::PxSphericalJointCreate(*PHYSICS->getSDK(), 
        pActor0->getInternalActor(), physx::PxTransform(localAttach0.getPhyicsMatrix()),
        pActor1->getInternalActor(), physx::PxTransform(localAttach1.getPhyicsMatrix()));
}


PhysicsSphericalJoint::~PhysicsSphericalJoint()
{
    m_pJoint->release();
}

void PhysicsSphericalJoint::setBreakForce( float force, float torque )
{
    m_pJoint->setBreakForce(force, torque);
}

void PhysicsSphericalJoint::setAsHardLimit(float yAngle, float zAngle, float restitution)
{
    physx::PxJointLimitCone limit(yAngle, zAngle, 0.01f);
    limit.restitution = restitution;
    m_pJoint->setLimitCone(limit);
    m_pJoint->setSphericalJointFlag(physx::PxSphericalJointFlag::eLIMIT_ENABLED, true);
}

void PhysicsSphericalJoint::setAsSpring( float yToAngle, float zToAngle, float springStrength, float springDamping )
{
    physx::PxJointLimitCone limit(yToAngle, zToAngle, 0.01f);
    limit.spring = springStrength;
    limit.damping = springDamping;
    m_pJoint->setLimitCone(limit);
    m_pJoint->setSphericalJointFlag(physx::PxSphericalJointFlag::eLIMIT_ENABLED, true);
}

} } //end namespace