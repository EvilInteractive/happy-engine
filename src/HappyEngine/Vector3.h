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

#ifndef _HE_VECTOR3_H_
#define _HE_VECTOR3_H_
#pragma once

#include "PxVec3.h"

namespace happyengine {
namespace math {

struct Vector3
{
public:
    float x, y, z;

    const static Vector3 up;
    const static Vector3 forward;
    const static Vector3 right;
    const static Vector3 one;
    const static Vector3 zero;

	Vector3();
    explicit Vector3(physx::pubfnd3::PxVec3 vec);
	Vector3(float x, float y, float z);
    ~Vector3();

    Vector3(const Vector3& other);
    Vector3& operator=(const Vector3& other);

    //>---------Operators--------------------->
	Vector3 operator-() const;

	Vector3 operator*(float a) const;
	Vector3 operator/(float a) const;

	Vector3 operator+(const Vector3& v) const;
	Vector3 operator-(const Vector3& v) const;

	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float a);
	Vector3& operator/=(float a);

	bool operator==(const Vector3& v) const;
	bool operator!=(const Vector3& v) const;
    //<----------------------------------------<
};

} } //end namespace

#endif
