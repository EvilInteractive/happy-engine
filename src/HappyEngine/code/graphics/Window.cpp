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
#include "HappyPCH.h" 

#include "Window.h"
#include "GraphicsEngine.h"
#include "ControlsManager.h"
#include "IKeyboard.h"
#include "IMouse.h"
#include "RenderTarget.h"
#include "Texture2D.h"
#include "ContentManager.h"
#include "OculusRiftBinding.h"
#include "ModelMesh.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialParameter.h"
#include "ShaderUniform.h"
#include "DrawContext.h"

#include "OpenGL.h"

namespace he {
namespace gfx {
IMPLEMENT_OBJECT(Window)

struct Window::OculusRiftBarrelDistorter
{
    enum EShaderParams
    {
        eShaderParams_Texture,
        eShaderParams_HmdWarpParam,
        eShaderParams_LensCenter,
        eShaderParams_ScreenCenter,
        eShaderParams_Scale,
        eShaderParams_ScaleIn,
        eShaderParams_TcTransform,

        eShaderParams_MAX
    };

    OculusRiftBarrelDistorter()
        : m_PreBarrelDistort(nullptr)
        , m_Material(nullptr)
        , m_Quad(nullptr)
    {
        he_memset(m_Params, 0, sizeof(int8) * eShaderParams_MAX);
    }

    void init(const uint32 width, const uint32 height);
    void destroy();

    void distort(const uint32 width, const uint32 height);

    void resize(const uint32 width, const uint32 height);

    void setupEye(io::OculusRiftDevice* const device, const RectI& viewport, const int eye);

    Texture2D* m_PreBarrelDistort;
    ModelMesh* m_Quad;
    MaterialInstance* m_Material;

    int8 m_Params[eShaderParams_MAX];
};

void Window::OculusRiftBarrelDistorter::init(const uint32 width, const uint32 height)
{
    gfx::Material* material(CONTENT->loadMaterial("engine/post/ovrbarreldistort.hm"));
    m_Material = material->createMaterialInstance(eShaderRenderType_Normal);
    material->release();


    m_Params[eShaderParams_Texture] = m_Material->findParameter(HEFS::strpreDistortMap);
    m_Params[eShaderParams_HmdWarpParam] = m_Material->findParameter(HEFS::strhmdWarpParam);
    m_Params[eShaderParams_LensCenter] = m_Material->findParameter(HEFS::strlensCenter);
    m_Params[eShaderParams_ScreenCenter] = m_Material->findParameter(HEFS::strscreenCenter);
    m_Params[eShaderParams_Scale] = m_Material->findParameter(HEFS::strscale);
    m_Params[eShaderParams_ScaleIn] = m_Material->findParameter(HEFS::strscaleIn);
    m_Params[eShaderParams_TcTransform] = m_Material->findParameter(HEFS::strtcTransform);

    TextureFactory* const textureFactory(TextureFactory::getInstance());
    m_PreBarrelDistort = textureFactory->get(textureFactory->create());
    m_PreBarrelDistort->init(TextureWrapType_Clamp, TextureFilterType_Linear, TextureFormat_RGBA8, false);
    m_PreBarrelDistort->setData(width, height, 0, TextureBufferLayout_BGRA, TextureBufferType_Byte);

    m_Quad = CONTENT->getFullscreenQuad();

    m_Material->calculateMaterialLayout(m_Quad->getVertexLayout());
}

void Window::OculusRiftBarrelDistorter::resize( const uint32 width, const uint32 height )
{
    m_PreBarrelDistort->setData(width, height, 0, TextureBufferLayout_BGRA, TextureBufferType_Byte);
}

void Window::OculusRiftBarrelDistorter::destroy()
{
    if (m_Material != nullptr)
    {
        delete m_Material;
        m_Material = nullptr;
    }
    if (m_PreBarrelDistort != nullptr)
    {
        m_PreBarrelDistort->release();
        m_PreBarrelDistort = nullptr;
    }
    if (m_Quad != nullptr)
    {
        m_Quad->release();
        m_Quad = nullptr;
    }
}

void Window::OculusRiftBarrelDistorter::setupEye( io ::OculusRiftDevice* const device, const RectI& viewport, const int eye )
{
    GL::heSetViewport(viewport);

    const float distShift(device->getDistortionShift());
    m_Material->getParameter(m_Params[eShaderParams_LensCenter]).setFloat2(vec2(0.25f + eye * 0.25f + (0.5f + distShift * -eye * 0.5f) * 0.5f, 0.5f));
    m_Material->getParameter(m_Params[eShaderParams_ScreenCenter]).setFloat2(vec2(0.5f + eye * 0.25f, 0.5f));
    m_Material->getParameter(m_Params[eShaderParams_TcTransform]).setFloat4(vec4(0.25f + eye * 0.25f, 0.0f, 0.5f, 1.0f));
}

void Window::OculusRiftBarrelDistorter::distort(const uint32 width, const uint32 height)
{
    io::OculusRiftBinding* const oculusBinding(CONTROLS->getOculusRiftBinding());
    io::OculusRiftDevice* const device(oculusBinding->getDevice(0));

    m_Material->getParameter(m_Params[eShaderParams_Texture]).setTexture2D(m_PreBarrelDistort);
    m_Material->getParameter(m_Params[eShaderParams_HmdWarpParam]).setFloat4(device->getWarpParams());

    RectI viewport(0, 0, width / 2, height);

    const float aspectRatio(viewport.width / static_cast<float>(viewport.height));
    const float scale(1.0f / device->getDistortionScale());
    m_Material->getParameter(m_Params[eShaderParams_Scale]).setFloat2(vec2(0.5f / 2.0f * scale, (1.0f / 2.0f) * scale * aspectRatio));
    m_Material->getParameter(m_Params[eShaderParams_ScaleIn]).setFloat2(vec2(2.0f / 0.5f, (2.0f / 1.0f) / aspectRatio));
    
    DrawContext context;
    context.m_VBO = m_Quad->getVBO();
    context.m_IBO = m_Quad->getIBO();

    setupEye(device, viewport, -1);
    m_Material->apply(context);
    m_Quad->draw();

    viewport.x += viewport.width;
    setupEye(device, viewport, 1);
    m_Material->apply(context);
    m_Quad->draw();
}


#pragma warning(disable:4355) // use of this in initializer list
Window::Window()
  : m_RenderTarget(nullptr)
  , m_ClearColor(0.0f, 0, 0)
  , m_WindowRect(-1, -1, 1280, 720)
  , m_Titel("")
  , m_OVRDistorter(nullptr)
  , m_Flags(eFlags_Resizeable | eFlags_IsVisible)
{
    he::eventCallback2<void, int32, int32> resizedCallback([this](const int32 width, const int32 height)
    {
       m_WindowRect.width = width;
       m_WindowRect.height = height;
       if (checkFlag(eFlags_EnableOculusRift))
       {
           m_OVRDistorter->resize(m_WindowRect.width, m_WindowRect.height);
           m_RenderTarget->setSize(m_WindowRect.width, m_WindowRect.height);
           m_RenderTarget->resizeDepthBuffer(m_WindowRect.width, m_WindowRect.height);
       }
    });
    Resized += resizedCallback;
    
    he::eventCallback2<void, int32, int32> movedCallback([this](const int32 x, const int32 y)
    {
       m_WindowRect.x = x;
       m_WindowRect.y = y;
    });
    Moved += movedCallback;
}
#pragma warning(default:4355)


Window::~Window()
{
}

bool Window::create(const bool show)
{
    bool result(false);
    if (getContext()->create(this))
    {
        setVSync(checkFlag(eFlags_VSyncEnabled));

        GraphicsEngine* gfxEngine(GRAPHICS);
        gfxEngine->setActiveWindow(this);
        if (gfxEngine->registerContext(getContext()) == true)
        {
            result = true;
        }

    }
    if (result == false)
    {
        HE_ERROR("Window open failed: %s", SDL_GetError());
        destroy();
    }
    else
    {
        m_RenderTarget = NEW RenderTarget(getContext());
        m_RenderTarget->init();
        setOculusRiftEnabled(checkFlag(eFlags_EnableOculusRift));
        if (show)
            raiseFlag(eFlags_IsVisible);
    }
    return result;
}

void Window::destroy()
{
    setOculusRiftEnabled(false);
    delete m_RenderTarget;
    m_RenderTarget = nullptr;
    GRAPHICS->unregisterContext(getContext());
}

void Window::show()
{
    raiseFlag(eFlags_IsVisible);
}

void Window::hide()
{
    clearFlag(eFlags_IsVisible);
}

void Window::setWindowTitle( const he::String& caption )
{
    m_Titel = caption;
}

void Window::setWindowPosition( int x, int y )
{
    m_WindowRect.x = x;
    m_WindowRect.y = y;
}

void Window::setWindowDimension( uint32 width, uint32 height )
{
    m_WindowRect.width = static_cast<int>(width);
    m_WindowRect.height = static_cast<int>(height);
}

void Window::setVSync( bool enable )
{
    setFlag(eFlags_VSyncEnabled, enable);
}

void Window::prepareForRendering()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    GLContext* context(getContext());
    context->makeCurrent();
    GRAPHICS->setActiveContext(context);
    if (checkFlag(eFlags_EnableOculusRift))
    {
        m_RenderTarget->clear(m_ClearColor);
    }
}

void Window::finishRendering()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    if (checkFlag(eFlags_EnableOculusRift))
    {
        const GLenum buffers(GL_BACK);
        GL::heBindFbo(0);
        glDrawBuffers(1, &buffers);
        m_OVRDistorter->distort(getWindowWidth(), getWindowHeight());
    }
}

void Window::present()
{
}

void Window::setFullscreen( bool fullscreen )
{
    setFlag(eFlags_Fullscreen, fullscreen);
}

void Window::setOculusRiftEnabled( const bool enable )
{
    if (m_RenderTarget != nullptr)
    {
        if (enable == true)
        {
            if (m_OVRDistorter == nullptr)
            {
                m_OVRDistorter = NEW OculusRiftBarrelDistorter();
                m_OVRDistorter->init(m_WindowRect.width, m_WindowRect.height);

                m_RenderTarget->removeAllTargets();
                m_RenderTarget->setSize(m_WindowRect.width, m_WindowRect.height);
                m_RenderTarget->addTextureTarget(m_OVRDistorter->m_PreBarrelDistort);
                m_RenderTarget->setDepthTarget();
                m_RenderTarget->init();
            }
        }
        else
        {
            if (m_OVRDistorter != nullptr)
            {
                m_RenderTarget->removeAllTargets();
                m_RenderTarget->init();

                m_OVRDistorter->destroy();
                delete m_OVRDistorter;
                m_OVRDistorter = nullptr;
            }
        }
    }
    setFlag(eFlags_EnableOculusRift, enable);
}

void Window::setResizable( bool resizable )
{
    setFlag(eFlags_Resizeable, resizable);
}

void Window::setMousePosition( const vec2& /*pos*/)
{
}

void Window::addViewAtBegin( const ObjectHandle& view )
{
    HE_IF_ASSERT(!m_Views.contains(view), "View already attached to Window!")
    {
        m_Views.insert(view, 0);
    }
}
void Window::addViewAtEnd( const ObjectHandle& view )
{
    HE_IF_ASSERT(!m_Views.contains(view), "View already attached to Window!")
    {
        m_Views.add(view);
    }
}

void Window::removeView( const ObjectHandle& view )
{
    HE_IF_ASSERT(m_Views.contains(view), "View not attached to Window!")
    {
        m_Views.remove(view);
    }
}
    
    
} } //end namespace
