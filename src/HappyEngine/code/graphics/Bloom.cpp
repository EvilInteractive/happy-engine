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

#include "StdAfx.h" 

#include "Bloom.h"
#include "HappyNew.h"
#include "HappyEngine.h"
#include "ContentManager.h"
#include "GraphicsEngine.h"
#include "Vertex.h"

#include "GL/glew.h"
#include "ExternalError.h"

namespace he {
namespace gfx {

Bloom::Bloom(): m_pMesh(NEW ModelMesh("DownSamplerQuad")), m_DownSamples(4),
                m_pDownSampleShader(NEW Shader()), m_pDownSampleBrightPassShader(NEW Shader())
{
    m_pBlurShaderPass[0] = Shader::pointer(NEW Shader());
    m_pBlurShaderPass[1] = Shader::pointer(NEW Shader());
}


Bloom::~Bloom()
{
}

void Bloom::init()
{
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
            glBindTexture(GL_TEXTURE_2D, downSampleTextureId[pass][i]);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 
                GRAPHICS->getScreenWidth() / ((i+1) * 2), GRAPHICS->getScreenHeight() / ((i+1) * 2), 
                0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
            m_Texture[pass][i] = Texture2D::pointer(NEW Texture2D());
            m_Texture[pass][i]->init(downSampleTextureId[pass][i], GRAPHICS->getScreenWidth() / ((i+1) * 2), GRAPHICS->getScreenHeight() / ((i+1) * 2), GL_RGBA16F);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        //////////////////////////////////////////////////////////////////////////
        ///                               Fbo's                                ///
        //////////////////////////////////////////////////////////////////////////

        //Downsample fbo's
        m_FboId[pass].resize(m_DownSamples);
        glGenFramebuffers(m_DownSamples, &m_FboId[pass][0]);

        for (int i = 0; i < m_DownSamples; ++i)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_FboId[pass][i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, downSampleTextureId[pass][i], 0);       
        }
    }
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
    m_DownSampleBrightPassInvScale = m_pDownSampleBrightPassShader->getShaderVarId("invScale");
    m_DownSampleBrightPassExposure = m_pDownSampleBrightPassShader->getShaderVarId("exposure");

    m_pDownSampleShader->init(folder + "deferred/post/deferredPostShaderQuad.vert", 
                              folder + "deferred/post/downSample.frag", layout);
    m_DownSampleMap = m_pDownSampleShader->getShaderSamplerId("map");
    m_DownSampleInvScale = m_pDownSampleShader->getShaderVarId("invScale");

    for (int pass = 0; pass < 2; ++pass)
    {
        std::set<std::string> definePass;
        if (pass == 0)
            definePass.insert("PASS1");
        else
            definePass.insert("PASS2");

        m_pBlurShaderPass[pass]->init(folder + "deferred/post/deferredPostShaderQuad.vert", 
                                      folder + "deferred/post/blur.frag", layout, definePass);
        m_BlurMapPos[pass] = m_pBlurShaderPass[pass]->getShaderSamplerId("map");
        m_BlurTexelSize[pass] = m_pBlurShaderPass[pass]->getShaderVarId("texelSize");
        m_BlurInvScale[pass] = m_pBlurShaderPass[pass]->getShaderVarId("invScale");
    }

    //////////////////////////////////////////////////////////////////////////
    ///                             Quad                                   ///
    //////////////////////////////////////////////////////////////////////////
    std::vector<VertexPos> vertices;
    vertices.push_back(VertexPos(vec3(-1, 1, 1.0f)));
    vertices.push_back(VertexPos(vec3(1, 1, 1.0f)));
    vertices.push_back(VertexPos(vec3(-1, -1, 1.0f)));
    vertices.push_back(VertexPos(vec3(1, -1, 1.0f)));

    std::vector<byte> indices;
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(1); indices.push_back(3); indices.push_back(2);

    VertexLayout vLayout;
    vLayout.addElement(VertexElement(0, VertexElement::Type_Vector3, VertexElement::Usage_Position, 12, 0));

    m_pMesh->init();
    m_pMesh->setVertices(&vertices[0], 4, vLayout);
    m_pMesh->setIndices(&indices[0], 6, IndexStride_Byte);
}

void Bloom::render( const Texture2D::pointer& pTexture, float exposure )
{
    glBindTexture(GL_TEXTURE_2D, pTexture->getID());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(m_pMesh->getVertexArraysID());

    //BrightPass
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboId[0][0]);
    m_pDownSampleBrightPassShader->begin();
    m_pDownSampleBrightPassShader->setShaderVar(m_DownSampleBrightPassMap, pTexture);
    m_pDownSampleBrightPassShader->setShaderVar(m_DownSampleBrightPassInvScale, 2.0f);
    m_pDownSampleBrightPassShader->setShaderVar(m_DownSampleBrightPassExposure, exposure);
    glDrawElements(GL_TRIANGLES, m_pMesh->getNumIndices(), m_pMesh->getIndexType(), 0);
    m_pDownSampleBrightPassShader->end();

    //DownSample further
    m_pDownSampleShader->begin();
    for (uint fboId = 1; fboId < m_FboId[0].size(); ++fboId)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FboId[0][fboId]);
        m_pDownSampleShader->setShaderVar(m_DownSampleMap, m_Texture[0][fboId - 1]);
        m_pDownSampleShader->setShaderVar(m_DownSampleInvScale, (fboId + 1) * 2.0f);

        glDrawElements(GL_TRIANGLES, m_pMesh->getNumIndices(), m_pMesh->getIndexType(), 0);
    }
    m_pDownSampleShader->end();

    //Blur
    for (int pass = 0; pass < 2; ++pass)
    {
        m_pBlurShaderPass[pass]->begin();
        for (uint fboId = 1; fboId < m_FboId[pass].size(); ++fboId)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_FboId[pass == 0?1:0][fboId]);
            m_pBlurShaderPass[pass]->setShaderVar(m_BlurMapPos[pass], m_Texture[pass][fboId]);
            float invScale(((fboId + 1) * 2.0f));
            m_pBlurShaderPass[pass]->setShaderVar(m_BlurTexelSize[pass], 
                vec2(1.0f / (GRAPHICS->getScreenWidth() / invScale), 
                     1.0f / (GRAPHICS->getScreenHeight() / invScale)));
            m_pBlurShaderPass[pass]->setShaderVar(m_BlurInvScale[pass], invScale);
            glDrawElements(GL_TRIANGLES, m_pMesh->getNumIndices(), m_pMesh->getIndexType(), 0);
        }
        m_pBlurShaderPass[pass]->end();
    }

    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const Texture2D::pointer& Bloom::getBloom( byte level ) const
{
    return m_Texture[0][level];
}



} } //end namespace