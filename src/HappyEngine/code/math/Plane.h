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

#ifndef _HE_Plane_H_
#define _HE_Plane_H_
#pragma once

namespace he {
class AABB;

// a*Nx + b*Ny + c*Nz + d = 0
class Plane
{
public:
    Plane(): m_Normal(0, 1, 0), m_Distance(0) {}
    Plane(const vec3& normal, float dist): m_Normal(normal), m_Distance(dist) {}
    Plane(float a, float b, float c, float d);

    inline const vec3& getNormal() const { return m_Normal; }
    inline float getDistance() const { return m_Distance; }
    inline void setNormal(const vec3& normal) { m_Normal = normal; }
    inline void setDistance(float dist) { m_Distance = dist; }

    bool isPointInFrontOfPlane(const vec3& point) const;
    float getDistanceToPoint(const vec3& point) const;
    IntersectResult intersect(const AABB& box) const;

    static vec3 getIntersectionPoint(const Plane& p1, const Plane& p2, const Plane& p3);

private:
    vec3 m_Normal;
    float m_Distance;
};

} //end namespace

#endif
