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
#include "ITickable.h"
#include "Singleton.h"

namespace he {
namespace gfx {
class Scene;
class Window;
class IRenderer;
class Forward3DRenderer;
class ShadowCaster;
class PostProcesser;
class RenderTarget;
class Renderer2D;
class Texture2D;
class ShapeRenderer;
class CameraPerspective;
class View;
class SkyBox;

class ViewFactory: public ObjectFactory<View>, public Singleton<ViewFactory>
{
    friend class Singleton<ViewFactory>;

    ViewFactory() { init(1, 2, "ViewFactory"); }

public:
    virtual ~ViewFactory() { }
    
private:
};

class HAPPY_ENTRY View : public ge::ITickable
{
    DECLARE_OBJECT(View)
public:
    View();
    virtual ~View();

    // Init
    virtual void init(const RenderSettings& settings);
    const RenderSettings& getSettings() const { return m_Settings; }

    // PLugin
    virtual void addRenderPlugin(IRenderer* renderer);
    
    // Window
    void setAbsoluteViewport(const RectI& viewport);
    void setRelativeViewport(const RectF& viewportPercentage);
    const RectI& getViewport() const { return m_Viewport; }

    void setWindow(Window* window);
    Window* getWindow() const { return m_Window; }
    
    event0<void> SettingsChanged;
    event0<void> ViewportSizeChanged;

    PostProcesser* getPostProcessor() const { return m_PostProcesser; }

    // Camera
    void setCamera(CameraPerspective* camera);
    CameraPerspective* getCamera() const { return m_Camera; }
    
    // Update / Draw
    virtual void tick( float dTime );
    virtual void draw();

private:
    void resize();
    void calcViewportFromPercentage();

    // Camera
    CameraPerspective* m_Camera;
    
    // Render Textures
    Texture2D* m_ColorRenderMap;
    Texture2D* m_NormalDepthRenderMap;
    RenderTarget* m_IntermediateRenderTarget;
    RenderTarget* m_OutputRenderTarget;
    
    // Post
    PostProcesser* m_PostProcesser;

    // Settings
    RectI m_Viewport;
    RectF m_ViewportPercentage;
    bool m_UsePercentage;

    gfx::Window* m_Window;

    RenderSettings m_Settings;

    he::PrimitiveList<IRenderer*> m_PrePostRenderPlugins;
    he::PrimitiveList<IRenderer*> m_PostPostRenderPlugins;

    // Events
    he::eventCallback0<void> m_WindowResizedCallback;

};

} } //end namespace

#endif
