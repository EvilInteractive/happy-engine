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

namespace he {
namespace gfx {

#pragma warning(disable:4355) //'this' : used in base member initializer list
View::View(): 
    m_Viewport(0, 0, 0, 0),
    m_ShadowCaster(nullptr), m_OpacRenderer(nullptr), 
    m_TransparentRenderer(nullptr), m_ShapeRenderer(nullptr),
    m_PostProcesser(nullptr), m_2DRenderer(nullptr),
    m_ColorRenderMap(ResourceFactory<Texture2D>::getInstance()->get(ResourceFactory<Texture2D>::getInstance()->create())), 
    m_NormalRenderMap(ResourceFactory<Texture2D>::getInstance()->get(ResourceFactory<Texture2D>::getInstance()->create())), 
    m_DepthRenderMap(ResourceFactory<Texture2D>::getInstance()->get(ResourceFactory<Texture2D>::getInstance()->create())),
    m_RenderDebugTextures(false), 
    m_Window(nullptr), m_Scene(nullptr),
    m_IntermediateRenderTarget(NEW RenderTarget()),
    m_OutputRenderTarget(NEW RenderTarget()),
    m_WindowResizedCallback(boost::bind(&View::calcViewportFromPercentage, this))
{
    m_ColorRenderMap->setName("View::m_ColorRenderMap");
    m_NormalRenderMap->setName("View::m_NormalRenderMap");
    m_DepthRenderMap->setName("View::m_DepthRenderMap");
}
#pragma warning(default:4355) 

View::~View()
{
    if (m_Window != nullptr)
        m_Window->Resized -= m_WindowResizedCallback;

    delete m_IntermediateRenderTarget;
    delete m_OutputRenderTarget;
    delete m_OpacRenderer;
    delete m_TransparentRenderer;
    delete m_PostProcesser;
    delete m_ShadowCaster;
    delete m_ShapeRenderer;
    delete m_2DRenderer;
    m_ColorRenderMap->release();
    m_NormalRenderMap->release();
    m_DepthRenderMap->release();
}

void View::init( const RenderSettings& settings )
{
    m_Settings = settings;

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
        
        // Normal
        m_NormalRenderMap->init(gfx::Texture2D::WrapType_Clamp, gfx::Texture2D::FilterType_Nearest, 
            gfx::Texture2D::TextureFormat_RG16, false);
        m_NormalRenderMap->setData(width, height, 0, 
            gfx::Texture2D::BufferLayout_RG, gfx::Texture2D::BufferType_Byte, 0 );
    }
    // Depth
    m_DepthRenderMap->init(gfx::Texture2D::WrapType_Clamp, gfx::Texture2D::FilterType_Nearest, 
        gfx::Texture2D::TextureFormat_Depth32, false);
    m_DepthRenderMap->setData(width, height, 0, 
        gfx::Texture2D::BufferLayout_Depth, gfx::Texture2D::BufferType_Float, 0 );
    
    m_IntermediateRenderTarget->addTextureTarget(m_ColorRenderMap);
    m_IntermediateRenderTarget->addTextureTarget(m_NormalRenderMap);
    m_IntermediateRenderTarget->setDepthTarget(m_DepthRenderMap);
    m_IntermediateRenderTarget->init();
    m_OutputRenderTarget->init();

    m_2DRenderer = NEW Renderer2D();
    m_2DRenderer->init(this, m_OutputRenderTarget);

    m_ShapeRenderer = NEW ShapeRenderer();
    m_ShapeRenderer->init(this, m_OutputRenderTarget);

    if (settings.enableDeferred)
        m_OpacRenderer = NEW Deferred3DRenderer();
    else
        m_OpacRenderer = NEW Forward3DRenderer(DrawListContainer::BlendFilter_Opac);
    m_OpacRenderer->init(this, m_IntermediateRenderTarget);

    m_TransparentRenderer = NEW Forward3DRenderer(DrawListContainer::BlendFilter_Blend);
    m_TransparentRenderer->init(this, m_IntermediateRenderTarget);

    if (settings.lightingSettings.enableShadows)
    {
        m_ShadowCaster = NEW ShadowCaster();
        m_ShadowCaster->init(this);
    }
    if (m_Settings.enablePost)
    {
        m_PostProcesser = NEW PostProcesser();
        m_PostProcesser->init(this, m_OutputRenderTarget, m_IntermediateRenderTarget);
    }

    CONSOLE->registerVar(&m_RenderDebugTextures, "debugRenderTex");
}

void View::setScene( Scene* scene )
{
    m_Scene = scene;
}

void View::setWindow( Window* window )
{
    HE_ASSERT(window != nullptr, "active window can not be nullptr!");
    if (m_Window != nullptr)
        m_Window->Resized -= m_WindowResizedCallback;
    m_Window = window;
    m_Window->Resized += m_WindowResizedCallback;
}

const RectI& View::getViewport() const
{
    return m_Viewport;
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

void View::draw()
{
    m_Window->prepareForRendering();
    m_Scene->prepareForRendering();
    GRAPHICS->setActiveView(this);

    if (m_Settings.lightingSettings.enableShadows)
        m_ShadowCaster->render();

    GL::heSetViewport(m_Viewport);
    m_OpacRenderer->draw();
    m_TransparentRenderer->draw();

    if (m_Settings.enablePost)
        m_PostProcesser->draw();

    m_ShapeRenderer->draw();

    m_2DRenderer->draw();

    m_Window->present();
}


} } //end namespace
