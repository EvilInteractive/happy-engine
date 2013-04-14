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
//Created: 06/11/2011
#include "HappyPCH.h" 

#include "PhysicsConvexMesh.h"

#include "PhysicsBinaryStream.h"
#include "PhysicsEngine.h"

#include "BinaryVisitor.h"

#include <PxPhysics.h>
#include <geometry/PxConvexMesh.h>


namespace he {
namespace px {
IMPLEMENT_OBJECT(PhysicsConvexMesh)

PhysicsConvexMesh::PhysicsConvexMesh()
{
}


PhysicsConvexMesh::~PhysicsConvexMesh()
{
    m_InternalMeshes.forEach([](physx::PxConvexMesh* mesh)
    {
        mesh->release();
    });
}

void PhysicsConvexMesh::load( PhysicsBinaryStream& stream )
{
    HE_ASSERT(m_InternalMeshes.size() == 0, "PhysicsConvexMesh %s already loaded!", getName().c_str());

    io::BinaryVisitor* const visitor(stream.getVisitor());

    uint8 numConvex(0);
    visitor->visit(numConvex);

    he::String name;
    for (uint8 i(0); i < numConvex; ++i)
    {
        visitor->visit(name);
        m_InternalMeshes.add(
            PHYSICS->getSDK()->createConvexMesh(stream)
        );
    }
}

const he::PrimitiveList<physx::PxConvexMesh*>& PhysicsConvexMesh::getInternalMeshes() const
{
    return m_InternalMeshes;
}


} } //end namespace