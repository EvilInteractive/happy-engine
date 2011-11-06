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
//Created: 20/08/2011

#ifndef _HE_PHYSICS_STATIC_ACTOR_H_
#define _HE_PHYSICS_STATIC_ACTOR_H_
#pragma once

#include "PxRigidStatic.h"
#include "vec3.h"
#include "mat44.h"
#include "IPhysicsShape.h"
#include "PhysicsMaterial.h"

namespace he {
namespace px {

class PhysicsStaticActor
{
public:
    PhysicsStaticActor(const mat44& pose, const IPhysicsShape* pShape, const PhysicsMaterial& material);
    PhysicsStaticActor(const mat44& pose);
    virtual ~PhysicsStaticActor();
    
    vec3 getPosition() const;
    mat44 getPose() const;

    void addShape(const IPhysicsShape* pShape, const PhysicsMaterial& material) const;

private:

    physx::PxRigidStatic* m_pActor;

    //Disable default copy constructor and default assignment operator
    PhysicsStaticActor(const PhysicsStaticActor&);
    PhysicsStaticActor& operator=(const PhysicsStaticActor&);
};

} } //end namespace

#endif
