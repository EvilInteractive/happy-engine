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
#include "HappyPCH.h" 

#include "vec3.h"
#include "foundation/PxVec3.h"
#include "MathFunctions.h"

namespace he {

const vec3 vec3::up(0, 1, 0);
const vec3 vec3::forward(0, 0, 1);
const vec3 vec3::right(1, 0, 0);
const vec3 vec3::one(1, 1, 1);
const vec3 vec3::zero(0, 0, 0);

vec3::vec3(): x(0), y(0), z(0)
{
}
vec3::vec3(float x_, float y_, float z_): x(x_), y(y_), z(z_)
{
}
vec3::vec3(const physx::PxVec3& vec) : x(vec.x), y(vec.y), z(vec.z)
{
}
vec3::~vec3()
{
}

vec3::vec3(const vec3& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}
vec3& vec3::operator=(const vec3& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

//>---------Operators--------------------->
vec3 vec3::operator-() const
{
    return vec3(-x, -y, -z);
}

vec3 vec3::operator*(float a) const
{
    return vec3(x*a, y*a, z*a);
}
vec3 vec3::operator/(float a) const
{
    return vec3(x/a, y/a, z/a);
}

vec3 vec3::operator+(const vec3& v) const
{
    return vec3(x + v.x, y + v.y, z + v.z);
}
vec3 vec3::operator-(const vec3& v) const
{
    return vec3(x - v.x, y - v.y, z - v.z);
}

vec3& vec3::operator+=(const vec3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}
vec3& vec3::operator-=(const vec3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}
vec3& vec3::operator*=(float a)
{
    x *= a;
    y *= a;
    z *= a;
    return *this;
}
vec3& vec3::operator/=(float a)
{
    x /= a;
    y /= a;
    z /= a;
    return *this;
}
vec3& vec3::operator*=(const vec3& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}
vec3& vec3::operator/=(const vec3& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
}

bool vec3::operator==(const vec3& v) const
{
    return x == v.x && y == v.y && z == v.z;
}
bool vec3::operator!=(const vec3& v) const
{
    return x != v.x || y != v.y || z != v.z;
}

bool vec3::operator<(const vec3& v) const //FOR std::MAP, it doesn't make any sense else.
{
    if (x < v.x)
        return true;
    if (x == v.x)
    {
        if (y < v.y)
            return true;
        if (y == v.y)
        {
            if (z < v.z)
                return true;
        }
    }
    return false;
}

he::vec2 vec3::xz() const
{
    return vec2(x, z);
}

he::vec2 vec3::xy() const
{
    return vec2(x, y);
}

//<----------------------------------------<

} //end namespace
