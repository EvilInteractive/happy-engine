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
    m_IndexVboID(UINT_MAX),
    m_VertexVboID(UINT_MAX)
{
    he_memset(m_VaoID, 0xff, MAX_VERTEX_ARRAY_OBJECTS * sizeof(VaoID));
    he_memset(m_VaoShadowID, 0xff, MAX_VERTEX_ARRAY_OBJECTS * sizeof(VaoID));
}
#pragma warning(default:4355) // use of this in initializer list


ModelMesh::~ModelMesh()
{
    GRAPHICS->ContextCreated -= m_ContextCreatedHandler;
    GRAPHICS->ContextRemoved -= m_ContextRemovedHandler;
    const std::vector<GLContext*>& contexts(GRAPHICS->getContexts());
    std::for_each(contexts.cbegin(), contexts.cend(), [&](GLContext* context)
    {
        destroyVAO(context);
    });
    if (m_VertexVboID != UINT_MAX)
        glDeleteBuffers(1, &m_VertexVboID);
    if (m_IndexVboID != UINT_MAX)
        glDeleteBuffers(1, &m_IndexVboID);
}

void ModelMesh::init(const BufferLayout& vertexLayout, MeshDrawMode mode)
{
    HE_IF_ASSERT(m_VertexVboID == UINT_MAX, "Only init ModelMesh once!")
    {
        m_VertexLayout = vertexLayout;
        m_DrawMode = mode;
        glGenBuffers(1, &m_IndexVboID);
        glGenBuffers(1, &m_VertexVboID);
        const std::vector<GLContext*>& contexts(GRAPHICS->getContexts());
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
    HE_IF_ASSERT(m_VaoID[context->id] == UINT_MAX, "vao already inited?")
    {
        glGenVertexArrays(1, m_VaoID + context->id);
        GL::heBindVao(m_VaoID[context->id]);
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
    HE_IF_ASSERT(m_VaoShadowID[context->id] == UINT_MAX, "shadow vao already inited?")
    {
        uint posOffset = UINT_MAX;
        uint boneIdOffset = UINT_MAX;
        uint boneWeightOffset = UINT_MAX;
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

        glGenVertexArrays(1, m_VaoShadowID + context->id);
        GL::heBindVao(m_VaoShadowID[context->id]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexVboID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexVboID);
        if (boneIdOffset == UINT_MAX)
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
    HE_IF_ASSERT(m_VaoID[context->id] != UINT_MAX, "Vao not initialized or already destroyed")
    {
        glDeleteVertexArrays(1, m_VaoID + context->id);
        m_VaoID[context->id] = UINT_MAX;
    }
    HE_IF_ASSERT(m_VaoShadowID[context->id] != UINT_MAX, "Shadow Vao not initialized or already destroyed")
    {
        glDeleteVertexArrays(1, m_VaoShadowID + context->id);
        m_VaoShadowID[context->id] = UINT_MAX;
    }
}


//Calling glBufferData with a NULL pointer before uploading new data can improve performance (tells the driver you don't care about the old cts)
void ModelMesh::setVertices(const void* pVertices, uint num, MeshUsage usage)
{
    m_NumVertices = num;

    uint posOffset = UINT_MAX;
    std::for_each(m_VertexLayout.getElements().cbegin(), m_VertexLayout.getElements().cend(), [&](const BufferElement& e)
    {
        if (e.getUsage() == gfx::BufferElement::Usage_Position)
        {
            posOffset = e.getByteOffset();
        }
    });
    m_Bound.fromAABB(AABB::calculateBoundingAABB(pVertices, num, m_VertexLayout.getSize(), posOffset));

    GL::heBindVao(getVertexArraysID());
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexVboID);
    glBufferData(GL_ARRAY_BUFFER, m_VertexLayout.getSize() * num, nullptr, usage);
    glBufferData(GL_ARRAY_BUFFER, m_VertexLayout.getSize() * num, pVertices, usage);
}
void ModelMesh::setIndices(const void* pIndices, uint num, IndexStride type, MeshUsage usage)
{
    m_NumIndices = num;

    GL::heBindVao(getVertexArraysID());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexVboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, type * num, nullptr, usage);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, type * num, pIndices, usage);

    switch (type)
    {
        case IndexStride_Byte: m_IndexType = GL_UNSIGNED_BYTE; break;
        case IndexStride_UShort: m_IndexType = GL_UNSIGNED_SHORT; break;
        case IndexStride_UInt: m_IndexType = GL_UNSIGNED_INT; break;
        default: HE_ASSERT(false, "unknown type: %d", type); break;
    }
}

void ModelMesh::setBones( const std::vector<Bone>& boneList )
{
    m_BoneList = boneList;
}

void ModelMesh::callbackOnceIfLoaded( const boost::function<void()>& callback )
{
    m_LoadMutex.lock();
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
    m_LoadMutex.lock();
    Loaded();
    Loaded.clear();
    m_LoadMutex.unlock();
}

} } //end namespace
