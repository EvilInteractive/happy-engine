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
#include "HappyPluginTestPCH.h"

#include "VRCamera.h"

#include <MathFunctions.h>
#include <ControlsManager.h>
#include <GraphicsEngine.h>
#include <IKeyboard.h>
#include <IMouse.h>
#include <OculusRiftBinding.h>

namespace 
{
    const float g_ThrottleMax = 5.0f;
    const float g_SpeedMax = 12.0f;
    const float g_ThrottleSensitivity = 5.0f;
    const float g_Acceleration = 2.5f;
    const float g_AirFriction = 0.5f;
    const float g_Gravity = 9.81f;
    const float g_AirBreakFriction = 10.0f;
}

namespace ht {

// CONSTRUCTOR - DESTRUCTOR
VRCamera::VRCamera()
: m_Moveable(true)
, m_Speed(0.0f)
, m_Throttle(0.0f)
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
    
    he::io::OculusRiftDevice* const device(controls->getOculusRiftBinding()->getDevice(0));
    if (m_Moveable && device)
    {
        const vec3 newPitchYawRoll(device->getPitchYawRoll());
        const vec3 diff(newPitchYawRoll - m_PrevDevicePitchYawRoll);
        m_PrevDevicePitchYawRoll = newPitchYawRoll;

        const mat44 velocityPitch(mat44::createRotation(he::vec3::right, -newPitchYawRoll.x));
        const mat44 velocityRoll(mat44::createRotation(he::vec3::forward, -newPitchYawRoll.z));
        const vec3 forceVector( velocityRoll * velocityPitch * he::vec3::forward );

        m_PitchYawRoll.x = newPitchYawRoll.x;
        m_PitchYawRoll.y += diff.y;
        m_PitchYawRoll.z = newPitchYawRoll.z;
        
        if (keyboard->isKeyDown(io::Key_Z) || keyboard->isKeyDown(io::Key_W))
        {
            m_Throttle = he::min(g_ThrottleMax, m_Throttle + g_ThrottleSensitivity * dTime);
        }
        if (keyboard->isKeyDown(io::Key_S))
        {
            m_Throttle = he::max(0.0f, m_Throttle - g_ThrottleSensitivity * dTime);
        }

        if (m_Speed < m_Throttle)
        {
            m_Speed += g_Acceleration * dTime;
        }
        else
        {
            m_Speed -= g_AirFriction * dTime;
        }

        if (m_PitchYawRoll.x > 0.0f) // Rising
        {
            if (m_Speed > m_Throttle)
            {
                m_Speed -= g_AirFriction * dTime * m_PitchYawRoll.x / he::piOverTwo;
            }
        }
        else
        {
            if (m_Speed < g_SpeedMax)
            {
                m_Speed += g_Gravity * dTime * -m_PitchYawRoll.x / he::piOverTwo;
            }
        }

        if (keyboard->isKeyDown(io::Key_Space))
        {
            m_Speed = he::max(0.0f, m_Speed - g_AirBreakFriction * dTime);
        }

        m_PitchYawRoll.y += newPitchYawRoll.z * m_Speed * 0.0002f;
        
        const mat44 pitch(mat44::createRotation(he::vec3::right, -m_PitchYawRoll.x));
        const mat44 yaw(mat44::createRotation(he::vec3::up, m_PitchYawRoll.y));
        const mat44 roll(mat44::createRotation(he::vec3::forward, -m_PitchYawRoll.z));
        const mat44 transf(yaw * pitch * roll);

        m_VelocityVector = transf * he::vec3::forward;
        m_PosWorld += m_VelocityVector * m_Speed * dTime;

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

} //end namespace