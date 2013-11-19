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
//Created: 15/09/2011 (copy of the previous Model class)
#include "HappyPCH.h" 

#include "ModelMesh.h"
#include "GraphicsEngine.h"

namespace he {
namespace gfx {

#define BUFFER_OFFSET(i) ((char*)nullptr + (i))
   
#pragma warning(disable:4355) // use of this in initializer list
ModelMesh::ModelMesh(): 
    m_NumVertices(0), 
    m_NumIndices(0), 
    m_IsLoaded(false), 
    m_isVisible(true),
    m_Bound(AABB(vec3(-1, -1, -1), vec3(1, 1, 1))),
    m_ContextCreatedHandler(boost::bind(&ModelMesh::initVAO, this, _1)),
    m_ContextRemovedHandler(boost::bind(&ModelMesh::destroyVAO, this, _1)),
    m_DrawMode(MeshDrawMode_Triangles),
    m_IndexVboID(UINT32_MAX),
    m_VertexVboID(UINT32_MAX)
{
    he_memset(m_VaoID, 0xff, MAX_VERTEX_ARRAY_OBJECTS * sizeof(VaoID));
    he_memset(m_VaoShadowID, 0xff, MAX_VERTEX_ARRAY_OBJECTS * sizeof(VaoID));
}
#pragma warning(default:4355) // use of this in initializer list


ModelMesh::~ModelMesh()
{
    GRAPHICS->ContextCreated -= m_ContextCreatedHandler;
    GRAPHICS->ContextRemoved -= m_ContextRemovedHandler;
    const he::PrimitiveList<GLContext*>& contexts(GRAPHICS->getContexts());
    std::for_each(contexts.cbegin(), contexts.cend(), [&](GLContext* context)
    {
        if (m_VaoID[context->getID()] != UINT32_MAX)
            destroyVAO(context);
    });
    if (m_VertexVboID != UINT32_MAX)
        glDeleteBuffers(1, &m_VertexVboID);
    if (m_IndexVboID != UINT32_MAX)
        glDeleteBuffers(1, &m_IndexVboID);
    destroyPickingData();
}

void ModelMesh::init(const BufferLayout& vertexLayout, MeshDrawMode mode)
{
    HE_IF_ASSERT(m_VertexVboID == UINT32_MAX, "Only init ModelMesh once!")
    {
        m_VertexLayout = vertexLayout;
        m_DrawMode = mode;
        glGenBuffers(1, &m_IndexVboID);
        glGenBuffers(1, &m_VertexVboID);
        const he::PrimitiveList<GLContext*>& contexts(GRAPHICS->getContexts());
        std::for_each(contexts.cbegin(), contexts.cend(), [&](GLContext* context)
        {
            initVAO(context);
        });
        GRAPHICS->ContextCreated += m_ContextCreatedHandler;
        GRAPHICS->ContextRemoved += m_ContextRemovedHandler;
    }
}

void ModelMesh::initVAO( GLContext* context )
{
    GRAPHICS->setActiveContext(context);
    const BufferLayout::layout& elements(m_VertexLayout.getElements());
    //////////////////////////////////////////////////////////////////////////
    ///                             Normal                                 ///
    //////////////////////////////////////////////////////////////////////////
    const uint32 contextID(context->getID());
    HE_IF_ASSERT(m_VaoID[contextID] == UINT32_MAX, "vao already inited?")
    {
        glGenVertexArrays(1, m_VaoID + contextID);
        GL::heBindVao(m_VaoID[contextID]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexVboID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexVboID);

        std::for_each(elements.cbegin(), elements.cend(), [&](const BufferElement& e)
        {
            GLint components = 1;
            GLenum type = 0;
            GL::getGLTypesFromBufferElement(e, components, type);
            glVertexAttribPointer(e.getElementIndex(), components, type, GL_FALSE, m_VertexLayout.getSize(), 
                BUFFER_OFFSET(e.getByteOffset())); 
            glEnableVertexAttribArray(e.getElementIndex());
        });
    }
    //////////////////////////////////////////////////////////////////////////
    ///                             Shadow                                 ///
    //////////////////////////////////////////////////////////////////////////
    HE_IF_ASSERT(m_VaoShadowID[contextID] == UINT32_MAX, "shadow vao already inited?")
    {
        uint32 posOffset = UINT32_MAX;
        uint32 boneIdOffset = UINT32_MAX;
        uint32 boneWeightOffset = UINT32_MAX;
        std::for_each(elements.cbegin(), elements.cend(), [&](const BufferElement& e)
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

        glGenVertexArrays(1, m_VaoShadowID + contextID);
        GL::heBindVao(m_VaoShadowID[contextID]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexVboID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexVboID);
        if (boneIdOffset == UINT32_MAX)
        {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_VertexLayout.getSize(), BUFFER_OFFSET(posOffset)); 
            glEnableVertexAttribArray(0);
        }
        else
        {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_VertexLayout.getSize(), BUFFER_OFFSET(posOffset)); 
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, m_VertexLayout.getSize(), BUFFER_OFFSET(boneIdOffset)); 
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, m_VertexLayout.getSize(), BUFFER_OFFSET(boneWeightOffset)); 
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
        }
    }
}
void ModelMesh::destroyVAO( GLContext* context )
{
    GRAPHICS->setActiveContext(context);
    const uint32 contextID(context->getID());
    HE_IF_ASSERT(m_VaoID[contextID] != UINT32_MAX, "Vao not initialized or already destroyed")
    {
        glDeleteVertexArrays(1, m_VaoID + contextID);
        m_VaoID[contextID] = UINT32_MAX;
    }
    HE_IF_ASSERT(m_VaoShadowID[contextID] != UINT32_MAX, "Shadow Vao not initialized or already destroyed")
    {
        glDeleteVertexArrays(1, m_VaoShadowID + contextID);
        m_VaoShadowID[contextID] = UINT32_MAX;
    }
}


//Calling glBufferData with a NULL pointer before uploading new data can improve performance (tells the driver you don't care about the old cts)
void ModelMesh::setVertices(const void* const vertices, const uint32 num, const MeshUsage usage, const bool calcBound)
{
    m_NumVertices = num;

    if (calcBound == true)
    {
        uint32 posOffset = UINT32_MAX;
        std::for_each(m_VertexLayout.getElements().cbegin(), m_VertexLayout.getElements().cend(), [&](const BufferElement& e)
        {
            if (e.getUsage() == gfx::BufferElement::Usage_Position)
            {
                posOffset = e.getByteOffset();
            }
        });
        m_Bound.fromAABB(AABB::calculateBoundingAABB(vertices, num, m_VertexLayout.getSize(), posOffset));
    }

    GL::heBindVao(getVertexArraysID());
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexVboID);
    glBufferData(GL_ARRAY_BUFFER, m_VertexLayout.getSize() * num, nullptr, usage);
    glBufferData(GL_ARRAY_BUFFER, m_VertexLayout.getSize() * num, vertices, usage);
}
void ModelMesh::setIndices(const void* const indices, const uint32 num, const IndexStride type, const MeshUsage usage)
{
    m_NumIndices = num;

    GL::heBindVao(getVertexArraysID());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexVboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, type * num, nullptr, usage);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, type * num, indices, usage);

    switch (type)
    {
        case IndexStride_Byte: m_IndexType = GL_UNSIGNED_BYTE; break;
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

void ModelMesh::callbackOnceIfLoaded( const boost::function<void()>& callback )
{
    m_LoadMutex.lock(FILE_AND_LINE);
    if (m_IsLoaded)
    {
        m_LoadMutex.unlock(); //we don't know how long callback will take, and it is not necessary to keep the lock
        callback();
    }
    else
    {
        eventCallback0<void> handler(callback);
        Loaded += handler;
        m_LoadMutex.unlock();
    }
}

void ModelMesh::setLoaded()
{
    m_IsLoaded = true;
    m_LoadMutex.lock(FILE_AND_LINE);
    Loaded();
    Loaded.clear();
    m_LoadMutex.unlock();
}

void ModelMesh::createPickingData(const void* const vertices, const size_t vertexCount, const BufferLayout& vertexLayout, const void* const indices, const size_t indexCount, const IndexStride indexStride)
{
    HE_IF_ASSERT(m_PickingData.m_Vertices == nullptr && m_PickingData.m_Indices == nullptr, "Picking data already initialized!")
    {
        m_PickingData.m_TriangleCount = indexCount / 3;
        m_PickingData.m_Vertices = static_cast<vec3*>(he_malloc(sizeof(vec3) * vertexCount));
        m_PickingData.m_Indices = he_malloc(indexStride * indexCount);
        m_PickingData.m_IndexStride = indexStride;
        he_memcpy(m_PickingData.m_Indices, indices, indexStride * indexCount);

        uint32 posOffset(UINT32_MAX);
        std::for_each(vertexLayout.getElements().cbegin(), vertexLayout.getElements().cend(), [&](const BufferElement& e)
        {
            if (e.getUsage() == gfx::BufferElement::Usage_Position)
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

} } //end namespace
