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
#include "Ray.h"

namespace he {

// a*Nx + b*Ny + c*Nz + d = 0
Plane::Plane( float a, float b, float c, float d ): m_Normal(a, b, c), m_Distance(-d)
{
    const float len(length(m_Normal));
    m_Normal /= len;
    m_Distance /= len;
}

Plane::Plane( const vec3& v1, const vec3& v2, const vec3& v3 )
    : m_Normal(0, 1, 0)
    , m_Distance(0.0f)
{
    const vec3 vec1(v2 - v1);
    const vec3 vec2(v3 - v1);
    m_Normal = normalize(cross(v2, v3));
    m_Distance = -dot(v1, m_Normal);
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

    uint32 insideCounter(0);
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

bool Plane::intersect( const Ray& ray, float& outDist ) const
{
    bool result(false);
    const float dotDirNorm(dot(ray.getDirection(), m_Normal));
    if (dotDirNorm < 0) // Plane is facing away from ray or parallel to ray
    {
        const float t(-(dot(ray.getOrigin(), m_Normal) + m_Distance) / dotDirNorm);
        if (t >= 0.0f) // Plane is not behind the ray
        {
            result = true;
            outDist = t;
        }
    }
    return result;
}

float Plane::getDistanceToPoint( const vec3& point ) const
{
    return dot(m_Normal, point) - m_Distance;
}

he::vec3 Plane::getIntersectionPoint( const Plane& p1, const Plane& p2, const Plane& p3 )
{
    vec3 crossN2N3(cross(p2.m_Normal, p3.m_Normal));
    vec3 crossN1N2(cross(p1.m_Normal, p2.m_Normal));
    vec3 crossN3N1(cross(p3.m_Normal, p1.m_Normal));
    vec3 outPoint(crossN1N2 * p3.m_Distance);
    outPoint += crossN3N1 * p2.m_Distance;
    outPoint += crossN2N3 * p1.m_Distance;
    outPoint /= dot(p1.m_Normal, crossN2N3);
    return outPoint;
}

} //end namespace
