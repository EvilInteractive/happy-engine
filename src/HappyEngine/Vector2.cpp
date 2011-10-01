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

#include "Vector2.h"

namespace happyengine {
namespace math {

Vector2::Vector2(): x(0), y(0)
{
}
Vector2::Vector2(float x_, float y_): x(x_), y(y_)
{
}
Vector2::Vector2(physx::pubfnd3::PxVec2 vec) : x(vec.x), y(vec.y)
{
}
Vector2::~Vector2()
{
}

Vector2::Vector2(const Vector2& other)
{
    x = other.x;
    y = other.y;
}
Vector2& Vector2::operator=(const Vector2& other)
{
    x = other.x;
    y = other.y;
    return *this;
}

//>---------Operators--------------------->
Vector2 Vector2::operator-() const
{
    return Vector2(-x, -y);
}

Vector2 Vector2::operator*(float a) const
{
    return Vector2(x*a, y*a);
}
Vector2 Vector2::operator/(float a) const
{
    return Vector2(x/a, y/a);
}

Vector2 Vector2::operator+(const Vector2& v) const
{
    return Vector2(x + v.x, y + v.y);
}
Vector2 Vector2::operator-(const Vector2& v) const
{
    return Vector2(x - v.x, y - v.y);
}

Vector2& Vector2::operator+=(const Vector2& v)
{
    x += v.x;
    y += v.y;
    return *this;
}
Vector2& Vector2::operator-=(const Vector2& v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}
Vector2& Vector2::operator*=(float a)
{
    x *= a;
    y *= a;
    return *this;
}
Vector2& Vector2::operator/=(float a)
{
    x /= a;
    y /= a;
    return *this;
}

Vector2& Vector2::operator*=(const Vector2& v)
{
	x *= v.x;
    y *= v.y;
    return *this;
}

Vector2& Vector2::operator/=(const Vector2& v)
{
	x /= v.x;
    y /= v.y;
    return *this;
}

bool Vector2::operator==(const Vector2& v) const
{
    return x == v.x && y == v.y;
}
bool Vector2::operator!=(const Vector2& v) const
{
    return x != v.x || y != v.y;
}
//<----------------------------------------<

} } //end namespace