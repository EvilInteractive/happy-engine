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
//Extended:Sebastiaan Sprengers

#ifndef _HE_DRAW_MANAGER_H_
#define _HE_DRAW_MANAGER_H_
#pragma once

#include "IDrawable.h"
#include "DrawListContainer.h"
#include "RenderSettings.h"
#include "Texture2D.h"

namespace he {
namespace gfx {

class ShadowCaster;
class IRenderer;
class PostProcesser;
class ICamera;

class DrawManager
{
public:
    /* CONSTRUCTOR - DESTRUCTOR */
    DrawManager();
    virtual ~DrawManager();

    /* GENERAL */
    void init(const RenderSettings& settings);
    void draw();

    void addDrawable(IDrawable* pDrawable);
    void removeDrawable(const IDrawable* pDrawable);

    static bool viewClip(const ICamera* pCamera, const shapes::Sphere& boundingSphere);
    static bool viewClip(const vec3& camPos, const vec3& camLook, float camFar, const shapes::Sphere& boundingSphere);

    void onScreenResized();

    /* GETTERS */
    const DrawListContainer& getDrawList() const;

private:
    void initSharedTextures();
    void renderShadow();

    /* DATAMEMBERS */
    DrawListContainer m_DrawList;

    ShadowCaster* m_pShadowCaster;

    IRenderer* m_pMainRenderer;
    uint m_MainRenderFlags;

    IRenderer* m_pFallbackRenderer;
    uint m_FallbackRenderFlags;

    PostProcesser* m_pPostProcesser;

    IRenderer* m_pAfterPostRenderer;
    uint m_AfterPostRenderFlags;

    Texture2D::pointer m_pColorRenderMap;
    Texture2D::pointer m_pNormalRenderMap;
    Texture2D::pointer m_pDepthRenderMap;

    bool m_RenderDebugTextures;

    RenderSettings m_RenderSettings;

    /* DEFAULT COPY & ASSIGNMENT */
    DrawManager(const DrawManager&);
    DrawManager& operator=(const DrawManager&);
};

} } //end namespace

#endif
