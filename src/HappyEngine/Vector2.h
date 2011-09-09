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

#ifndef _HE_VECTOR2_H_
#define _HE_VECTOR2_H_
#pragma once

#include "PxVec2.h"

namespace happyengine {
namespace math {

struct Vector2
{
public:
    float x, y;

	Vector2();
    explicit Vector2(physx::pubfnd3::PxVec2 vec);
	Vector2(float x, float y);
    ~Vector2();

    Vector2(const Vector2& other);
    Vector2& operator=(const Vector2& other);

    //>---------Operators--------------------->
	Vector2 operator-() const;

	Vector2 operator*(float a) const;
	Vector2 operator/(float a) const;

	Vector2 operator+(const Vector2& v) const;
	Vector2 operator-(const Vector2& v) const;

	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(float a);
	Vector2& operator/=(float a);

	bool operator==(const Vector2& v) const;
	bool operator!=(const Vector2& v) const;
    //<----------------------------------------<
};

} } //end namespace

#endif
