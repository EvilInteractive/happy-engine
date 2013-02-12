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
#include "PhysicsActor.h"

namespace he {
namespace px {

PhysicsFixedJoint::PhysicsFixedJoint(const PhysicsActor* actor0, const mat44& localAttach0,
                                     const PhysicsActor* actor1, const mat44& localAttach1)
{
    m_Joint = physx::PxFixedJointCreate(*PHYSICS->getSDK(), 
        actor0 != nullptr ? actor0->getInternalActor() : nullptr, physx::PxTransform(localAttach0.getPhyicsMatrix()),
        actor1 != nullptr ? actor1->getInternalActor() : nullptr, physx::PxTransform(localAttach1.getPhyicsMatrix()));

#if DEBUG || _DEBUG
    m_Joint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, true);
#endif
}


PhysicsFixedJoint::~PhysicsFixedJoint()
{
    m_Joint->release();
}

void PhysicsFixedJoint::setBreakForce( float force, float torque )
{
    m_Joint->setBreakForce(force, torque);
}

} } //end namespace