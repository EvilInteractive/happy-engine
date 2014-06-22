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
#include "HappySandBoxPCH.h"

#include <qopenglfunctions.h>
#include "FlyCamera.h"
#include "MathFunctions.h"
#include "ControlsManager.h"
#include "GraphicsEngine.h"
#include "IKeyboard.h"
#include "IMouse.h"

namespace hs {

// CONSTRUCTOR - DESTRUCTOR
FlyCamera::FlyCamera() :	m_bMoveable(true),
                            m_Speed(10.0f),
                            m_FastForward(20.0f),
                            m_PreviousMousePos(0,0),
                            m_MouseSensitivity(100.0f)
{
}

FlyCamera::~FlyCamera()
{
}

// GENERAL
void FlyCamera::tick(float dTime)
{
    using namespace he;

    he::io::ControlsManager* const controls(CONTROLS);
    const he::io::IKeyboard* const keyboard(controls->getKeyboard());
    const he::io::IMouse* const mouse(controls->getMouse());
    controls->getFocus(this);

    if (m_bMoveable)
    {
        bool bRunning = false;

        // camera controls
        vec3 dir(0.0f, 0.0f, 0.0f);

        if (controls->hasFocus(this))
        {
            if (keyboard->isKeyDown(io::Key_Z) || keyboard->isKeyDown(io::Key_W))
                dir += m_LookWorld;
            if (keyboard->isKeyDown(io::Key_Q) || keyboard->isKeyDown(io::Key_A))
                dir -= m_RightWorld;

            if (keyboard->isKeyDown(io::Key_S))
                dir -= m_LookWorld;
            if (keyboard->isKeyDown(io::Key_D))
                dir += m_RightWorld;

            // fast forward
            if (keyboard->isKeyDown(io::Key_Lshift))
                bRunning = true;
        }

        dir = normalize(dir);

        float finalSpeed = m_Speed;
        if (bRunning) finalSpeed *= m_FastForward;

        m_PosWorld += dir * finalSpeed * dTime;
    }

    if (mouse->isButtonDown(io::MouseButton_Right) && controls->hasFocus(this))
    {
        vec2 mouseMovement = mouse->getPosition() - m_PreviousMousePos;
        m_PreviousMousePos = mouse->getPosition();
        float pitch = mouseMovement.y / m_MouseSensitivity;
        float yAngle = mouseMovement.x / m_MouseSensitivity;

        mat44 R(mat44::createRotation(m_RightWorld, -pitch));
        m_LookWorld = normalize(R * m_LookWorld);
        m_UpWorld = normalize(R * m_UpWorld);

        R = mat44::createRotation(vec3(0, 1, 0), -yAngle);
        m_LookWorld = normalize(R * m_LookWorld);
        m_RightWorld = normalize(R * m_RightWorld);
        m_UpWorld = -normalize(cross(m_LookWorld, m_RightWorld));
    }
    else
    {
        m_PreviousMousePos = mouse->getPosition();
    }

    controls->returnFocus(this);

    m_RegenViewMatrix = true;
}


// SETTERS
void FlyCamera::moveable(bool bMoveable)
{
    m_bMoveable = bMoveable;
}

void FlyCamera::setMouseSensitivty(float sens)
{
    m_MouseSensitivity = sens;
}

} //end namespace
