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
//Created: 07/12/2011
#include "HappyPCH.h" 

#include "InstancingController.h"
#include "CameraManager.h"
#include "GraphicsEngine.h"
#include "OpenGL.h"
#include "DynamicBuffer.h"
#include "IInstancible.h"
#include "IInstanceFiller.h"

#include "ModelMesh.h"
#include "Material.h"
#include "Scene.h"
#include "Game.h"

#include "ContentManager.h"
#include "Model.h"

namespace he {
namespace gfx {

#define BUFFER_OFFSET(i) ((char*)nullptr + (i))

#pragma warning(disable:4355) // use of this in initializer list
InstancingController::InstancingController(const he::String& name, bool dynamic, const ObjectHandle& meshHandle, const ObjectHandle& material):
    m_Dynamic(dynamic), m_ModelMesh(nullptr), m_NeedsUpdate(false), m_BufferCapacity(32),
    m_ManualMode(false), m_Name(name), m_Material(nullptr), m_Scene(nullptr), m_Bound(AABB(vec3(-1, -1, -1), vec3(1, 1, 1))),
    m_ContextCreatedHandler(boost::bind(&InstancingController::initVao, this, _1)),
    m_ContextRemovedHandler(boost::bind(&InstancingController::destroyVao, this, _1)), m_AttachedToScene(false)
{
    he_memset(m_Vao, 0xffff, MAX_VERTEX_ARRAY_OBJECTS * sizeof(VaoID));
    he_memset(m_ShadowVao, 0xffff, MAX_VERTEX_ARRAY_OBJECTS * sizeof(VaoID));

    ResourceFactory<Material>::getInstance()->instantiate(material);
    m_Material = ResourceFactory<Material>::getInstance()->get(material);
    m_InstancingLayout = BufferLayout(m_Material->getCompatibleInstancingLayout());
    m_CpuBuffer = details::InstancingBuffer(m_Material->getCompatibleInstancingLayout().getSize(), 32);

    m_ModelMesh = ResourceFactory<ModelMesh>::getInstance()->get(meshHandle);
    m_ModelMesh->instantiate();
    m_ModelMesh->callbackOnceIfLoaded(this, boost::bind(&InstancingController::init, this));
}

InstancingController::InstancingController( const he::String& name, bool dynamic, const he::String& materialAsset, const he::String& modelAsset, const he::String& mesh ):
    m_Dynamic(dynamic), m_ModelMesh(nullptr), m_NeedsUpdate(false), m_BufferCapacity(32),
    m_ManualMode(false), m_Name(name), m_Material(nullptr), m_Scene(nullptr), m_Bound(AABB(vec3(-1, -1, -1), vec3(1, 1, 1))),
    m_ContextCreatedHandler(boost::bind(&InstancingController::initVao, this, _1)),
    m_ContextRemovedHandler(boost::bind(&InstancingController::destroyVao, this, _1)), m_AttachedToScene(false)
{
        he_memset(m_Vao, 0xffff, MAX_VERTEX_ARRAY_OBJECTS * sizeof(VaoID));
        he_memset(m_ShadowVao, 0xffff, MAX_VERTEX_ARRAY_OBJECTS * sizeof(VaoID));

        m_Material = ResourceFactory<Material>::getInstance()->get(CONTENT->loadMaterial(materialAsset));
        m_InstancingLayout = BufferLayout(m_Material->getCompatibleInstancingLayout());
        m_CpuBuffer = details::InstancingBuffer(m_Material->getCompatibleInstancingLayout().getSize(), 32);

        m_ModelMesh = CONTENT->asyncLoadModelMesh(modelAsset, mesh, m_Material->getCompatibleVertexLayout());
        m_ModelMesh->callbackOnceIfLoaded(this, [this](const ELoadResult result)
        {
            if (result == eLoadResult_Success)
            {
                this->init();
            }
            else
            {
                m_ModelMesh->release();
                m_ModelMesh = nullptr;
            }
        });
}

#pragma warning(default:4355)


InstancingController::~InstancingController()
{
    if (isAttachedToScene())
        detachFromScene();

    if (m_ModelMesh != nullptr)
    {
        GAME->removeFromTickList(this);

        GRAPHICS->ContextCreated -= m_ContextCreatedHandler;
        GRAPHICS->ContextRemoved -= m_ContextRemovedHandler;
        const he::PrimitiveList<GLContext*>& contexts(GRAPHICS->getContexts());
        std::for_each(contexts.cbegin(), contexts.cend(), [&](GLContext* context)
        {
            destroyVao(context);
        });
        glDeleteBuffers(1, &m_GpuBuffer);

        m_ModelMesh->cancelLoadCallback(this);
        m_ModelMesh->release();
    }
    if (m_Material != nullptr)
    {
        m_Material->release();
    }
}


void InstancingController::initVao( GLContext* context )
{
    GRAPHICS->setActiveContext(context);
    const BufferLayout::layout& vertexElements(m_ModelMesh->getVertexLayout().getElements());
    //////////////////////////////////////////////////////////////////////////
    ///  Regular Draw
    //////////////////////////////////////////////////////////////////////////
    const uint32 contextID(context->getID());
    HE_IF_ASSERT(m_Vao[contextID] == UINT32_MAX, "vao already inited?")
    {
        glGenVertexArrays(1, m_Vao + contextID);
        GL::heBindVao(m_Vao[contextID]);
        //////////////////////////////////////////////////////////////////////////
        ///  Vertex Buffer
        //////////////////////////////////////////////////////////////////////////
        glBindBuffer(GL_ARRAY_BUFFER, m_ModelMesh->getVBOID());
        const BufferLayout::layout& vertexElements(m_ModelMesh->getVertexLayout().getElements());
        std::for_each(vertexElements.cbegin(), vertexElements.cend(), [&](const BufferElement& e)
        {
            GLint components = 1;
            GLenum type = 0;
            GL::getGLTypesFromBufferElement(e, components, type);
            glVertexAttribPointer(e.getElementIndex(), components, type, 
                GL_FALSE, m_ModelMesh->getVertexLayout().getSize(), 
                BUFFER_OFFSET(e.getByteOffset())); 
            glEnableVertexAttribArray(e.getElementIndex());
        });

        //////////////////////////////////////////////////////////////////////////
        ///  Index Buffer
        //////////////////////////////////////////////////////////////////////////
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ModelMesh->getVBOIndexID());

        //////////////////////////////////////////////////////////////////////////
        ///  Instance Buffer
        //////////////////////////////////////////////////////////////////////////
        glBindBuffer(GL_ARRAY_BUFFER, m_GpuBuffer);

        std::for_each(m_InstancingLayout.getElements().cbegin(), m_InstancingLayout.getElements().cend(), [&](const BufferElement& element)
        {
            GLint components(1);
            GLenum type(0);
            GL::getGLTypesFromBufferElement(element, components, type);
            glEnableVertexAttribArray(static_cast<GLsizei>(vertexElements.size()) + element.getElementIndex());
            glVertexAttribPointer(static_cast<GLsizei>(vertexElements.size()) + element.getElementIndex(), components, type, 
                GL_FALSE, m_InstancingLayout.getSize(), BUFFER_OFFSET(element.getByteOffset())); 
            glVertexAttribDivisor(static_cast<GLsizei>(vertexElements.size()) + element.getElementIndex(), 1);
        });
    }
    //////////////////////////////////////////////////////////////////////////
    ///  Shadow Draw
    //////////////////////////////////////////////////////////////////////////
    HE_IF_ASSERT(m_ShadowVao[context->getID()] == UINT32_MAX, "shadow vao already inited?")
    {
        uint32 posOffset(UINT32_MAX);
        BufferLayout::layout::const_iterator it(vertexElements.cbegin());
        for(; it != vertexElements.cend(); ++it)
        {
            if (it->getUsage() == gfx::BufferElement::Usage_Position)
            {
                posOffset = it->getByteOffset();
                break;
            }
        }

        glGenVertexArrays(1, m_ShadowVao + contextID);
        GL::heBindVao(m_ShadowVao[contextID]);

        //////////////////////////////////////////////////////////////////////////
        ///  Vertex Buffer
        //////////////////////////////////////////////////////////////////////////
        glBindBuffer(GL_ARRAY_BUFFER, m_ModelMesh->getVBOID());
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_ModelMesh->getVertexLayout().getSize(), BUFFER_OFFSET(posOffset)); 
        glEnableVertexAttribArray(0);

        //////////////////////////////////////////////////////////////////////////
        ///  Index Buffer
        //////////////////////////////////////////////////////////////////////////
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ModelMesh->getVBOIndexID());

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
    const uint32 contextID(context->getID());
    HE_IF_ASSERT(m_Vao[contextID] != UINT32_MAX, "Vao not initialized or already destroyed")
    {
        glDeleteVertexArrays(1, m_Vao + contextID);
        m_Vao[contextID] = UINT32_MAX;
    }
    HE_IF_ASSERT(m_ShadowVao[contextID] != UINT32_MAX, "Shadow Vao not initialized or already destroyed")
    {
        glDeleteVertexArrays(1, m_ShadowVao + contextID);
        m_ShadowVao[contextID] = UINT32_MAX;
    }
}
void InstancingController::init()
{
    if (m_Scene != nullptr)
    {
        m_AttachedToScene = true;
        m_Scene->attachToScene(this);
    }

    GAME->addToTickList(this);

    //////////////////////////////////////////////////////////////////////////
    ///  Instance Buffer
    //////////////////////////////////////////////////////////////////////////
    glGenBuffers(1, &m_GpuBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_GpuBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_CpuBuffer.getSizeCapacity(), 0, m_Dynamic?GL_STREAM_DRAW:GL_STATIC_DRAW);


    const he::PrimitiveList<GLContext*>& contexts(GRAPHICS->getContexts());
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

                // TODO: optimize this
                const he::AABB& aabb(getModelMesh()->getBound().getAABB());
                const mat44& world(pObj->getWorldMatrix());
                AABB newAABB(world * aabb.getTopFrontLeft(),
                             world * aabb.getBottomBackRight());
                newBound.merge(newAABB);
            });
        }
        else
        {
            m_ManualCpuBufferFillers.forEach([&](const IInstanceFiller* filler)
            {
                filler->fillInstancingBuffer(m_CpuBuffer);
                newBound.merge(filler->getAABB());
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
    GL::heBindVao(m_Vao[GL::s_CurrentContext->getID()]);
    glDrawElementsInstanced(GL_TRIANGLES, m_ModelMesh->getNumIndices(), m_ModelMesh->getIndexType(), BUFFER_OFFSET(0), m_CpuBuffer.getCount());
}
void InstancingController::drawShadow()
{
    GL::heBindVao(m_ShadowVao[GL::s_CurrentContext->getID()]);
    glDrawElementsInstanced(GL_TRIANGLES, m_ModelMesh->getNumIndices(), m_ModelMesh->getIndexType(), BUFFER_OFFSET(0), m_CpuBuffer.getCount());
}



uint32 InstancingController::addInstance(const IInstancible* pObj)
{
    m_NeedsUpdate = true;
    return static_cast<uint32>(m_Instances.insert(pObj));
}
uint32 InstancingController::addInstance()
{
    HE_ASSERT(m_ManualCpuBufferFillers.empty() == false, "Only valid in manual mode");
    m_NeedsUpdate = true;
    return static_cast<uint32>(m_Instances.insert(nullptr)); // HACK: is a bit hacky
}


void InstancingController::removeInstance( uint32 id )
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
    return m_ModelMesh;
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

uint32 InstancingController::getCount() const
{
    return m_CpuBuffer.getCount();
}

void InstancingController::addManualFiller( const IInstanceFiller* filler )
{
    HE_IF_ASSERT(m_ManualCpuBufferFillers.contains(filler) == false, "filler is already bound to this controller")
    {
        m_ManualMode = true;
        m_ManualCpuBufferFillers.add(filler);
    }
}

void InstancingController::removeManualFiller( const IInstanceFiller* filler )
{
    m_ManualCpuBufferFillers.remove(filler);
}

void InstancingController::tick( float /*dTime*/ )
{
    updateBuffer();
}

void InstancingController::detachFromScene()
{
    HE_IF_ASSERT(m_Scene != nullptr, "Drawable not attached to scene when detaching")
    {
        if (m_AttachedToScene)
        {
            m_Scene->detachFromScene(this);
            m_Scene = nullptr;
            m_AttachedToScene = false;
        }
    }
}

void InstancingController::attachToScene( Scene* scene )
{
    HE_IF_ASSERT(m_Scene == nullptr, "Drawable already attached to scene when attaching")
    {
        m_Scene = scene;
        if (m_AttachedToScene == false && m_ModelMesh != nullptr && m_ModelMesh->isLoaded())
        {
            m_AttachedToScene = true;
            m_Scene->attachToScene(this);
        }
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
    return m_Scene != nullptr && m_AttachedToScene;
}

void InstancingController::calculateBound()
{

}



} } //end namespace