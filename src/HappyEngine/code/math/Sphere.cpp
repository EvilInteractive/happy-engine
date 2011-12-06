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
//Author:  Bastian Damman
//Created: 28/09/2011
#include "HappyPCH.h" 

#include "Sphere.h"
#include "HappyNew.h"
#include "MathFunctions.h"
#include <algorithm>

namespace he {

namespace shapes {

Sphere::Sphere(): m_Position(0, 0, 0), m_Radius(0.0f)
{
}
Sphere::Sphere(const vec3& pos, float radius): m_Position(pos), m_Radius(radius)
{
}


Sphere::~Sphere()
{
}

const vec3& Sphere::getPosition() const
{
    return m_Position;
}
float Sphere::getRadius() const
{
    return m_Radius;
}

Sphere Sphere::getBoundingSphere(const void* pointCloud, uint num, uint stride, uint posOffset)
{
    vec3 min(FLT_MAX, FLT_MAX, FLT_MAX), 
         max(FLT_MIN, FLT_MIN, FLT_MIN);
    const char* charPointCloud = static_cast<const char*>(pointCloud);
    for(uint i = 0; i < num; ++i)
    {
        const vec3& p(*reinterpret_cast<const vec3*>(charPointCloud + stride * i + posOffset));
        min = he::minPerComponent(min, p);
        max = he::maxPerComponent(max, p);
    }
    vec3 center((min + max) / 2.0f);
    float radius(length(min - center));

    return Sphere(center, radius);
}

} }//end namespace