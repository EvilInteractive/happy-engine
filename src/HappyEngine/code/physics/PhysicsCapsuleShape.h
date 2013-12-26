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
//
//Author:  Bastian Damman
//Created: 20/01/2012

#ifndef _HE_PHYSICS_CAPUSLE_SHAPE_H_
#define _HE_PHYSICS_CAPUSLE_SHAPE_H_
#pragma once

#include "IPhysicsShape.h"

namespace he {
namespace px {

class HAPPY_ENTRY PhysicsCapsuleShape : public IPhysicsShape
{
public:
    explicit PhysicsCapsuleShape(float radius, float height);
    virtual ~PhysicsCapsuleShape();
    //Default copy constructor and assignment operator are OK

    virtual PhysicsShapeType getType() const { return PhysicsShapeType_Capsule; }

    const float getRadius() const;
    const float getHeight() const;

private:
    float m_Radius;
    float m_Height;

};

} } //end namespace

#endif
