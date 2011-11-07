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

#ifndef _HE_MATHFUNCTIONS_H_
#define _HE_MATHFUNCTIONS_H_
#pragma once

#include "vec3.h"
#include "vec2.h"
#include "vec4.h"
#include "PxMat44.h"
#include "mat44.h"
#include "MathConstants.h"

namespace he {

//template
template<typename T>
inline T min(T a, T b)
{
    return a <= b? a : b;
}
template<typename T>
inline T max(T a, T b)
{
    return a >= b? a : b;
}
template<typename T>
inline T clamp(T p_value, T p_min, T p_max)
{
    return max(p_min, min(p_max, p_value));
}

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

//template
//linear interpolation, t[0, 1]
//T must support T+T, T-T, T*float,
template<typename T>
inline T lerp(const T& p0, const T& p1, float t)
{
    return (p1 - p0) * t + p0;
}

// Float
inline float abs(float a)
{
	return ::fabs(a);
}
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
inline float length(float a)
{
    return ::abs(a);
}

// vec2
inline vec2 abs(const vec2& vector)
{
	return vec2(::abs(vector.x), ::abs(vector.y));
}
inline float lengthSqr(const vec2& vector)
{
    return sqr(vector.x) + sqr(vector.y);
}
inline float length(const vec2& vector)
{
    return sqrtf(lengthSqr(vector));
}
inline vec2 normalize(const vec2& vector)
{
	if (vector != vec2(0, 0))
		return vector / length(vector);
	else
		return vector;
}
inline float dot(const vec2& vector1, const vec2& vector2)
{
    return vector1.x * vector2.x + vector1.y * vector2.y;
}
inline const vec2& min(const vec2& a, const vec2& b)
{
    return lengthSqr(a) <= lengthSqr(b)? a : b;
}
inline const vec2& max(const vec2& a, const vec2& b)
{
    return lengthSqr(a) >= lengthSqr(b)? a : b;
}

// vec3
inline vec3 abs(const vec3& vector)
{
	return vec3(::abs(vector.x), ::abs(vector.y), ::abs(vector.z));
}
inline float lengthSqr(const vec3& vector)
{
    return sqr(vector.x) + sqr(vector.y) + sqr(vector.z);
}
inline float length(const vec3& vector)
{
    return sqrtf(lengthSqr(vector));
}
inline vec3 normalize(const vec3& vector)
{
	if (vector != vec3(0,0,0))
		return vector / length(vector);
	else
		return vector;
}
inline float dot(const vec3& vector1, const vec3& vector2)
{
    return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}
inline vec3 cross(const vec3& vector1, const vec3& vector2)
{
    return vec3(
        vector1.y * vector2.z - vector1.z * vector2.y,
        vector1.z * vector2.x - vector1.x * vector2.z,
        vector1.x * vector2.y - vector1.y * vector2.x);
}
inline const vec3& min(const vec3& a, const vec3& b)
{
    return lengthSqr(a) <= lengthSqr(b)? a : b;
}
inline const vec3& max(const vec3& a, const vec3& b)
{
    return lengthSqr(a) >= lengthSqr(b)? a : b;
}
inline vec3 minPerComponent(const vec3& a, const vec3& b)
{
    vec3 minP;
    minP.x = min<float>(a.x, b.x);
    minP.y = min<float>(a.y, b.y);
    minP.z = min<float>(a.z, b.z);

    return minP;
}
inline vec3 maxPerComponent(const vec3& a, const vec3& b)
{
    vec3 maxP;
    maxP.x = max<float>(a.x, b.x);
    maxP.y = max<float>(a.y, b.y);
    maxP.z = max<float>(a.z, b.z);

    return maxP;
}

//vec4
inline float lengthSqr(const vec4& vector)
{
    return sqr(vector.x) + sqr(vector.y) + sqr(vector.z) + sqr(vector.w);
}
inline float length(const vec4& vector)
{
    return sqrtf(lengthSqr(vector));
}
inline vec4 normalize(const vec4& vector)
{
    if (vector != vec4(0,0,0,0))
        return vector / length(vector);
    else
        return vector;
}


} //end namespace

#endif