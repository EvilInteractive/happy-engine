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
//Author:  Sebastiaan Sprengers
//Created: 17/11/2012

#include "HappyPCH.h" 

#include "Canvas2DBuffer.h"
#include "GraphicsEngine.h"
#include "Texture2D.h"

namespace he {
namespace gfx {

Canvas2DBuffer::Canvas2DBuffer() :  frameBufferId(UINT32_MAX),
                                    //depthRenderBufferId(UINT32_MAX),
                                    glContext(nullptr),
                                    renderTextureHandle(ObjectHandle::unassigned),
                                    size(vec2(0,0))

{
}

Canvas2DBuffer::~Canvas2DBuffer()
{
    GRAPHICS->setActiveContext(glContext);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ResourceFactory<Texture2D>::getInstance()->release(renderTextureHandle);

    /*if (depthRenderBufferId != UINT32_MAX)
        glDeleteRenderbuffers(1, &depthRenderBufferId);*/
    if (frameBufferId != UINT32_MAX)
        glDeleteFramebuffers(1, &frameBufferId);

    glContext = nullptr;
    frameBufferId = UINT32_MAX;
    //depthRenderBufferId = UINT32_MAX;
}

/* GENERAL */
void Canvas2DBuffer::init(GLContext* context, const vec2& size)
{
    this->size = size;

    glContext = context;
    GRAPHICS->setActiveContext(glContext);

    // output texture
    renderTextureHandle = ResourceFactory<Texture2D>::getInstance()->create();
    Texture2D* texture(ResourceFactory<Texture2D>::getInstance()->get(renderTextureHandle));

    texture->init(TextureWrapType_Clamp, TextureFilterType_Linear, TextureFormat_RGBA8, false);
    texture->setData(static_cast<uint32>(size.x), static_cast<uint32>(size.y), nullptr, TextureBufferLayout_RGBA, TextureBufferType_Byte, 0);
    texture->setLoadFinished();

    // FBO & RB
    glGenFramebuffers(1, &frameBufferId);
    GL::heBindFbo(frameBufferId);

    // color
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getID(), 0);

    // depth
    /*glGenRenderbuffers(1, &depthRenderBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y));
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBufferId);*/

    GLenum status(glCheckFramebufferStatus(GL_FRAMEBUFFER));
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG(LogType_ProgrammerAssert, "Failed to init Canvas2DBuffer!");
        ResourceFactory<Texture2D>::getInstance()->release(renderTextureHandle);
        glContext = nullptr;
        frameBufferId = UINT32_MAX;
        //depthRenderBufferId = UINT32_MAX;
    }
}

void Canvas2DBuffer::clear()
{
    HE_ASSERT(glContext == GL::s_CurrentContext, "Access Violation: wrong context is bound!");

    GL::heBindFbo(frameBufferId);
    GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffers);
    GL::heClearColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
    GL::heSetDepthWrite(false);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Canvas2DBuffer::resize(const vec2& size)
{
    GRAPHICS->setActiveContext(glContext);

    Texture2D* texture(ResourceFactory<Texture2D>::getInstance()->get(renderTextureHandle));
    texture->setData(static_cast<uint32>(size.x), static_cast<uint32>(size.y), nullptr, gfx::TextureBufferLayout_RGBA, gfx::TextureBufferType_Byte, 0);

    this->size = size;

    //glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBufferId);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y));
}

void Canvas2DBuffer::store()
{
    GRAPHICS->setActiveContext(glContext);

    //glDeleteRenderbuffers(1, &depthRenderBufferId);
    glDeleteFramebuffers(1, &frameBufferId);

    frameBufferId = UINT32_MAX;
    //depthRenderBufferId = UINT32_MAX;
}

}} //end namespace