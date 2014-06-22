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
//Created: 14/07/2012

#ifndef _HE_View_H_
#define _HE_View_H_
#pragma once

#include "ITickable.h"
#include "Singleton.h"
#include "RenderSettings.h"

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
class ICamera;
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

struct CameraSettings
{
    CameraSettings(): fov(pi / 3.0f), useRelativeViewport(true) 
    { 
        viewport.relativeViewport[0] = 0.0f; 
        viewport.relativeViewport[1] = 0.0f;
        viewport.relativeViewport[2] = 1.0f;
        viewport.relativeViewport[3] = 1.0f;
    }

    void setRelativeViewport(const RectF& rect)
    {
        useRelativeViewport = true;
        viewport.relativeViewport[0] = rect.x; 
        viewport.relativeViewport[1] = rect.y;
        viewport.relativeViewport[2] = rect.width;
        viewport.relativeViewport[3] = rect.height;
    }
    void setAbsoluteViewport(const RectI& rect)
    {
        useRelativeViewport = false;
        viewport.absoluteViewport[0] = rect.x; 
        viewport.absoluteViewport[1] = rect.y;
        viewport.absoluteViewport[2] = rect.width;
        viewport.absoluteViewport[3] = rect.height;
    }

    float fov;
    bool useRelativeViewport;
    union 
    {
        float relativeViewport[4];
        uint32 absoluteViewport[4];
    } viewport;
};

class HAPPY_ENTRY View : public ge::ITickable
{
    DECLARE_OBJECT(View)
public:
    enum EViewInsertMode
    {
        eViewInsertMode_Last,
        eViewInsertMode_First
    };

    View();
    ~View();

    // Init
    void init(const CameraSettings& cameraSettings, const bool forceDisablePost = false);

    // Setters
    void setStereo(const StereoSetting stereo, const bool force = false);

    // PLugin
    void addRenderPlugin(IRenderer* renderer);
    
    // Window
    const RectI& getViewport() const { return m_Viewport; }

    void setWindow(Window* window, const EViewInsertMode mode = eViewInsertMode_Last);
    Window* getWindow() const { return m_Window; }
    
    event0<void> ViewportSizeChanged;

    PostProcesser* getPostProcessor() const { return m_PostProcesser; }

    // Camera
    void setCamera(ICamera* const camera);
    ICamera* getCamera() const { return m_Camera; }

    gfx::StereoSetting getStereo() const { return m_Stereo; }
    
    // Update / Draw
    void tick( float dTime );
    void draw();

private:
    void resize(const int32 width, const int32 height);
    void calcViewportFromPercentage();

    void render();

    void setAbsoluteViewport(const RectI& viewport, const bool force = false);
    void setRelativeViewport(const RectF& viewportPercentage, const bool force = false);

    // Camera
    ICamera* m_Camera;
    
    // Render Textures
    Texture2D* m_ColorRenderMap;
    Texture2D* m_NormalDepthRenderMap;
    RenderTarget* m_IntermediateRenderTarget;
    
    // Post
    PostProcesser* m_PostProcesser;

    // Settings
    RectI m_Viewport;
    RectF m_ViewportPercentage;
    bool m_UsePercentage;

    gfx::Window* m_Window;

    gfx::CameraSettings m_CameraSettings;
    gfx::StereoSetting m_Stereo;

    he::PrimitiveList<IRenderer*> m_PrePostRenderPlugins;
    he::PrimitiveList<IRenderer*> m_PostPostRenderPlugins;

    // Events
    he::eventCallback2<void, int32, int32> m_WindowResizedCallback;

};

} } //end namespace

#endif
