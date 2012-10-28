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
#include "HappyPCH.h" 

#include "View.h"
#include "CameraManager.h"
#include "Window.h"
#include "Scene.h"
#include "IRenderer.h"
#include "Texture2D.h"

#include "Deferred3DRenderer.h"
#include "Forward3DRenderer.h"
#include "ShapeRenderer.h"
#include "Renderer2D.h"
#include "PostProcesser.h"
#include "ShadowCaster.h"
#include "RenderTarget.h"
#include "GraphicsEngine.h"
#include "CameraPerspective.h"
#include "Game.h"

namespace he {
namespace gfx {
IMPLEMENT_OBJECT(View)

#pragma warning(disable:4355) //'this' : used in base member initializer list
#pragma region View
View::View(): 
    m_Viewport(0, 0, 0, 0),
    m_Window(nullptr),
    m_WindowResizedCallback(boost::bind(&View::resize, this))
{

}
#pragma warning(default:4355) 
View::~View()
{
    if (m_Window != nullptr)
        m_Window->Resized -= m_WindowResizedCallback;
}

void View::setWindow( Window* window )
{
    HE_ASSERT(window != nullptr, "active window can not be nullptr!");
    if (m_Window != nullptr)
        m_Window->Resized -= m_WindowResizedCallback;
    m_Window = window;
    m_Window->Resized += m_WindowResizedCallback;
}

void View::init( const RenderSettings& settings )
{
    m_Settings = settings;
}

void View::setAbsoluteViewport( const RectI& viewport )
{
    m_Viewport = viewport;
    m_UsePercentage = false;
    ViewportSizeChanged();
}

void View::setRelativeViewport( const RectF& viewportPercentage )
{
    m_ViewportPercentage = viewportPercentage;
    m_UsePercentage = true;
    if (m_Window != nullptr)
    {
        calcViewportFromPercentage();
        ViewportSizeChanged();
    }
}

void View::resize()
{
    calcViewportFromPercentage();

    ViewportSizeChanged();
}

void View::calcViewportFromPercentage()
{
    HE_IF_ASSERT(m_Window != nullptr, "Window must first be set!")
    {
        m_Viewport.x      = static_cast<int>(m_Window->getWindowWidth()  * m_ViewportPercentage.x);
        m_Viewport.y      = static_cast<int>(m_Window->getWindowHeight() * m_ViewportPercentage.y);
        m_Viewport.width  = static_cast<int>(m_Window->getWindowWidth()  * m_ViewportPercentage.width);
        m_Viewport.height = static_cast<int>(m_Window->getWindowHeight() * m_ViewportPercentage.height);
    }
}

#pragma endregion

#pragma region View2D

View2D::View2D(): View(), 
    m_2DRenderer(nullptr),
    m_OutputRenderTarget(nullptr)
{

}

View2D::~View2D()
{
    delete m_2DRenderer;
    delete m_OutputRenderTarget;
}

void View2D::init( const RenderSettings& settings )
{
    View::init(settings);

    m_OutputRenderTarget = NEW RenderTarget(m_Window->getContext());
    m_OutputRenderTarget->init();

    m_2DRenderer = NEW Renderer2D();
    m_2DRenderer->init(this, m_OutputRenderTarget);
}

void View2D::draw()
{
    if (m_Window->isOpen())
    {
        m_OutputRenderTarget->prepareForRendering();
        m_OutputRenderTarget->clear(Color(128ui8, 128, 128));
        m_2DRenderer->draw();
        m_Window->present();
    }
}

#pragma endregion

#pragma region View3D
View3D::View3D(): 
    m_ShadowCaster(nullptr), m_OpacRenderer(nullptr), 
    m_TransparentRenderer(nullptr), m_ShapeRenderer(nullptr),
    m_PostProcesser(nullptr), m_2DRenderer(nullptr),
    m_ColorRenderMap(ResourceFactory<Texture2D>::getInstance()->get(ResourceFactory<Texture2D>::getInstance()->create())), 
    m_NormalDepthRenderMap(ResourceFactory<Texture2D>::getInstance()->get(ResourceFactory<Texture2D>::getInstance()->create())), 
    m_RenderDebugTextures(false), m_Scene(nullptr),
    m_IntermediateRenderTarget(nullptr),
    m_OutputRenderTarget(nullptr)
{
    m_ColorRenderMap->setName("View::m_ColorRenderMap");
    m_NormalDepthRenderMap->setName("View::m_NormalRenderMap");

    GAME->addToTickList(this);
}
View3D::~View3D()
{
    GAME->removeFromTickList(this);

    delete m_IntermediateRenderTarget;
    delete m_OutputRenderTarget;
    delete m_OpacRenderer;
    delete m_TransparentRenderer;
    delete m_PostProcesser;
    delete m_ShadowCaster;
    delete m_ShapeRenderer;
    delete m_2DRenderer;

    m_ColorRenderMap->release();
    m_NormalDepthRenderMap->release();
}
void View3D::init( const RenderSettings& settings )
{
    View::init(settings);

    HE_ASSERT(ViewportSizeChanged.empty() == true, "Do not register events before View::init"); // we want to be first in line
    he::eventCallback0<void> resizeCallback([&]()
    {
        if (m_Settings.enablePost)
        {
            // Color
            m_ColorRenderMap->setData(m_Viewport.width, m_Viewport.height, 0,
                gfx::Texture2D::BufferLayout_BGRA, gfx::Texture2D::BufferType_Byte, 0 );

            // Normal
            m_NormalDepthRenderMap->setData(m_Viewport.width, m_Viewport.height, 0, 
                gfx::Texture2D::BufferLayout_RGB, gfx::Texture2D::BufferType_Float, 0 );
        }
    });
    ViewportSizeChanged += resizeCallback;

    m_IntermediateRenderTarget = NEW RenderTarget(m_Window->getContext());
    m_OutputRenderTarget = NEW RenderTarget(m_Window->getContext());

    uint width(m_Viewport.width), 
        height(m_Viewport.height);

    m_Settings.postSettings.shaderSettings.enableHDR = m_Settings.postSettings.shaderSettings.enableHDR && m_Settings.enablePost;
    if (m_Settings.enablePost)
    {
        // Color
        m_ColorRenderMap->init(gfx::Texture2D::WrapType_Clamp,  gfx::Texture2D::FilterType_Nearest,
            m_Settings.postSettings.shaderSettings.enableHDR ? gfx::Texture2D::TextureFormat_RGBA16F : gfx::Texture2D::TextureFormat_RGBA8, false);
        m_ColorRenderMap->setData(width, height, 0,
            gfx::Texture2D::BufferLayout_BGRA, gfx::Texture2D::BufferType_Byte, 0 );

        // Normal - Depth
        m_NormalDepthRenderMap->init(gfx::Texture2D::WrapType_Clamp, gfx::Texture2D::FilterType_Nearest, 
            gfx::Texture2D::TextureFormat_RGB16, false);
        m_NormalDepthRenderMap->setData(width, height, 0, 
            gfx::Texture2D::BufferLayout_RGB, gfx::Texture2D::BufferType_Float, 0 );
    }

    m_IntermediateRenderTarget->addTextureTarget(m_ColorRenderMap);
    m_IntermediateRenderTarget->addTextureTarget(m_NormalDepthRenderMap);
    m_IntermediateRenderTarget->setDepthTarget();
    m_IntermediateRenderTarget->init();
    m_OutputRenderTarget->init();

    m_2DRenderer = NEW Renderer2D();
    m_2DRenderer->init(this, m_OutputRenderTarget);

    m_ShapeRenderer = NEW ShapeRenderer();
    m_ShapeRenderer->init(this, m_IntermediateRenderTarget);

    if (settings.enableDeferred)
    {
        Deferred3DRenderer* d3d;
        m_OpacRenderer = d3d = NEW Deferred3DRenderer();
        m_2DRenderer->attachToRender(d3d);
    }
    else
        m_OpacRenderer = NEW Forward3DRenderer(DrawListContainer::BlendFilter_Opac);
    m_OpacRenderer->init(this, m_IntermediateRenderTarget);

    m_TransparentRenderer = NEW Forward3DRenderer(DrawListContainer::BlendFilter_Blend);
    m_TransparentRenderer->init(this, m_IntermediateRenderTarget);

    //if (settings.lightingSettings.enableShadows)
    {
        m_ShadowCaster = NEW ShadowCaster();
        m_ShadowCaster->init(this);
    }
    if (m_Settings.enablePost)
    {
        m_PostProcesser = NEW PostProcesser();
        m_PostProcesser->init(this, m_OutputRenderTarget, m_IntermediateRenderTarget);
    }
}

void View3D::setScene( Scene* scene )
{
    m_Scene = scene;
    if (m_CameraId != "")
        m_Camera = m_Scene->getCameraManager()->getCamera(m_CameraId);
}

void View3D::draw()
{
    if (m_Window->isOpen())
    {
        m_DebugIndices.clear();
        m_DebugVertices.clear();

        m_Window->prepareForRendering();
        m_Scene->prepareForRendering(); // Not a good way, scene can be attached to multiple views
        GRAPHICS->setActiveView(this);
        m_Camera->setAspectRatio(m_Viewport.width / (float)m_Viewport.height);
        m_Camera->prepareForRendering();

        //if (m_Settings.lightingSettings.enableShadows)
        // Move to Scene
            m_ShadowCaster->render(m_Scene);

        GL::heSetViewport(m_Viewport);
        m_IntermediateRenderTarget->clear(Color(0.0f, 0.0f, 0.0f, 0.0f));
        m_OutputRenderTarget->clear(Color(0.2f, 0.4f, 0.6f, 1.0f));
        m_OpacRenderer->draw();
        m_TransparentRenderer->draw();

        m_ShapeRenderer->draw();

        if (m_Settings.enablePost)
            m_PostProcesser->draw();

        m_2DRenderer->draw();

        m_Window->present();
    }
}

void View3D::setCamera( const std::string& camera )
{
    m_CameraId = camera;
    if (m_Scene != nullptr)
        m_Camera = m_Scene->getCameraManager()->getCamera(m_CameraId);
}

void View3D::tick( float dTime )
{
    if (m_Window == GRAPHICS->getActiveWindow()) // not a good way but good for now
        m_Camera->tick(dTime);
}

#pragma endregion


ObjectHandle ViewFactory::createView3D()
{
    return registerObject(NEW View3D());
}

ObjectHandle ViewFactory::createView2D()
{
    return registerObject(NEW View2D());
}

} } //end namespace
