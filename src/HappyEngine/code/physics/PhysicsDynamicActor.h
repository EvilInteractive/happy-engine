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
//Created: 22/08/2011
//Updated: 14/09/2011 -added addForce/velocity and kinematic methods

#ifndef _HE_PHYSICS_DYNAMIC_ACTOR_H_
#define _HE_PHYSICS_DYNAMIC_ACTOR_H_
#pragma once

#include "PxRigidDynamic.h"
#include "vec3.h"
#include "mat44.h"
#include "IPhysicsShape.h"
#include "PhysicsMaterial.h"
#include <vector>
#include "IPhysicsActor.h"

namespace he {
namespace px {

class PhysicsDynamicActor : public IPhysicsActor
{
public:
    PhysicsDynamicActor(const mat44& pose, const IPhysicsShape* pShape, const PhysicsMaterial& material, float mass);
    PhysicsDynamicActor(const mat44& pose);
    virtual ~PhysicsDynamicActor();

    virtual vec3 getPosition() const;
    virtual mat44 getPose() const;
    virtual physx::PxRigidActor* getInternalActor() const;

    void addShape(const IPhysicsShape* pShape, const PhysicsMaterial& material, float mass) const;

    void setVelocity(const vec3& velocity);
    void addVelocity(const vec3& velocity);
    void addForce(const vec3& force);

    void setKeyframed(bool keyframed);
    void keyframedSetPose(const vec3& move, const vec3& axis, float angle);

private:

    physx::PxRigidDynamic* m_pActor;

    //Disable default copy constructor and default assignment operator
    PhysicsDynamicActor(const PhysicsDynamicActor&);
    PhysicsDynamicActor& operator=(const PhysicsDynamicActor&);
};

} } //end namespace

#endif
