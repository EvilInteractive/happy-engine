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
#include "DrawContext.h"
#include "Texture2D.h"
#include "ModelMesh.h"
#include "MaterialInstance.h"
#include "RenderTarget.h"
#include "View.h"
#include "Window.h"

namespace he {
namespace gfx {

#pragma warning(disable:4355) // use of this in member initializer list
Bloom::Bloom()
: m_ToneMapBuffer(nullptr)
, m_Quad(nullptr)
, m_View(nullptr)
, m_DownSampleBrightPassMaterial(nullptr)
, m_DownSampleMaterial(nullptr)
, m_DownSampleBrightPassMap(-1)
, m_DownSampleBrightPassLumMap(-1)
, m_DownSampleMap(-1)
, m_Hdr(false)
, m_ViewportSizeChangedHandler(std::bind(&Bloom::resize, this))
{
    for (uint32 i(0); i < s_BlurPasses; ++i)
    {
        m_BlurPassMaterial[i] = nullptr;	
        m_BlurMapPos[i] = -1;
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
    if (m_Quad != nullptr)
    {
        m_Quad->release();
        m_Quad = nullptr;
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
    m_Quad = CONTENT->getFullscreenQuad();

    //////////////////////////////////////////////////////////////////////////
    ///                             Materials                              ///
    //////////////////////////////////////////////////////////////////////////

    // DownSample
    {
        const Material* const downSampleMaterial(CONTENT->loadMaterial("engine/post/downSample.hm"));
        m_DownSampleMaterial = downSampleMaterial->createMaterialInstance(eShaderType_Normal);
        downSampleMaterial->release();

        m_DownSampleMaterial->calculateMaterialLayout(m_Quad->getVertexLayout());
        m_DownSampleBrightPassMap = m_DownSampleMaterial->findParameter(HEFS::strmap);
    }

    // DownSampleBrightPass
    {
        const Material* const downSampleBPMaterial(CONTENT->loadMaterial("engine/post/downSampleBrightPass.hm"));
        m_DownSampleBrightPassMaterial = downSampleBPMaterial->createMaterialInstance(eShaderType_Normal);
        downSampleBPMaterial->release();

        m_DownSampleBrightPassLumMap = m_DownSampleBrightPassMaterial->findParameter(HEFS::strlumMap);
        m_DownSampleMap = m_DownSampleMaterial->findParameter(HEFS::strmap);
    }

    const Material* const blurHMaterial(CONTENT->loadMaterial("engine/post/gaussBlurH.hm"));
    m_BlurPassMaterial[0] = blurHMaterial->createMaterialInstance(eShaderType_Normal);
    blurHMaterial->release();

    const Material* const blurVMaterial(CONTENT->loadMaterial("engine/post/gaussBlurV.hm"));
    m_BlurPassMaterial[1] = blurVMaterial->createMaterialInstance(eShaderType_Normal);
    blurVMaterial->release();

    for (size_t i(0); i < 2; ++i)
    {
        m_BlurMapPos[i] = m_BlurPassMaterial[i]->findParameter(HEFS::strmap);
    }
    
    //////////////////////////////////////////////////////////////////////////
    ///                             Textures                               ///
    //////////////////////////////////////////////////////////////////////////
    ResourceFactory<Texture2D>* textFac(ResourceFactory<Texture2D>::getInstance());
    GraphicsEngine* const gfxEngine(GRAPHICS);
    gfxEngine->setActiveContext(m_View->getWindow()->getContext());
    for (int pass(0); pass < 2; ++pass)
    {
        m_Texture[pass].resize(s_DownSamples);

        for (int i(0); i < s_DownSamples; ++i)
        {
            ObjectHandle handle(textFac->create());
            m_Texture[pass][i] = textFac->get(handle);
            m_Texture[pass][i]->init(gfx::TextureWrapType_Clamp, gfx::TextureFilterType_Linear, gfx::TextureFormat_RGBA16F, false);
            m_Texture[pass][i]->setName("Bloom::m_Texture[pass][i]");
        }
    }

    //////////////////////////////////////////////////////////////////////////
    ///                             RenderTargets                          ///
    //////////////////////////////////////////////////////////////////////////
    m_RenderTarget.resize(s_DownSamples);
    for (int i(0); i < s_DownSamples; ++i)
    {
        RenderTarget* rt(NEW RenderTarget(gfxEngine->getSharedContext()));
        m_RenderTarget[i] = rt;
    }

    resize();

    for (int i(0); i < s_DownSamples; ++i)
    {
        m_RenderTarget[i]->addTextureTarget(m_Texture[0][i]);
        m_RenderTarget[i]->init();
    }
}

void Bloom::resize()
{
    //////////////////////////////////////////////////////////////////////////
    ///                     Textures & Rendertargets                       ///
    //////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < s_DownSamples; ++i)
    {
        const int width(m_View->getViewport().width / ((i+2) * 2));
        const int height(m_View->getViewport().height / ((i+2) * 2));
        for (int pass = 0; pass < 2; ++pass)
        {
            m_Texture[pass][i]->setData(width, height, 0, gfx::TextureBufferLayout_RGBA, gfx::TextureBufferType_Float, 0 );
        }
        m_RenderTarget[i]->setSize(width, height);
    }
}

void Bloom::render( const Texture2D* texture, const Texture2D* lumMap )
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    HE_ASSERT(m_Hdr == true && lumMap != nullptr || m_Hdr == false && lumMap == nullptr, "no valid lumMap provided");

    DrawContext context;
    context.m_VBO = m_Quad->getVBO();
    context.m_IBO = m_Quad->getIBO();
        
    //BrightPass
    GL::heSetViewport(he::RectI(0, 0, static_cast<int>(m_Texture[0][0]->getWidth()), static_cast<int>(m_Texture[0][0]->getHeight())));

    m_RenderTarget[0]->prepareForRendering();
    m_DownSampleBrightPassMaterial->getParameter(m_DownSampleBrightPassMap).setTexture2D(texture);
    if (m_DownSampleBrightPassLumMap >= 0)
        m_DownSampleBrightPassMaterial->getParameter(m_DownSampleBrightPassLumMap).setTexture2D(lumMap);
    m_DownSampleBrightPassMaterial->apply(context);
    m_Quad->draw();

    //DownSample further
    for (uint32 fboId(1); fboId < s_DownSamples; ++fboId)
    {
        m_RenderTarget[fboId]->prepareForRendering();

        const Texture2D* const tex(m_Texture[0][fboId - 1]);
        m_DownSampleMaterial->getParameter(m_DownSampleMap).setTexture2D(tex);
        GL::heSetViewport(he::RectI(0, 0, static_cast<int>(tex->getWidth()), static_cast<int>(tex->getHeight())));
        m_DownSampleMaterial->apply(context);

        m_Quad->draw();
    }

    //Blur
    for (uint32 fboId(1); fboId < s_DownSamples; ++fboId)
    {
        RenderTarget* const rt(m_RenderTarget[fboId]);
        for (int pass(0); pass < 2; ++pass)
        {
            Texture2D* const fromTex(m_Texture[pass][fboId]);
            Texture2D* const toTex(m_Texture[(pass+1)%2][fboId]);
            MaterialInstance* const mat(m_BlurPassMaterial[pass]);

            GL::heSetViewport(he::RectI(0, 0, static_cast<int>(toTex->getWidth()), static_cast<int>(toTex->getHeight())));
            mat->getParameter(m_BlurMapPos[pass]).setTexture2D(fromTex);
            rt->switchTextureTarget(0, toTex);

            rt->prepareForRendering();
            mat->apply(context);
            m_Quad->draw();
        }
    }
}

const Texture2D* Bloom::getBloom( uint8 level ) const
{
    return m_Texture[0][level];
}



} } //end namespace
