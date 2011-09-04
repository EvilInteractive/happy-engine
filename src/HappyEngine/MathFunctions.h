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

#ifndef _MATHFUNCTIONS_H_
#define _MATHFUNCTIONS_H_
#pragma once

#include "Vector3.h"
#include "PxMat44.h"
#include "MathConstants.h"

namespace happyengine {
namespace math {
   
// Float
inline float sqr(float a)
{
    return a * a;
}
inline float toRadians(float degrees)
{
    return degrees / 180.0f * pi;
}
inline float toDegrees(float radians)
{
    return radians * 180.0f / pi;
}

// Vector3
inline float lengthSqr(const Vector3& vector)
{
    return sqr(vector.x) + sqr(vector.y) + sqr(vector.z);
}
inline float length(const Vector3& vector)
{
    return sqrtf(lengthSqr(vector));
}
inline Vector3 normalize(const Vector3& vector)
{
    return vector / length(vector);
}
inline float dot(const Vector3& vector1, const Vector3& vector2)
{
    return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}
inline Vector3 cross(const Vector3& vector1, const Vector3& vector2)
{
    return Vector3(
        vector1.y * vector2.z - vector1.z * vector2.y,
        vector1.z * vector2.x - vector1.x * vector2.z,
        vector1.x * vector2.y - vector1.y * vector2.x);
}

//template
//interpolation passes through every point, returns value between p1 and p2, t[0, 1]
//T must support -T, T*T, T*float, T+T
template<typename T>
inline T catmullrom(const T& p0, const T& p1, const T& p2, const T& p3, float t)
{
    return ((p1 * 2) +
            (-p0 + p2) * t + 
            (p0 * 2 - p1 * 5 + p2 * 4 - p3) * sqr(t) +
            (-p0 + p1 * 3 - p2 * 3 + p3) * pow(t, 3.0f)) * 0.5f;
}

} } //end namespace

#endif
