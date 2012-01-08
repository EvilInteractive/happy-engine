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

#ifndef _HE_PHYSICS_SPHERICAL_JOINT_H_
#define _HE_PHYSICS_SPHERICAL_JOINT_H_
#pragma once

#include "IPhysicsJoint.h"

namespace physx {
    class PxSphericalJoint;
}

namespace he {
namespace px {

class IPhysicsActor;

class PhysicsSphericalJoint : public IPhysicsJoint
{
public:
    PhysicsSphericalJoint(const IPhysicsActor* pActor0, const vec3& localAttach0,
                          const IPhysicsActor* pActor1, const vec3& localAttach1);
    virtual ~PhysicsSphericalJoint();

    void setBreakForce( float force, float torque );
    void setAsHardLimit(float yAngle, float zAngle, float restitution = 1.0f);
    void setAsSpring(float yToAngle, float zToAngle, float springStrength, float springDamping);

private:

    physx::PxSphericalJoint* m_pJoint;

    //Disable default copy constructor and default assignment operator
    PhysicsSphericalJoint(const PhysicsSphericalJoint&);
    PhysicsSphericalJoint& operator=(const PhysicsSphericalJoint&);
};

} } //end namespace

#endif
