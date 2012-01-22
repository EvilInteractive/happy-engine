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
//Created: 18/01/2012

#include "HappyPCH.h" 

#include "ModelMesh2D.h"

#include "OpenGL.h"
#include <algorithm>
#include "HeAssert.h"

namespace he {
namespace gfx {

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

ModelMesh2D::ModelMesh2D(): m_NumVertices(0), m_NumIndices(0)
{
    glGenVertexArrays(1, &m_VaoID[0]);
}

ModelMesh2D::~ModelMesh2D()
{
    glDeleteVertexArrays(1, m_VaoID);
    glDeleteBuffers(1, m_VertexVboID);
    glDeleteBuffers(1, m_IndexVboID);
}

void ModelMesh2D::setVertices(const void* pVertices, uint num, const BufferLayout& vertexLayout, bool bStatic)
{
    m_NumVertices = num;

    uint posOffset = UINT_MAX;
    uint boneIdOffset = UINT_MAX;
    uint boneWeightOffset = UINT_MAX;

    std::for_each(vertexLayout.getElements().cbegin(), vertexLayout.getElements().cend(), [&](const BufferElement& e)
    {
        if (e.getUsage() == gfx::BufferElement::Usage_Position)
        {
            posOffset = e.getByteOffset();
        }
        else if (e.getUsage() == gfx::BufferElement::Usage_BoneIDs)
        {
            boneIdOffset = e.getByteOffset();
        }
        else if (e.getUsage() == gfx::BufferElement::Usage_BoneWeights)
        {
            boneWeightOffset = e.getByteOffset();
        }
    });

    GL::heBindVao(m_VaoID[0]);

    BufferLayout::layout elements(vertexLayout.getElements());

    glGenBuffers(1, m_VertexVboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexVboID[0]);

    if (bStatic)
        glBufferData(GL_ARRAY_BUFFER, vertexLayout.getSize() * num, pVertices, GL_STATIC_DRAW);
    else
        glBufferData(GL_ARRAY_BUFFER, vertexLayout.getSize() * num, pVertices, GL_STREAM_DRAW);

    std::for_each(elements.cbegin(), elements.cend(), [&](const BufferElement& e)
    {
        GLint components = 1;
        GLenum type = 0;
        switch (e.getType())
        {
            case BufferElement::Type_Vec2: type = GL_FLOAT; components = 2; break;
            case BufferElement::Type_Vec3: type = GL_FLOAT; components = 3; break;
            case BufferElement::Type_Vec4: type = GL_FLOAT; components = 4; break;
            case BufferElement::Type_Float: type = GL_FLOAT; break;

            case BufferElement::Type_Int: type = GL_INT; break;
            case BufferElement::Type_IVec4: type = GL_INT; components = 4; break;
            case BufferElement::Type_UInt: type = GL_UNSIGNED_INT; break;
            
            #pragma warning(disable:4127)
            default: ASSERT(false, "unknown type"); break;
            #pragma warning(default:4127)
        }

        glVertexAttribPointer(e.getElementIndex(), components, type, GL_FALSE, vertexLayout.getSize(), BUFFER_OFFSET(e.getByteOffset())); 
        glEnableVertexAttribArray(e.getElementIndex());
    });

    GL::heBindVao(0);
}
void ModelMesh2D::setIndices(const void* pIndices, uint num, IndexStride type, bool bStatic)
{
    m_NumIndices = num;
    
    GL::heBindVao(m_VaoID[0]);
    glGenBuffers(1, m_IndexVboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexVboID[0]);

    if (bStatic)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, type * num, pIndices, GL_STATIC_DRAW);
    else
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, type * num, pIndices, GL_STREAM_DRAW);

    switch (type)
    {
        case IndexStride_Byte: m_IndexType = GL_UNSIGNED_BYTE; break;
        case IndexStride_UShort: m_IndexType = GL_UNSIGNED_SHORT; break;
        case IndexStride_UInt: m_IndexType = GL_UNSIGNED_INT; break;
        default: ASSERT("unkown type"); break;
    }

    GL::heBindVao(0);
}

uint ModelMesh2D::getVertexArraysID() const
{
    return m_VaoID[0];
}

uint ModelMesh2D::getNumVertices() const
{
    return m_NumVertices;
}
uint ModelMesh2D::getNumIndices() const
{
    return m_NumIndices;
}

uint ModelMesh2D::getIndexType() const
{
    return m_IndexType;
}

uint ModelMesh2D::getVBOID() const
{
    return m_VertexVboID[0];
}

uint ModelMesh2D::getVBOIndexID() const
{
    return m_IndexVboID[0];
}

} } //end namespace