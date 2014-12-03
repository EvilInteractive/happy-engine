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
//Created: 15/09/2011 (copy of the previous Model class)
#include "HappyPCH.h" 

#include "ModelMesh.h"
#include "GraphicsEngine.h"

namespace he {
namespace gfx {
   
#pragma warning(disable:4355) // use of this in initializer list
ModelMesh::ModelMesh(): 
    m_NumVertices(0), 
    m_NumIndices(0),
    m_Bound(AABB(vec3(-1, -1, -1), vec3(1, 1, 1))),
    m_DrawMode(MeshDrawMode_Triangles),
    m_IndexVboID(UINT32_MAX),
    m_VertexVboID(UINT32_MAX)
{
}
#pragma warning(default:4355) // use of this in initializer list


ModelMesh::~ModelMesh()
{
    if (m_VertexVboID != UINT32_MAX)
        glDeleteBuffers(1, &m_VertexVboID);
    if (m_IndexVboID != UINT32_MAX)
        glDeleteBuffers(1, &m_IndexVboID);
    destroyPickingData();
}

void ModelMesh::init(const VertexLayout& vertexLayout, MeshDrawMode mode)
{
    HE_IF_ASSERT(m_VertexVboID == UINT32_MAX, "Only init ModelMesh once!")
    {
        m_VertexLayout = vertexLayout;
        m_DrawMode = mode;
        glGenBuffers(1, &m_IndexVboID);
        glGenBuffers(1, &m_VertexVboID);
    }
}

//Calling glBufferData with a NULL pointer before uploading new data can improve performance (tells the driver you don't care about the old cts)
void ModelMesh::setVertices(const void* const vertices, const uint32 num, const MeshUsage usage, const bool calcBound)
{
    m_NumVertices = num;

    if (calcBound == true)
    {
        uint32 posOffset = UINT32_MAX;
        EShaderAttributeTypeComponents comp(eShaderAttributeTypeComponents_2);
        std::for_each(m_VertexLayout.getElements().cbegin(), m_VertexLayout.getElements().cend(), [&](const VertexElement& e)
        {
            if (e.getAttribute() == gfx::eShaderAttribute_Position)
            {
                posOffset = e.getByteOffset();
                comp = e.getComponents();
            }
        });
        m_Bound.fromAABB(AABB::calculateBoundingAABB(vertices, num, m_VertexLayout.getSize(), posOffset, comp));
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_VertexVboID);
    glBufferData(GL_ARRAY_BUFFER, m_VertexLayout.getSize() * num, nullptr, usage);
    glBufferData(GL_ARRAY_BUFFER, m_VertexLayout.getSize() * num, vertices, usage);
}
void ModelMesh::setIndices(const void* const indices, const uint32 num, const IndexStride type, const MeshUsage usage)
{
    m_NumIndices = num;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexVboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, type * num, nullptr, usage);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, type * num, indices, usage);

    switch (type)
    {
        case IndexStride_UShort: m_IndexType = GL_UNSIGNED_SHORT; break;
        case IndexStride_UInt: m_IndexType = GL_UNSIGNED_INT; break;
        default: LOG(LogType_ProgrammerAssert, "unknown type: %d", type); break;
    }
}

void ModelMesh::setBones( const he::ObjectList<Bone>& boneList )
{
    m_BoneList.clear();
    m_BoneList.append(boneList);
}

void ModelMesh::createPickingData(const void* const vertices, const size_t vertexCount, const VertexLayout& vertexLayout, const void* const indices, const size_t indexCount, const IndexStride indexStride)
{
    HE_IF_ASSERT(m_PickingData.m_Vertices == nullptr && m_PickingData.m_Indices == nullptr, "Picking data already initialized!")
    {
        m_PickingData.m_TriangleCount = indexCount / 3;
        m_PickingData.m_Vertices = static_cast<vec3*>(he_malloc(sizeof(vec3) * vertexCount));
        m_PickingData.m_Indices = he_malloc(indexStride * indexCount);
        m_PickingData.m_IndexStride = indexStride;
        he_memcpy(m_PickingData.m_Indices, indices, indexStride * indexCount);

        uint32 posOffset(UINT32_MAX);
        std::for_each(vertexLayout.getElements().cbegin(), vertexLayout.getElements().cend(), [&](const VertexElement& e)
        {
            if (e.getAttribute() == gfx::eShaderAttribute_Position)
            {
                posOffset = e.getByteOffset();
            }
        });

        const uint32 stride(vertexLayout.getSize());
        for (size_t i(0); i < vertexCount; ++i)
        {
            he_memcpy(m_PickingData.m_Vertices + i, 
                reinterpret_cast<const vec3*>(static_cast<const char*>(vertices) + stride * i + posOffset), sizeof(vec3));
        }
    }
}

void ModelMesh::destroyPickingData()
{
    m_PickingData.m_TriangleCount = 0;
    he_free(m_PickingData.m_Vertices);
    m_PickingData.m_Vertices = nullptr;
    he_free(m_PickingData.m_Indices);
    m_PickingData.m_Indices = nullptr;
}

void ModelMesh::draw() const
{
    glDrawElements(m_DrawMode, m_NumIndices, m_IndexType, 0);
}

} } //end namespace
