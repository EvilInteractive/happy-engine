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
//Author:  Sebastiaan Sprengers
//Created: 22/01/2012

#ifndef _HE_PHYSICS_TRIGGER_H_
#define _HE_PHYSICS_TRIGGER_H_
#pragma once

#include "event.h"
#include "PhysicsDynamicActor.h"

namespace he {
namespace px {

class PhysicsTrigger
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    PhysicsTrigger(const mat44& pose);
    virtual ~PhysicsTrigger();

    /* GENERAL */
    void addTriggerShape(const IPhysicsShape* shape, const mat44& localPose = mat44::Identity);

    /* SETTERS */
    void setPose(const vec3& move, const vec3& axis, float angle);
    void setPose(const mat44& pose);

    /* CALLBACKS */
    void onTriggerEnter(physx::PxShape* shape);
    void onTriggerLeave(physx::PxShape* shape);

    event0<void> OnTriggerEnter;
    event0<void> OnTriggerLeave;

private:
    void addShape(physx::PxShape* shape);

    /* DATAMEMBERS */
    PhysicsDynamicActor* m_Actor;


    /* DEFAULT COPY & ASSIGNENT */
    PhysicsTrigger(const PhysicsTrigger&);
    PhysicsTrigger& operator=(const PhysicsTrigger&);
};

} } //end namespace

#endif
