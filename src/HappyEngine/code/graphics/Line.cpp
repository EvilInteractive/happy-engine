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
//Created: 13/09/2011
#include "HappyPCH.h" 

#include "Line.h"
#include "HappyNew.h"

namespace he {
namespace gfx {

Line::Line()
{
}


Line::~Line()
{
}

void Line::setVertices(const std::vector<vec3>& vert)
{
    m_Vertices = vert;
}
void Line::setIndices(const std::vector<ushort>& ind)
{
    m_Indices = ind;
}

const std::vector<vec3>& Line::getVertices() const
{
    return m_Vertices;
}
const std::vector<ushort>& Line::getIndices() const
{
    return m_Indices;
}

} } //end namespace