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

#ifndef _FLY_CAMERA_H_
#define _FLY_CAMERA_H_
#pragma once

#include "Camera.h"

namespace hmv {

class FlyCamera : public he::gfx::Camera
{
public:
    // CONSTRUCTOR - DESTRUCTOR
    FlyCamera(int viewportWidth, int viewportHeight);
    virtual ~FlyCamera();

    // GENERAL
    virtual void tick(float dTime);

    // SETTERS
    void moveable(bool bMoveable);
    void setMouseSensitivty(float sens = 100.0f);

private:
    bool m_bMoveable;

    float m_Speed;
    float m_FastForward;
    float m_MouseSensitivity;

    he::vec2 m_PreviousMousePos;

    //Disable default copy constructor and default assignment operator
    FlyCamera(const FlyCamera&);
    FlyCamera& operator=(const FlyCamera&);
};

} //end namespace

#endif
