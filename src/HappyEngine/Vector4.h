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
//Created: 10/08/2011

#ifndef _VECTOR4_H_
#define _VECTOR4_H_
#pragma once

#include "PxVec4.h"

namespace happyengine {
namespace math {

struct Vector4
{
public:
    float x, y, z, w;

	Vector4();
    explicit Vector4(physx::pubfnd3::PxVec4 vec);
	Vector4(float x, float y, float z, float w);
    ~Vector4();

    Vector4(const Vector4& other);
    Vector4& operator=(const Vector4& other);

    //>---------Operators--------------------->
	Vector4 operator-() const;

	Vector4 operator*(float a) const;
	Vector4 operator/(float a) const;

	Vector4 operator+(const Vector4& v) const;
	Vector4 operator-(const Vector4& v) const;

	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(float a);
	Vector4& operator/=(float a);

	bool operator==(const Vector4& v) const;
	bool operator!=(const Vector4& v) const;
    //<----------------------------------------<
};

} } //end namespace

#endif
