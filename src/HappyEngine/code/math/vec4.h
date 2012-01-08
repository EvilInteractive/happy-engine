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
//Created: 10/08/2011

#ifndef _HE_VECTOR4_H_
#define _HE_VECTOR4_H_
#pragma once

#include "PxVec4.h"

namespace he {


struct vec3;

struct vec4
{
public:
    float x, y, z, w;

	vec4();
    explicit vec4(physx::pubfnd3::PxVec4 vec);
    vec4(const vec3& vec, float w);
	vec4(float x, float y, float z, float w);
    ~vec4();

    vec4(const vec4& other);
    vec4& operator=(const vec4& other);

    
    //>---------Getters----------------------->
    vec3 xyz() const;
    //<---------------------------------------<

    //>---------Operators--------------------->
	vec4 operator-() const;

	vec4 operator*(float a) const;
	vec4 operator/(float a) const;

	vec4 operator+(const vec4& v) const;
	vec4 operator-(const vec4& v) const;

	vec4& operator+=(const vec4& v);
	vec4& operator-=(const vec4& v);
	vec4& operator*=(float a);
	vec4& operator/=(float a);

	bool operator==(const vec4& v) const;
	bool operator!=(const vec4& v) const;
    //<---------------------------------------<
};

} //end namespace

#endif
