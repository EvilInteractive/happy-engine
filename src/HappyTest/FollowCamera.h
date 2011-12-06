//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#ifndef _FOLLOW_CAMERA_H_
#define _FOLLOW_CAMERA_H_
#pragma once

#include "Camera.h"
#include "vec2.h"
#include <deque>

#include "Entity.h"

#include "boost/thread.hpp"

namespace happytest {

class FollowCamera : public he::gfx::Camera
{
public:
    // CONSTRUCTOR - DESTRUCTOR
    FollowCamera();
    virtual ~FollowCamera();

    // GENERAL
    virtual void tick(float dTime);

    // SETTERS
    void setFollowObject(const he::game::Entity* pEntity);
    void setDistance(float distance);
    void setLocalLook(const he::vec3& look);
    
private:

    void smoothThread();
    boost::thread m_SmoothThread;
    bool m_RunSmoothThread;
    std::deque<he::vec3> m_LookAtSmooth;
    std::deque<he::vec3> m_PositionSmooth;
    std::deque<he::vec3> m_UpSmooth;
    const static int MAX_SMOOTH = 50;

    he::vec3 m_Position, m_LookAt;
    he::vec3 m_Up;


    const he::game::Entity* m_pFollowObject;

    he::vec3 m_LocalLook;
    float m_Distance;


    //Disable default copy constructor and default assignment operator
    FollowCamera(const FollowCamera&);
    FollowCamera& operator=(const FollowCamera&);
};

} //end namespace

#endif
