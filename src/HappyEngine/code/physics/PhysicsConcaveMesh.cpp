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
//Created: 08/11/2011
#include "HappyPCH.h" 

#include "PhysicsConcaveMesh.h"
#include "HappyNew.h"

#include "HappyEngine.h"
#include "PhysicsEngine.h"

namespace he {
namespace px {

PhysicsConcaveMesh::PhysicsConcaveMesh(const io::BinaryStream& stream)
{
    m_Name = stream.readString();
    m_pInternalMesh = PHYSICS->getSDK()->createTriangleMesh(stream);
}


PhysicsConcaveMesh::~PhysicsConcaveMesh()
{
    m_pInternalMesh->release();
}

physx::PxTriangleMesh* PhysicsConcaveMesh::getInternalMesh() const
{
    return m_pInternalMesh;
}

const std::string& PhysicsConcaveMesh::getName() const
{
    return m_Name;
}

} } //end namespace