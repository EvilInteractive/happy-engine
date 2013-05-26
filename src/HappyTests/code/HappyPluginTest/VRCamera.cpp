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
#include "HappyPluginTestPCH.h"

#include "VRCamera.h"

#include <MathFunctions.h>
#include <ControlsManager.h>
#include <GraphicsEngine.h>
#include <IKeyboard.h>
#include <IMouse.h>
#include <OculusRiftBinding.h>

namespace ht {

// CONSTRUCTOR - DESTRUCTOR
VRCamera::VRCamera()
:	m_Moveable(true),
    m_Speed(10.0f),
    m_FastForward(20.0f),
    m_PreviousMousePos(0,0),
    m_MouseSensitivity(100.0f)
{
}

VRCamera::~VRCamera()
{
}

// GENERAL
void VRCamera::tick(const float dTime)
{
    using namespace he;

    he::io::ControlsManager* const controls(CONTROLS);
    const he::io::IKeyboard* const keyboard(controls->getKeyboard());
    //const he::io::IMouse* const mouse(controls->getMouse());
    controls->getFocus(this);
    
    if (m_Moveable)
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

    he::io::OculusRiftDevice* const device(controls->getOculusRiftBinding()->getDevice(0));
    if (device)
    {
        const vec3 pitchYawRoll(device->getPitchYawRoll());

        const mat44 pitch(mat44::createRotation(he::vec3::right, -pitchYawRoll.x));
        const mat44 yaw(mat44::createRotation(he::vec3::up, pitchYawRoll.y));
        const mat44 roll(mat44::createRotation(he::vec3::forward, -pitchYawRoll.z));
        const mat44 transf(yaw * pitch * roll);

        m_LookWorld = transf * he::vec3::forward;
        m_UpWorld = transf * he::vec3::up;
        m_RightWorld = transf * he::vec3::right;
    }

    controls->returnFocus(this);

    m_RegenViewMatrix = true;
}


// SETTERS
void VRCamera::moveable(bool moveable)
{
    m_Moveable = moveable;
}

void VRCamera::setMouseSensitivty(float sens)
{
    m_MouseSensitivity = sens;
}

} //end namespace