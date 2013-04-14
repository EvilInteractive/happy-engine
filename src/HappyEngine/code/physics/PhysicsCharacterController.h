//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 25/11/2012

#ifndef _HE_PhysicsCharacterController_H_
#define _HE_PhysicsCharacterController_H_
#pragma once

#include "PhysicsActor.h"
#include "PhysicsUserData.h"
#include <characterkinematic/PxController.h>
#include <characterkinematic/PxCapsuleController.h>

namespace he {
namespace px {

class PhysicsCharacterController : physx::PxUserControllerHitReport, public IPhysicsUserDataContainer
{
public:
    PhysicsCharacterController();
    virtual ~PhysicsCharacterController();

    void attachToScene();
    void detachFromScene();

    void setHeight(const float height);
    void setRadius(const float radius);
    void setFootPosition(const vec3& position);
    void setGravity(const vec3& gravity);
    void setMaxStepSize(const float meters);

    const vec3& getFootPosition() const { return m_Position; }
    bool getStandsOnFloor() const { return m_StandsOnFloor; }

    void setMoveSpeed(const vec3& moveSpeed);
    void addSpeed(const vec3& speed);

    void tick(float dTime);

    event0<void> HitFloor;
    event0<void> HitRoof;
    event0<void> HitSide;

    // PxUserControllerHitReport
    virtual void onShapeHit(const physx::PxControllerShapeHit& hit);
    virtual void onControllerHit(const physx::PxControllersHit& hit);
    virtual void onObstacleHit(const physx::PxControllerObstacleHit& hit);

    // IPhysicsUserDataContainer
    virtual PhysicsUserDataContainerType getType() const { return PhysicsUserDataContainerType_Controller; }
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

private:
    physx::PxCapsuleControllerDesc m_ControllerDesc;
    physx::PxCapsuleController* m_Controller;
    physx::PxControllerFilters m_Filters;

    vec3 m_Gravity;
    vec3 m_Speed;
    vec3 m_MoveSpeed;
    vec3 m_Position;

    bool m_StandsOnFloor;

    PhysicsUserData m_UserData;

    //Disable default copy constructor and default assignment operator
    PhysicsCharacterController(const PhysicsCharacterController&);
    PhysicsCharacterController& operator=(const PhysicsCharacterController&);
};

} } //end namespace

#endif
