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
//Author:  Sebastiaan Sprengers
//Created: 15/10/2011

#include "HappyPCH.h" 

#include "Polygon.h"
#include "Triangulator.h"

namespace he {

/* CONSTRUCTOR - DESTRUCTOR */
Polygon::Polygon()
{
}

Polygon::~Polygon()
{
}

Polygon::Polygon(const Polygon& p)
{
    m_Vertices.append(p.m_Vertices);
    m_Indices.append(p.m_Indices);
}

Polygon& Polygon::operator=(const Polygon& p)
{
    m_Vertices.clear();
    m_Indices.clear();
    m_Vertices.append(p.m_Vertices);
    m_Indices.append(p.m_Indices);

    return *this;
}

/* GENERAL */
void Polygon::addPoint(const vec2& p)
{
    m_Vertices.add(p);
    m_Indices.clear();
}

bool Polygon::outline()
{
    if (m_Vertices.size() < 3)
        return false;

    m_Indices.clear();

    for (uint32 i(0); i < m_Vertices.size(); ++i)
    {
        m_Indices.add(i);
    }

    return true;
}

bool Polygon::triangulate()
{
    if (m_Vertices.size() < 3)
        return false;

    m_Indices.clear();

    bool b(Triangulator::triangulatePolygon(m_Vertices, m_Indices));

    if (b == false)
        m_Indices.clear();

    return b;
}

void Polygon::clear()
{
    m_Vertices.clear();
    m_Indices.clear();
}

/* GETTERS */
const he::PrimitiveList<vec2>& Polygon::getVertices() const
{
    return m_Vertices;
}

const he::PrimitiveList<uint32>& Polygon::getIndices() const
{
    return m_Indices;
}

bool Polygon::isTriangulated() const
{
    return m_Indices.size() != 0;
}

bool Polygon::hitTest(const vec2& hitPoint) const
{
    if (m_Indices.size() != 0)
    {
        for (uint32 i(0); i < m_Indices.size(); i += 3)
        {
            if (Triangulator::hitTestTriangle(	m_Vertices[m_Indices[i]],
                                                m_Vertices[m_Indices[i + 1]],
                                                m_Vertices[m_Indices[i + 2]],
                                                hitPoint))
                return true;
        }

        return false;
    }
    else
        return false;
}

float Polygon::getArea() const
{
    return Triangulator::calculateArea(m_Vertices);
}

uint32 Polygon::getVertexCount() const
{
    return (uint32)m_Vertices.size();
}

uint32 Polygon::getIndexCount() const
{
    return (uint32)m_Indices.size();
}

uint32 Polygon::getTriangleCount() const
{
    if (isTriangulated())
        return (uint32)m_Indices.size() / 3;
    else
        return 0;
}

/* OPERATORS */
bool Polygon::operator==(const Polygon& p) const
{
    if (p.getVertexCount() != getVertexCount())
        return false;

    for (uint32 i(0); i < getVertexCount(); ++i)
    {
        if (m_Vertices[i] != p.getVertices()[i])
            return false;
    }
    
    return true;
}

bool Polygon::operator!=(const Polygon& p) const
{
    return (!(*this == p));
}

} //end namespace