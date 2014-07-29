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
//Created: 25/11/2012
#include "HappyPCH.h" 

#include "PhysicsCharacterController.h"

#include "PhysicsEngine.h"

#include <characterkinematic/PxControllerManager.h>

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
    m_ControllerDesc.height = 2.0f;
    m_ControllerDesc.climbingMode = physx::PxCapsuleClimbingMode::eCONSTRAINED;
    m_ControllerDesc.interactionMode = physx::PxCCTInteractionMode::eINCLUDE;
    m_ControllerDesc.nonWalkableMode = physx::PxCCTNonWalkableMode::eFORCE_SLIDING;
    m_ControllerDesc.radius = 0.5f;
    m_ControllerDesc.slopeLimit = cosf(piOverFour);
    m_ControllerDesc.stepOffset = 0.3f;
    m_ControllerDesc.callback = this;
    m_ControllerDesc.userData = static_cast<IPhysicsUserDataContainer*>(this);
    PhysicsMaterial material(0.1f, 0.1f, 0.2f);
    m_ControllerDesc.material = material.getInternalMaterial();
}

void PhysicsCharacterController::attachToScene()
{
    HE_IF_ASSERT(m_Controller == nullptr, "Controller already attached to scene!")
    {
        PhysicsEngine* const physics(PHYSICS);
        m_Controller = static_cast<physx::PxCapsuleController*>(
            physics->getControllerManager()->createController(*physics->getSDK(), physics->getScene(), m_ControllerDesc));
        setFootPosition(m_Position);
        HE_ASSERT(m_Controller != nullptr, "Creation of character controller failed!");
    }
}

void PhysicsCharacterController::detachFromScene()
{
    if (m_Controller != nullptr)
    {
        m_Controller->release();
        m_Controller = nullptr;
    }
}

PhysicsCharacterController::~PhysicsCharacterController()
{
}

void PhysicsCharacterController::setHeight( const float height )
{
    m_ControllerDesc.height = height / 2.0f;
    if (m_Controller != nullptr)
    {
        m_Controller->resize(height/2.0f);
    }
}

void PhysicsCharacterController::setRadius( const float radius )
{
    m_ControllerDesc.radius = radius;
    if (m_Controller != nullptr)
    {
        m_Controller->setRadius(radius);
    }
}

void PhysicsCharacterController::setFootPosition( const vec3& position )
{
    m_Position = position;
    if (m_Controller != nullptr)
    {
        physx::PxExtendedVec3 pos(position.x, position.y, position.z);
        m_Controller->setFootPosition(pos);
    }
}

void PhysicsCharacterController::setGravity( const vec3& gravity )
{
    m_Gravity = gravity;
}

void PhysicsCharacterController::setMaxStepSize( const float meters )
{
    m_ControllerDesc.stepOffset = meters;
    if (m_Controller != nullptr)
    {
        m_Controller->setStepOffset(meters);
    }
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

    physx::PxVec3 disp;
    speed.toPxVec3(&disp);
    const uint32 moveResult(m_Controller->move(disp, 0.0001f, dTime, m_Filters));

    const physx::PxExtendedVec3 pxFootPos(m_Controller->getFootPosition());
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
void PhysicsCharacterController::onShapeHit( const physx::PxControllerShapeHit& /*hit*/ )
{
}

void PhysicsCharacterController::onControllerHit( const physx::PxControllersHit& /*hit*/ )
{
}

void PhysicsCharacterController::onObstacleHit( const physx::PxControllerObstacleHit& /*hit*/ )
{
}

} } //end namespace
