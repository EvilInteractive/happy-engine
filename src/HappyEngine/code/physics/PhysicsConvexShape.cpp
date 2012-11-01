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
#include "HappyPCH.h" 

#include "PhysicsConvexShape.h"

#include "ResourceFactory.h"
#include "PhysicsConvexMesh.h"

namespace he {
namespace px {
IMPLEMENT_OBJECT(PhysicsConvexShape)

PhysicsConvexShape::PhysicsConvexShape(const ObjectHandle& convexMesh, const vec3& scale): 
        m_ConvexMesh(convexMesh), m_Scale(scale)
{ 
    ResourceFactory<PhysicsConvexMesh>::getInstance()->instantiate(m_ConvexMesh);
}

PhysicsConvexShape::PhysicsConvexShape(): m_ConvexMesh(ObjectHandle::unassigned), m_Scale()
{
}

PhysicsConvexShape::PhysicsConvexShape( const PhysicsConvexShape& other ): 
        m_ConvexMesh(other.m_ConvexMesh), m_Scale(other.m_Scale)
{
    ResourceFactory<PhysicsConvexMesh>::getInstance()->instantiate(m_ConvexMesh);
}
PhysicsConvexShape& PhysicsConvexShape::operator=( const PhysicsConvexShape& other )
{
    if (m_ConvexMesh != ObjectHandle::unassigned)
        ResourceFactory<PhysicsConvexMesh>::getInstance()->release(m_ConvexMesh);
    m_ConvexMesh = other.m_ConvexMesh;
    m_Scale = other.m_Scale;
    ResourceFactory<PhysicsConvexMesh>::getInstance()->instantiate(m_ConvexMesh);
    return *this;
}



PhysicsConvexShape::~PhysicsConvexShape()
{
    if (m_ConvexMesh != ObjectHandle::unassigned)
        ResourceFactory<PhysicsConvexMesh>::getInstance()->release(m_ConvexMesh);
}

const ObjectHandle& PhysicsConvexShape::getConvexMesh() const
{
    return m_ConvexMesh;
}

const vec3& PhysicsConvexShape::getScale() const
{
    return m_Scale;
}


} } //end namespace