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
#include "HappyBasTestPCH.h"

#include "FlyCamera.h"
#include "MathFunctions.h"
#include "ControlsManager.h"
#include "GraphicsEngine.h"
#include "IKeyboard.h"
#include "IMouse.h"

namespace ht {

// CONSTRUCTOR - DESTRUCTOR
FlyCamera::FlyCamera(int viewportWidth, int viewportHeight) :	CameraPerspective(viewportWidth, viewportHeight),
                                                                m_bMoveable(true),
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

    CONTROLS->getFocus(this);
    
    if (m_bMoveable)
    {
        bool bRunning = false;

        // camera controls
        vec3 dir(0.0f, 0.0f, 0.0f);

        if (CONTROLS->hasFocus(this))
        {
            if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Z))
                dir += m_LookWorld;
            if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Q))
                dir -= m_RightWorld;

            if (CONTROLS->getKeyboard()->isKeyDown(io::Key_S))
                dir -= m_LookWorld;
            if (CONTROLS->getKeyboard()->isKeyDown(io::Key_D))
                dir += m_RightWorld;

            // fast forward
            if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Lshift))
                bRunning = true;
        }

        dir = normalize(dir);

        float finalSpeed = m_Speed;
        if (bRunning) finalSpeed *= m_FastForward;

        m_PosWorld += dir * finalSpeed * dTime;
    }

    /*float angle = static_cast<float>((CONTROLS->getMouse()-> / 120) / 200.0f);

    if (angle != 0)
    {
        m_Speed += angle*200;

        if (m_Speed < 2)
            m_Speed = 2;
    }*/

    if (CONTROLS->getMouse()->isButtonDown(io::MouseButton_Right) && CONTROLS->hasFocus(this))
    {
        vec2 mouseMovement = CONTROLS->getMouse()->getPosition() - m_PreviousMousePos;
        m_PreviousMousePos = CONTROLS->getMouse()->getPosition();
        float pitch = mouseMovement.y / m_MouseSensitivity;
        float yAngle = mouseMovement.x / m_MouseSensitivity;

        mat44 R(mat44::createRotation(m_RightWorld, -pitch));
        m_LookWorld = normalize(R * m_LookWorld);
        m_UpWorld = normalize(R * m_UpWorld);

        R = mat44::createRotation(vec3(0,1,0), -yAngle);
        m_LookWorld = normalize(R * m_LookWorld);
        m_RightWorld = normalize(R * m_RightWorld);
        m_UpWorld = -normalize(cross(m_LookWorld, m_RightWorld));
    }
    else
        m_PreviousMousePos = CONTROLS->getMouse()->getPosition();

    CONTROLS->returnFocus(this);

    m_AspectRatio = GRAPHICS->getScreenHeight() / (float)GRAPHICS->getScreenWidth();

    buildProjectionMatrix();
    buildViewMatrix();
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