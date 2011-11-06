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
//Created: 20/08/2011

#ifndef _HE_IPHYSICS_SHAPE_H_
#define _HE_IPHYSICS_SHAPE_H_
#pragma once

#include "HeAssert.h"
#undef assert
#define assert ASSERT

#include "PxShape.h"
#include "boost/shared_ptr.hpp"

namespace he {
namespace px {

enum PhysicsShapeType
{
    PhysicsShapeType_Box,
    PhysicsShapeType_Convex,
    PhysicsShapeType_Sphere,
    PhysicsShapeType_Capsule,

    //Only for static actors
    PhysicsShapeType_TriangleMesh,
    PhysicsShapeType_Plane,
    PhysicsShapeType_HeightField
};

class IPhysicsShape
{
public:
    virtual ~IPhysicsShape() {}

    virtual PhysicsShapeType getType() const = 0;
};

} } //end namespace

#endif
