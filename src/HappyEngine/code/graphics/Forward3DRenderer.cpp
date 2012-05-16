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
//Created: 06/11/2011

#include "HappyPCH.h" 

#include "Forward3DRenderer.h"
#include "GraphicsEngine.h"
#include "Deferred3DRenderer.h"
#include "Renderer2D.h"
#include "ContentManager.h"
#include "Shader.h"
#include "IDrawable.h"
#include "CameraManager.h"
#include "Camera.h"
#include "Renderer2D.h"

namespace he {
namespace gfx {

const char* vertQuadShader = 
"#version 150 core                                              \n"
"                                                               \n"
"in vec3 inPosition;                                            \n"
"                                                               \n"
"noperspective out vec2 texCoord;                               \n"
"                                                               \n"
"void main()                                                    \n"
"{                                                              \n"
"    gl_Position = vec4(inPosition, 1.0f);                      \n"
"    texCoord = vec2(-inPosition.x, inPosition.y) * 0.5 + 0.5f; \n"
"}                                                              \n";

const char* fragQuadShader = 
"#version 150 core                              \n"
"                                               \n"
"noperspective in vec2 texCoord;                \n"
"out vec4 outColor;                             \n"
"                                               \n"
"uniform sampler2D colorTex;                    \n"
"                                               \n"
"void main()                                    \n"
"{                                              \n"
"    vec4 color = texture(colorTex, texCoord);  \n"
"    if (color.a < 0.01f)                       \n"
"        discard;                               \n"
"    outColor = color;                          \n"
"}                                              \n";


/* CONSTRUCTOR - DESCTRUCTOR */
Forward3DRenderer::Forward3DRenderer():
    m_FboId(UINT_MAX), m_pQuadShader(nullptr),
    m_pOutColorTexture(nullptr), m_pOutDepthTexture(nullptr), m_pOutNormalTexture(nullptr),
    m_pQuad(nullptr)
{
}

Forward3DRenderer::~Forward3DRenderer()
{
    if (m_pOutColorTexture != nullptr)
        m_pOutColorTexture->release();
    if (m_pOutNormalTexture != nullptr)
        m_pOutNormalTexture->release();
    if (m_pOutDepthTexture != nullptr)
        m_pOutDepthTexture->release();
    if (m_pQuad != nullptr)
        m_pQuad->release();
    if (m_FboId != 0 && m_FboId != UINT_MAX)
        glDeleteFramebuffers(1, &m_FboId);
    delete m_pQuadShader;
}

void Forward3DRenderer::init( const RenderSettings& settings, 
    const Texture2D* pOutTarget, const Texture2D* pOutNormalTarget, const Texture2D* pOutDepthTarget )
{
    m_pQuad = CONTENT->getFullscreenQuad();

    m_pOutColorTexture = pOutTarget;
    m_OwnsColorBuffer = m_pOutColorTexture == nullptr;

    m_pOutNormalTexture = pOutNormalTarget;
    m_pOutDepthTexture = pOutDepthTarget;

    if (m_pOutColorTexture != nullptr)
        ResourceFactory<Texture2D>::getInstance()->instantiate(m_pOutColorTexture->getHandle());
    if (m_pOutNormalTexture != nullptr)
        ResourceFactory<Texture2D>::getInstance()->instantiate(m_pOutNormalTexture->getHandle());
    if (m_pOutDepthTexture != nullptr)
        ResourceFactory<Texture2D>::getInstance()->instantiate(m_pOutDepthTexture->getHandle());

    initFbo();
    setRenderSettings(settings);
}
void Forward3DRenderer::initFbo()
{
    HE_ASSERT(m_pOutDepthTexture != nullptr, "Please supply a valid depthBuffer");
    if (m_OwnsColorBuffer)
    {
        if (m_pOutColorTexture == nullptr)
        {
            ObjectHandle handle(ResourceFactory<Texture2D>::getInstance()->create());
            m_pOutColorTexture = ResourceFactory<Texture2D>::getInstance()->get(handle);
            ResourceFactory<Texture2D>::getInstance()->get(handle)->setName("Forward3DRenderer::m_pOutColorTexture");
        }
        ResourceFactory<Texture2D>::getInstance()->get(m_pOutColorTexture->getHandle())->setData(
            m_pOutDepthTexture->getWidth(), m_pOutDepthTexture->getHeight(), 
            gfx::Texture2D::TextureFormat_RGBA8, 0, 
            gfx::Texture2D::BufferLayout_BGRA, gfx::Texture2D::BufferType_Byte,
            gfx::Texture2D::WrapType_Clamp,  gfx::Texture2D::FilterType_Nearest, false, false );
    }

    if ((m_FboId == 0 || m_FboId == UINT_MAX) == false)
        glDeleteFramebuffers(1, &m_FboId);

    m_DrawBuffers[0] = GL_COLOR_ATTACHMENT0;
    m_DrawBuffers[1] = GL_COLOR_ATTACHMENT1;

    glGenFramebuffers(1, &m_FboId);
    GL::heBindFbo(m_FboId);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pOutColorTexture->getID(),  0);
    if (m_pOutNormalTexture != nullptr)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_pOutNormalTexture->getID(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, m_pOutDepthTexture->getID(),  0);
    err::checkFboStatus("forward");
}
void Forward3DRenderer::compileShaders()
{
    delete m_pQuadShader;
    m_pQuadShader = nullptr;
    if (m_OwnsColorBuffer)
    {
        ShaderLayout layout;
        layout.addElement(ShaderLayoutElement(0, "inPosition"));

        m_pQuadShader = NEW Shader();
        m_pQuadShader->initFromMem(vertQuadShader, fragQuadShader, layout, "Forward3DRenderer.cpp-QuadShaderVert", "Forward3DRenderer.cpp-QuadShaderFrag");
        m_QuadShaderTexPos = m_pQuadShader->getShaderSamplerId("colorTex");
    }
}

void Forward3DRenderer::setRenderSettings( const RenderSettings& settings )
{
    m_Settings = settings;
    compileShaders();
}

void Forward3DRenderer::onScreenResized()
{
    initFbo();
}

void Forward3DRenderer::draw( const DrawListContainer& drawList, uint renderFlags )
{
    GL::heBindFbo(m_FboId);
    GL::heSetCullFace(false);
    GL::heSetDepthFunc(DepthFunc_LessOrEqual);
    GL::heSetDepthRead(true);
    GL::heSetDepthWrite(true);
    GL::heBlendEnabled(false);
    if (m_OwnsColorBuffer)
    {
        const static GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, m_DrawBuffers);
        GL::heClearColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
        glClear(GL_COLOR_BUFFER_BIT);
    }
    glDrawBuffers(m_pOutNormalTexture == nullptr? 1 : 2, m_DrawBuffers);

    drawList.for_each(renderFlags &~DrawListContainer::F_Main_Blended, [](IDrawable* pDrawable)
    {
        if (pDrawable->isInCamera(CAMERAMANAGER->getActiveCamera()))
        {
            pDrawable->applyMaterial(CAMERAMANAGER->getActiveCamera());
            pDrawable->draw();
        }
    });

    GL::heBlendEnabled(true);
    drawList.for_each(renderFlags &~DrawListContainer::F_Main_Opac, [](IDrawable* pDrawable)
    {
        if (pDrawable->isInCamera(CAMERAMANAGER->getActiveCamera()))
        {
            pDrawable->applyMaterial(CAMERAMANAGER->getActiveCamera());
            pDrawable->draw();
        }
    });

    // Render on back buffer
    if (m_OwnsColorBuffer)
    {
        /*GL::heBindFbo(0);
        GL::heSetDepthRead(false);
        GL::heSetDepthWrite(false);
        GL::heBlendEnabled(true);
        GL::heBlendEquation(BlendEquation_Add);
        GL::heBlendFunc(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);
        const static GLenum buffers[1] = { GL_BACK_LEFT };
        glDrawBuffers(1, buffers);

        m_pQuadShader->bind();
        m_pQuadShader->setShaderVar(m_QuadShaderTexPos, m_pOutColorTexture);

        GL::heBindVao(m_pQuad->getVertexArraysID());
        glDrawElements(GL_TRIANGLES, m_pQuad->getNumIndices(), m_pQuad->getIndexType(), 0);*/

        GUI_NEW->drawTexture2DToScreen(m_pOutColorTexture, vec2(), true, vec2((float)m_pOutColorTexture->getWidth() * -1.0f, (float)m_pOutColorTexture->getHeight()));
    }
}

void Forward3DRenderer::clear( bool color, bool normal, bool depth )
{
    GL::heBindFbo(m_FboId);

    int numBuffers(0);
    GLenum buffers[2];
    if (color)
        buffers[numBuffers++] = GL_COLOR_ATTACHMENT0;
    if (normal)
        buffers[numBuffers++] = GL_COLOR_ATTACHMENT1;

    glDrawBuffers(numBuffers, buffers);
    GL::heClearColor(Color(0.0f, 0.0f, 0.0f, 0.0f));

    GLbitfield flags(0);
    if (color || normal)
        flags |= GL_COLOR_BUFFER_BIT;
    if (depth)
    {
        GL::heSetDepthWrite(true);
        flags |= GL_DEPTH_BUFFER_BIT;
    }

    glClear(flags);
}

} } //end namespace