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
#include "HappyPCH.h" 

#include "PhysicsCharacterController.h"

#include "PhysicsEngine.h"

namespace he {
namespace px {

PhysicsCharacterController::PhysicsCharacterController()
    : m_Controller(nullptr)
    , m_Speed(0, 0, 0)
    , m_Gravity(0, -9.81f, 0)
    , m_Position(0, 0, 0)
    , m_StandsOnFloor(false)
    , m_MoveSpeed(0, 0, 0)
{
    PxCapsuleControllerDesc desc;
    desc.height = 2.0f;
    desc.climbingMode = physx::PxCapsuleClimbingMode::eCONSTRAINED;
    desc.interactionMode = physx::PxCCTInteractionMode::eINCLUDE;
    desc.nonWalkableMode = physx::PxCCTNonWalkableMode::eFORCE_SLIDING;
    desc.radius = 0.5f;
    desc.slopeLimit = cosf(piOverFour);
    desc.stepOffset = 0.3f;
    desc.callback = this;
    desc.userData = static_cast<IPhysicsUserDataContainer*>(this);
    PhysicsMaterial material(0.1f, 0.1f, 0.2f);
    desc.material = material.getInternalMaterial();

    PhysicsEngine* physics(PHYSICS);
    m_Controller = static_cast<physx::PxCapsuleController*>(
        physics->getControllerManager()->createController(*physics->getSDK(), physics->getScene(), desc));
    HE_ASSERT(m_Controller != nullptr, "Creation of character controller failed!");
}


PhysicsCharacterController::~PhysicsCharacterController()
{
    m_Controller->release();
}

void PhysicsCharacterController::setHeight( const float height )
{
    m_Controller->resize(height/2.0f);
}

void PhysicsCharacterController::setRadius( const float radius )
{
    m_Controller->setRadius(radius);
}

void PhysicsCharacterController::setFootPosition( const vec3& position )
{
    PxExtendedVec3 pos(position.x, position.y, position.z);
    m_Controller->setFootPosition(pos);
}

void PhysicsCharacterController::setGravity( const vec3& gravity )
{
    m_Gravity = gravity;
}

void PhysicsCharacterController::setMaxStepSize( const float meters )
{
    m_Controller->setStepOffset(meters);
}

void PhysicsCharacterController::setMoveSpeed( const vec3& moveSpeed )
{
    m_MoveSpeed = moveSpeed;
}

void PhysicsCharacterController::addSpeed( const vec3& speed )
{
    m_Speed += speed;
}

void PhysicsCharacterController::tick( float dTime )
{
    m_Speed += m_Gravity * 10 * dTime;
    const vec3 speed((m_Speed + m_MoveSpeed) * dTime);

    PxVec3 disp;
    speed.toPxVec3(&disp);
    const uint32 moveResult(m_Controller->move(disp, 0.0001f, dTime, m_Filters));

    const PxExtendedVec3 pxFootPos(m_Controller->getFootPosition());
    m_Position.x = static_cast<float>(pxFootPos.x);
    m_Position.y = static_cast<float>(pxFootPos.y);
    m_Position.z = static_cast<float>(pxFootPos.z);
    m_StandsOnFloor = false;
    if (moveResult & physx::PxControllerFlag::eCOLLISION_DOWN)
    {
        m_StandsOnFloor = true;
        m_Speed.y = 0;
        HitFloor();
    }
    if (moveResult & physx::PxControllerFlag::eCOLLISION_UP)
    {
        m_Speed.y = fabs(m_Speed.y);
        HitRoof();
    }
    if (moveResult & physx::PxControllerFlag::eCOLLISION_SIDES)
    {
        HitSide();
    }
}

// PxUserControllerHitReport
void PhysicsCharacterController::onShapeHit( const PxControllerShapeHit& /*hit*/ )
{
}

void PhysicsCharacterController::onControllerHit( const PxControllersHit& /*hit*/ )
{
}

void PhysicsCharacterController::onObstacleHit( const PxControllerObstacleHit& /*hit*/ )
{
}

} } //end namespace
