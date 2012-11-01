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
//Created: 19/05/2012
#include "HappyPCH.h" 

#include "Ray.h"

#include "ICamera.h"
#include "View.h"

namespace he {

Ray::Ray( const vec3& position, const vec3& direction, float maxDist /*= FLT_MAX*/ )
: m_Origin(position)
, m_Direction(direction)
, m_MaxDistance(maxDist)
{

}

Ray::Ray(const gfx::View* view, const gfx::ICamera* camera, const vec2& viewCoord, float maxDist /*= FLT_MAX*/ )
{
    vec2 ndc((viewCoord.x / view->getViewport().width) * 2.0f - 1.0f,
             (viewCoord.y / view->getViewport().height) * 2.0f - 1.0f);
    vec4 nearPoint(-ndc, 0.0f, 1.0f);
    vec4  farPoint(-ndc, 1.0f, 1.0f);

    mat44 unproject(camera->getViewProjection().inverse());

    nearPoint = unproject * nearPoint;
    farPoint = unproject * farPoint;

    m_Origin = nearPoint.xyz() / nearPoint.w;
    m_Direction = normalize(farPoint.xyz() / farPoint.w - m_Origin);
    m_MaxDistance = maxDist;
}


Ray::~Ray()
{
}

const vec3& Ray::getOrigin() const
{
    return m_Origin;
}

const vec3& Ray::getDirection() const
{
    return m_Direction;
}

float Ray::getMaxDistance() const
{
    return m_MaxDistance;
}

} //end namespace
