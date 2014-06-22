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
//Created: 22/08/2011
//Updated: 14/09/2011 -added addForce/velocity and kinematic methods

#ifndef _HE_PHYSICS_DYNAMIC_ACTOR_H_
#define _HE_PHYSICS_DYNAMIC_ACTOR_H_
#pragma once

#include "PhysicsActor.h"
#include "PhysicsUserData.h"

namespace physx {
    class PxShape;
    class PxRigidDynamic;
}

namespace he {
namespace px {
class IPhysicsShape;
class PhysicsMaterial;

class PhysicsDynamicActor : public PhysicsActor
{
public:
    PhysicsDynamicActor(const mat44& pose);
    virtual ~PhysicsDynamicActor();

    virtual void getTranslation(vec3& translation) const;
    virtual void getRotation(mat33& rotation) const;
    virtual void getPose(mat44& pose) const;
    virtual physx::PxRigidActor* getInternalActor() const;

    void addShape(const IPhysicsShape* pShape, const PhysicsMaterial& material, float mass, 
                  uint32 collisionGroup = 0xffffffff, uint32 collisionAgainstGroup = 0xffffffff, 
                  const mat44& localPose = mat44::Identity);

    void setVelocity(const vec3& velocity);
    void addVelocity(const vec3& velocity);
    void addForce(const vec3& force);

    void setKeyframed(bool keyframed);
    bool isKeyframed() const;
    void keyframedSetPose(const vec3& position, const vec3& axis = vec3::up, float angle = 0.0f);
    void keyframedSetPose(const mat44& pose);

    bool isSleeping() const;

    virtual PhysicsUserDataContainerType getType() const { return PhysicsUserDataContainerType_Dynamic; }
    virtual const PhysicsUserData& getUserData() const { return m_UserData; }
    template<typename T>
    void setUserData(T* rttiType)
    {
        if (rttiType != nullptr)
            m_UserData.setRTTI(T::getRTTI());
        else
            m_UserData.setRTTI(0);
        m_UserData.setData(rttiType);
    }

protected:
    virtual uint32 getCompatibleShapes() const;

private:
    void addShape(physx::PxShape* shape, float mass, uint32 collisionGroup, uint32 collisionAgainstGroup);

    physx::PxRigidDynamic* m_Actor;
    PhysicsUserData m_UserData;

    //Disable default copy constructor and default assignment operator
    PhysicsDynamicActor(const PhysicsDynamicActor&);
    PhysicsDynamicActor& operator=(const PhysicsDynamicActor&);
};

} } //end namespace

#endif
