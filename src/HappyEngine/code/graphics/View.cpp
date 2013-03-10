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
#include "SkyBox.h"

namespace he {
namespace gfx {
IMPLEMENT_OBJECT(View)
    
int rendererSorter(const IRenderer* a, const IRenderer* b)
{
    RenderPass aPass(a->getRenderPass());
    RenderPass bPass(b->getRenderPass());

    if (aPass < bPass) return -1;
    else if (aPass > bPass) return 1;
    else
    {
        const int aPriority(a->getRenderPriority());
        const int bPriority(b->getRenderPriority());
        return bPriority - aPriority;
    }
}

#pragma warning(disable:4355) // this in initializer list
View::View(): 
    m_Viewport(0, 0, 0, 0),
    m_ViewportPercentage(0, 0, 1, 1),
    m_UsePercentage(true),
    m_Window(nullptr),
    m_WindowResizedCallback(boost::bind(&View::resize, this)),
    m_PostProcesser(nullptr),
    m_ColorRenderMap(ResourceFactory<Texture2D>::getInstance()->get(ResourceFactory<Texture2D>::getInstance()->create())), 
    m_NormalDepthRenderMap(ResourceFactory<Texture2D>::getInstance()->get(ResourceFactory<Texture2D>::getInstance()->create())), 
    m_IntermediateRenderTarget(nullptr),
    m_OutputRenderTarget(nullptr),
    m_Camera(nullptr)
{
    if (m_Window != nullptr)
        m_Window->Resized -= m_WindowResizedCallback;

    m_ColorRenderMap->setName("View::m_ColorRenderMap");
    m_NormalDepthRenderMap->setName("View::m_NormalRenderMap");

    GAME->addToTickList(this);
}
#pragma warning(default:4355)

View::~View()
{
    GAME->removeFromTickList(this);

    delete m_IntermediateRenderTarget;
    delete m_OutputRenderTarget;
    delete m_PostProcesser;

    m_ColorRenderMap->release();
    m_NormalDepthRenderMap->release();
}

void View::init( const RenderSettings& settings )
{
    m_Settings = settings;

    HE_ASSERT(ViewportSizeChanged.empty() == true, "Do not register events before View::init"); // we want to be first in line
    if (settings.enablePost)
    {
        he::eventCallback0<void> resizeCallback([&]()
        {
            if (m_Settings.enablePost)
            {
                // Color
                m_ColorRenderMap->setData(m_Viewport.width, m_Viewport.height, 0,
                    gfx::TextureBufferLayout_BGRA, gfx::TextureBufferType_Byte, 0 );

                // Normal
                m_NormalDepthRenderMap->setData(m_Viewport.width, m_Viewport.height, 0, 
                    gfx::TextureBufferLayout_RGB, gfx::TextureBufferType_Float, 0 );

                m_IntermediateRenderTarget->resizeDepthBuffer(m_Viewport.width, m_Viewport.height);
            }
        });
        ViewportSizeChanged += resizeCallback;

        m_IntermediateRenderTarget = NEW RenderTarget(m_Window->getContext());
    }
    m_OutputRenderTarget = NEW RenderTarget(m_Window->getContext());

    uint32 width(m_Viewport.width), 
        height(m_Viewport.height);

    m_Settings.postSettings.shaderSettings.enableHDR = m_Settings.postSettings.shaderSettings.enableHDR && m_Settings.enablePost;
    if (m_Settings.enablePost)
    {
        // Color
        m_ColorRenderMap->init(gfx::TextureWrapType_Clamp,  gfx::TextureFilterType_Nearest,
            m_Settings.postSettings.shaderSettings.enableHDR ? gfx::TextureFormat_RGBA16F : gfx::TextureFormat_RGBA8, false);
        m_ColorRenderMap->setData(width, height, 0,
            gfx::TextureBufferLayout_BGRA, gfx::TextureBufferType_Byte, 0 );

        // Normal - Depth
        m_NormalDepthRenderMap->init(gfx::TextureWrapType_Clamp, gfx::TextureFilterType_Nearest, 
            gfx::TextureFormat_RGB16, false);
        m_NormalDepthRenderMap->setData(width, height, 0, 
            gfx::TextureBufferLayout_RGB, gfx::TextureBufferType_Float, 0 );

        m_IntermediateRenderTarget->addTextureTarget(m_ColorRenderMap);
        m_IntermediateRenderTarget->addTextureTarget(m_NormalDepthRenderMap);
        m_IntermediateRenderTarget->setDepthTarget();
        m_IntermediateRenderTarget->init();
    }
    m_OutputRenderTarget->init();

    m_PrePostRenderPlugins.sort(&rendererSorter);
    m_PostPostRenderPlugins.sort(&rendererSorter);
    m_PrePostRenderPlugins.forEach([this, &settings](IRenderer* renderer)
    {
        renderer->init(this, settings.enablePost? m_IntermediateRenderTarget : m_OutputRenderTarget);
    });
    if (m_Settings.enablePost)
    {
        m_PostProcesser = NEW PostProcesser();
        m_PostProcesser->init(this, m_OutputRenderTarget, m_IntermediateRenderTarget);
    }
    m_PostPostRenderPlugins.forEach([this](IRenderer* renderer)
    {
        renderer->init(this, m_OutputRenderTarget);
    });

}

//  Plugin  //////////////////////////////////////////////////////////////
void View::addRenderPlugin( IRenderer* renderer )
{
    if (renderer->getRenderPass() < RenderPass_AfterPost)
        m_PrePostRenderPlugins.add(renderer);
    else
        m_PostPostRenderPlugins.add(renderer);
}

//  Window  //////////////////////////////////////////////////////////////
void View::setWindow( Window* window )
{
    HE_ASSERT(window != nullptr, "active window can not be nullptr!");
    if (m_Window != nullptr)
    {
        m_Window->Resized -= m_WindowResizedCallback;
        m_Window->removeView(getHandle());
    }
    m_Window = window;
    m_Window->Resized += m_WindowResizedCallback;
    m_Window->addView(getHandle());
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


void View::setCamera( CameraPerspective* camera )
{
    m_Camera = camera;
}

void View::tick( float dTime )
{
    if (m_Camera != nullptr && m_Window == GRAPHICS->getActiveWindow()) // not a good way but good for now
        m_Camera->tick(dTime);
}

void View::draw()
{
    HE_ASSERT(GL::s_CurrentContext == m_Window->getContext(), "Context Access violation!");
    if (m_Camera != nullptr)
    {
        m_Camera->setAspectRatio(m_Viewport.width / (float)m_Viewport.height);
        m_Camera->prepareForRendering();
    }

    GL::heSetViewport(m_Viewport);
    if (m_IntermediateRenderTarget != nullptr)
        m_IntermediateRenderTarget->clear(Color(0.0f, 0.0f, 0.0f, 0.0f));

    m_PrePostRenderPlugins.forEach([](IRenderer* renderer) { renderer->render(); });

    HE_ASSERT(GL::s_CurrentContext == m_Window->getContext(), "Context Access violation!");
    if (m_Settings.enablePost)
        m_PostProcesser->draw();

    HE_ASSERT(GL::s_CurrentContext == m_Window->getContext(), "Context Access violation!");
    if (m_IntermediateRenderTarget != nullptr)
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_IntermediateRenderTarget->getFboId());
        m_OutputRenderTarget->prepareForRendering();
        glBlitFramebuffer(0, 0, m_Viewport.width, m_Viewport.height, 0, 0, m_Viewport.width, m_Viewport.height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }

    HE_ASSERT(GL::s_CurrentContext == m_Window->getContext(), "Context Access violation!");
    m_PostPostRenderPlugins.forEach([](IRenderer* renderer) { renderer->render(); });
}

//////////////////////////////////////////////////////////////////////////

} } //end namespace
