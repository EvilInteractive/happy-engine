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
//Created: 07/12/2011
#include "HappyPCH.h" 

#include "InstancingController.h"
#include "CameraManager.h"
#include "GraphicsEngine.h"
#include "OpenGL.h"
#include "I3DObject.h"
#include "DynamicBuffer.h"
#include "IInstancible.h"
#include "IInstanceFiller.h"

#include "ModelMesh.h"
#include "Material.h"
#include "Scene.h"
#include "Game.h"

namespace he {
namespace gfx {

#define BUFFER_OFFSET(i) ((char*)nullptr + (i))

#pragma warning(disable:4355) // use of this in initializer list
InstancingController::InstancingController(const std::string& name, bool dynamic, const ObjectHandle& modelHandle, const ObjectHandle& material):
    m_Dynamic(dynamic), m_pModelMesh(nullptr), m_NeedsUpdate(false), m_BufferCapacity(32),
    m_ManualMode(false), m_Name(name), m_Material(nullptr), m_Scene(nullptr), m_Bound(AABB(vec3(-1, -1, -1), vec3(1, 1, 1))),
    m_ContextCreatedHandler(boost::bind(&InstancingController::initVao, this, _1)),
    m_ContextRemovedHandler(boost::bind(&InstancingController::destroyVao, this, _1))
{
    he_memset(m_Vao, 0xffff, MAX_VERTEX_ARRAY_OBJECTS * sizeof(VaoID));
    he_memset(m_ShadowVao, 0xffff, MAX_VERTEX_ARRAY_OBJECTS * sizeof(VaoID));

    ResourceFactory<Material>::getInstance()->instantiate(material);
    m_Material = ResourceFactory<Material>::getInstance()->get(material);
    m_InstancingLayout = BufferLayout(m_Material->getCompatibleInstancingLayout());
    m_CpuBuffer = details::InstancingBuffer(m_Material->getCompatibleInstancingLayout().getSize(), 32);

    ResourceFactory<ModelMesh>::getInstance()->instantiate(modelHandle);
    m_pModelMesh = ResourceFactory<ModelMesh>::getInstance()->get(modelHandle);
    ResourceFactory<ModelMesh>::getInstance()->get(modelHandle)->callbackOnceIfLoaded(boost::bind(&InstancingController::init, this));
}
#pragma warning(default:4355)


InstancingController::~InstancingController()
{
    GRAPHICS->ContextCreated -= m_ContextCreatedHandler;
    GRAPHICS->ContextRemoved -= m_ContextRemovedHandler;
    const std::vector<GLContext*>& contexts(GRAPHICS->getContexts());
    std::for_each(contexts.cbegin(), contexts.cend(), [&](GLContext* context)
    {
        destroyVao(context);
    });
    glDeleteBuffers(1, &m_GpuBuffer);
    m_pModelMesh->release();
    m_Material->release();
    GAME->removeFromTickList(this);
    if (isAttachedToScene())
        detachFromScene();
}


void InstancingController::initVao( GLContext* context )
{
    GRAPHICS->setActiveContext(context);
    const BufferLayout::layout& vertexElements(m_pModelMesh->getVertexLayout().getElements());
    //////////////////////////////////////////////////////////////////////////
    ///  Regular Draw
    //////////////////////////////////////////////////////////////////////////
    HE_IF_ASSERT(m_Vao[context->id] == UINT_MAX, "vao already inited?")
    {
        glGenVertexArrays(1, m_Vao + context->id);
        GL::heBindVao(m_Vao[context->id]);
        //////////////////////////////////////////////////////////////////////////
        ///  Vertex Buffer
        //////////////////////////////////////////////////////////////////////////
        glBindBuffer(GL_ARRAY_BUFFER, m_pModelMesh->getVBOID());
        const BufferLayout::layout& vertexElements(m_pModelMesh->getVertexLayout().getElements());
        std::for_each(vertexElements.cbegin(), vertexElements.cend(), [&](const BufferElement& e)
        {
            GLint components = 1;
            GLenum type = 0;
            GL::getGLTypesFromBufferElement(e, components, type);
            glVertexAttribPointer(e.getElementIndex(), components, type, 
                GL_FALSE, m_pModelMesh->getVertexLayout().getSize(), 
                BUFFER_OFFSET(e.getByteOffset())); 
            glEnableVertexAttribArray(e.getElementIndex());
        });

        //////////////////////////////////////////////////////////////////////////
        ///  Index Buffer
        //////////////////////////////////////////////////////////////////////////
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pModelMesh->getVBOIndexID());

        //////////////////////////////////////////////////////////////////////////
        ///  Instance Buffer
        //////////////////////////////////////////////////////////////////////////
        glBindBuffer(GL_ARRAY_BUFFER, m_GpuBuffer);

        std::for_each(m_InstancingLayout.getElements().cbegin(), m_InstancingLayout.getElements().cend(), [&](const BufferElement& element)
        {
            GLint components(1);
            GLenum type(0);
            GL::getGLTypesFromBufferElement(element, components, type);
            glEnableVertexAttribArray(vertexElements.size() + element.getElementIndex());
            glVertexAttribPointer(vertexElements.size() + element.getElementIndex(), components, type, 
                GL_FALSE, m_InstancingLayout.getSize(), BUFFER_OFFSET(element.getByteOffset())); 
            glVertexAttribDivisor(vertexElements.size() + element.getElementIndex(), 1);
        });
    }
    //////////////////////////////////////////////////////////////////////////
    ///  Shadow Draw
    //////////////////////////////////////////////////////////////////////////
    HE_IF_ASSERT(m_ShadowVao[context->id] == UINT_MAX, "shadow vao already inited?")
    {
        uint posOffset(UINT_MAX);
        BufferLayout::layout::const_iterator it(vertexElements.cbegin());
        for(; it != vertexElements.cend(); ++it)
        {
            if (it->getUsage() == gfx::BufferElement::Usage_Position)
            {
                posOffset = it->getByteOffset();
                break;
            }
        }

        glGenVertexArrays(1, m_ShadowVao + context->id);
        GL::heBindVao(m_ShadowVao[context->id]);

        //////////////////////////////////////////////////////////////////////////
        ///  Vertex Buffer
        //////////////////////////////////////////////////////////////////////////
        glBindBuffer(GL_ARRAY_BUFFER, m_pModelMesh->getVBOID());
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_pModelMesh->getVertexLayout().getSize(), BUFFER_OFFSET(posOffset)); 
        glEnableVertexAttribArray(0);

        //////////////////////////////////////////////////////////////////////////
        ///  Index Buffer
        //////////////////////////////////////////////////////////////////////////
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pModelMesh->getVBOIndexID());

        //////////////////////////////////////////////////////////////////////////
        ///  Instance Buffer
        //////////////////////////////////////////////////////////////////////////
        glBindBuffer(GL_ARRAY_BUFFER, m_GpuBuffer);
        glEnableVertexAttribArray(1 + 0);
        glEnableVertexAttribArray(1 + 1);
        glEnableVertexAttribArray(1 + 2);
        glEnableVertexAttribArray(1 + 3);
        glVertexAttribPointer(1 + 0, 4, GL_FLOAT, GL_FALSE, sizeof(mat44), BUFFER_OFFSET(sizeof(vec4) * 0)); 
        glVertexAttribPointer(1 + 1, 4, GL_FLOAT, GL_FALSE, sizeof(mat44), BUFFER_OFFSET(sizeof(vec4) * 1)); 
        glVertexAttribPointer(1 + 2, 4, GL_FLOAT, GL_FALSE, sizeof(mat44), BUFFER_OFFSET(sizeof(vec4) * 2)); 
        glVertexAttribPointer(1 + 3, 4, GL_FLOAT, GL_FALSE, sizeof(mat44), BUFFER_OFFSET(sizeof(vec4) * 3)); 
        glVertexAttribDivisor(1 + 0, 1);
        glVertexAttribDivisor(1 + 1, 1);
        glVertexAttribDivisor(1 + 2, 1);
        glVertexAttribDivisor(1 + 3, 1);
    }
}

void InstancingController::destroyVao( GLContext* context )
{
    GRAPHICS->setActiveContext(context);
    HE_IF_ASSERT(m_Vao[context->id] != UINT_MAX, "Vao not initialized or already destroyed")
    {
        glDeleteVertexArrays(1, m_Vao + context->id);
        m_Vao[context->id] = UINT_MAX;
    }
    HE_IF_ASSERT(m_ShadowVao[context->id] != UINT_MAX, "Shadow Vao not initialized or already destroyed")
    {
        glDeleteVertexArrays(1, m_ShadowVao + context->id);
        m_ShadowVao[context->id] = UINT_MAX;
    }
}
void InstancingController::init()
{
    GAME->addToTickList(this);

    //////////////////////////////////////////////////////////////////////////
    ///  Instance Buffer
    //////////////////////////////////////////////////////////////////////////
    glGenBuffers(1, &m_GpuBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_GpuBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_CpuBuffer.getSizeCapacity(), 0, m_Dynamic?GL_STREAM_DRAW:GL_STATIC_DRAW);


    const std::vector<GLContext*>& contexts(GRAPHICS->getContexts());
    std::for_each(contexts.cbegin(), contexts.cend(), [&](GLContext* context)
    {
        initVao(context);
    });
    GRAPHICS->ContextCreated += m_ContextCreatedHandler;
    GRAPHICS->ContextRemoved += m_ContextRemovedHandler;
}

void InstancingController::updateBuffer()
{
    if ((m_Dynamic || m_NeedsUpdate) && isAttachedToScene())
    {
        PROFILER_BEGIN("InstancingController::updateBuffer");

        glBindBuffer(GL_ARRAY_BUFFER, m_GpuBuffer);
        if (m_BufferCapacity < m_Instances.size()) // GL buffer to small -> enlarge
        {
            while(m_BufferCapacity < m_Instances.size())
                m_BufferCapacity *= 2;
            m_CpuBuffer.resize(m_BufferCapacity * m_InstancingLayout.getSize());
            glBufferData(GL_ARRAY_BUFFER, m_CpuBuffer.getSizeCapacity(), 0, m_Dynamic?GL_STREAM_DRAW:GL_STATIC_DRAW);

            HE_INFO("Increasing instancing controller's (%s) capacity to %d", m_Name.c_str(), (int)m_BufferCapacity);
        }

        m_CpuBuffer.reset();
        AABB newBound;
        if (m_ManualMode == false)
        {
            DynamicBuffer b(m_InstancingLayout);
            he::for_each(m_Instances.cbegin(), m_Instances.cend(), [&](const IInstancible* pObj)
            {
                b.setBuffer(m_CpuBuffer.addItem());
                pObj->fillInstancingBuffer(b);
                newBound.merge(pObj->getBound().getAABB());
            });
        }
        else
        {
            std::for_each(m_ManualCpuBufferFillers.cbegin(), m_ManualCpuBufferFillers.cend(), [&](IInstanceFiller* pFiller)
            {
                pFiller->fillInstancingBuffer(m_CpuBuffer);
                newBound.merge(pFiller->getAABB());
            });
        }
        m_Bound.fromAABB(newBound);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_CpuBuffer.getSize(), m_CpuBuffer.getSize() > 0 ? m_CpuBuffer.getBuffer() : 0);

        m_NeedsUpdate = false;
        m_PrevUpdateTime = boost::chrono::high_resolution_clock::now();
        PROFILER_END();

        getScene()->forceReevalute(this);
    }
}

void InstancingController::draw()
{
    GL::heBindVao(m_Vao[GL::s_CurrentContext->id]);
    glDrawElementsInstanced(GL_TRIANGLES, m_pModelMesh->getNumIndices(), m_pModelMesh->getIndexType(), BUFFER_OFFSET(0), m_CpuBuffer.getCount());
}
void InstancingController::drawShadow()
{
    GL::heBindVao(m_ShadowVao[GL::s_CurrentContext->id]);
    glDrawElementsInstanced(GL_TRIANGLES, m_pModelMesh->getNumIndices(), m_pModelMesh->getIndexType(), BUFFER_OFFSET(0), m_CpuBuffer.getCount());
}



uint InstancingController::addInstance(const IInstancible* pObj)
{
    m_NeedsUpdate = true;
    return m_Instances.insert(pObj);
}
uint InstancingController::addInstance()
{
    HE_ASSERT(m_ManualCpuBufferFillers.empty() == false, "Only valid in manual mode");
    m_NeedsUpdate = true;
    return m_Instances.insert(nullptr); // HACK: is a bit hacky
}


void InstancingController::removeInstance( uint id )
{
    m_Instances.remove(id);
    m_NeedsUpdate = true;
}

const Material* InstancingController::getMaterial() const
{
    return m_Material;
}
const ModelMesh* InstancingController::getModelMesh() const
{
    return m_pModelMesh;
}

void InstancingController::applyMaterial(const ICamera* pCamera) const
{
    m_Material->apply(this, pCamera);
}
void InstancingController::applyMaterial( const Material* customMaterial, const ICamera* pCamera ) const
{
    customMaterial->apply(this, pCamera);
}

bool InstancingController::getCastsShadow() const
{
    return m_CastShadows;
}

void InstancingController::setCastsShadow( bool castShadow )
{
    m_CastShadows = castShadow;
}

uint InstancingController::getCount() const
{
    return m_CpuBuffer.getCount();
}

void InstancingController::addManualFiller( IInstanceFiller* pFiller )
{
    HE_ASSERT(std::find(m_ManualCpuBufferFillers.cbegin(), m_ManualCpuBufferFillers.cend(), pFiller) == m_ManualCpuBufferFillers.cend(), "filler is already bound to this controller");
    m_ManualMode = true;
    m_ManualCpuBufferFillers.push_back(pFiller);
}

void InstancingController::removeManualFiller( const IInstanceFiller* pFiller )
{
    m_ManualCpuBufferFillers.erase(std::remove(m_ManualCpuBufferFillers.begin(), m_ManualCpuBufferFillers.end(),
        pFiller), m_ManualCpuBufferFillers.end());
}

void InstancingController::tick( float /*dTime*/ )
{
    updateBuffer();
}

void InstancingController::detachFromScene()
{
    HE_IF_ASSERT(isAttachedToScene() == true, "Drawable not attached to scene when detaching")
    {
        m_Scene->detachFromScene(this);
    }
}

void InstancingController::attachToScene( Scene* scene, bool /*autoReevalute*/ )
{
    HE_IF_ASSERT(isAttachedToScene() == false, "Drawable already attached to scene when attaching")
    {
        m_Scene = scene;
        m_Scene->attachToScene(this, false);
    }
}

void InstancingController::setScene( Scene* scene )
{
    m_Scene = scene;
}
Scene* InstancingController::getScene() const
{
    return m_Scene;
}

bool InstancingController::isAttachedToScene() const
{
    return m_Scene != nullptr;
}

void InstancingController::calculateBound()
{

}



} } //end namespace