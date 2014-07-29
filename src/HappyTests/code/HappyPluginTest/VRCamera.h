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

#ifndef _VRCamera_H_
#define _VRCamera_H_
#pragma once

#include "CameraPerspective.h"

namespace ht {

class VRCamera : public he::gfx::CameraPerspective
{
public:
    // CONSTRUCTOR - DESTRUCTOR
    VRCamera();
    virtual ~VRCamera();

    // GENERAL
    virtual void tick(const float dTime);
    
    // SETTERS
    void moveable(bool moveable);

    const he::vec3& getVelocityVector() { return m_VelocityVector; }
    float getSpeed() const { return m_Speed; }
    float getThrottle() const { return m_Throttle; }

private:
    bool m_Moveable;

    he::vec3 m_PrevDevicePitchYawRoll;
    he::vec3 m_PitchYawRoll;
    he::vec3 m_VelocityVector;
    float m_Speed;
    float m_Throttle;

    //Disable default copy constructor and default assignment operator
    VRCamera(const VRCamera&);
    VRCamera& operator=(const VRCamera&);
};

} //end namespace

#endif
