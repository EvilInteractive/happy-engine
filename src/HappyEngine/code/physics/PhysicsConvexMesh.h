//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

#include "Resource.h"

namespace physx {
    class PxConvexMesh;
}

namespace he {
namespace px {
class PhysicsBinaryStream;

class PhysicsConvexMesh : public Resource<PhysicsConvexMesh>
{
DECLARE_OBJECT(PhysicsConvexMesh)
public:
    PhysicsConvexMesh();
    virtual ~PhysicsConvexMesh();

    void load(PhysicsBinaryStream& stream);

    const he::PrimitiveList<physx::PxConvexMesh*>& getInternalMeshes() const;

private:

    he::PrimitiveList<physx::PxConvexMesh*> m_InternalMeshes;

    //Disable default copy constructor and default assignment operator
    PhysicsConvexMesh(const PhysicsConvexMesh&);
    PhysicsConvexMesh& operator=(const PhysicsConvexMesh&);
};

} } //end namespace

#endif
