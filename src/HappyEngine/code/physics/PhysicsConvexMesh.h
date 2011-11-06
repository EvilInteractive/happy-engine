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
//Created: 06/11/2011

#ifndef _HE_PHYSICS_CONVEX_MESH_H_
#define _HE_PHYSICS_CONVEX_MESH_H_
#pragma once

#include "boost/shared_ptr.hpp"
#include "geometry/PxConvexMesh.h"
#include "BinaryStream.h"

namespace he {
namespace px {

class PhysicsConvexMesh
{
public:
	PhysicsConvexMesh(const io::BinaryStream& stream);
    virtual ~PhysicsConvexMesh();

    physx::PxConvexMesh* getInternalMesh() const;

    typedef boost::shared_ptr<PhysicsConvexMesh> pointer;

private:

    physx::PxConvexMesh* m_pInternalMesh;

    //Disable default copy constructor and default assignment operator
    PhysicsConvexMesh(const PhysicsConvexMesh&);
    PhysicsConvexMesh& operator=(const PhysicsConvexMesh&);
};

} } //end namespace

#endif
