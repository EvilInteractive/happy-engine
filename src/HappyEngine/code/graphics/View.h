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
class IRenderer3D;
class Forward3DRenderer;
class ShadowCaster;
class PostProcesser;
class RenderTarget;
class Renderer2D;
class Texture2D;
class ShapeRenderer;
class CameraPerspective;
class View;
class View2D;
class View3D;
class SkyBox;

class ViewFactory: public ObjectFactory<View>, public Singleton<ViewFactory>
{
    friend class Singleton<ViewFactory>;

    ViewFactory() { init(1, 2, "ViewFactory"); }

public:
    virtual ~ViewFactory() { }

    ObjectHandle createView3D();
    ObjectHandle createView2D();

private:
    // Disable
    virtual ObjectHandle create() { HE_ASSERT(false, "DO NOT USE"); return ObjectHandle::unassigned; }
};

class View
{
DECLARE_OBJECT(View)
    friend class ObjectFactory<View>;
protected:
    View();
public:
    virtual ~View();

    virtual void init(const RenderSettings& settings);

    void setAbsoluteViewport(const RectI& viewport);
    void setRelativeViewport(const RectF& viewportPercentage);
    const RectI& getViewport() const { return m_Viewport; }

    void setWindow(Window* window);
    Window* getWindow() const { return m_Window; }

    virtual Renderer2D* get2DRenderer() const { HE_ASSERT(false, "Pure virtual method call"); return nullptr; }

    const RenderSettings& getSettings() const { return m_Settings; }

    event0<void> SettingsChanged;
    event0<void> ViewportSizeChanged;

    // INTERNAL
    virtual void draw() {};

protected:
    void resize();
    void calcViewportFromPercentage();

    RectI m_Viewport;
    RectF m_ViewportPercentage;
    bool m_UsePercentage;

    gfx::Window* m_Window;

    RenderSettings m_Settings;
    
    // Events
    he::eventCallback0<void> m_WindowResizedCallback;

    //Disable default copy constructor and default assignment operator
    View(const View&);
    View& operator=(const View&);

};

class View2D : public View
{
    friend class ViewFactory;
private:
    View2D();
public:
    virtual ~View2D();

    virtual void init(const RenderSettings& settings);
    virtual Renderer2D* get2DRenderer() const { return m_2DRenderer; }

    virtual void draw();
private:
    Renderer2D* m_2DRenderer;
    RenderTarget* m_OutputRenderTarget;
};

class View3D : public View, public ge::ITickable
{
    friend class ViewFactory;
private:
    View3D();
public:
    virtual ~View3D();

    // Init
    virtual void init(const RenderSettings& settings);

    // Debug
    std::vector<vec3>& getDebugVertices() { return m_DebugVertices; }
    std::vector<uint>& getDebugIndices() { return m_DebugIndices; }

    // Camera
    void setCamera(const std::string& camera);
    CameraPerspective* getCamera() const { return m_Camera; }

    // Renderers
    virtual Renderer2D* get2DRenderer() const { return m_2DRenderer; }
    ShapeRenderer* getShapeRenderer() const { return m_ShapeRenderer; }

    // Scene
    void setScene(Scene* scene);
    const Scene* getScene() const { return m_Scene; }


    // Update / Draw
    virtual void tick( float dTime );
    virtual void draw();

private:
    // Camera
    CameraPerspective* m_Camera;
    std::string m_CameraId;

    // Scene
    gfx::Scene*  m_Scene;
    gfx::SkyBox* m_SkyBox;

    // Render Textures
    Texture2D* m_ColorRenderMap;
    Texture2D* m_NormalDepthRenderMap;
    RenderTarget* m_IntermediateRenderTarget;
    RenderTarget* m_OutputRenderTarget;

    // Renderers
    IRenderer3D* m_OpacRenderer;
    Forward3DRenderer* m_TransparentRenderer;
    ShapeRenderer* m_ShapeRenderer;
    Renderer2D* m_2DRenderer;

    // Shadow
    ShadowCaster* m_ShadowCaster;

    // Post
    PostProcesser* m_PostProcesser;

    // Debug
    bool m_RenderDebugTextures;
    std::vector<vec3> m_DebugVertices;
    std::vector<uint> m_DebugIndices;

};

} } //end namespace

#endif
