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
#include "geometry/PxBoxGeometry.h"

namespace happyengine {
namespace physics {
namespace shapes {

PhysicsBoxShape::PhysicsBoxShape(const math::Vector3& dimension): m_Dimension(dimension)
{
}


PhysicsBoxShape::~PhysicsBoxShape()
{
}

PxGeometry PhysicsBoxShape::getNewGeometry() const
{
    return PxBoxGeometry(m_Dimension.x / 2.0f, m_Dimension.y / 2.0f, m_Dimension.z / 2.0f);
}

} } } //end namespace