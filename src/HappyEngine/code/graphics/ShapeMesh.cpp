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

VertexLayout* ShapeMesh::s_VertexLayout(nullptr);

struct ShapeMesh::InternalData
{
    he::ObjectList<he::vec3> m_Vertices;
    he::PrimitiveList<uint16> m_Indices;
    bool m_SupportLineBreaks;
};

ShapeMesh::ShapeMesh() 
    : m_Points(nullptr)
    , m_InternalMesh(HENew(ModelMesh)())
{

}

ShapeMesh::~ShapeMesh()
{
    HEDelete(m_Points);
    HEDelete(m_InternalMesh);
}

void ShapeMesh::init( const MeshDrawMode mode )
{
    m_InternalMesh->init(*s_VertexLayout, mode);
}

void ShapeMesh::beginEditing()
{
    if (!m_Points)
    {
        m_Points = HENew(ShapeMesh::InternalData)();
    }
    m_Points->m_Vertices.clear();
    m_Points->m_Indices.clear();
    m_Points->m_SupportLineBreaks = m_InternalMesh->getDrawMode() == MeshDrawMode_Lines;
}

void ShapeMesh::addPoint( const vec3& point, const bool startNewLine /*= false*/ )
{
    HE_ASSERT(m_Points, "beginEditing was not called when the call clearPoints was made!");
    const size_t vertexCount(m_Points->m_Vertices.size());
    if (m_Points->m_SupportLineBreaks && vertexCount > 0)
    {
        if (m_Points->m_Indices.size() % 2 == 0)
        {
            if (!startNewLine)
                m_Points->m_Indices.add(he::checked_numcast<uint16>(vertexCount - 1));
        }
        else
        {
            HE_ASSERT(!startNewLine, "Could not start newline! Last point just started a new line!");
        }
    }
    m_Points->m_Indices.add(he::checked_numcast<uint16>(vertexCount));
    m_Points->m_Vertices.add(point);
}

void ShapeMesh::addBuffer( const void* vertices, const void* indices, const size_t indexCount, const he::gfx::IndexStride indexStride, const he::gfx::VertexLayout& layout )
{
    HE_ASSERT(m_Points, "beginEditing was not called when the call clearPoints was made!");

    if (indexStride != IndexStride_UShort)
    {
        LOG(LogType_ProgrammerAssert, "Invalid indexStride for ShapeMesh!");
        return;
    }

    size_t posOffset(0);
    size_t elIndex(0);
    if (layout.getElements().find_if([](const VertexElement& el)
    {
        if (el.getAttribute() == eShaderAttribute_Position)
        {
            return true;
        }
        return false;
    }, elIndex))
    {
        const VertexElement& posEl(layout.getElements()[elIndex]);
        if (posEl.getComponents() == eShaderAttributeTypeComponents_3 && posEl.getType() == eShaderAttributeType_Float)
        {
            posOffset = posEl.getByteOffset();
        }
        else
        {
            LOG(LogType_ProgrammerAssert, "Invalid vertexlayout for ShapeMesh!");
            return;
        }
    }
    else
    {
        LOG(LogType_ProgrammerAssert, "Could not find position in vertexlayout!");
        return;
    }

    const uint16 indexOffset(checked_numcast<uint16>(m_Points->m_Vertices.size()));
    const size_t vstride(layout.getSize());
    for (size_t i(0); i < indexCount; ++i)
    {
        uint16 index(*(reinterpret_cast<const uint16*>(indices) + i));
        const vec3& pos(*reinterpret_cast<const vec3*>(reinterpret_cast<const char*>(vertices) + index * vstride + posOffset));

        uint16 realIndex(indexOffset + index);
        m_Points->m_Indices.add(realIndex);
        if (m_Points->m_Vertices.size() <= realIndex)
            m_Points->m_Vertices.resize(realIndex + 1);
        m_Points->m_Vertices[realIndex] = pos;
    }
}

void ShapeMesh::endEditing( const bool close, const bool keepBuffer )
{
    if (close && m_InternalMesh->getDrawMode() != MeshDrawMode_Points)
        m_Points->m_Indices.add(0);

    m_InternalMesh->setIndices(&m_Points->m_Indices[0], m_Points->m_Indices.size(), IndexStride_UShort, keepBuffer? MeshUsage_Stream : MeshUsage_Static);
    m_InternalMesh->setVertices(&m_Points->m_Vertices[0], m_Points->m_Vertices.size(), keepBuffer? MeshUsage_Stream : MeshUsage_Static, true);
    if (!keepBuffer)
    {
        HEDelete(m_Points);
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
    s_VertexLayout = HENew(VertexLayout)();
    s_VertexLayout->addElement(VertexElement(eShaderAttribute_Position, eShaderAttributeType_Float, eShaderAttributeTypeComponents_3, 0));
}

void ShapeMesh::sdmDestroy()
{
    HEDelete(s_VertexLayout);
    s_VertexLayout = nullptr;
}

void ShapeMesh::draw() const
{
    m_InternalMesh->draw();
}

} } //end namespace
