//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

namespace physx {
    class PxVec3;
}

namespace he {
struct vec2;

struct HAPPY_ENTRY vec3
{
public:
    float x, y, z;

    const static vec3 up;
    const static vec3 forward;
    const static vec3 right;
    const static vec3 one;
    const static vec3 zero;

    vec3();
    explicit vec3(const physx::PxVec3& vec);
    explicit vec3(float val);
    vec3(float x, float y, float z);
    vec3(const vec2& xy, float z);
    ~vec3();

    vec3(const vec3& other);
    inline vec3& operator=(const vec3& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    //>---------Getters----------------------->
    inline vec3 x0z() const { return vec3(x, 0, z); }
    inline vec2 xz() const { return vec2(x, z); }
    inline vec2 xy() const { return vec2(x, y); }
    void toPxVec3(physx::PxVec3* outVec) const;
    //<---------------------------------------<

    //>---------Operators--------------------->
    inline vec3 operator-() const
    {
        return vec3(-x, -y, -z);
    }

    inline vec3 operator*(const float a) const
    {
        return vec3(x * a, y * a, z * a);
    }
    inline vec3 operator/(const float a) const
    {
        return vec3(x / a, y / a, z / a);
    }

    inline vec3 operator+(const vec3& v) const
    {
        return vec3(x + v.x, y + v.y, z + v.z);
    }
    inline vec3 operator-(const vec3& v) const
    {
        return vec3(x - v.x, y - v.y, z - v.z);
    }

    inline vec3& operator+=(const vec3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    inline vec3& operator-=(const vec3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    inline vec3& operator*=(const float a)
    {
        x *= a;
        y *= a;
        z *= a;
        return *this;
    }
    inline vec3& operator/=(const float a)
    {
        x /= a;
        y /= a;
        z /= a;
        return *this;
    }

    inline vec3& operator*=(const vec3& v) //component wise mult
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    inline vec3& operator/=(const vec3& v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    inline bool operator==(const vec3& v) const
    {
        return x == v.x && y == v.y && z == v.z;
    }
    inline bool operator!=(const vec3& v) const
    {
        return x != v.x || y != v.y || z != v.z;
    }

    bool operator<(const vec3& v) const; //for he::Map/sorting purposes, it doesn't make any sense else, compares first x than y than z
    //<----------------------------------------<
};

} //end namespace

#endif
