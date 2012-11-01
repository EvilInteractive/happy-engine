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
vec3::vec3( float val ): x(val), y(val), z(val)
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
: x(other.x)
, y(other.y)
, z(other.z)
{
}

vec3::vec3( const vec2& p_xy, float p_z )
: x(p_xy.x)
, y(p_xy.y)
, z(p_z)
{
}

//>---------Operators--------------------->
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

void vec3::toPxVec3( physx::PxVec3* outVec ) const
{
    HE_ASSERT(sizeof(physx::PxVec3) == sizeof(vec3), "memcpy will fail, classes have different size");
    he_memcpy(outVec, this, sizeof(vec3));
}

//<----------------------------------------<

} //end namespace
