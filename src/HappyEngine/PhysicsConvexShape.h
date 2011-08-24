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

#ifndef _PHYSICS_CONVEX_SHAPE_H_
#define _PHYSICS_CONVEX_SHAPE_H_
#pragma once

#include "IPhysicsShape.h"
#include "geometry/PxConvexMesh.h"
#include "geometry/PxConvexMeshGeometry.h"
#include <string>
#include "Vector3.h"

namespace happyengine {
namespace physics {
namespace shapes {

class PhysicsConvexShape : public IPhysicsShape
{
public:
    explicit PhysicsConvexShape(const std::string& path, const math::Vector3& scale);
    virtual ~PhysicsConvexShape();

    virtual const PxGeometry& getGeometry() const;

private:

    PxConvexMesh* m_pInternalMesh;
    PxConvexMeshGeometry m_Geometry;

    //Disable default copy constructor and default assignment operator
    PhysicsConvexShape(const PhysicsConvexShape&);
    PhysicsConvexShape& operator=(const PhysicsConvexShape&);
};

} } } //end namespace

#endif
