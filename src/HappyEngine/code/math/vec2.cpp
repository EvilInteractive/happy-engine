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
#include "HappyPCH.h" 

#include "vec2.h"

namespace he {

vec2::vec2(): x(0), y(0)
{
}
vec2::vec2(float x_, float y_): x(x_), y(y_)
{
}
vec2::vec2(physx::pubfnd3::PxVec2 vec) : x(vec.x), y(vec.y)
{
}
vec2::~vec2()
{
}

vec2::vec2(const vec2& other)
{
    x = other.x;
    y = other.y;
}
vec2& vec2::operator=(const vec2& other)
{
    x = other.x;
    y = other.y;
    return *this;
}

//>---------Operators--------------------->
vec2 vec2::operator-() const
{
    return vec2(-x, -y);
}

vec2 vec2::operator*(float a) const
{
    return vec2(x*a, y*a);
}
vec2 vec2::operator/(float a) const
{
    return vec2(x/a, y/a);
}

vec2 vec2::operator+(const vec2& v) const
{
    return vec2(x + v.x, y + v.y);
}
vec2 vec2::operator-(const vec2& v) const
{
    return vec2(x - v.x, y - v.y);
}

vec2& vec2::operator+=(const vec2& v)
{
    x += v.x;
    y += v.y;
    return *this;
}
vec2& vec2::operator-=(const vec2& v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}
vec2& vec2::operator*=(float a)
{
    x *= a;
    y *= a;
    return *this;
}
vec2& vec2::operator/=(float a)
{
    x /= a;
    y /= a;
    return *this;
}

vec2& vec2::operator*=(const vec2& v)
{
	x *= v.x;
    y *= v.y;
    return *this;
}

vec2& vec2::operator/=(const vec2& v)
{
	x /= v.x;
    y /= v.y;
    return *this;
}

bool vec2::operator==(const vec2& v) const
{
    return x == v.x && y == v.y;
}
bool vec2::operator!=(const vec2& v) const
{
    return x != v.x || y != v.y;
}
//<----------------------------------------<

} //end namespace