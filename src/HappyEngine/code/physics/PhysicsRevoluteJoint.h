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

#ifndef _HE_PHYSICS_SPHERICAL_JOINT_H_
#define _HE_PHYSICS_SPHERICAL_JOINT_H_
#pragma once

#include "IPhysicsJoint.h"

namespace physx {
    class PxRevoluteJoint;
}

namespace he {
namespace px {

class PhysicsActor;

class PhysicsRevoluteJoint : public IPhysicsJoint
{
public:
    PhysicsRevoluteJoint(const PhysicsActor* pActor0, const vec3& localAttach0,
                          const PhysicsActor* pActor1, const vec3& localAttach1);
    virtual ~PhysicsRevoluteJoint();

    void setBreakForce( float force, float torque );
    void setAsHardLimit(float lower, float upper, float restitution = 0.5f);
    void setAsSpring(float lower, float upper, float springStrength, float springDamping);
    
private:

    physx::PxRevoluteJoint* m_Joint;

    //Disable default copy constructor and default assignment operator
    PhysicsRevoluteJoint(const PhysicsRevoluteJoint&);
    PhysicsRevoluteJoint& operator=(const PhysicsRevoluteJoint&);
};

} } //end namespace

#endif
