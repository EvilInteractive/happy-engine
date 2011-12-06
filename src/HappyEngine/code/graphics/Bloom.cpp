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
//Author:  Bastian Damman
//Created: 16/10/2011

#include "HappyPCH.h" 

#include "Bloom.h"
#include "HappyNew.h"
#include "HappyEngine.h"
#include "ContentManager.h"
#include "GraphicsEngine.h"
#include "Vertex.h"

#include "OpenGL.h"
#include "ExternalError.h"

namespace he {
namespace gfx {

Bloom::Bloom(): m_DownSamples(4),
                m_pDownSampleShader(NEW Shader()), m_pDownSampleBrightPassShader(NEW Shader())
{
}


Bloom::~Bloom()
{
    for (int pass = 0; pass < 2; ++pass)
    {
        std::for_each(m_FboId[pass].cbegin(), m_FboId[pass].cend(), [](const uint& id)
        {
            glDeleteFramebuffers(1, &id);
        });
    }
}

void Bloom::init()
{
    //////////////////////////////////////////////////////////////////////////
    ///                             Shaders                                ///
    //////////////////////////////////////////////////////////////////////////
    ShaderLayout layout;
    layout.addElement(ShaderLayoutElement(0, "inPosition"));

    std::string folder(CONTENT->getRootDir() + CONTENT->getShaderFolder());

    std::set<std::string> defineBrightPass;
    defineBrightPass.insert("BRIGHTPASS");
    m_pDownSampleBrightPassShader->init(folder + "deferred/post/deferredPostShaderQuad.vert", 
                                        folder +  "deferred/post/downSample.frag", layout, defineBrightPass);
    m_DownSampleBrightPassMap = m_pDownSampleBrightPassShader->getShaderSamplerId("map");
    m_DownSampleBrightPassLumMap = m_pDownSampleBrightPassShader->getShaderSamplerId("lumMap");

    m_pDownSampleShader->init(folder + "deferred/post/deferredPostShaderQuad.vert", 
                              folder + "deferred/post/downSample.frag", layout);
    m_DownSampleMap = m_pDownSampleShader->getShaderSamplerId("map");

    for (int pass = 0; pass < 2; ++pass)
    {
        m_pBlurShaderPass[pass] = Shader::pointer(NEW Shader());

        std::set<std::string> definePass;
        if (pass == 0)
            definePass.insert("PASS1");
        else
            definePass.insert("PASS2");

        m_pBlurShaderPass[pass]->init(folder + "deferred/post/deferredPostShaderQuad.vert", 
                                      folder + "deferred/post/blur.frag", layout, definePass);
        m_BlurMapPos[pass] = m_pBlurShaderPass[pass]->getShaderSamplerId("map");
    }

    //////////////////////////////////////////////////////////////////////////
    ///                             Quad                                   ///
    //////////////////////////////////////////////////////////////////////////
    m_pMesh = CONTENT->getFullscreenQuad();
}

void Bloom::resize()
{
    for (int pass = 0; pass < 2; ++pass)
    {
        std::for_each(m_FboId[pass].cbegin(), m_FboId[pass].cend(), [](const uint& id)
        {
            glDeleteFramebuffers(1, &id);
        });
    }
    for (int pass = 0; pass < 2; ++pass)
    {
        //////////////////////////////////////////////////////////////////////////
        ///                             Textures                               ///
        //////////////////////////////////////////////////////////////////////////
        std::vector<uint> downSampleTextureId[2];
        downSampleTextureId[pass].resize(m_DownSamples);
        m_Texture[pass].resize(m_DownSamples);

        glGenTextures(m_DownSamples, &downSampleTextureId[pass][0]);
        for (int i = 0; i < m_DownSamples; ++i)
        {
            GL::heBindTexture2D(0, downSampleTextureId[pass][i]);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 
                GRAPHICS->getScreenWidth() / ((i+1) * 2), GRAPHICS->getScreenHeight() / ((i+1) * 2), 
                0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
            m_Texture[pass][i] = Texture2D::pointer(NEW Texture2D());
            m_Texture[pass][i]->init(downSampleTextureId[pass][i], GRAPHICS->getScreenWidth() / ((i+1) * 2), GRAPHICS->getScreenHeight() / ((i+1) * 2), GL_RGBA16F);
        }

        //////////////////////////////////////////////////////////////////////////
        ///                               Fbo's                                ///
        //////////////////////////////////////////////////////////////////////////

        //Downsample fbo's
        m_FboId[pass].resize(m_DownSamples);
        glGenFramebuffers(m_DownSamples, &m_FboId[pass][0]);

        for (int i = 0; i < m_DownSamples; ++i)
        {
            GL::heBindFbo(m_FboId[pass][i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, downSampleTextureId[pass][i], 0);       
        }
    }
}

void Bloom::render( const Texture2D::pointer& pTexture, const Texture2D::pointer& lumMap )
{
    PROFILER_BEGIN("Bloom::render");
    GL::heBindVao(m_pMesh->getVertexArraysID());

    //BrightPass
    GL::heBindFbo(m_FboId[0][0]);
    m_pDownSampleBrightPassShader->bind();
    m_pDownSampleBrightPassShader->setShaderVar(m_DownSampleBrightPassMap, pTexture);
    m_pDownSampleBrightPassShader->setShaderVar(m_DownSampleBrightPassLumMap, lumMap);
    GRAPHICS->setViewport(he::RectI(0, 0, (int)m_Texture[0][0]->getWidth(), (int)m_Texture[0][0]->getHeight()));
    glDrawElements(GL_TRIANGLES, m_pMesh->getNumIndices(), m_pMesh->getIndexType(), 0);

    //DownSample further
    m_pDownSampleShader->bind();
    for (uint fboId = 1; fboId < m_FboId[0].size(); ++fboId)
    {
        GL::heBindFbo(m_FboId[0][fboId]);
        m_pDownSampleShader->setShaderVar(m_DownSampleMap, m_Texture[0][fboId - 1]);
        GRAPHICS->setViewport(he::RectI(0, 0, (int)m_Texture[0][fboId]->getWidth(), (int)m_Texture[0][fboId]->getHeight()));

        glDrawElements(GL_TRIANGLES, m_pMesh->getNumIndices(), m_pMesh->getIndexType(), 0);
    }

    //Blur
    for (int pass = 0; pass < 2; ++pass)
    {
        m_pBlurShaderPass[pass]->bind();
        for (uint fboId = 1; fboId < m_FboId[pass].size(); ++fboId)
        {
            GL::heBindFbo(m_FboId[pass == 0?1:0][fboId]);
            m_pBlurShaderPass[pass]->setShaderVar(m_BlurMapPos[pass], m_Texture[pass][fboId]);
            GRAPHICS->setViewport(he::RectI(0, 0, (int)m_Texture[pass == 0?1:0][fboId]->getWidth(), (int)m_Texture[pass == 0?1:0][fboId]->getHeight()));
            glDrawElements(GL_TRIANGLES, m_pMesh->getNumIndices(), m_pMesh->getIndexType(), 0);
        }
    }
    GRAPHICS->setViewport(he::RectI(0, 0, GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight()));
    PROFILER_END("Bloom::render");
}

const Texture2D::pointer& Bloom::getBloom( byte level ) const
{
    return m_Texture[0][level];
}



} } //end namespace