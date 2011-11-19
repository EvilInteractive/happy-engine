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

PhysicsConvexShape::PhysicsConvexShape(const PhysicsConvexMesh::pointer& mesh, const vec3& scale): 
        m_pInternalMesh(mesh->getInternalMesh()), m_Scale(scale)
{ 
}

PhysicsConvexShape::PhysicsConvexShape(): m_pInternalMesh(nullptr), m_Scale()
{

}


PhysicsConvexShape::~PhysicsConvexShape()
{
}

physx::PxConvexMesh* PhysicsConvexShape::getInternalMesh() const
{
    return m_pInternalMesh;
}

const vec3 PhysicsConvexShape::getScale() const
{
    return m_Scale;
}


} } //end namespace