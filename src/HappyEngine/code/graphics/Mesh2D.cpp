//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
#include "Polygon.h"
#include "GraphicsEngine.h"

namespace he {
namespace gfx {

VertexLayout* Mesh2D::s_VertexLayout(nullptr);

Mesh2D::Mesh2D(bool staticDraw) :
    m_Polygon(HENew(Polygon)()),
    m_StaticDraw(staticDraw),
    m_HasBuffer(false),
    m_VertexVboID(0),
    m_IndexVboID(0),
    m_DrawMode(MeshDrawMode_LineLoop)
{
    glGenBuffers(1, &m_VertexVboID);
    glGenBuffers(1, &m_IndexVboID);
}

Mesh2D::~Mesh2D()
{
    glDeleteBuffers(1, &m_VertexVboID);
    glDeleteBuffers(1, &m_IndexVboID);

    HEDelete(m_Polygon);
}

/* GENERAL */
void Mesh2D::addVertex(const vec2& point)
{
    m_Polygon->addPoint(point);
    m_HasBuffer = false;
}

void Mesh2D::clear()
{
    m_Polygon->clear();
    m_HasBuffer = false;
}

bool Mesh2D::triangulate()
{
    m_DrawMode = MeshDrawMode_Triangles;
    return m_Polygon->triangulate();
}

bool Mesh2D::outline()
{
    m_DrawMode = MeshDrawMode_LineLoop;
    return m_Polygon->outline();
}

void Mesh2D::createBuffer(bool doOutline)
{
    if (doOutline == false)
    {
        if (!triangulate())
        {
            HE_ERROR("Failed to triangulate Mesh2D!");
        }
    }
    else
    {
        if (!outline())
        {
            HE_ERROR("Failed to outline Mesh2D!");
        }
    }

    GLenum drawType(m_StaticDraw == true ? GL_STATIC_DRAW : GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VertexVboID);
    const size_t vbSize(m_Polygon->getVertexCount() * sizeof(vec2));
    glBufferData(GL_ARRAY_BUFFER, vbSize, nullptr, drawType);
    glBufferData(GL_ARRAY_BUFFER, vbSize, &m_Polygon->getVertices()[0], drawType);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexVboID);
    const size_t ibSize(m_Polygon->getIndexCount() * sizeof(uint32));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibSize, nullptr, drawType);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibSize, &m_Polygon->getIndices()[0], drawType);

    m_HasBuffer = true;
}

/* GETTERS */
const he::PrimitiveList<vec2>& Mesh2D::getVertices() const
{
    return m_Polygon->getVertices();
}

const he::PrimitiveList<uint32>& Mesh2D::getIndices() const
{
    return m_Polygon->getIndices();
}

void Mesh2D::draw()
{
    glDrawElements(m_DrawMode, m_Polygon->getIndexCount(), GL_UNSIGNED_INT, 0);
}

void Mesh2D::sdmInit()
{
    s_VertexLayout = HENew(VertexLayout)();
    s_VertexLayout->addElement(VertexElement(eShaderAttribute_Position, eShaderAttributeType_Float, eShaderAttributeTypeComponents_2, 0));
}

void Mesh2D::sdmDestroy()
{
    HEDelete(s_VertexLayout);
}


} } //end namespace
