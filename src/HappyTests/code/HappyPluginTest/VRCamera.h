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
    void setMouseSensitivty(float sens = 100.0f);

private:
    bool m_Moveable;

    float m_Speed;
    float m_FastForward;
    float m_MouseSensitivity;

    he::vec2 m_PreviousMousePos;

    //Disable default copy constructor and default assignment operator
    VRCamera(const VRCamera&);
    VRCamera& operator=(const VRCamera&);
};

} //end namespace

#endif
