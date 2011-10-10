//HappyTest Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyTest.
//
//    HappyTest is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyTest is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyTest.  If not, see <http://www.gnu.org/licenses/>.
//
//Author:  Bastian Damman
//Created: 11/08/2011

#include "VertexPNT.h"

namespace happytest {

VertexPNT::VertexPNT()
{
}
VertexPNT::VertexPNT(const he::vec3& position_,
                     const he::vec2& texCoord_,
                     const he::vec3& normal_): position(position_), texCoord(texCoord_), normal(normal_)
{}


VertexPNT::~VertexPNT()
{
}

} //end namespace