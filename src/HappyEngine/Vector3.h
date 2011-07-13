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

#ifndef _VECTOR3_H_
#define _VECTOR3_H_
#pragma once

namespace happyengine {
namespace math {

struct Vector3
{
public:
    float x, y, z;

	Vector3();
	Vector3(float x_, float y_, float z_);
    ~Vector3();

    Vector3(const Vector3& other);
    Vector3& operator=(const Vector3& other);
};

} } //end namespace

#endif
