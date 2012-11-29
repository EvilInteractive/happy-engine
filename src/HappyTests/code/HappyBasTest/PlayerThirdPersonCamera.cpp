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
//Created: 28/11/2012
#include "HappyBasTestPCH.h" 

#include "PlayerThirdPersonCamera.h"
#include "Player.h"

#include "ControlsManager.h"
#include "IMouse.h"

#define SENSITIVITY 100.0f

// 80 degrees
#define MAX_PITCH 1.396263402f
// 10 degrees
#define MIN_PITCH 0.174532925f

#define CAM_DISTANCE 20.0f

namespace ht {

PlayerThirdPersonCamera::PlayerThirdPersonCamera(Player* const player)
    : m_Player(player)
    , m_PreviousMousePos(0, 0)
    , m_Yaw(0.0f), m_Pitch(he::piOverFour)
{
}


PlayerThirdPersonCamera::~PlayerThirdPersonCamera()
{
}

void PlayerThirdPersonCamera::tick( float /*dTime*/ )
{
    using namespace he;

    io::ControlsManager* const controls(CONTROLS);
    const io::IMouse* const mouse(controls->getMouse());
    const vec2 mousePos(mouse->getPosition());

    controls->getFocus(this);

    if (controls->hasFocus(this))
    {
        const vec2 mouseMovement = mousePos - m_PreviousMousePos;
        m_PreviousMousePos = mousePos;
        m_Pitch += mouseMovement.y / SENSITIVITY;
        m_Pitch = clamp(m_Pitch, MIN_PITCH, MAX_PITCH);
        m_Yaw -= mouseMovement.x / SENSITIVITY;

        const mat44 pitchRotationMatrix(mat44::createRotation(vec3::right, m_Pitch));
        const mat44 yawRotationMatrix(mat44::createRotation(vec3::up, m_Yaw));
        const vec3 lookDir(yawRotationMatrix * (pitchRotationMatrix * vec3::forward));
        const vec3 upDir(yawRotationMatrix * (pitchRotationMatrix * vec3::up));

        const vec3 lookAtPos(m_Player->getCenterPosition());
        lookAt(lookAtPos - lookDir * CAM_DISTANCE, lookAtPos, upDir);
    }
    else
    {
        m_PreviousMousePos = mousePos;
    }

    controls->returnFocus(this);
}

} //end namespace