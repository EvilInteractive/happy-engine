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
//Created: 28/09/2011

#ifndef _HE_SPHERE_H_
#define _HE_SPHERE_H_
#pragma once

#include "Vector3.h"
#include <vector>
#include "HappyTypes.h"

namespace happyengine {
namespace math {
namespace shapes {

class Sphere
{
public:
    Sphere();
    Sphere(const Vector3& pos, float radius);
    virtual ~Sphere();
    //default copy and assignment are fine

    const Vector3& getPosition() const;
    float getRadius() const;

    static Sphere getBoundingSphere(const void* pointCloud, uint num, uint stride, uint posOffset);

private:

    Vector3 m_Position;
    float m_Radius;
};

} } } //end namespace

#endif
