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
#include "HappyPCH.h" 

#include "PhysicsConcaveMesh.h"

#include "PhysicsEngine.h"
#include "BinaryStream.h"

namespace he {
namespace px {

PhysicsConcaveMesh::PhysicsConcaveMesh()
{
}


PhysicsConcaveMesh::~PhysicsConcaveMesh()
{
    m_InternalMeshes.forEach([](physx::PxTriangleMesh* mesh)
    {
        mesh->release();
    });
}

void PhysicsConcaveMesh::load( const io::BinaryStream& stream )
{
    HE_ASSERT(m_InternalMeshes.size() == 0, "PhysicsConcaveMesh %s already loaded!", getName().c_str());

    uint8 numConcave(stream.readByte());

    for (uint8 i(0); i < numConcave; ++i)
    {
        stream.readString(); // Name not used..
        m_InternalMeshes.add(
            PHYSICS->getSDK()->createTriangleMesh(const_cast<io::BinaryStream&>(stream))
        );
    }
}

const he::PrimitiveList<physx::PxTriangleMesh*>& PhysicsConcaveMesh::getInternalMeshes() const
{
    return m_InternalMeshes;
}

} } //end namespace