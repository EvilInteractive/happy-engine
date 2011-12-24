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

namespace he {
namespace gfx {

DrawManager::DrawManager(): m_pShadowCaster(nullptr), m_pMainRenderer(nullptr), 
                            m_pFallbackRenderer(nullptr), m_pAfterPostRenderer(nullptr),
                            m_pPostProcesser(nullptr),
                            m_pColorRenderMap(NEW Texture2D()), m_pNormalRenderMap(NEW Texture2D()), m_pDepthRenderMap(NEW Texture2D())
{
}


DrawManager::~DrawManager()
{
    delete m_pMainRenderer;
    delete m_pFallbackRenderer;
    delete m_pAfterPostRenderer;
    delete m_pPostProcesser;
    delete m_pShadowCaster;

    delete m_pColorRenderMap;
    delete m_pNormalRenderMap;
    delete m_pDepthRenderMap;
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
        m_FallbackRenderFlags = DrawListContainer::F_Main_Tranlucent | 
                                DrawListContainer::F_Loc_BeforePost  |
                                DrawListContainer::F_Sub_Single      |
                                DrawListContainer::F_Sub_Skinned     |
                                DrawListContainer::F_Sub_Instanced;
    }
    else
    {
        m_pMainRenderer = NEW Forward3DRenderer();
        m_pMainRenderer->init(settings, nullptr, m_pNormalRenderMap, m_pDepthRenderMap);
        m_MainRenderFlags = DrawListContainer::F_Main_Tranlucent | 
                            DrawListContainer::F_Main_Opac       |
                            DrawListContainer::F_Loc_BeforePost  |
                            DrawListContainer::F_Sub_Single      |
                            DrawListContainer::F_Sub_Skinned     |
                            DrawListContainer::F_Sub_Instanced;
    }

    m_pAfterPostRenderer = NEW Forward3DRenderer();
    m_pAfterPostRenderer->init(settings, nullptr, m_pNormalRenderMap, m_pDepthRenderMap);
    m_AfterPostRenderFlags = DrawListContainer::F_Main_Tranlucent | 
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
}

void DrawManager::draw()
{
    PROFILER_BEGIN("DrawManager::draw");

    renderShadow();

    PROFILER_BEGIN("DrawManager::draw - main renderer");
    m_pMainRenderer->clear(true, true, true);
    m_pMainRenderer->draw(m_DrawList, m_MainRenderFlags);
    if (m_pMainRenderer->getSupportsTranslucency() == false)
    {
        PROFILER_BEGIN("DrawManager::draw - fallback renderer");
        m_pFallbackRenderer->draw(m_DrawList, m_FallbackRenderFlags);   
        PROFILER_END("DrawManager::draw - fallback renderer");
    }
    PROFILER_END("DrawManager::draw - main renderer");

    PROFILER_BEGIN("DrawManager::draw - post processer");
    m_pPostProcesser->draw(m_pColorRenderMap, m_pNormalRenderMap, m_pDepthRenderMap);
    PROFILER_END("DrawManager::draw - post processer");

    /*PROFILER_BEGIN("DrawManager::draw - after post renderer");
    m_pAfterPostRenderer->draw(m_DrawList, m_AfterPostRenderFlags);
    PROFILER_END("DrawManager::draw - after post renderer");*/


    PROFILER_END("DrawManager::draw");
}

void DrawManager::renderShadow()
{
    if (m_RenderSettings.enableShadows)
        m_pShadowCaster->render(m_DrawList, GRAPHICS->getLightManager()->getDirectionalLight());
}

void DrawManager::addDrawable( IDrawable* pDrawable )
{
    ASSERT(pDrawable != nullptr, "adding a nullptr drawable");
    m_DrawList.insert(pDrawable);
}
void DrawManager::removeDrawable( const IDrawable* pDrawable )
{
    ASSERT(pDrawable != nullptr, "removing a nullptr drawable");
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
    if (m_pAfterPostRenderer != nullptr)
        m_pAfterPostRenderer->onScreenResized();
}

void DrawManager::initSharedTextures()
{
    uint width(GRAPHICS->getScreenWidth()), 
         height(GRAPHICS->getScreenHeight());

    //Color
    uint renderTextureId;
    glGenTextures(1, &renderTextureId);
    GL::heBindTexture2D(0, renderTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, m_RenderSettings.enableHDR?GL_RGBA16F:GL_RGBA8, 
        width, height,
        0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
    m_pColorRenderMap->init(renderTextureId, width, height, m_RenderSettings.enableHDR?GL_RGBA16F:GL_RGBA8);

    //Normal
    uint normalId;
    glGenTextures(1, &normalId);
    GL::heBindTexture2D(0, normalId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, 0);
    m_pNormalRenderMap->init(normalId, width, height, GL_RG16F);

    //Depth
    uint depthId;
    glGenTextures(1, &depthId);
    GL::heBindTexture2D(0, depthId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    m_pDepthRenderMap->init(depthId, width, height, GL_DEPTH_COMPONENT16);
}

} } //end namespace