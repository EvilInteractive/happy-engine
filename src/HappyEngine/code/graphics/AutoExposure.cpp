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
//Created: 26/10/2011

#include "HappyPCH.h" 
#include "AutoExposure.h"

#include "ContentManager.h"
#include "DrawContext.h"
#include "Game.h"
#include "GraphicsEngine.h"
#include "MaterialInstance.h"
#include "ModelMesh.h"
#include "Texture2D.h"
#include "RenderTarget.h"
#include "View.h"
#include "Window.h"

namespace he {
    namespace gfx {

AutoExposure::AutoExposure()
: m_LumMaterial(nullptr)
, m_MaterialparamHDRmapIndex(-1)
, m_MaterialparamPrevLumMapIndex(-1)
, m_MaterialparamDTimeIndex(-1)
, m_ExposureSpeed(2.0f)
, m_RenderTarget(nullptr)
, m_Quad(nullptr)
, m_FirstBuffer(true)
, m_IsInitialized(false)
{
    ObjectHandle handle1(ResourceFactory<Texture2D>::getInstance()->create());
    ObjectHandle handle2(ResourceFactory<Texture2D>::getInstance()->create());
    m_LumTexture[0] = ResourceFactory<Texture2D>::getInstance()->get(handle1);
    m_LumTexture[0]->setName("AutoExposure::m_LumTexture[0]");
    m_LumTexture[0]->init(gfx::TextureWrapType_Clamp, gfx::TextureFilterType_Nearest,
            gfx::TextureFormat_R16F, false);
    m_LumTexture[1] = ResourceFactory<Texture2D>::getInstance()->get(handle2);
    m_LumTexture[1]->setName("AutoExposure::m_LumTexture[1]");
    m_LumTexture[1]->init(gfx::TextureWrapType_Clamp, gfx::TextureFilterType_Nearest,
            gfx::TextureFormat_R16F, false);
}

AutoExposure::~AutoExposure()
{
    m_LumTexture[0]->release();
    m_LumTexture[1]->release();
    HEDelete(m_RenderTarget);
    if (m_IsInitialized && GAME != nullptr)
        GAME->removeFromTickList(this);
    if (m_Quad != nullptr)
        m_Quad->release();
    HEDelete(m_LumMaterial);
}

void AutoExposure::init(View* view, const PostSettings::HdrSettings& settings)
{
    //////////////////////////////////////////////////////////////////////////
    ///                          LOAD RENDER TARGETS                       ///
    //////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < 2; ++i)
    {
        m_LumTexture[i]->setData(1, 1, 0, 
            gfx::TextureBufferLayout_R, gfx::TextureBufferType_Float, 0);
    }

    //////////////////////////////////////////////////////////////////////////
    ///                            LOAD FBO's                              ///
    //////////////////////////////////////////////////////////////////////////
    m_RenderTarget = HENew(RenderTarget)(view->getWindow()->getContext());
    m_RenderTarget->setSize(1, 1);
    m_RenderTarget->addTextureTarget(m_LumTexture[0]);
    m_RenderTarget->init();

    //////////////////////////////////////////////////////////////////////////
    ///                         LOAD RENDER QUAD                           ///
    //////////////////////////////////////////////////////////////////////////
    m_Quad = CONTENT->getFullscreenQuad();

    //////////////////////////////////////////////////////////////////////////
    ///                          LOAD MATERIAL                             ///
    //////////////////////////////////////////////////////////////////////////
    he::gfx::Material* const mat(CONTENT->loadMaterial("engine/post/autolum.hm"));
    m_LumMaterial = mat->createMaterialInstance(eShaderRenderType_Normal);
    mat->release();
    
    m_LumMaterial->calculateMaterialLayout(m_Quad->getVertexLayout());

    m_MaterialparamHDRmapIndex = m_LumMaterial->findParameter(HEFS::strhdrMap);
    m_MaterialparamPrevLumMapIndex = m_LumMaterial->findParameter(HEFS::strprevLumMap);
    m_MaterialparamDTimeIndex = m_LumMaterial->findParameter(HEFS::strdTime);
    
    m_ExposureSpeed = settings.exposureSpeed;

    GAME->addToTickList(this);

    m_IsInitialized = true;
}

void AutoExposure::tick( float dTime )
{
    m_LumMaterial->getParameter(m_MaterialparamDTimeIndex).setFloat(dTime);
}

void AutoExposure::calculate( const Texture2D* hdrMap)
{
    m_FirstBuffer = !m_FirstBuffer;

    m_LumMaterial->getParameter(m_MaterialparamHDRmapIndex).setTexture2D(hdrMap);
    m_LumMaterial->getParameter(m_MaterialparamPrevLumMapIndex).setTexture2D(m_LumTexture[m_FirstBuffer? 1 : 0]);

    m_RenderTarget->switchTextureTarget(0, m_LumTexture[m_FirstBuffer? 0 : 1]);
    m_RenderTarget->prepareForRendering();

    GL::heSetViewport(RectI(0, 0, 1, 1));

    DrawContext context;
    context.m_VBO = m_Quad->getVBO();
    context.m_IBO = m_Quad->getIBO();
    m_LumMaterial->apply(context);

    m_Quad->draw();
}

const Texture2D* AutoExposure::getLuminanceMap() const
{
    return m_LumTexture[m_FirstBuffer? 0 : 1];
}


} } //end namespace
