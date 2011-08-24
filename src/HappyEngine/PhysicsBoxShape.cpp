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

#include "PhysicsBoxShape.h"

namespace happyengine {
namespace physics {
namespace shapes {

PhysicsBoxShape::PhysicsBoxShape(const math::Vector3& dimension): m_Dimension(dimension), 
    m_Geometry(PxBoxGeometry(dimension.x / 2.0f, dimension.y / 2.0f, dimension.z / 2.0f))
{
   
}


PhysicsBoxShape::~PhysicsBoxShape()
{
}

const PxGeometry& PhysicsBoxShape::getGeometry() const
{
    return m_Geometry;
}

} } } //end namespace