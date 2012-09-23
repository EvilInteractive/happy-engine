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
//Created: 20/08/2011

#ifndef _HE_PHYSICS_STATIC_ACTOR_H_
#define _HE_PHYSICS_STATIC_ACTOR_H_
#pragma once

#include "PxRigidStatic.h"
#include "vec3.h"
#include "mat44.h"
#include "IPhysicsShape.h"
#include "PhysicsMaterial.h"
#include "IPhysicsActor.h"

namespace he {
namespace px {

class PhysicsStaticActor : public IPhysicsActor
{
public:
    PhysicsStaticActor(const mat44& pose, const IPhysicsShape* shape, const PhysicsMaterial& material);
    PhysicsStaticActor(const mat44& pose);
    virtual ~PhysicsStaticActor();

    virtual void getTranslation(vec3& translation) const;
    virtual void getRotation(mat33& rotation) const;
    virtual void getPose(mat44& pose) const;
    virtual physx::PxRigidActor* getInternalActor() const;

    void addShape(const IPhysicsShape* shape, const PhysicsMaterial& material, 
                uint32 collisionGroup = 0xffffffff, 
                const mat44& localPose = mat44::Identity);

private:
    void addShape(physx::PxShape* shape, uint32 collisionGroup);

    physx::PxRigidStatic* m_Actor;

    //Disable default copy constructor and default assignment operator
    PhysicsStaticActor(const PhysicsStaticActor&);
    PhysicsStaticActor& operator=(const PhysicsStaticActor&);
};

} } //end namespace

#endif
