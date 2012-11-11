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
#include "Shader.h"
#include "View.h"
#include "Window.h"

namespace he {
namespace gfx {

#pragma warning(disable:4355) // use of this in member initializer list
Bloom::Bloom(): m_DownSamples(4),
                m_DownSampleShader(nullptr), 
                m_DownSampleBrightPassShader(nullptr),
                m_Mesh(nullptr), m_Hdr(true),
                m_ViewportSizeChangedHandler(boost::bind(&Bloom::resize, this)),
                m_View(nullptr)
{
    for (uint32 i(0); i < s_BlurPasses; ++i)
    {
        m_BlurShaderPass[i] = nullptr;	
        m_FboId[i].forEach([](uint32& id)
        {
            id = UINT32_MAX;
        });
    }
}
#pragma warning(default:4355)


Bloom::~Bloom()
{
    if (m_View != nullptr)
        m_View->ViewportSizeChanged -= m_ViewportSizeChangedHandler;
    cleanTextures();
    cleanShaders();
}
void Bloom::cleanTextures()
{
    for (int pass(0); pass < s_BlurPasses; ++pass)
    {
        for (uint32 i(0); i < m_Texture[pass].size(); ++i)
        {
            if ( m_Texture[pass][i] != nullptr)
            {
                m_Texture[pass][i]->release();
                m_Texture[pass][i] = nullptr;
            }
        }
        m_FboId[pass].forEach([](uint32& id)
        {
            if (id != UINT32_MAX)
            {
                glDeleteFramebuffers(1, &id);
                id = UINT32_MAX;
            }
        });
    }
}
void Bloom::cleanShaders()
{
    for (int pass(0); pass < s_BlurPasses; ++pass)
    {
        if (m_BlurShaderPass[pass] != nullptr)
        {
            m_BlurShaderPass[pass]->release();
            m_BlurShaderPass[pass] = nullptr;
        }
    }
    if (m_DownSampleShader != nullptr)
    {
        m_DownSampleShader->release();
        m_DownSampleShader = nullptr;
    }

    if (m_DownSampleBrightPassShader != nullptr)
    {
        m_DownSampleBrightPassShader->release();
        m_DownSampleBrightPassShader = nullptr;
    }
    if (m_Mesh != nullptr)
    {
        m_Mesh->release();
        m_Mesh = nullptr;
    }
}


void Bloom::init(View* view, bool hdr)
{
    m_View = view;
    m_View->ViewportSizeChanged += m_ViewportSizeChangedHandler;
    cleanShaders();

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
    m_DownSampleBrightPassShader = ResourceFactory<Shader>::getInstance()->get(ResourceFactory<Shader>::getInstance()->create());
    m_DownSampleBrightPassShader->initFromFile(folder + "shared/postShaderQuad.vert", 
                                                folder + "post/bloom.frag", layout, defineBrightPass);
    m_DownSampleBrightPassMap = m_DownSampleBrightPassShader->getShaderSamplerId("map");

    if (hdr)
        m_DownSampleBrightPassLumMap = m_DownSampleBrightPassShader->getShaderSamplerId("lumMap");

    m_DownSampleShader = ResourceFactory<Shader>::getInstance()->get(ResourceFactory<Shader>::getInstance()->create());
    m_DownSampleShader->initFromFile(folder + "shared/postShaderQuad.vert", 
                                      folder + "post/bloom.frag", layout);
    m_DownSampleMap = m_DownSampleShader->getShaderSamplerId("map");

    for (int pass = 0; pass < 2; ++pass)
    {
        m_BlurShaderPass[pass] = ResourceFactory<Shader>::getInstance()->get(ResourceFactory<Shader>::getInstance()->create());

        std::set<std::string> definePass;
        if (pass == 0)
            definePass.insert("PASS1");
        else
            definePass.insert("PASS2");

        m_BlurShaderPass[pass]->initFromFile(folder + "shared/postShaderQuad.vert", 
                                              folder + "post/gaussblur.frag", layout, definePass);
        m_BlurMapPos[pass] = m_BlurShaderPass[pass]->getShaderSamplerId("map");
    }

    //////////////////////////////////////////////////////////////////////////
    ///                             Quad                                   ///
    //////////////////////////////////////////////////////////////////////////
    m_Mesh = CONTENT->getFullscreenQuad();


    GRAPHICS->setActiveContext(m_View->getWindow()->getContext());
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
            m_Texture[pass][i]->init(gfx::TextureWrapType_Clamp,  gfx::TextureFilterType_Linear, 
                gfx::TextureFormat_RGBA16F, false);
            m_Texture[pass][i]->setName("Bloom::m_Texture[pass][i]");
        }

        //////////////////////////////////////////////////////////////////////////
        ///                               Fbo's                                ///
        //////////////////////////////////////////////////////////////////////////

        //Downsample fbo's
        m_FboId[pass].resize(m_DownSamples);
        glGenFramebuffers(m_DownSamples, &m_FboId[pass][0]);

        for (int i(0); i < m_DownSamples; ++i)
        {
            GL::heBindFbo(m_FboId[pass][i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture[pass][i]->getID(), 0);       
        }
    }
    resize();
}

void Bloom::resize()
{
    for (int pass = 0; pass < 2; ++pass)
    {
        //////////////////////////////////////////////////////////////////////////
        ///                             Textures                               ///
        //////////////////////////////////////////////////////////////////////////
        for (int i = 0; i < m_DownSamples; ++i)
        {
            m_Texture[pass][i]->setData(m_View->getViewport().width / ((i+2) * 2), m_View->getViewport().height / ((i+2) * 2), 
                0, gfx::TextureBufferLayout_RGBA, gfx::TextureBufferType_Float, 0 );
        }
    }
}

void Bloom::render( const Texture2D* pTexture, const Texture2D* pLumMap )
{
    HE_ASSERT(m_Hdr == true && pLumMap != nullptr || m_Hdr == false && pLumMap == nullptr, "no valid lumMap provided");

    GL::heBlendEnabled(false);
    GL::heSetCullFace(false);
    GL::heSetDepthWrite(false);
    GL::heSetDepthRead(false);

    GL::heBindVao(m_Mesh->getVertexArraysID());
    
    //BrightPass
    GL::heBindFbo(m_FboId[0][0]);
    m_DownSampleBrightPassShader->bind();
    m_DownSampleBrightPassShader->setShaderVar(m_DownSampleBrightPassMap, pTexture);
    if (m_Hdr)
        m_DownSampleBrightPassShader->setShaderVar(m_DownSampleBrightPassLumMap, pLumMap);
    GL::heSetViewport(he::RectI(0, 0, (int)m_Texture[0][0]->getWidth(), (int)m_Texture[0][0]->getHeight()));
    glDrawElements(GL_TRIANGLES, m_Mesh->getNumIndices(), m_Mesh->getIndexType(), 0);

    //DownSample further
    m_DownSampleShader->bind();
    for (uint32 fboId = 1; fboId < m_FboId[0].size(); ++fboId)
    {
        GL::heBindFbo(m_FboId[0][fboId]);
        m_DownSampleShader->setShaderVar(m_DownSampleMap, m_Texture[0][fboId - 1]);
        GL::heSetViewport(he::RectI(0, 0, (int)m_Texture[0][fboId]->getWidth(), (int)m_Texture[0][fboId]->getHeight()));

        glDrawElements(GL_TRIANGLES, m_Mesh->getNumIndices(), m_Mesh->getIndexType(), 0);
    }

    //Blur
    for (int pass = 0; pass < 2; ++pass)
    {
        m_BlurShaderPass[pass]->bind();
        for (uint32 fboId = 1; fboId < m_FboId[pass].size(); ++fboId)
        {
            GL::heBindFbo(m_FboId[pass == 0?1:0][fboId]);
            m_BlurShaderPass[pass]->setShaderVar(m_BlurMapPos[pass], m_Texture[pass][fboId]);
            GL::heSetViewport(he::RectI(0, 0, (int)m_Texture[pass == 0?1:0][fboId]->getWidth(), (int)m_Texture[pass == 0?1:0][fboId]->getHeight()));
            glDrawElements(GL_TRIANGLES, m_Mesh->getNumIndices(), m_Mesh->getIndexType(), 0);
        }
    }
}

const Texture2D* Bloom::getBloom( uint8 level ) const
{
    return m_Texture[0][level];
}



} } //end namespace