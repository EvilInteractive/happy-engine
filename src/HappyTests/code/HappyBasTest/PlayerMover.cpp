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

#include "PlayerMover.h"
#include "Player.h"

#include "ControlsManager.h"
#include "IKeyboard.h"
#include "PhysicsCharacterController.h"
#include "PlayerThirdPersonCamera.h"

namespace ht {

#define JUMP_SPEED 30.0f
#define MOVE_SPEED 20.0f

PlayerMover::PlayerMover(Player* const player)
    : m_Player(player)
{
}


PlayerMover::~PlayerMover()
{
}

void PlayerMover::tick( float dTime )
{
    he::io::ControlsManager* const controls(CONTROLS);
    he::px::PhysicsCharacterController* const controller(getCharacterController());
    const he::io::IKeyboard* const keyboard(controls->getKeyboard());

    controls->getFocus(this);

    if (controls->hasFocus(this))
    {
        he::vec3 speed(he::vec3::zero);
        float jump(0.0f);
        if (controller->getStandsOnFloor() && keyboard->isKeyDown(he::io::Key_Space))
        {
            jump = JUMP_SPEED;
        }

        const he::vec3 forward(he::normalize(m_Player->getCamera()->getLook().x0z()));
        const he::vec3 right(he::normalize(he::cross(forward, he::vec3::up)));

        // Forward / backwards
        if (keyboard->isKeyDown(he::io::Key_Up) || keyboard->isKeyDown(he::io::Key_W) || keyboard->isKeyDown(he::io::Key_Z))
        {
            speed += forward * MOVE_SPEED;
        }
        if (keyboard->isKeyDown(he::io::Key_Down) || keyboard->isKeyDown(he::io::Key_S))
        {
            speed -= forward * MOVE_SPEED;
        }

        // Strafe
        if (keyboard->isKeyDown(he::io::Key_Left) || keyboard->isKeyDown(he::io::Key_A) || keyboard->isKeyDown(he::io::Key_Q))
        {
            speed -= right * MOVE_SPEED;
        }
        if (keyboard->isKeyDown(he::io::Key_Right) || keyboard->isKeyDown(he::io::Key_D))
        {
            speed += right * MOVE_SPEED;
        }

        if (jump > 0.0f)
        {
            controller->addSpeed(he::vec3::up * jump);
        }

        controller->setMoveSpeed(speed);
    }

    controls->returnFocus(this);

    he::ge::CharacterPhysicsComponent::tick(dTime);
}

} //end namespace