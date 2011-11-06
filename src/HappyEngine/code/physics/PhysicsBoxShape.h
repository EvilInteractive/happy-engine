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

#ifndef _HE_PHYSICS_BOX_SHAPE_H_
#define _HE_PHYSICS_BOX_SHAPE_H_
#pragma once

#include "IPhysicsShape.h"
#include "geometry/PxBoxGeometry.h"
#include "vec3.h"

namespace he {
namespace px {

class PhysicsBoxShape : public IPhysicsShape
{
public:
    explicit PhysicsBoxShape(const vec3& dimension);
    virtual ~PhysicsBoxShape();
    //Default copy constructor and assignment operator are OK

    virtual PhysicsShapeType getType() const { return PhysicsShapeType_Box; }

    const vec3 getDimension() const;

private:
    vec3 m_Dimension;

};

} } //end namespace

#endif
