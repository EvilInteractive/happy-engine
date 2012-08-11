//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 14/07/2012

#ifndef _HE_View_H_
#define _HE_View_H_
#pragma once

#include "RenderSettings.h"

namespace he {
namespace gfx {
class Scene;
class Window;
class CameraPerspective;
class IRenderer;
class ShadowCaster;
class PostProcesser;
class RenderTarget;
class Renderer2D;
class Texture2D;

class View
{
public:
    View();
    virtual ~View();

    void init(const RenderSettings& settings);

    void setAbsoluteViewport(const RectI& viewport);
    void setRelativeViewport(const RectF& viewportPercentage);
    const RectI& getViewport() const;

    void setCamera(const std::string& cameraId);
    void setScene(Scene* scene);
    void setWindow(Window* window);

    Renderer2D* get2DRenderer() const { return m_2DRenderer; }

    const Scene* getScene() const { return m_Scene; }
    const RenderSettings& getSettings() const { return m_Settings; }

    event0<void> SettingsChanged;
    event0<void> ViewportSizeChanged;

    // INTERNAL
    void draw();

private:
    void calcViewportFromPercentage();

    RectI m_Viewport;
    RectF m_ViewportPercentage;
    bool m_UsePercentage;

    gfx::CameraPerspective* m_Camera;
    gfx::Scene*  m_Scene;
    gfx::Window* m_Window;

    RenderSettings m_Settings;

    // Render Textures
    Texture2D* m_ColorRenderMap;
    Texture2D* m_NormalRenderMap;
    Texture2D* m_DepthRenderMap;
    RenderTarget* m_IntermediateRenderTarget;
    RenderTarget* m_OutputRenderTarget;

    // Renderers
    IRenderer* m_OpacRenderer;
    IRenderer* m_TransparentRenderer;
    IRenderer* m_ShapeRenderer;
    Renderer2D* m_2DRenderer;

    // Shadow
    ShadowCaster* m_ShadowCaster;

    // Post
    PostProcesser* m_PostProcesser;

    // Debug
    bool m_RenderDebugTextures;

    // Events
    he::eventCallback0<void> m_WindowResizedCallback;


    //Disable default copy constructor and default assignment operator
    View(const View&);
    View& operator=(const View&);
};

} } //end namespace

#endif
