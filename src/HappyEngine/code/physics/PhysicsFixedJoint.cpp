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

#include "PhysicsFixedJoint.h"

#include "PhysicsEngine.h"
#include "IPhysicsActor.h"

namespace he {
namespace px {

PhysicsFixedJoint::PhysicsFixedJoint(const IPhysicsActor* pActor0, const vec3& localAttach0,
                                     const IPhysicsActor* pActor1, const vec3& localAttach1)
{
    m_pJoint = physx::PxFixedJointCreate(*PHYSICS->getSDK(), 
        pActor0->getInternalActor(), physx::PxTransform(physx::PxVec3(localAttach0.x, localAttach0.y, localAttach0.z)),
        pActor1->getInternalActor(), physx::PxTransform(physx::PxVec3(localAttach1.x, localAttach1.y, localAttach1.z)));
}


PhysicsFixedJoint::~PhysicsFixedJoint()
{
    m_pJoint->release();
}

void PhysicsFixedJoint::setBreakForce( float force, float torque )
{
    m_pJoint->setBreakForce(force, torque);
}

} } //end namespace