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
//Author:  Bastian Damman
//Created: 28/09/2011
#include "HappyPCH.h" 

#include "Sphere.h"
#include "Ray.h"

namespace he {

Sphere::Sphere(): m_Position(0, 0, 0), m_Radius(0.0f)
{
}
Sphere::Sphere(const vec3& pos, float radius): m_Position(pos), m_Radius(radius)
{
}

bool Sphere::intersectTest(const Sphere& other) const
{
    vec3 axis(other.m_Position - m_Position);
    float intersectDist(m_Radius + other.m_Radius);

    if (lengthSqr(axis) < intersectDist * intersectDist)
        return true;

    return false;
}

bool Sphere::intersectTest( const Ray& ray ) const
{
    // http://www.cs.princeton.edu/courses/archive/fall00/cs426/lectures/raycast/sld013.htm
    bool result(false);
    const vec3 tocenter(m_Position - ray.getOrigin());
    const float toCenterLenSqr(lengthSqr(tocenter));
    const float lenToCenterPP(dot(tocenter, ray.getDirection()));
    const float radSqr(sqr(m_Radius));
    if (toCenterLenSqr <= radSqr) // inside sphere
    {
        result = true;
    }
    else if ( lenToCenterPP >= 0) // ray points to sphere
    {
        const float distToChordSqr(toCenterLenSqr - sqr(lenToCenterPP));
        if (distToChordSqr <= radSqr)
        {
            result = true;
        }
    }

    return result;
}

bool Sphere::isOtherInside(const Sphere& other) const
{
    vec3 axis(other.m_Position - m_Position);
    if (lengthSqr(axis) < sqr(m_Radius - other.getRadius()))
        return true;

    return false;
}
IntersectResult Sphere::intersect(const Sphere& other) const
{
    vec3 axis(other.m_Position - m_Position);

    float intersectDistSq(m_Radius + other.m_Radius);
    intersectDistSq *= intersectDistSq;

    float distSq(lengthSqr(axis));

    if (distSq < intersectDistSq)
    {
        if (distSq + sqr(other.m_Radius) < sqr(m_Radius))
        {
            return IntersectResult_Inside;
        }
        else
        {
            return IntersectResult_Intersecting;
        }
    }

    return IntersectResult_Outside;
}

Sphere Sphere::calculateBoundingSphere(const void* pointCloud, uint32 num, uint32 stride, uint32 posOffset)
{
    vec3 min(FLT_MAX, FLT_MAX, FLT_MAX), 
         max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    const char* charPointCloud = static_cast<const char*>(pointCloud);
    for(uint32 i = 0; i < num; ++i)
    {
        const vec3& p(*reinterpret_cast<const vec3*>(charPointCloud + stride * i + posOffset));
        he::minPerComponent(min, p, min);
        he::maxPerComponent(max, p, max);
    }
    vec3 center((min + max) / 2.0f);
    float radius(length(min - center));

    return Sphere(center, radius);
}

} //end namespace