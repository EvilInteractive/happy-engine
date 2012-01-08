
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
//Created: 20/08/2011

#ifndef _HE_PHYSICS_CONVEX_SHAPE_H_
#define _HE_PHYSICS_CONVEX_SHAPE_H_
#pragma once

#include "IPhysicsShape.h"
#include "PhysicsConvexMesh.h"
#include "vec3.h"

namespace he {
namespace px {

class PhysicsConvexShape : public IPhysicsShape
{
public:
    PhysicsConvexShape();
    explicit PhysicsConvexShape(const PhysicsConvexMesh::pointer& mesh, const vec3& scale = vec3(1.0f, 1.0f, 1.0f));
    virtual ~PhysicsConvexShape();
    //copy OK

    virtual PhysicsShapeType getType() const { return PhysicsShapeType_Convex; }

    physx::PxConvexMesh* getInternalMesh() const;
    const vec3 getScale() const;

private:

    physx::PxConvexMesh* m_pInternalMesh;
    vec3 m_Scale;

};

} } //end namespace

#endif
