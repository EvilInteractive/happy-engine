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

#include "ResourceFactory.h"

#include "PhysicsConcaveShape.h"
#include "PhysicsConcaveMesh.h"
#include "ContentManager.h"

namespace he {
namespace px {

PhysicsConcaveShape::PhysicsConcaveShape(const ObjectHandle& concaveMesh, const vec3& scale): 
        m_ConcaveMesh(concaveMesh), m_Scale(scale)
{ 
    ResourceFactory<PhysicsConcaveMesh>::getInstance()->instantiate(m_ConcaveMesh);
}
PhysicsConcaveShape::PhysicsConcaveShape(const he::String& asset, const vec3& scale): 
        m_ConcaveMesh(CONTENT->loadPhysicsConcave(asset)), m_Scale(scale)
{ 
}

PhysicsConcaveShape::PhysicsConcaveShape(): 
        m_ConcaveMesh(ObjectHandle::unassigned), m_Scale(vec3::one)
{

}

PhysicsConcaveShape::PhysicsConcaveShape( const PhysicsConcaveShape& other ): 
        m_ConcaveMesh(other.m_ConcaveMesh), m_Scale(other.m_Scale)
{
    ResourceFactory<PhysicsConcaveMesh>::getInstance()->instantiate(m_ConcaveMesh);
}

PhysicsConcaveShape& PhysicsConcaveShape::operator=( const PhysicsConcaveShape& other )
{
    ResourceFactory<PhysicsConcaveMesh>::getInstance()->release(m_ConcaveMesh);
    m_ConcaveMesh = other.m_ConcaveMesh;
    m_Scale = other.m_Scale;
    ResourceFactory<PhysicsConcaveMesh>::getInstance()->instantiate(m_ConcaveMesh);
    return *this;
}



PhysicsConcaveShape::~PhysicsConcaveShape()
{
    ResourceFactory<PhysicsConcaveMesh>::getInstance()->release(m_ConcaveMesh);
}

const ObjectHandle& PhysicsConcaveShape::getConcaveMesh() const
{
    return m_ConcaveMesh;
}

const vec3& PhysicsConcaveShape::getScale() const
{
    return m_Scale;
}


} } //end namespace