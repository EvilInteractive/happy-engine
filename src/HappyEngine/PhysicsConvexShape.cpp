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

#include "PhysicsConvexShape.h"
#include "geometry/PxConvexMeshGeometry.h"
#include "HappyEngine.h"
#include "BinaryStream.h"

namespace happyengine {
namespace physics {
namespace shapes {

PhysicsConvexShape::PhysicsConvexShape(const std::string& path): m_pInternalMesh(nullptr)
{ 
    m_pInternalMesh = PHYSICS->getSDK()->createConvexMesh(io::BinaryStream(path, io::BinaryStream::Read));
}


PhysicsConvexShape::~PhysicsConvexShape()
{
    m_pInternalMesh->release();
}

PxGeometry PhysicsConvexShape::getNewGeometry() const
{
    return PxConvexMeshGeometry(m_pInternalMesh, PxMeshScale());
}

} } } //end namespace