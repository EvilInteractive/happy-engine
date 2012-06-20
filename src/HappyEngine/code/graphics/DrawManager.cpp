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
//Created: 04/10/2011
#include "HappyPCH.h" 

#include "DrawManager.h"
#include "HappyNew.h"
#include "HappyEngine.h"
#include "HeAssert.h"
#include "GraphicsEngine.h"
#include "ShadowCaster.h"
#include "LightManager.h"
#include "CameraManager.h"
#include "InstancingManager.h"
#include "ICamera.h"

#include "Deferred3DRenderer.h"
#include "Forward3DRenderer.h"
#include "PostProcesser.h"
#include "Renderer2D.h"

namespace he {
namespace gfx {

DrawManager::DrawManager(): m_pShadowCaster(nullptr), m_pMainRenderer(nullptr), 
                            m_pFallbackRenderer(nullptr), m_pAfterPostRenderer(nullptr), 
                            m_pBackgroundRenderer(nullptr), m_pPostProcesser(nullptr),
                            m_pColorRenderMap(ResourceFactory<Texture2D>::getInstance()->get(ResourceFactory<Texture2D>::getInstance()->create())), 
                            m_pNormalRenderMap(ResourceFactory<Texture2D>::getInstance()->get(ResourceFactory<Texture2D>::getInstance()->create())), 
                            m_pDepthRenderMap(ResourceFactory<Texture2D>::getInstance()->get(ResourceFactory<Texture2D>::getInstance()->create())),
                            m_RenderDebugTextures(false)
{
    m_pColorRenderMap->setName("DrawManager::m_pColorRenderMap");
    m_pNormalRenderMap->setName("DrawManager::m_pNormalRenderMap");
    m_pDepthRenderMap->setName("DrawManager::m_pDepthRenderMap");
}


DrawManager::~DrawManager()
{
    delete m_pMainRenderer;
    delete m_pFallbackRenderer;
    delete m_pBackgroundRenderer;
    delete m_pAfterPostRenderer;
    delete m_pPostProcesser;
    delete m_pShadowCaster;
    m_pColorRenderMap->release();
    m_pNormalRenderMap->release();
    m_pDepthRenderMap->release();
}

bool DrawManager::viewClip(const ICamera* pCamera, const shapes::Sphere& boundingSphere)
{
    return viewClip(pCamera->getPosition(), 
                    pCamera->getLook(), 
                    pCamera->getFarClip(), boundingSphere);
}
bool DrawManager::viewClip(const vec3& camPos, const vec3& camLook, float camFar, const shapes::Sphere& boundingSphere)
{
    vec3 vec(boundingSphere.getPosition() - camPos);
    float len(length(vec));
    if (len < boundingSphere.getRadius() == false) //if not in bounding sphere
    {
        if (len - boundingSphere.getRadius() > camFar) //if bounding outside clip
        {
            return true;
        }
        else //check behind camera
        {
            vec /= len;
            return dot(camLook, vec) < 0;
        }
    }
    else
        return false;
}

void DrawManager::init(const RenderSettings& settings)
{
    m_RenderSettings = settings;

    // Color
    m_pColorRenderMap->init(gfx::Texture2D::WrapType_Clamp,  gfx::Texture2D::FilterType_Nearest,
            m_RenderSettings.enableHDR ? gfx::Texture2D::TextureFormat_RGBA16F : gfx::Texture2D::TextureFormat_RGBA8, false);
    // Normal
    m_pNormalRenderMap->init(gfx::Texture2D::WrapType_Clamp, gfx::Texture2D::FilterType_Nearest, 
        gfx::Texture2D::TextureFormat_RG16, false);
    // Depth
    m_pDepthRenderMap->init(gfx::Texture2D::WrapType_Clamp, gfx::Texture2D::FilterType_Nearest, 
        gfx::Texture2D::TextureFormat_Depth32, false);

    initSharedTextures();
    if (settings.enableDeferred)
    {
        m_pMainRenderer = NEW Deferred3DRenderer();
        m_pMainRenderer->init(settings, m_pColorRenderMap, m_pNormalRenderMap, m_pDepthRenderMap);
        m_MainRenderFlags = DrawListContainer::F_Main_Opac      | 
                            DrawListContainer::F_Loc_BeforePost |
                            DrawListContainer::F_Sub_Single     |
                            DrawListContainer::F_Sub_Skinned    |
                            DrawListContainer::F_Sub_Instanced;
        m_pFallbackRenderer = NEW Forward3DRenderer();
        m_pFallbackRenderer->init(settings, m_pColorRenderMap, m_pNormalRenderMap, m_pDepthRenderMap);
        m_FallbackRenderFlags = DrawListContainer::F_Main_Blended | 
                                DrawListContainer::F_Loc_BeforePost  |
                                DrawListContainer::F_Sub_Single      |
                                DrawListContainer::F_Sub_Skinned     |
                                DrawListContainer::F_Sub_Instanced;
    }
    else
    {
        m_pMainRenderer = NEW Forward3DRenderer();
        m_pMainRenderer->init(settings, m_pColorRenderMap, m_pNormalRenderMap, m_pDepthRenderMap);
        m_MainRenderFlags = DrawListContainer::F_Main_Blended | 
                            DrawListContainer::F_Main_Opac       |
                            DrawListContainer::F_Loc_BeforePost  |
                            DrawListContainer::F_Sub_Single      |
                            DrawListContainer::F_Sub_Skinned     |
                            DrawListContainer::F_Sub_Instanced;
    }

    m_pBackgroundRenderer = NEW Forward3DRenderer();
    m_pBackgroundRenderer->init(settings, nullptr, nullptr, m_pDepthRenderMap);
    m_BackgroundRenderFlags = DrawListContainer::F_Main_Blended    | 
                              DrawListContainer::F_Main_Opac       |
                              DrawListContainer::F_Loc_Background  |
                              DrawListContainer::F_Sub_Single      |
                              DrawListContainer::F_Sub_Skinned     |
                              DrawListContainer::F_Sub_Instanced;

    m_pAfterPostRenderer = NEW Forward3DRenderer();
    m_pAfterPostRenderer->init(settings, nullptr, nullptr, m_pDepthRenderMap);
    m_AfterPostRenderFlags =  DrawListContainer::F_Main_Blended    | 
                              DrawListContainer::F_Main_Opac       |
                              DrawListContainer::F_Loc_AfterPost   |
                              DrawListContainer::F_Sub_Single      |
                              DrawListContainer::F_Sub_Skinned     |
                              DrawListContainer::F_Sub_Instanced;

    if (settings.enableShadows)
    {
        m_pShadowCaster = NEW ShadowCaster();
        m_pShadowCaster->init(settings);
    }

    m_pPostProcesser = NEW PostProcesser();
    m_pPostProcesser->init(settings);

    CONSOLE->registerVar(&m_RenderDebugTextures, "debugRenderTex");
}

void DrawManager::draw()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);

    HE_ASSERT(CAMERAMANAGER->getActiveCamera() != nullptr, "Please set an active camera.");

    PROFILER_BEGIN("ShadowCaster::render");
    renderShadow();
    PROFILER_END();

    PROFILER_BEGIN("DrawManager::draw - background renderer");
    m_pBackgroundRenderer->clear(false, false, true);
    m_pBackgroundRenderer->draw(m_DrawList, m_BackgroundRenderFlags);
    PROFILER_END();

    PROFILER_BEGIN("main renderer");
    m_pMainRenderer->clear(true, true, true);
    m_pMainRenderer->draw(m_DrawList, m_MainRenderFlags);
    if (m_pMainRenderer->getSupportsTranslucency() == false)
    {
        PROFILER_BEGIN("fallback renderer");
        m_pFallbackRenderer->draw(m_DrawList, m_FallbackRenderFlags);   
        PROFILER_END();
    }
    PROFILER_END();

    PROFILER_BEGIN("post processer");
    m_pPostProcesser->draw(m_pColorRenderMap, m_pNormalRenderMap, m_pDepthRenderMap);
    PROFILER_END();

    PROFILER_BEGIN("DrawManager::draw - after post renderer");
    m_pAfterPostRenderer->draw(m_DrawList, m_AfterPostRenderFlags);
    PROFILER_END();

    if (m_RenderDebugTextures)
    {
        GUI->drawTexture2DToScreen(m_pColorRenderMap,  vec2(12 * 1 + 256 * 0, 12), false, vec2(256, 144));
        GUI->drawTexture2DToScreen(m_pNormalRenderMap, vec2(12 * 2 + 256 * 1, 12), false, vec2(256, 144));
        GUI->drawTexture2DToScreen(m_pDepthRenderMap,  vec2(12 * 3 + 256 * 2, 12), false, vec2(256, 144));
    }
}

void DrawManager::renderShadow()
{
    if (m_RenderSettings.enableShadows)
        m_pShadowCaster->render(m_DrawList, GRAPHICS->getLightManager()->getDirectionalLight());
}

void DrawManager::addDrawable( IDrawable* pDrawable )
{
    HE_ASSERT(pDrawable != nullptr, "adding a nullptr drawable");
    m_DrawList.insert(pDrawable);
}
void DrawManager::removeDrawable( const IDrawable* pDrawable )
{
    HE_ASSERT(pDrawable != nullptr, "removing a nullptr drawable");
    m_DrawList.remove(pDrawable);
}


const DrawListContainer& DrawManager::getDrawList() const
{
    return m_DrawList;
}

void DrawManager::onScreenResized()
{
    initSharedTextures();

    if (m_pMainRenderer != nullptr)
        m_pMainRenderer->onScreenResized();
    if (m_pFallbackRenderer != nullptr)
        m_pFallbackRenderer->onScreenResized();
    if (m_pPostProcesser != nullptr)
        m_pPostProcesser->onScreenResized();
    if (m_pBackgroundRenderer != nullptr)
        m_pBackgroundRenderer->onScreenResized();
    if (m_pAfterPostRenderer != nullptr)
        m_pAfterPostRenderer->onScreenResized();
}

void DrawManager::initSharedTextures()
{
    uint width(GRAPHICS->getScreenWidth()), 
         height(GRAPHICS->getScreenHeight());

    //Color
    m_pColorRenderMap->setData(width, height, 0,
        gfx::Texture2D::BufferLayout_BGRA, gfx::Texture2D::BufferType_Byte, 0 );

    //Normal
    m_pNormalRenderMap->setData(width, height, 0, 
        gfx::Texture2D::BufferLayout_RG, gfx::Texture2D::BufferType_Byte, 0 );

    //Depth
    m_pDepthRenderMap->setData(width, height, 0, 
        gfx::Texture2D::BufferLayout_Depth, gfx::Texture2D::BufferType_Float, 0 );
}

uint DrawManager::getDepthID() const
{
    return m_pDepthRenderMap->getID();
}

PostProcesser* DrawManager::getPostProcesser() const
{
    return m_pPostProcesser;
}

} } //end namespace