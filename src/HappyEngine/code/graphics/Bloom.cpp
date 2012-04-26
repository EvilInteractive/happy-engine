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
//Created: 16/10/2011

#include "HappyPCH.h" 

#include "Bloom.h"
#include "ContentManager.h"
#include "GraphicsEngine.h"
#include "Vertex.h"

#include "ExternalError.h"

#include "Texture2D.h"
#include "ModelMesh.h"

namespace he {
namespace gfx {

Bloom::Bloom(): m_DownSamples(4),
                m_pDownSampleShader(NEW Shader()), m_pDownSampleBrightPassShader(NEW Shader()),
                m_pMesh(nullptr)
{
}


Bloom::~Bloom()
{
    for (int pass = 0; pass < 2; ++pass)
    {
        for (int i(0); i < m_DownSamples; ++i)
        {
            m_Texture[pass][i]->release();
        }
        std::for_each(m_FboId[pass].cbegin(), m_FboId[pass].cend(), [](const uint& id)
        {
            glDeleteFramebuffers(1, &id);
        });
    }
    if (m_pMesh != nullptr)
        m_pMesh->release();
}

void Bloom::init(bool hdr)
{
    m_Hdr = hdr;
    //////////////////////////////////////////////////////////////////////////
    ///                             Shaders                                ///
    //////////////////////////////////////////////////////////////////////////
    ShaderLayout layout;
    layout.addElement(ShaderLayoutElement(0, "inPosition"));

    const std::string& folder(CONTENT->getShaderFolderPath().str());

    std::set<std::string> defineBrightPass;
    defineBrightPass.insert("BRIGHTPASS");
    if (hdr)
        defineBrightPass.insert("HDR");
    m_pDownSampleBrightPassShader->initFromFile(folder + "shared/postShaderQuad.vert", 
                                                folder + "post/bloom.frag", layout, defineBrightPass);
    m_DownSampleBrightPassMap = m_pDownSampleBrightPassShader->getShaderSamplerId("map");

    if (hdr)
        m_DownSampleBrightPassLumMap = m_pDownSampleBrightPassShader->getShaderSamplerId("lumMap");

    m_pDownSampleShader->initFromFile(folder + "shared/postShaderQuad.vert", 
                                      folder + "post/bloom.frag", layout);
    m_DownSampleMap = m_pDownSampleShader->getShaderSamplerId("map");

    for (int pass = 0; pass < 2; ++pass)
    {
        m_pBlurShaderPass[pass] = Shader::pointer(NEW Shader());

        std::set<std::string> definePass;
        if (pass == 0)
            definePass.insert("PASS1");
        else
            definePass.insert("PASS2");

        m_pBlurShaderPass[pass]->initFromFile(folder + "shared/postShaderQuad.vert", 
                                              folder + "post/gaussblur.frag", layout, definePass);
        m_BlurMapPos[pass] = m_pBlurShaderPass[pass]->getShaderSamplerId("map");
    }

    //////////////////////////////////////////////////////////////////////////
    ///                             Quad                                   ///
    //////////////////////////////////////////////////////////////////////////
    m_pMesh = CONTENT->getFullscreenQuad();

    resize();
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
        m_Texture[pass].resize(m_DownSamples);

        for (int i = 0; i < m_DownSamples; ++i)
        {
            ObjectHandle handle(ResourceFactory<Texture2D>::getInstance()->create());
            m_Texture[pass][i] = ResourceFactory<Texture2D>::getInstance()->get(handle);
            m_Texture[pass][i]->setName("Bloom::m_Texture[pass][i]");
            m_Texture[pass][i]->setData(GRAPHICS->getScreenWidth() / ((i+1) * 2), GRAPHICS->getScreenHeight() / ((i+1) * 2), 
                gfx::Texture2D::TextureFormat_RGBA16F, 0, 
                gfx::Texture2D::BufferLayout_RGBA, gfx::Texture2D::BufferType_Float,
                gfx::Texture2D::WrapType_Clamp,  gfx::Texture2D::FilterType_Linear, false, false );
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
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture[pass][i]->getID(), 0);       
        }
    }
}

void Bloom::render( const Texture2D* pTexture, const Texture2D* pLumMap )
{
    HE_ASSERT(m_Hdr == true && pLumMap != nullptr || m_Hdr == false && pLumMap == nullptr, "no valid lumMap provided");

    GL::heBindVao(m_pMesh->getVertexArraysID());

    //BrightPass
    GL::heBindFbo(m_FboId[0][0]);
    m_pDownSampleBrightPassShader->bind();
    m_pDownSampleBrightPassShader->setShaderVar(m_DownSampleBrightPassMap, pTexture);
    if (m_Hdr)
        m_pDownSampleBrightPassShader->setShaderVar(m_DownSampleBrightPassLumMap, pLumMap);
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
}

const Texture2D* Bloom::getBloom( byte level ) const
{
    return m_Texture[0][level];
}



} } //end namespace