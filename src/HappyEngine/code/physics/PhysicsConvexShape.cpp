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
#include "StdAfx.h" 

#include "PhysicsConvexShape.h"
#include "HappyEngine.h"
#include "PhysicsEngine.h"

namespace he {
namespace px {
namespace shapes {

PhysicsConvexShape::PhysicsConvexShape(const io::BinaryStream& stream, const vec3& scale): m_pInternalMesh(nullptr)
{ 
    m_pInternalMesh = PHYSICS->getSDK()->createConvexMesh(stream);
    m_Geometry = PxConvexMeshGeometry(m_pInternalMesh, PxMeshScale(PxVec3(scale.x, scale.y, scale.z), PxQuat::createIdentity()));
}


PhysicsConvexShape::~PhysicsConvexShape()
{
}

const PxGeometry& PhysicsConvexShape::getGeometry() const
{
    return m_Geometry;
}

} } } //end namespace