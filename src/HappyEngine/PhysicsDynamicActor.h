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
//Created: 22/08/2011
//Updated: 14/09/2011 -added addForce/velocity and kinematic methods

#ifndef _HE_PHYSICS_DYNAMIC_ACTOR_H_
#define _HE_PHYSICS_DYNAMIC_ACTOR_H_
#pragma once

#include "PxRigidDynamic.h"
#include "Vector3.h"
#include "Matrix.h"
#include "IPhysicsShape.h"
#include "PhysicsMaterial.h"
#include <vector>

namespace happyengine {
namespace physics {

class PhysicsDynamicActor
{
public:
	PhysicsDynamicActor(const math::Matrix& pose, const shapes::IPhysicsShape::pointer& shape, float density, PhysicsMaterial* pMaterial);
    PhysicsDynamicActor(const math::Matrix& pose, const std::vector<shapes::IPhysicsShape::pointer>& shapes, float density, PhysicsMaterial* pMaterial);
    virtual ~PhysicsDynamicActor();

    math::Vector3 getPosition() const;
    math::Matrix getPose() const;

    void setVelocity(const math::Vector3& velocity);
    void addVelocity(const math::Vector3& velocity);
    void addForce(const math::Vector3& force);

    void setKeyframed(bool keyframed);
    void PhysicsDynamicActor::keyframedSetPose(const math::Vector3& move, const math::Vector3& axis, float angle);

private:

    PxRigidDynamic* m_pActor;

    //Disable default copy constructor and default assignment operator
    PhysicsDynamicActor(const PhysicsDynamicActor&);
    PhysicsDynamicActor& operator=(const PhysicsDynamicActor&);
};

} } //end namespace

#endif
