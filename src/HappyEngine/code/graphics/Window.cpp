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
#include "SDL2/SDL.h"

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

    void setupEye(io ::OculusRiftDevice* const device, const RectI& viewport, const int eye);

    Texture2D* m_PreBarrelDistort;
    ModelMesh* m_Quad;
    MaterialInstance* m_Material;

    int8 m_Params[eShaderParams_MAX];
};

void Window::OculusRiftBarrelDistorter::init(const uint32 width, const uint32 height)
{
    gfx::Material* material(CONTENT->loadMaterial("engine/ovrbarreldistort.hm"));
    m_Material = material->createMaterialInstance(eShaderType_Normal);
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
    context.m_CurrentMesh = m_Quad;

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
  : m_ID(0)
  , m_Window(nullptr)
  , m_RenderTarget(nullptr)
  , m_Parent(nullptr)
  , m_ClearColor(0.0f, 0, 0)
  , m_WindowRect(-1, -1, 1280, 720)
  , m_Titel("")
  , m_OVRDistorter(nullptr)
  , m_Flags(eFlags_Resizeable | eFlags_IsVisible)
#ifdef HE_WINDOWS
#elif HE_LINUX
  , m_Display(nullptr)
#endif
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
    destroy();

#ifdef HE_LINUX
    XFreeCursor(m_Display, m_Cursor);
    delete m_Display;
    m_Display = NULL;
#endif
}

bool Window::create()
{
    bool result(false);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#ifdef HE_DEBUG
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    
    uint32 flags(SDL_WINDOW_OPENGL);
    if (checkFlag(eFlags_Resizeable))
        flags |= SDL_WINDOW_RESIZABLE;
    
    m_Window = SDL_CreateWindow(m_Titel.c_str(), 
        m_WindowRect.x == -1? SDL_WINDOWPOS_CENTERED : m_WindowRect.x, 
        m_WindowRect.y == -1? SDL_WINDOWPOS_CENTERED : m_WindowRect.y, 
        m_WindowRect.width, m_WindowRect.height, flags);
    if (m_Window != nullptr &&  m_Context.create(this))
    {
        //m_Window->setKeyRepeatEnabled(true);
        setVSync(checkFlag(eFlags_VSyncEnabled));
        
        GRAPHICS->setActiveWindow(this);
        if (GRAPHICS->registerContext(&m_Context) == true)
        {
            m_RenderTarget = NEW RenderTarget(&m_Context);
            m_RenderTarget->init();
            setOculusRiftEnabled(checkFlag(eFlags_EnableOculusRift));
            raiseFlag(eFlags_IsVisible);
            m_ID = SDL_GetWindowID(m_Window);
            result = true;
        }

        SDL_GetWindowPosition(m_Window, &m_WindowRect.x, &m_WindowRect.y);
    }
    if (result == false)
    {
        HE_ERROR("Window open failed: %s", SDL_GetError());
        if (m_Window != nullptr)
        {
            SDL_DestroyWindow(m_Window);
            m_Window = nullptr;
        }
        m_Context.destroy();
    }
    return result;
}
void Window::destroy()
{
    if (m_Window != nullptr)
    {
        setOculusRiftEnabled(false);
        delete m_RenderTarget;
        m_RenderTarget = nullptr;
        GRAPHICS->unregisterContext(&m_Context);
        SDL_DestroyWindow(m_Window);
        m_Window = nullptr;
        m_Context.destroy();
    }
}
bool Window::isOpen()
{
    return m_Window != nullptr && checkFlag(eFlags_IsVisible);
}
    
void Window::show()
{
    HE_ASSERT(isOpen(), "Window is not open! - fatal");
    if (checkFlag(eFlags_IsVisible) == false)
    {
        SDL_ShowWindow(m_Window);
        raiseFlag(eFlags_IsVisible);
    }
}
    
void Window::hide()
{
    HE_ASSERT(isOpen(), "Window is not open! - fatal");
    if (checkFlag(eFlags_IsVisible) == true)
    {
        SDL_HideWindow(m_Window);
        clearFlag(eFlags_IsVisible);
    }
}

void Window::getWindowPosition( int& x, int& y ) const
{
    x = m_WindowRect.x;
    y = m_WindowRect.y;
}

he::uint32 Window::getWindowWidth() const
{
    return m_WindowRect.width;
}

he::uint32 Window::getWindowHeight() const
{
    return m_WindowRect.height;
}

void Window::setWindowTitle( const he::String& caption )
{
    SDL_SetWindowTitle(m_Window, caption.c_str());
    m_Titel = caption;
}

void Window::setWindowPosition( int x, int y )
{
    SDL_SetWindowPosition(m_Window, x, y);
    m_WindowRect.x = x;
    m_WindowRect.y = y;
}

void Window::setWindowDimension( uint32 width, uint32 height )
{
    m_WindowRect.width = static_cast<int>(width);
    m_WindowRect.height = static_cast<int>(height);
    SDL_SetWindowSize(m_Window, m_WindowRect.width, m_WindowRect.height);
}

void Window::setVSync( bool enable )
{
    setFlag(eFlags_VSyncEnabled, enable);
    SDL_GL_SetSwapInterval(enable? 1 : 0);
}

void Window::prepareForRendering()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    m_Context.makeCurrent();
    GRAPHICS->setActiveContext(&m_Context);
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
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    SDL_GL_SwapWindow(m_Window);
}

void Window::setFullscreen( bool fullscreen )
{
    if (checkFlag(eFlags_Fullscreen) != fullscreen)
    {
        setFlag(eFlags_Fullscreen, fullscreen);
        SDL_SetWindowFullscreen(m_Window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
    }
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

void Window::setMousePosition( const vec2& pos )
{
    SDL_WarpMouseInWindow(m_Window, static_cast<int>(pos.x), static_cast<int>(pos.y));
}

void Window::addViewAtBegin( const ObjectHandle& view )
{
    HE_IF_ASSERT(!m_Views.contains(view), "View already attached to window!")
    {
        m_Views.insert(view, 0);
    }
}
void Window::addViewAtEnd( const ObjectHandle& view )
{
    HE_IF_ASSERT(!m_Views.contains(view), "View already attached to window!")
    {
        m_Views.add(view);
    }
}

void Window::removeView( const ObjectHandle& view )
{
    HE_IF_ASSERT(m_Views.contains(view), "View not attached to window!")
    {
        m_Views.remove(view);
    }
}
    
    
} } //end namespace
