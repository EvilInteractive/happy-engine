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
//Created: 25/04/2012

#include "HappyPCH.h" 

#include "Mesh2D.h"

namespace he {

Mesh2D::Mesh2D() :  m_pPolygon(NEW Polygon()),
                    m_WorldMatrix(mat44::Identity)
{
    glGenBuffers(1, &m_VBOID);
    glGenBuffers(1, &m_IBOID);
    glGenVertexArrays(1, &m_VAOID);
}

Mesh2D::~Mesh2D()
{
    glDeleteBuffers(1, &m_VBOID);
    glDeleteBuffers(1, &m_IBOID);
    glDeleteVertexArrays(1, &m_VAOID);

    delete m_pPolygon;
}

/* GENERAL */
void Mesh2D::addVertex(const vec2& point)
{
    m_pPolygon->addPoint(point);
}

void Mesh2D::clear()
{
    m_pPolygon->clear();
}

bool Mesh2D::triangulate()
{
    return m_pPolygon->triangulate();
}

void Mesh2D::createBuffer()
{
    if (m_pPolygon->triangulate() == false)
    {
        HE_ERROR("Failed to triangulate Mesh2D!");
    }

    GL::heBindVao(m_VAOID);

    /*float* vertices = NEW float[m_pPolygon->getVertexCount() * 2];
    uint* indices = NEW uint[m_pPolygon->getIndexCount()];

    for (uint i(0); i < m_pPolygon->getVertexCount(); ++i)
    {
    vertices[i * 2] = m_pPolygon->getVertices()[i].x;
    vertices[(i * 2) + 1] = m_pPolygon->getVertices()[i].y;
    }

    for (uint i(0); i < m_pPolygon->getIndexCount(); ++i)
    {
    indices[i] = m_pPolygon->getIndices()[i];
    }*/

    glBindBuffer(GL_ARRAY_BUFFER, m_VBOID);
    glBufferData(GL_ARRAY_BUFFER, m_pPolygon->getVertexCount() * 2 * sizeof(vec2), &m_pPolygon->getVertices()[0], GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_pPolygon->getIndexCount() * sizeof(uint), &m_pPolygon->getIndices()[0], GL_STREAM_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    GL::heBindVao(0);
}

/* GETTERS */
uint Mesh2D::getBufferID() const
{
    return m_VAOID;
}

const mat44& Mesh2D::getWorldMatrix() const
{
    return m_WorldMatrix;
}

const std::vector<vec2>& Mesh2D::getVertices() const
{
    return m_pPolygon->getVertices();
}

const std::vector<uint>& Mesh2D::getIndices() const
{
    return m_pPolygon->getIndices();
}

/* SETTERS */
void Mesh2D::setWorldMatrix(const mat44& mat)
{
    m_WorldMatrix = mat;
}

} //end namespace