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

#ifndef _PHYSICS_BOX_SHAPE_H_
#define _PHYSICS_BOX_SHAPE_H_
#pragma once

#include "IPhysicsShape.h"
#include "Vector3.h"

namespace happyengine {
namespace physics {
namespace shapes {

class PhysicsBoxShape : public IPhysicsShape
{
public:
    explicit PhysicsBoxShape(const math::Vector3& dimension);
    virtual ~PhysicsBoxShape();

    virtual PxGeometry getNewGeometry() const;

private:

    math::Vector3 m_Dimension;

    //Disable default copy constructor and default assignment operator
    PhysicsBoxShape(const PhysicsBoxShape&);
    PhysicsBoxShape& operator=(const PhysicsBoxShape&);
};

} } } //end namespace

#endif
