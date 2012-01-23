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
#include "HappyEngine.h"
#include "CameraManager.h"
#include "GraphicsEngine.h"
#include "OpenGL.h"
#include "I3DObject.h"
#include "DynamicBuffer.h"
#include "IInstancible.h"
#include "IInstanceFiller.h"

namespace he {
namespace gfx {

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

InstancingController::InstancingController(bool dynamic, const ModelMesh::pointer& mesh, const Material& material):
    m_Dynamic(dynamic), m_pModelMesh(mesh), m_Material(material), m_NeedsUpdate(false), m_BufferCapacity(32),
    m_InstancingLayout(material.getCompatibleInstancingLayout()), m_CpuBuffer(material.getCompatibleInstancingLayout().getSize(), 32),
    m_ManualMode(false)
{
    m_pModelMesh->callbackIfLoaded(boost::bind(&InstancingController::init, this));
}


InstancingController::~InstancingController()
{
    glDeleteVertexArrays(1, &m_Vao);
    glDeleteBuffers(1, &m_GpuBuffer);
}


void InstancingController::init()
{
    //////////////////////////////////////////////////////////////////////////
    ///  Regular Draw
    //////////////////////////////////////////////////////////////////////////

        glGenVertexArrays(1, &m_Vao);
        GL::heBindVao(m_Vao);
        //////////////////////////////////////////////////////////////////////////
        ///  Vertex Buffer
        //////////////////////////////////////////////////////////////////////////
        glBindBuffer(GL_ARRAY_BUFFER, m_pModelMesh->getVBOID());
        BufferLayout::layout vertexElements(m_pModelMesh->getVertexLayout().getElements());
        std::for_each(vertexElements.cbegin(), vertexElements.cend(), [&](const BufferElement& e)
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
                default: ASSERT(false, "unknown/unsupported attribute type for instancing"); break;
                #pragma warning(default:4127)
            }
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
        glGenBuffers(1, &m_GpuBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_GpuBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_CpuBuffer.getSizeCapacity(), 0, m_Dynamic?GL_STREAM_DRAW:GL_STATIC_DRAW);

        std::for_each(m_InstancingLayout.getElements().cbegin(), m_InstancingLayout.getElements().cend(), [&](const BufferElement& element)
        {
            GLint components(1);
            GLenum type(0);
            switch (element.getType())
            {
                case BufferElement::Type_Vec2: type = GL_FLOAT; components = 2; break;
                case BufferElement::Type_Vec3: type = GL_FLOAT; components = 3; break;
                case BufferElement::Type_Vec4: type = GL_FLOAT; components = 4; break;
                case BufferElement::Type_Float: type = GL_FLOAT; break;

                case BufferElement::Type_Int: type = GL_INT; break;
                case BufferElement::Type_IVec4: type = GL_INT; components = 4; break;
                case BufferElement::Type_UInt: type = GL_UNSIGNED_INT; break;

                #pragma warning(disable:4127)
                default: ASSERT(false, "unknown/unsupported attribute type for instancing"); break;
                #pragma warning(default:4127)
            }
            glEnableVertexAttribArray(vertexElements.size() + element.getElementIndex());
            glVertexAttribPointer(vertexElements.size() + element.getElementIndex(), components, type, 
                                  GL_FALSE, m_InstancingLayout.getSize(), BUFFER_OFFSET(element.getByteOffset())); 
            glVertexAttribDivisor(vertexElements.size() + element.getElementIndex(), 1);
        });

    //////////////////////////////////////////////////////////////////////////
    ///  Shadow Draw
    //////////////////////////////////////////////////////////////////////////
        glGenVertexArrays(1, &m_ShadowVao);
        GL::heBindVao(m_ShadowVao);

        //////////////////////////////////////////////////////////////////////////
        ///  Vertex Buffer
        //////////////////////////////////////////////////////////////////////////
        glBindBuffer(GL_ARRAY_BUFFER, m_pModelMesh->getVBOShadowID());
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), BUFFER_OFFSET(0)); 
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


    GL::heBindVao(0);

    GRAPHICS->addToDrawList(this);
}

void InstancingController::updateBuffer()
{
    // only need to update once a frame
    boost::chrono::high_resolution_clock::duration elapsedTime(boost::chrono::high_resolution_clock::now() - m_PrevUpdateTime);

    if ((m_Dynamic || m_NeedsUpdate) && elapsedTime.count() / static_cast<float>(boost::nano::den) * 1000 > 12.0f) //12ms ~62.5fps
    {
        PROFILER_BEGIN("InstancingController::updateBuffer");

        glBindBuffer(GL_ARRAY_BUFFER, m_GpuBuffer);
        if (m_BufferCapacity < m_Instances.size()) // GL buffer to small -> enlarge
        {
            while(m_BufferCapacity < m_Instances.size())
                m_BufferCapacity *= 2;
            m_CpuBuffer.resize(m_BufferCapacity * m_InstancingLayout.getSize());
            glBufferData(GL_ARRAY_BUFFER, m_CpuBuffer.getSizeCapacity(), 0, m_Dynamic?GL_STREAM_DRAW:GL_STATIC_DRAW);

            HE_INFO("Increasing instancing controller's capacity to " + itoa(m_BufferCapacity));
        }

        m_CpuBuffer.reset();
        if (m_ManualMode == false)
        {
            DynamicBuffer b(m_InstancingLayout);
            he::for_each(m_Instances.cbegin(), m_Instances.cend(), [&](const IInstancible* pObj)
            {
                //check for culling here
                b.setBuffer(m_CpuBuffer.addItem());
                pObj->fillInstancingBuffer(b);
            });
        }
        else
        {
            std::for_each(m_ManualCpuBufferFillers.cbegin(), m_ManualCpuBufferFillers.cend(), [&](IInstanceFiller* pFiller)
            {
                pFiller->fillInstancingBuffer(m_CpuBuffer);
            });
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_CpuBuffer.getSize(), m_CpuBuffer.getSize() > 0 ? m_CpuBuffer.getBuffer() : 0);

        m_NeedsUpdate = false;
        m_PrevUpdateTime = boost::chrono::high_resolution_clock::now();
        PROFILER_END();
    }
}

void InstancingController::draw()
{
    updateBuffer();

    GL::heBindVao(m_Vao);
    glDrawElementsInstanced(GL_TRIANGLES, m_pModelMesh->getNumIndices(), m_pModelMesh->getIndexType(), BUFFER_OFFSET(0), m_CpuBuffer.getCount());
}
void InstancingController::drawShadow()
{
    updateBuffer();

    GL::heBindVao(m_ShadowVao);
    glDrawElementsInstanced(GL_TRIANGLES, m_pModelMesh->getNumIndices(), m_pModelMesh->getIndexType(), BUFFER_OFFSET(0), m_CpuBuffer.getCount());
}



uint InstancingController::addInstance(const IInstancible* pObj)
{
    m_NeedsUpdate = true;
    return m_Instances.insert(pObj);
}
uint InstancingController::addInstance()
{
    ASSERT(m_ManualCpuBufferFillers.empty() == false, "Only valid in manual mode");
    m_NeedsUpdate = true;
    return m_Instances.insert(nullptr); // HACK: is a bit hacky
}


void InstancingController::removeInstance( uint id )
{
    m_Instances.remove(id);
    m_NeedsUpdate = true;
}

const Material& InstancingController::getMaterial() const
{
    return m_Material;
}
const ModelMesh::pointer& InstancingController::getModelMesh() const
{
    return m_pModelMesh;
}

void InstancingController::applyMaterial(const ICamera* pCamera) const
{
    m_Material.apply(this, pCamera);
}
void InstancingController::applyMaterial( const Material& customMaterial, const ICamera* pCamera ) const
{
    customMaterial.apply(this, pCamera);
}

bool InstancingController::getCastsShadow() const
{
    return m_CastShadows;
}

void InstancingController::setCastsShadow( bool castShadow )
{
    m_CastShadows = castShadow;
}

bool InstancingController::isVisible() const
{
    return m_IsVisible;
}

void InstancingController::setVisible( bool visible )
{
    m_IsVisible = visible;
}

uint InstancingController::getCount() const
{
    return m_CpuBuffer.getCount();
}

void InstancingController::addManualFiller( IInstanceFiller* pFiller )
{
    ASSERT(std::find(m_ManualCpuBufferFillers.cbegin(), m_ManualCpuBufferFillers.cend(), pFiller) == m_ManualCpuBufferFillers.cend(), "filler is already bound to this controller");
    m_ManualMode = true;
    m_ManualCpuBufferFillers.push_back(pFiller);
}

void InstancingController::removeManualFiller( const IInstanceFiller* pFiller )
{
    m_ManualCpuBufferFillers.erase(std::remove(m_ManualCpuBufferFillers.begin(), m_ManualCpuBufferFillers.end(),
        pFiller), m_ManualCpuBufferFillers.end());
}

} } //end namespace