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
#include "ICamera.h"
#include "Game.h"
#include "SkyBox.h"
#include "ControlsManager.h"
#include "OculusRiftBinding.h"
#include "GlobalSettings.h"

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
    m_WindowResizedCallback(boost::bind(&View::resize, this, _1, _2)),
    m_PostProcesser(nullptr),
    m_ColorRenderMap(ResourceFactory<Texture2D>::getInstance()->get(ResourceFactory<Texture2D>::getInstance()->create())), 
    m_NormalDepthRenderMap(ResourceFactory<Texture2D>::getInstance()->get(ResourceFactory<Texture2D>::getInstance()->create())), 
    m_IntermediateRenderTarget(nullptr),
    m_Camera(nullptr),
    m_Stereo(StereoSetting_None)
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
    delete m_PostProcesser;

    m_ColorRenderMap->release();
    m_NormalDepthRenderMap->release();
}

void View::init( const CameraSettings& cameraSettings, const bool forceDisablePost)
{
    m_CameraSettings = cameraSettings;
    const gfx::RenderSettings& renderSettings(GlobalSettings::getInstance()->getRenderSettings());
    setStereo(renderSettings.stereoSetting, true);

    HE_ASSERT(ViewportSizeChanged.empty() == true, "Do not register events before View::init"); // we want to be first in line
    he::eventCallback0<void> updateCameraAspect([this]()
    {
        if (m_Camera != nullptr)
        {
            m_Camera->setAspectRatio(m_Viewport.width / static_cast<float>(m_Viewport.height));
        }
    });
    ViewportSizeChanged += updateCameraAspect;
    const bool postEnabled(renderSettings.enablePost && forceDisablePost == false && m_PrePostRenderPlugins.empty() == false);
    if (postEnabled)
    {
        he::eventCallback0<void> resizeCallback([this, postEnabled]()
        {
            if (postEnabled)
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
    uint32 width(m_Viewport.width), 
        height(m_Viewport.height);

    if (postEnabled)
    {
        // Color
        m_ColorRenderMap->init(gfx::TextureWrapType_Clamp,  gfx::TextureFilterType_Nearest,
            renderSettings.postSettings.shaderSettings.enableHDR ? gfx::TextureFormat_RGBA16F : gfx::TextureFormat_RGBA8, false);
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

    const RenderTarget* const outputRT(m_Window->getRenderTarget());

    m_PrePostRenderPlugins.sort(&rendererSorter);
    m_PostPostRenderPlugins.sort(&rendererSorter);
    m_PrePostRenderPlugins.forEach([this, outputRT, postEnabled](IRenderer* renderer)
    {
        renderer->init(this, postEnabled? m_IntermediateRenderTarget : outputRT);
    });
    if (postEnabled)
    {
        m_PostProcesser = NEW PostProcesser();
        m_PostProcesser->init(this, outputRT, m_IntermediateRenderTarget);
    }
    m_PostPostRenderPlugins.forEach([this, outputRT](IRenderer* renderer)
    {
        renderer->init(this, outputRT);
    });

}
//  Setters  //////////////////////////////////////////////////////////////
void View::setStereo( const StereoSetting stereo, const bool force )
{
    if (stereo != m_Stereo || force)
    {
        switch (stereo)
        {
            case StereoSetting_None:
            {
                if (m_Camera != nullptr)
                {
                    m_Camera->setAspectRatio(m_Viewport.width / static_cast<float>(m_Viewport.height));
                    if (m_Camera->getCameraType() == eCameraType_Perspective)
                    {
                        m_Camera->setEyeShift(0.0f, 0.0f);
                        m_Camera->setFov(m_CameraSettings.fov);
                    }
                }
                if (m_CameraSettings.useRelativeViewport)
                {
                    setRelativeViewport(RectF(
                        m_CameraSettings.viewport.relativeViewport[0], 
                        m_CameraSettings.viewport.relativeViewport[1], 
                        m_CameraSettings.viewport.relativeViewport[2], 
                        m_CameraSettings.viewport.relativeViewport[3]));
                }
                else
                {
                    setAbsoluteViewport(RectI(
                        m_CameraSettings.viewport.absoluteViewport[0], 
                        m_CameraSettings.viewport.absoluteViewport[1], 
                        m_CameraSettings.viewport.absoluteViewport[2], 
                        m_CameraSettings.viewport.absoluteViewport[3]));
                }
            } break;
            case StereoSetting_OculusRift:
            {
                HE_ASSERT(m_CameraSettings.useRelativeViewport == true, "When using the oculus we must have a relative viewport!");
                // Oculus can only work if we go fullscreen! ( + horizontal splitscreen)
                setRelativeViewport(RectF(0.0f, m_CameraSettings.viewport.relativeViewport[1], 0.5f, m_CameraSettings.viewport.relativeViewport[3]));
            } break;
            default:
            {
                LOG(LogType_ProgrammerAssert, "Unknown stereo mode");
            }
        }
        m_Stereo = stereo;
    }
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
void View::setWindow( Window* window, const EViewInsertMode mode )
{
    HE_ASSERT(window != nullptr, "active window can not be nullptr!");
    if (m_Window != nullptr)
    {
        m_Window->Resized -= m_WindowResizedCallback;
        m_Window->removeView(getHandle());
    }
    m_Window = window;
    m_Window->Resized += m_WindowResizedCallback;
    if (mode == eViewInsertMode_Last)
    {
        m_Window->addViewAtEnd(getHandle());
    }
    else
    {
        m_Window->addViewAtBegin(getHandle());
    }
}
void View::setAbsoluteViewport( const RectI& viewport, const bool force )
{
    if (force || m_Viewport.width != viewport.width || m_Viewport.height != viewport.height)
    {
        m_Viewport = viewport;
        m_UsePercentage = false;
        ViewportSizeChanged();
    }
    else
    {
        m_Viewport = viewport;
    }
}
void View::setRelativeViewport( const RectF& viewportPercentage, const bool force )
{
    m_ViewportPercentage = viewportPercentage;
    m_UsePercentage = true;
    if (m_Window != nullptr)
    {
        RectI oldViewPort(m_Viewport);
        calcViewportFromPercentage();
        if (force || m_Viewport.width != oldViewPort.width || m_Viewport.height != oldViewPort.height)
        {
            ViewportSizeChanged();
        }
    }
}
void View::resize(const int32 /*width*/, const int32 /*height*/)
{
    RectI oldViewPort(m_Viewport);
    calcViewportFromPercentage();

    if (m_Viewport.width != oldViewPort.width || m_Viewport.height != oldViewPort.height)
    {
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


void View::setCamera( ICamera* const camera )
{
    m_Camera = camera;
    if (m_Camera != nullptr)
    {
        if (m_Camera->getCameraType() == eCameraType_Ortho)
        {
            setStereo(StereoSetting_None, false);
        }
        m_Camera->setAspectRatio(m_Viewport.width / static_cast<float>(m_Viewport.height));
    }
}

void View::tick( float dTime )
{
    if (m_Camera != nullptr && m_Window == GRAPHICS->getActiveWindow()) // not a good way but good for now
        m_Camera->tick(dTime);
}

void View::draw()
{
    HE_ASSERT(GL::s_CurrentContext == m_Window->getContext(), "Context Access violation!");

    m_PrePostRenderPlugins.forEach([](IRenderer* const renderer){ renderer->preRender(); });
    m_PostPostRenderPlugins.forEach([](IRenderer* const renderer){ renderer->preRender(); });

    if (m_Stereo == StereoSetting_OculusRift)
    {
        RectI newViewport(m_Viewport);

        io::OculusRiftDevice* const oculus(CONTROLS->getOculusRiftBinding()->getDevice(0));

        const float aspectratio(m_Viewport.width / static_cast<float>(m_Viewport.height));
        oculus->prepareForRendering(aspectratio);

        const float eyeShift(oculus->getEyeShift());
        const float projectedEyeShift(oculus->getProjectedEyeShift());

        if (m_Camera != nullptr)
        {
            m_Camera->setFov(oculus->getFov());
            m_Camera->setAspectRatio(aspectratio);
            m_Camera->setEyeShift(eyeShift, projectedEyeShift);
        }
        render();

        newViewport.x += newViewport.width;
        setAbsoluteViewport(newViewport);
        if (m_Camera != nullptr)
        {
            m_Camera->setEyeShift(-eyeShift, -projectedEyeShift);
        }
        render();

        newViewport.x -= newViewport.width;
        setAbsoluteViewport(newViewport);
    }
    else
    {
        render();
    }
}

void View::render()
{
    if (m_Camera != nullptr)
    {
        m_Camera->prepareForRendering();
    }

    GL::heSetViewport(m_Viewport);

    if (m_IntermediateRenderTarget != nullptr)
        m_IntermediateRenderTarget->clear(Color(0.0f, 0.0f, 0.0f, 0.0f));

    m_PrePostRenderPlugins.forEach([](IRenderer* renderer) { renderer->render(); });

    HE_ASSERT(GL::s_CurrentContext == m_Window->getContext(), "Context Access violation!");
    if (nullptr != m_PostProcesser)
        m_PostProcesser->draw();

    HE_ASSERT(GL::s_CurrentContext == m_Window->getContext(), "Context Access violation!");
    if (m_IntermediateRenderTarget != nullptr)
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_IntermediateRenderTarget->getFboId());
        m_Window->getRenderTarget()->prepareForRendering();
        glBlitFramebuffer(0, 0, m_Viewport.width, m_Viewport.height, 0, 0, m_Viewport.width, m_Viewport.height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }

    HE_ASSERT(GL::s_CurrentContext == m_Window->getContext(), "Context Access violation!");
    m_PostPostRenderPlugins.forEach([](IRenderer* renderer) { renderer->render(); });
}

//////////////////////////////////////////////////////////////////////////

} } //end namespace
