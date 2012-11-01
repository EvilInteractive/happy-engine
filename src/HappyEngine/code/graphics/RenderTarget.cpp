//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 18/07/2012
#include "HappyPCH.h" 

#include "RenderTarget.h"

#include "Texture2D.h"
#include "GraphicsEngine.h"

namespace he {
namespace gfx {

RenderTarget::RenderTarget(GLContext* context)
    : m_FboId(0)
    , m_DepthBuffer(UINT_MAX)
    , m_DepthTarget(nullptr)
    , m_Width(0)
    , m_Height(0)
    , m_DrawBufferCount(0)
    , m_Context(context)
{
}


RenderTarget::~RenderTarget()
{
    if (m_FboId != UINT_MAX && m_FboId != 0)
    {
        GRAPHICS->setActiveContext(m_Context);
        glDeleteFramebuffers(1, &m_FboId);
    }
    removeAllTargets();
}

void RenderTarget::addTextureTarget( const Texture2D* tex2D )
{
    HE_IF_ASSERT(m_TextureTargets.size() < MAX_TEXTURE_TARGETS, "Only %d targets are allowed in a TextureTarget!", MAX_TEXTURE_TARGETS)
    HE_IF_ASSERT(m_Width == 0 || m_Width == tex2D->getWidth() && m_Height == tex2D->getHeight(), "Textures assigned to a RenderTarget must be of equal size!")
    {
        ResourceFactory<Texture2D>::getInstance()->instantiate(tex2D->getHandle());
        m_TextureTargets.push_back(tex2D);
        m_Width = tex2D->getWidth();
        m_Height = tex2D->getHeight();
    }
}

void RenderTarget::switchTextureTarget( uint index, const Texture2D* tex2D )
{
    HE_IF_ASSERT(index < m_TextureTargets.size(), "index out of range (%d - %d)", index, m_TextureTargets.size())
    HE_IF_ASSERT(m_FboId != 0 && m_FboId != UINT_MAX, "Call init first or invalid fbo (m_Fbo == 0?)")
    HE_IF_ASSERT(m_Context == GL::s_CurrentContext, "Access violation, wrong context is bound")
    {
        if (m_TextureTargets[index]->getID() != tex2D->getID())
        {
            ResourceFactory<Texture2D>* texFactory(ResourceFactory<Texture2D>::getInstance());
            m_TextureTargets[index]->release();
            texFactory->instantiate(tex2D->getHandle());
            m_TextureTargets[index] = tex2D;
            GL::heBindFbo(m_FboId);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, tex2D->getID(), 0);
        }
    }
}

void RenderTarget::setDepthTarget( const Texture2D* tex2D )
{
    HE_IF_ASSERT(m_DepthBuffer == UINT_MAX, "Depth was already assigned - ignoring")    
    HE_IF_ASSERT(m_Width == 0 || m_Width == tex2D->getWidth() && m_Height == tex2D->getHeight(), "Textures assigned to a RenderTarget must be of equal size!")
    {
        ResourceFactory<Texture2D>::getInstance()->instantiate(tex2D->getHandle());
        m_DepthBuffer = tex2D->getID();
        m_DepthTarget = tex2D;
        m_Width = tex2D->getWidth();
        m_Height = tex2D->getHeight();
    }
}

void RenderTarget::setDepthTarget()
{
    glGenRenderbuffers(1, &m_DepthBuffer);
    m_DepthTarget = nullptr;
}
void RenderTarget::init()
{
    GRAPHICS->setActiveContext(m_Context);
    if (m_TextureTargets.size() == 0 && m_DepthTarget == nullptr && m_DepthBuffer == UINT_MAX)
    {
        m_DrawBufferCount = 1;
        m_DrawBuffers[0] = GL_BACK;
    }
    else
    {
        HE_IF_ASSERT(m_Width != 0 && m_Height != 0, "Initing fbo with unknown size!")
        {
            if (m_FboId != UINT_MAX && m_FboId != 0)
                glDeleteFramebuffers(1, &m_FboId);

            glGenFramebuffers(1, &m_FboId);
            GL::heBindFbo(m_FboId);

            for (uint i(0); i < m_TextureTargets.size(); ++i)
            {
                m_DrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_TextureTargets[i]->getID(), 0);
            }
            if (m_DepthBuffer != UINT_MAX)
            {
                if (m_DepthTarget != nullptr)
                {
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthBuffer, 0);
                }
                else
                {
                    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
                    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
                    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);
                }
            }
            m_DrawBufferCount = m_TextureTargets.size();
            err::checkFboStatus("RenderTarget");
        }
    }
}


void RenderTarget::clear( const Color& color ) const
{
    GL::heBindFbo(m_FboId);
    glDrawBuffers(m_DrawBufferCount, m_DrawBuffers);
    GL::heSetDepthWrite(true);
    GL::heClearColor(color);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTarget::prepareForRendering() const
{
    prepareForRendering(m_DrawBufferCount);
}

void RenderTarget::prepareForRendering( uint numTextureTargets, uint offset) const
{
    HE_ASSERT(m_Context == GL::s_CurrentContext, "Access violation: wrong context is bound!");
    GL::heBindFbo(m_FboId);
    glDrawBuffers(numTextureTargets, m_DrawBuffers + offset);
}

const Texture2D* RenderTarget::getTextureTarget(uint index) const
{
    HE_ASSERT(index < m_TextureTargets.size(), "Index out of bounds");
    return m_TextureTargets[index];
}

const Texture2D* RenderTarget::getDepthTarget() const
{
    return m_DepthTarget;
}

void RenderTarget::removeAllTargets()
{
    std::for_each(m_TextureTargets.cbegin(), m_TextureTargets.cend(), [](const Texture2D* tex)
    {
        tex->release();
    });
    if (m_DepthBuffer != UINT_MAX && m_DepthTarget == nullptr)
        glDeleteRenderbuffers(1, &m_DepthBuffer);
    if (m_DepthTarget != nullptr)
        m_DepthTarget->release();

    m_DepthTarget = nullptr;
    m_TextureTargets.clear();
    m_DepthBuffer = UINT_MAX;
    m_Width = 0;
    m_Height = 0;
    m_DrawBufferCount = 0;
}

void RenderTarget::setSize( uint width, uint height )
{
    HE_IF_ASSERT(m_Width == 0 && m_Height == 0, "Size is already assigned, I cannot change this on the fly")
    {
        m_Width = width;
        m_Height = height;
    }
}


} } //end namespace
