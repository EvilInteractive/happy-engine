
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
//Created: 08/11/2011

#ifndef _HE_PHYSICS_CONCAVE_SHAPE_H_
#define _HE_PHYSICS_CONCAVE_SHAPE_H_
#pragma once

#include "IPhysicsShape.h"

namespace he {
namespace px {

class PhysicsConcaveShape : public IPhysicsShape
{
public:
    PhysicsConcaveShape();
    explicit PhysicsConcaveShape(const ObjectHandle& concaveMesh, const vec3& scale = vec3::one);
    explicit PhysicsConcaveShape(const he::String& concaveMesh, const vec3& scale = vec3::one);
    virtual ~PhysicsConcaveShape();
    PhysicsConcaveShape(const PhysicsConcaveShape& other);
    PhysicsConcaveShape& operator=(const PhysicsConcaveShape& other);

    virtual PhysicsShapeType getType() const { return PhysicsShapeType_Concave; }

    const ObjectHandle& getConcaveMesh() const;
    const vec3& getScale() const;

private:

    ObjectHandle m_ConcaveMesh;
    vec3 m_Scale;
};

} } //end namespace

#endif
