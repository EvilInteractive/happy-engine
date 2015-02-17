//HappyEngine Copyright (C) 2011 - 2015  Evil Interactive
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
//Created: 2015/02/17
#include "HappyPCH.h"
#include "PickingData.h"

#include "VertexLayout.h"

namespace he {
namespace gfx {

PickingData::PickingData() : m_Vertices(nullptr), m_Indices(nullptr), m_IndexStride(IndexStride_UShort), m_TriangleCount(0)
{

}

PickingData::~PickingData()
{
    destroyPickingData();
}

void PickingData::destroyPickingData()
{
    m_TriangleCount = 0;
    he_free(m_Vertices);
    m_Vertices = nullptr;
    he_free(m_Indices);
    m_Indices = nullptr;
}

void PickingData::createPickingData( const void* const vertices, const size_t vertexCount, const VertexLayout& vertexLayout, const void* const indices, const size_t indexCount, const IndexStride indexStride )
{
    HE_IF_ASSERT(m_Vertices == nullptr && m_Indices == nullptr, "Picking data already initialized!")
    {
        m_TriangleCount = indexCount / 3;
        m_Vertices = static_cast<vec3*>(he_malloc("ModelMesh::m_Vertices", sizeof(vec3) * vertexCount));
        m_Indices = he_malloc("ModelMesh::m_Indices", indexStride * indexCount);
        m_IndexStride = indexStride;
        he_memcpy(m_Indices, indices, indexStride * indexCount);

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
            he_memcpy(m_Vertices + i, 
                reinterpret_cast<const vec3*>(static_cast<const char*>(vertices) + stride * i + posOffset), sizeof(vec3));
        }
    }
}

} }

