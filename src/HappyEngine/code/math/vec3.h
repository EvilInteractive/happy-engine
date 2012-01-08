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

#ifndef _HE_VECTOR3_H_
#define _HE_VECTOR3_H_
#pragma once

#include "PxVec3.h"

namespace he {

struct vec3
{
public:
    float x, y, z;

    const static vec3 up;
    const static vec3 forward;
    const static vec3 right;
    const static vec3 one;
    const static vec3 zero;

    vec3();
    explicit vec3(const physx::pubfnd3::PxVec3& vec);
    vec3(float x, float y, float z);
    ~vec3();

    vec3(const vec3& other);
    vec3& operator=(const vec3& other);

    //>---------Operators--------------------->
    vec3 operator-() const;

    vec3 operator*(float a) const;
    vec3 operator/(float a) const;

    vec3 operator+(const vec3& v) const;
    vec3 operator-(const vec3& v) const;

    vec3& operator+=(const vec3& v);
    vec3& operator-=(const vec3& v);
    vec3& operator*=(float a);
    vec3& operator/=(float a);

    vec3& operator*=(const vec3& v); //component wise mult
    vec3& operator/=(const vec3& v);

    bool operator==(const vec3& v) const;
    bool operator!=(const vec3& v) const;

    bool operator<(const vec3& v) const; //FOR std::MAP doesn't make any sense else
    //<----------------------------------------<
};

} //end namespace

#endif
