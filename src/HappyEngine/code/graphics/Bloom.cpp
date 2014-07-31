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
//Created: 16/10/2011

#include "HappyPCH.h" 

#include "Bloom.h"
#include "ContentManager.h"
#include "GraphicsEngine.h"
#include "Vertex.h"

#include "Texture2D.h"
#include "ModelMesh.h"
#include "MaterialInstance.h"
#include "View.h"
#include "Window.h"

namespace he {
namespace gfx {

#pragma warning(disable:4355) // use of this in member initializer list
Bloom::Bloom()
: m_ToneMapBuffer(nullptr)
, m_Mesh(nullptr)
, m_View(nullptr)
, m_DownSampleBrightPassMaterial(nullptr)
, m_DownSampleMaterial(nullptr)
, m_DownSampleBrightPassMap(-1)
, m_DownSampleBrightPassLumMap(-1)
, m_DownSampleBrightPassToneMapData(-1)
, m_DownSampleMap(-1)
, m_Hdr(false)
, m_ViewportSizeChangedHandler(std::bind(&Bloom::resize, this))
{
    for (uint32 i(0); i < s_BlurPasses; ++i)
    {
        m_BlurPassMaterial[i] = nullptr;	
        m_BlurMapPos[i] = -1;
        m_BlurTexelSize[i] = -1;
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
        m_RenderTarget.forEach([](RenderTarget* renderTarget)
        {
            delete renderTarget;
        });
    }
}
void Bloom::cleanShaders()
{
    for (int pass(0); pass < s_BlurPasses; ++pass)
    {
        if (m_BlurPassMaterial[pass] != nullptr)
        {
            delete m_BlurPassMaterial[pass];
            m_BlurPassMaterial[pass] = nullptr;
        }
    }
    if (m_DownSampleMaterial != nullptr)
    {
        delete m_DownSampleMaterial;
        m_DownSampleMaterial = nullptr;
    }

    if (m_DownSampleBrightPassMaterial != nullptr)
    {
        delete m_DownSampleBrightPassMaterial;
        m_DownSampleBrightPassMaterial = nullptr;
    }
    if (m_Mesh != nullptr)
    {
        m_Mesh->release();
        m_Mesh = nullptr;
    }
}


void Bloom::init(View* view, bool hdr, UniformBuffer* toneMapBuffer)
{
    m_ToneMapBuffer = toneMapBuffer;
    m_View = view;
    m_View->ViewportSizeChanged += m_ViewportSizeChangedHandler;
    cleanShaders();

    m_Hdr = hdr;
    //////////////////////////////////////////////////////////////////////////
    ///                             Quad                                   ///
    //////////////////////////////////////////////////////////////////////////
    m_Mesh = CONTENT->getFullscreenQuad();

    //////////////////////////////////////////////////////////////////////////
    ///                             Materials                              ///
    //////////////////////////////////////////////////////////////////////////

    // DownSample
    {
        const Material* const downSampleMaterial(CONTENT->loadMaterial("engine/post/downSample.hm"));
        m_DownSampleMaterial = downSampleMaterial->createMaterialInstance(eShaderType_Normal);
        downSampleMaterial->release();

        m_DownSampleMaterial->calculateMaterialLayout(m_Mesh->getVertexLayout());
        m_DownSampleBrightPassMap = m_DownSampleMaterial->findParameter(HEFS::strmap);
    }

    // DownSampleBrightPass
    {
        const Material* const downSampleBPMaterial(CONTENT->loadMaterial("engine/post/downSampleBrightPass.hm"));
        m_DownSampleBrightPassMaterial = downSampleBPMaterial->createMaterialInstance(eShaderType_Normal);
        downSampleBPMaterial->release();

        if (hdr)
        {
            m_DownSampleBrightPassLumMap = m_DownSampleBrightPassMaterial->findParameter(HEFS::strlumMap);
            //m_DownSampleBrightPassToneMapData = m_DownSampleBrightPassShader->getBufferId("SharedToneMapBuffer");
            //m_DownSampleBrightPassShader->setBuffer(m_DownSampleBrightPassToneMapData, m_ToneMapBuffer);
        }

        m_DownSampleMap = m_DownSampleMaterial->findParameter(HEFS::strmap);
    }

    for (int pass = 0; pass < 2; ++pass)
    {
        m_BlurMapPos[pass] = m_BlurPassMaterial[pass]->getShaderSamplerId("map");
    }



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

void Bloom::render( const Texture2D* texture, const Texture2D* lumMap )
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    HE_ASSERT(m_Hdr == true && lumMap != nullptr || m_Hdr == false && lumMap == nullptr, "no valid lumMap provided");

    GL::heBlendEnabled(false);
    GL::heSetCullFace(false);
    GL::heSetDepthWrite(false);
    GL::heSetDepthRead(false);

    GL::heBindVao(m_Mesh->getVertexArraysID());
    
    //BrightPass
    GL::heBindFbo(m_FboId[0][0]);
    m_DownSampleBrightPassMaterial->bind();
    m_DownSampleBrightPassMaterial->setShaderVar(m_DownSampleBrightPassMap, texture);
    if (m_Hdr)
    {
        m_DownSampleBrightPassMaterial->setShaderVar(m_DownSampleBrightPassLumMap, lumMap);
    }
    GL::heSetViewport(he::RectI(0, 0, (int)m_Texture[0][0]->getWidth(), (int)m_Texture[0][0]->getHeight()));
    m_Mesh->draw();

    //DownSample further
    m_DownSampleMaterial->bind();
    for (uint32 fboId = 1; fboId < m_DownSamples; ++fboId)
    {
        GL::heBindFbo(m_FboId[0][fboId]);
        m_DownSampleMaterial->setShaderVar(m_DownSampleMap, m_Texture[0][fboId - 1]);
        GL::heSetViewport(he::RectI(0, 0, (int)m_Texture[0][fboId]->getWidth(), (int)m_Texture[0][fboId]->getHeight()));

        m_Quad->draw();
    }

    //Blur
    for (int pass = 0; pass < 2; ++pass)
    {
        m_BlurPassMaterial[pass]->bind();
        for (uint32 fboId = 1; fboId < m_FboId[pass].size(); ++fboId)
        {
            GL::heBindFbo(m_FboId[pass == 0?1:0][fboId]);
            m_BlurPassMaterial[pass]->setShaderVar(m_BlurMapPos[pass], m_Texture[pass][fboId]);
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
