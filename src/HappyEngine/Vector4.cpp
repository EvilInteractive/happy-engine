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

#include "Vector4.h"

namespace happyengine {
namespace math {

Vector4::Vector4(): x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}
Vector4::Vector4(float x_, float y_, float z_, float w_): x(x_), y(y_), z(z_), w(w_)
{
}
Vector4::Vector4(physx::pubfnd3::PxVec4 vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w)
{
}
Vector4::~Vector4()
{
}

Vector4::Vector4(const Vector4& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
}
Vector4& Vector4::operator=(const Vector4& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
    return *this;
}

//>---------Operators--------------------->
Vector4 Vector4::operator-() const
{
    return Vector4(-x, -y, -z, -w);
}

Vector4 Vector4::operator*(float a) const
{
    return Vector4(x*a, y*a, z*a, w*a);
}
Vector4 Vector4::operator/(float a) const
{
    return Vector4(x/a, y/a, z/a, w/a);
}

Vector4 Vector4::operator+(const Vector4& v) const
{
    return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}
Vector4 Vector4::operator-(const Vector4& v) const
{
    return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4& Vector4::operator+=(const Vector4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}
Vector4& Vector4::operator-=(const Vector4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}
Vector4& Vector4::operator*=(float a)
{
    x *= a;
    y *= a;
    z *= a;
    w *= a;
    return *this;
}
Vector4& Vector4::operator/=(float a)
{
    x /= a;
    y /= a;
    z /= a;
    w /= a;
    return *this;
}

bool Vector4::operator==(const Vector4& v) const
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}
bool Vector4::operator!=(const Vector4& v) const
{
    return x != v.x || y != v.y || z != v.z || w != v.w;
}
//<----------------------------------------<

} } //end namespace