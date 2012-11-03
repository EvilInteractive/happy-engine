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

#include "IPhysicsActor.h"
#include "PhysicsDynamicActor.h"

namespace he {
namespace px {
class PhysicsDynamicActor;

class PhysicsTrigger : public IPhysicsActor
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    PhysicsTrigger(const mat44& pose);
    virtual ~PhysicsTrigger();

    /* GENERAL */
    virtual physx::PxRigidActor* getInternalActor() const;
    virtual void getTranslation(vec3& translation) const;
    virtual void getRotation(mat33& rotation) const;
    virtual void getPose(mat44& pose) const;


    void addTriggerShape(const IPhysicsShape* shape, uint32 collisionGroup = 0xffffffff, 
                         uint32 collisionAgainstGroup = 0xffffffff, const mat44& localPose = mat44::Identity);

    /* SETTERS */
    void setPose(const vec3& move, const vec3& axis, float angle);
    void setPose(const mat44& pose);

    /* USERDATA */
    virtual const PhysicsUserData& getUserData();
    template<typename T>
    void setUserData(T* rttiType)
    {
        m_Actor->setUserData<T>(rttiType);
    }

    /* CALLBACKS */
    void onTriggerEnter(physx::PxShape* shape);
    void onTriggerLeave(physx::PxShape* shape);

    event1<void, IPhysicsActor*> OnTriggerEnter;
    event1<void, IPhysicsActor*> OnTriggerLeave;

protected:
    virtual uint32 getCompatibleShapes() const;

private:
    void addShape(physx::PxShape* shape, uint32 collisionGroup, uint32 collisionAgainstGroup);

    /* DATAMEMBERS */
    PhysicsDynamicActor* m_Actor;


    /* DEFAULT COPY & ASSIGNENT */
    PhysicsTrigger(const PhysicsTrigger&);
    PhysicsTrigger& operator=(const PhysicsTrigger&);
};

} } //end namespace

#endif
