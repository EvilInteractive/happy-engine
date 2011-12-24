//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
#include "OpenGL.h"
#include "HappyEngine.h"
#include "GraphicsEngine.h"
#include "Deferred3DRenderer.h"
#include "Happy2DRenderer.h"
#include "ExternalError.h"
#include "ContentManager.h"
#include "Shader.h"
#include "IDrawable.h"
#include "CameraManager.h"
#include "Camera.h"

namespace he {
namespace gfx {

const char* vertQuadShader = 
"#version 150 core                              \n"
"                                               \n"
"in vec3 inPosition;                            \n"
"                                               \n"
"noperspective out vec2 texCoord;               \n"
"                                               \n"
"void main()                                    \n"
"{                                              \n"
"    gl_Position = vec4(inPosition, 1.0f);      \n"
"    texCoord = inPosition.xy * 0.5 + 0.5f;     \n"
"}                                              \n";

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
"    if (color.a < 0.2f)                        \n"
"        discard;                               \n"
"    outColor = vec4(color.rgb, 1.0f);          \n"
"}                                              \n";


/* CONSTRUCTOR - DESCTRUCTOR */
Forward3DRenderer::Forward3DRenderer(): m_pOutColorTexture(nullptr), m_pOutNormalTexture(nullptr), m_pOutDepthTexture(nullptr),
    m_FboId(UINT_MAX), m_pQuadShader(nullptr)
{
}

Forward3DRenderer::~Forward3DRenderer()
{
    if (m_FboId != 0 || m_FboId != UINT_MAX)
        glDeleteFramebuffers(1, &m_FboId);
    if (m_OwnsColorBuffer)
        delete m_pOutColorTexture;
    delete m_pQuadShader;
}

void Forward3DRenderer::init( const RenderSettings& settings, 
    const Texture2D* pOutTarget, const Texture2D* pOutNormalTarget, const Texture2D* pOutDepthTarget )
{
    m_pQuad = CONTENT->getFullscreenQuad();

    m_pOutColorTexture = pOutTarget;
    if (m_pOutColorTexture == nullptr)
    {
        m_OwnsColorBuffer = true;
    }
    else
    {
        m_OwnsColorBuffer = false;
    }

    m_pOutNormalTexture = pOutNormalTarget;
    m_pOutDepthTexture = pOutDepthTarget;

    initFbo();
    setRenderSettings(settings);
}
void Forward3DRenderer::initFbo()
{
    ASSERT(m_pOutDepthTexture != nullptr, "Please supply a valid depthBuffer");
    if (m_OwnsColorBuffer)
    {
        uint colorId;
        glGenTextures(1, &colorId);
        GL::heBindTexture2D(0, colorId);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_pOutDepthTexture->getWidth(), m_pOutDepthTexture->getHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
        //const cast to keep code clean
        if (m_pOutColorTexture == nullptr)
            m_pOutColorTexture = NEW Texture2D();
        const_cast<Texture2D*>(m_pOutColorTexture)->init(colorId, m_pOutDepthTexture->getWidth(), m_pOutDepthTexture->getHeight(), GL_RGBA8);
    }

    if (m_FboId != 0 || m_FboId != UINT_MAX)
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

    drawList.for_each(renderFlags, [](IDrawable* pDrawable)
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
        GL::heBindFbo(0);
        GL::heSetDepthRead(false);
        GL::heSetDepthWrite(false);
        const static GLenum buffers[1] = { GL_BACK_LEFT };
        glDrawBuffers(1, buffers);

        m_pQuadShader->bind();
        m_pQuadShader->setShaderVar(m_QuadShaderTexPos, m_pOutColorTexture);

        GL::heBindVao(m_pQuad->getVertexArraysID());
        glDrawElements(GL_TRIANGLES, m_pQuad->getNumIndices(), m_pQuad->getIndexType(), 0);
    }
}

void Forward3DRenderer::clear( bool color, bool normal, bool depth )
{
    GL::heBindFbo(m_FboId);

    int numBuffers = 0;
    GLenum buffers[2];
    if (color)
        buffers[numBuffers++] = GL_COLOR_ATTACHMENT0;
    if (normal)
        buffers[numBuffers++] = GL_COLOR_ATTACHMENT1;

    glDrawBuffers(numBuffers, m_DrawBuffers);
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