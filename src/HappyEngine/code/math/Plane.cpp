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
//Created: 19/07/2012
#include "HappyPCH.h" 

#include "Plane.h"
#include "AABB.h"

namespace he {

// a*Nx + b*Ny + c*Nz + d = 0
Plane::Plane( float a, float b, float c, float d ): m_Normal(a, b, c), m_Distance(d)
{
    float len(length(m_Normal));
    m_Normal /= len;
    m_Distance /= len;
}

bool Plane::isPointInFrontOfPlane( const vec3& point ) const
{
    float dist(getDistanceToPoint(point));
    return dist > 0.0f;
}

he::IntersectResult Plane::intersect( const AABB& box ) const
{
    const vec3& tfl(box.getTopFrontLeft());
    vec3 dim(box.getBottomBackRight() - tfl);
    vec3 checkPoint(tfl);

    uint insideCounter(0);
    if (isPointInFrontOfPlane(checkPoint))
        ++insideCounter;

    checkPoint.x += dim.x;
    if (isPointInFrontOfPlane(checkPoint))
        ++insideCounter;

    checkPoint.y += dim.y;
    if (isPointInFrontOfPlane(checkPoint))
        ++insideCounter;

    checkPoint.z += dim.z;
    if (isPointInFrontOfPlane(checkPoint))
        ++insideCounter;

    checkPoint.y = tfl.y;
    if (isPointInFrontOfPlane(checkPoint))
        ++insideCounter;

    checkPoint.x = tfl.x;
    if (isPointInFrontOfPlane(checkPoint))
        ++insideCounter;

    checkPoint.y += dim.y;
    if (isPointInFrontOfPlane(checkPoint))
        ++insideCounter;

    checkPoint.z += tfl.z;
    if (isPointInFrontOfPlane(checkPoint))
        ++insideCounter;

    if (insideCounter == 0)
        return IntersectResult_Outside;
    else if (insideCounter == 8)
        return IntersectResult_Inside;
    else 
        return IntersectResult_Intersecting;
}

float Plane::getDistanceToPoint( const vec3& point ) const
{
    return dot(m_Normal, point) + m_Distance;
}

} //end namespace
