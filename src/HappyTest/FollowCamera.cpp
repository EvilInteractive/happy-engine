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

#include "FollowCamera.h"
#include "HappyNew.h"
#include "MathFunctions.h"
#include "HappyEngine.h"
#include "GraphicsEngine.h"


namespace happytest {

// CONSTRUCTOR - DESTRUCTOR
#pragma warning(disable:4355) //this used in base member init list
FollowCamera::FollowCamera() :	Camera(16, 9), m_RunSmoothThread(true), m_Up(0, 1, 0), m_SmoothThread(boost::bind(&FollowCamera::smoothThread, this)), m_pFollowObject(nullptr)
{
}
#pragma warning(default:4355) //this used in base member init list

FollowCamera::~FollowCamera()
{
    m_RunSmoothThread = false;
    m_SmoothThread.join();
}

// GENERAL
void FollowCamera::tick(const float dTime)
{
    using namespace he;


    lookAt(m_Position, m_LookAt, m_Up);

    m_AspectRatio = GRAPHICS->getScreenHeight() / (float)GRAPHICS->getScreenWidth();
    buildProjectionMatrix();
    buildViewMatrix();
}

// SETTERS

void FollowCamera::setFollowObject( const he::game::Entity* pEntity )
{
    m_pFollowObject = pEntity;
}

void FollowCamera::setDistance( float distance )
{
    m_Distance = distance;
}

void FollowCamera::setLocalLook( const he::vec3& look )
{
    m_LocalLook = he::normalize(look);
}

void FollowCamera::smoothThread()
{
    using namespace he;
    while (m_RunSmoothThread)
    {
        if (m_pFollowObject != nullptr)
        {
            vec3 lookAt(m_pFollowObject->getWorldMatrix().getTranslation());
            vec3 position(m_pFollowObject->getWorldMatrix() * (m_LocalLook * m_Distance));
            //position.y = lookAt.y + m_LocalLook.y * m_Distance;
            //lookAt += normalize(vec3(lookAt.x - position.x, 0, lookAt.z - position.z)) * 5;

            vec3 dir(normalize(lookAt - position));

            vec3 up(m_pFollowObject->getWorldMatrix().getPhyicsMatrix().column1.getXYZ());
            //vec3 up(vec3::up);
            //if (dot(up, dir) > 0.99f)
            //    up = vec3::forward;
            ////vec3 right(normalize(cross(dir, up)));
            //vec3 right(m_pFollowObject->getWorldMatrix().getPhyicsMatrix().column0.getXYZ());
            //up = normalize(cross(dir, right));

            //LOOKAT
            if (m_LookAtSmooth.size() >= MAX_SMOOTH/5)
                m_LookAtSmooth.pop_front();
            m_LookAtSmooth.push_back(lookAt);
            
            lookAt = vec3::zero;
            std::for_each(m_LookAtSmooth.cbegin(), m_LookAtSmooth.cend(), [&lookAt](const vec3& la)
            {
                lookAt += la;
            });
            lookAt /= static_cast<float>(m_LookAtSmooth.size());
            
            //POSITION
            if (m_PositionSmooth.size() >= MAX_SMOOTH)
                m_PositionSmooth.pop_front();
            m_PositionSmooth.push_back(position);
            
            position = vec3::zero;
            std::for_each(m_PositionSmooth.cbegin(), m_PositionSmooth.cend(), [&position](const vec3& pos)
            {
                position += pos;
            });
            position /= static_cast<float>(m_PositionSmooth.size());

            //UP
            if (m_UpSmooth.size() >= MAX_SMOOTH)
                m_UpSmooth.pop_front();
            m_UpSmooth.push_back(up);

            up = vec3::zero;
            std::for_each(m_UpSmooth.cbegin(), m_UpSmooth.cend(), [&up](const vec3& u)
            {
                up += u;
            });
            up /= static_cast<float>(m_UpSmooth.size());

            m_LookAt = lookAt;
            m_Position = position;
            m_Up = up;

            
        }
        boost::this_thread::sleep(boost::posix_time::millisec(12)); //~60fps
    }
}

} //end namespace