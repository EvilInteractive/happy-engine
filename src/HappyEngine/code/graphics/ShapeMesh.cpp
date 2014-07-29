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
//Author:  Bastian Damman
//Created: 29/07/2014

#include "HappyPCH.h" 
#include "ShapeMesh.h"

#include "Polygon.h"
#include "ModelMesh.h"

namespace he {
namespace gfx {

VertexLayout ShapeMesh::s_VertexLayout;

struct ShapeMesh::InternalData
{
    he::ObjectList<he::vec3> m_Vertices;
    he::PrimitiveList<uint16> m_Indices;
    bool m_SupportLineBreaks;
};

ShapeMesh::ShapeMesh() 
    : m_Points(nullptr)
    , m_InternalMesh(NEW ModelMesh())
{

}

ShapeMesh::~ShapeMesh()
{
    delete m_Points;
    delete m_InternalMesh;
}

void ShapeMesh::init( const MeshDrawMode mode )
{
    HE_ASSERT(mode != MeshDrawMode_Triangles, "Triangles are not supported in a ShapeMesh!");
    m_InternalMesh->init(s_VertexLayout, mode);
}

void ShapeMesh::beginEditing()
{
    if (!m_Points)
    {
        m_Points = NEW ShapeMesh::InternalData();
    }
    m_Points->m_Vertices.clear();
    m_Points->m_Indices.clear();
    m_Points->m_SupportLineBreaks = m_InternalMesh->getDrawMode() == MeshDrawMode_Lines;
}

void ShapeMesh::addPoint( const vec3& point, const bool startNewLine /*= false*/ )
{
    HE_ASSERT(m_Points, "beginEditing was not called when the call clearPoints was made!");
    const size_t vertexCount(m_Points->m_Vertices.size());
    if (m_Points->m_SupportLineBreaks && !startNewLine && vertexCount > 0)
        m_Points->m_Indices.add(he::checked_numcast<uint16>(vertexCount - 1));
    m_Points->m_Indices.add(he::checked_numcast<uint16>(vertexCount));
    m_Points->m_Vertices.add(point);
}

void ShapeMesh::endEditing( const bool close, const bool keepBuffer )
{
    if (close && m_InternalMesh->getDrawMode() != MeshDrawMode_Points)
        m_Points->m_Indices.add(0);

    m_InternalMesh->setIndices(&m_Points->m_Indices[0], m_Points->m_Indices.size(), IndexStride_UShort, keepBuffer? MeshUsage_Stream : MeshUsage_Static);
    m_InternalMesh->setVertices(&m_Points->m_Vertices[0], m_Points->m_Vertices.size(), keepBuffer? MeshUsage_Stream : MeshUsage_Static, true);
    if (!keepBuffer)
    {
        delete m_Points;
        m_Points = nullptr;
    }
}

he::uint32 ShapeMesh::getVBO() const
{
    return m_InternalMesh->getVBO();
}

he::uint32 ShapeMesh::getIBO() const
{
    return m_InternalMesh->getIBO();
}

void ShapeMesh::sdmInit()
{
    s_VertexLayout.addElement(VertexElement(eShaderAttribute_Position, eShaderAttributeType_Float, eShaderAttributeTypeComponents_3, 0));
}

void ShapeMesh::sdmDestroy()
{

}

void ShapeMesh::draw() const
{
    m_InternalMesh->draw();
}

} } //end namespace
