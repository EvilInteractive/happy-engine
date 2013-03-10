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

#ifndef _HE_VECTOR2_H_
#define _HE_VECTOR2_H_
#pragma once

namespace physx {
    class PxVec2;
}

namespace he {

struct HAPPY_ENTRY vec2
{
public:
    float x, y;

    static const vec2 zero;
    static const vec2 one;
    static const vec2 unitX;
    static const vec2 unitY;

    vec2();
    explicit vec2(const physx::PxVec2& vec);
    vec2(float x, float y);
    explicit vec2(const float v);
    ~vec2();

    vec2(const vec2& other);
    vec2& operator=(const vec2& other);

    //>---------Operators--------------------->
    vec2 operator-() const;

    vec2 operator*(float a) const;
    vec2 operator/(float a) const;
    vec2 operator+(const vec2& v) const;
    vec2 operator-(const vec2& v) const;
    vec2 operator*(const vec2& v) const;
    vec2 operator/(const vec2& v) const;

    vec2& operator+=(const vec2& v);
    vec2& operator-=(const vec2& v);
    vec2& operator*=(float a);
    vec2& operator/=(float a);
    vec2& operator*=(const vec2& v);
    vec2& operator/=(const vec2& v);

    bool operator==(const vec2& v) const;
    bool operator!=(const vec2& v) const;
    //<----------------------------------------<
};

} //end namespace

#endif
