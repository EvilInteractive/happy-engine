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

#include "Vector3.h"

namespace happyengine {
namespace math {

Vector3::Vector3(): x(0), y(0), z(0)
{
}
Vector3::Vector3(float x_, float y_, float z_): x(x_), y(y_), z(z_)
{
}
Vector3::~Vector3()
{
}

Vector3::Vector3(const Vector3& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}
Vector3& Vector3::operator=(const Vector3& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

} } //end namespace