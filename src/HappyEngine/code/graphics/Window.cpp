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
#include "Shader.h"
#include "ContentManager.h"
#include "OculusRiftBinding.h"
#include "ModelMesh.h"

#include "OpenGL.h"
#include <SFML/Window.hpp>

#ifdef HE_WINDOWS
#include <windows.h>
#elif defined(HE_LINUX)
#include <X11/cursorfont.h>
#include <X11/Xlib.h>
#else
//#error This OS is not yet supported for changing the cursor.
#endif

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
        , m_Shader(nullptr)
        , m_Quad(nullptr)
    {
        he_memset(m_Params, 0, sizeof(uint32) * eShaderParams_MAX);
    }

    void init(const uint32 width, const uint32 height);
    void destroy();

    void distort(const uint32 width, const uint32 height);

    void resize(const uint32 width, const uint32 height);

    void setupEye(io ::OculusRiftDevice* const device, const RectI& viewport, const int eye);

    Texture2D* m_PreBarrelDistort;
    Shader* m_Shader;
    ModelMesh* m_Quad;

    uint32 m_Params[eShaderParams_MAX];
};

void Window::OculusRiftBarrelDistorter::init(const uint32 width, const uint32 height)
{
    HE_ASSERT(m_Shader == nullptr, "OVR barrel distort shader already initialized");

    ShaderFactory* const shaderFactory(ShaderFactory::getInstance());
    m_Shader = shaderFactory->get(shaderFactory->create());

    ShaderLayout shaderLayout;
    shaderLayout.addElement(ShaderLayoutElement(0, "inPosition"));

    const he::String& folder(CONTENT->getShaderFolderPath().str());

    m_Shader->initFromFile(folder + "shared/postShaderQuad.vert", 
        folder + "post/vrbarreldistort.frag", shaderLayout);

    m_Params[eShaderParams_Texture] = m_Shader->getShaderSamplerId("preDistortMap"); 
    m_Params[eShaderParams_HmdWarpParam] = m_Shader->getShaderVarId("hmdWarpParam"); 
    m_Params[eShaderParams_LensCenter] = m_Shader->getShaderVarId("lensCenter"); 
    m_Params[eShaderParams_ScreenCenter] = m_Shader->getShaderVarId("screenCenter"); 
    m_Params[eShaderParams_Scale] = m_Shader->getShaderVarId("scale"); 
    m_Params[eShaderParams_ScaleIn] = m_Shader->getShaderVarId("scaleIn"); 
    m_Params[eShaderParams_TcTransform] = m_Shader->getShaderVarId("tcTransform"); 


    TextureFactory* const textureFactory(TextureFactory::getInstance());
    m_PreBarrelDistort = textureFactory->get(textureFactory->create());
    m_PreBarrelDistort->init(TextureWrapType_Clamp, TextureFilterType_Linear, TextureFormat_RGBA8, false);
    m_PreBarrelDistort->setData(width, height, 0, TextureBufferLayout_BGRA, TextureBufferType_Byte);

    m_Quad = CONTENT->getFullscreenQuad();
}

void Window::OculusRiftBarrelDistorter::resize( const uint32 width, const uint32 height )
{
    m_PreBarrelDistort->setData(width, height, 0, TextureBufferLayout_BGRA, TextureBufferType_Byte);
}

void Window::OculusRiftBarrelDistorter::destroy()
{
    if (m_Shader != nullptr)
    {
        m_Shader->release();
        m_Shader = nullptr;
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
    m_Shader->setShaderVar(m_Params[eShaderParams_LensCenter], vec2(0.25f + eye * 0.25f + (0.5f + distShift * -eye * 0.5f) * 0.5f, 0.5f));
    m_Shader->setShaderVar(m_Params[eShaderParams_ScreenCenter], vec2(0.5f + eye * 0.25f, 0.5f));
    m_Shader->setShaderVar(m_Params[eShaderParams_TcTransform], vec4(0.25f + eye * 0.25f, 0.0f, 0.5f, 1.0f));
}

void Window::OculusRiftBarrelDistorter::distort(const uint32 width, const uint32 height)
{
    io::OculusRiftBinding* const oculusBinding(CONTROLS->getOculusRiftBinding());
    io ::OculusRiftDevice* const device(oculusBinding->getDevice(0));

    GL::heBlendEnabled(false);
    GL::heSetDepthWrite(false);
    GL::heSetDepthRead(false);
    GL::heSetCullFace(false);

    m_Shader->bind();
    m_Shader->setShaderVar(m_Params[eShaderParams_Texture], m_PreBarrelDistort);
    m_Shader->setShaderVar(m_Params[eShaderParams_HmdWarpParam], device->getWarpParams());

    RectI viewport(0, 0, width / 2, height);

    const float aspectRatio(viewport.width / static_cast<float>(viewport.height));
    const float scale(1.0f / device->getDistortionScale());
    m_Shader->setShaderVar(m_Params[eShaderParams_Scale], vec2(0.5f / 2.0f * scale, (1.0f / 2.0f) * scale * aspectRatio));
    m_Shader->setShaderVar(m_Params[eShaderParams_ScaleIn], vec2(2.0f / 0.5f, (2.0f / 1.0f) / aspectRatio));
    
    setupEye(device, viewport, -1);
    GL::heBindVao(m_Quad->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_Quad->getNumIndices(), m_Quad->getIndexType(), 0);

    viewport.x += viewport.width;
    setupEye(device, viewport, 1);
    GL::heBindVao(m_Quad->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_Quad->getNumIndices(), m_Quad->getIndexType(), 0);
}


#pragma warning(disable:4355) // use of this in initializer list
Window::Window() 
  : m_Window(NEW sf::Window())
  , m_RenderTarget(nullptr)
  , m_Parent(nullptr)
  , m_ClearColor(0.0f, 0, 0)
  , m_WindowRect(-1, -1, 1280, 720)
  , m_Titel("")
  , m_Context(this)
  , m_OVRDistorter(nullptr)
  , m_Flags(eFlags_IsCursorVisible | eFlags_Resizeable | eFlags_IsVisible)
#ifdef HE_WINDOWS
  , m_Cursor(0)
#elif HE_LINUX
  , m_Cursor(0)
  , m_Display(nullptr)
#endif
{
}
#pragma warning(default:4355)


Window::~Window()
{
    destroy();
    delete m_Window;

#ifdef HE_LINUX
    XFreeCursor(m_Display, m_Cursor);
    delete m_Display;
    m_Display = NULL;
#endif
}

void Window::create(Window* parent)
{
    m_Parent = parent;
    HE_ASSERT(m_Parent == nullptr || m_Parent->isOpen(), "Parent window is not open!");
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 3;
    settings.minorVersion = 2;
#if defined(DEBUG) | defined(_DEBUG)
    settings.debug = true;
#endif
    settings.compatibility = false;
    if (m_Parent == nullptr)
    {
        m_Window->create(sf::VideoMode(m_WindowRect.width, m_WindowRect.height, 32), m_Titel, 
            checkFlag(eFlags_Fullscreen)? sf::Style::Fullscreen : (checkFlag(eFlags_Resizeable)? sf::Style::Resize | sf::Style::Close : sf::Style::Close), settings);
    }
    else
    {
        m_Window->create(m_Parent->m_Window->getSystemHandle(), settings);
        m_Window->setTitle(m_Titel);
        m_Window->setSize(sf::Vector2u(m_WindowRect.width, m_WindowRect.height));
    }
    m_Window->setKeyRepeatEnabled(true);
    setWindowPosition(m_WindowRect.x, m_WindowRect.y);
    setCursorVisible(checkFlag(eFlags_IsCursorVisible));
    setVSync(checkFlag(eFlags_VSyncEnabled));
    setCursor(io::MouseCursor_Pointer);
    
    GRAPHICS->setActiveWindow(this);

    GRAPHICS->setActiveContext(&m_Context);
    GL::init();

    if (GRAPHICS->registerContext(&m_Context) == false)
    {
        m_Window->close();
    }

    m_RenderTarget = NEW RenderTarget(&m_Context);
    m_RenderTarget->init();
    setOculusRiftEnabled(checkFlag(eFlags_EnableOculusRift));
}
void Window::destroy()
{
    setOculusRiftEnabled(false);
    delete m_RenderTarget;
    m_RenderTarget = nullptr;
    GRAPHICS->unregisterContext(&m_Context);
    if (m_Window->isOpen())
    {
        close();
        m_Window->close();
    }
}
bool Window::isOpen()
{
    return m_Window->isOpen() && checkFlag(eFlags_IsVisible);
}

void Window::open()
{
    m_Window->setVisible(true);
    raiseFlag(eFlags_IsVisible);
}

void Window::close()
{
    m_Window->setVisible(false);
    clearFlag(eFlags_IsVisible);
    Closed();
}

void Window::doEvents( float /*dTime*/ )
{
    if (isOpen() == false)
        return;

    sf::Event event;
    while (m_Window->pollEvent(event))
    {
        io::IMouse* mouse(CONTROLS->getMouse());
        io::IKeyboard* keyboard(CONTROLS->getKeyboard());
        bool hasFocus(GRAPHICS->getActiveWindow() == this);

        switch (event.type)
        {
        // Window
        case sf::Event::Closed:
            {
                m_WindowRect.x = m_Window->getPosition().x;
                m_WindowRect.y = m_Window->getPosition().y;
                close();
            } break;
        case sf::Event::Resized:
            {
                m_WindowRect.width = static_cast<int>(m_Window->getSize().x);
                m_WindowRect.height = static_cast<int>(m_Window->getSize().y);
                if (checkFlag(eFlags_EnableOculusRift))
                {
                    m_OVRDistorter->resize(m_WindowRect.width, m_WindowRect.height);
                    m_RenderTarget->setSize(m_WindowRect.width, m_WindowRect.height);
                    m_RenderTarget->resizeDepthBuffer(m_WindowRect.width, m_WindowRect.height);
                }
                Resized();
            } break;
        case sf::Event::GainedFocus:
            {
                GRAPHICS->setActiveWindow(this);
                GainedFocus();
            } break;
        case sf::Event::LostFocus:
            {
                LostFocus();
            } break;

        // Mouse
        case sf::Event::MouseButtonPressed:
            {
                if (hasFocus == true) 
                    mouse->MouseButtonPressed(static_cast<io::MouseButton>(event.mouseButton.button));
            } break;
        case sf::Event::MouseButtonReleased:
            {
                if (hasFocus == true) 
                    mouse->MouseButtonReleased(static_cast<io::MouseButton>(event.mouseButton.button));
            } break;
        case sf::Event::MouseMoved:
            {
                if (hasFocus == true) 
                    mouse->MouseMoved(vec2((float)event.mouseMove.x, (float)event.mouseMove.y));
            } break;            
        case sf::Event::MouseWheelMoved:
            {
                if (hasFocus == true) 
                    mouse->MouseWheelMoved(event.mouseWheel.delta);
            } break;

        // Keyboard
        case sf::Event::KeyPressed:
            {
                if (hasFocus == true)
                    keyboard->KeyPressed(static_cast<io::Key>(event.key.code));
            } break;
        case sf::Event::KeyReleased:
            {
                if (hasFocus == true)
                    keyboard->KeyReleased(static_cast<io::Key>(event.key.code));
            } break;
        case sf::Event::TextEntered:
            {
                if (hasFocus == true)
                    keyboard->TextCharEntered(static_cast<uint32>(event.text.unicode));
            } break;
        }
    }
}

void Window::getWindowPosition( int& x, int& y ) const
{
    x = m_Window->getPosition().x;
    y = m_Window->getPosition().y;
}

he::uint32 Window::getWindowWidth() const
{
    return m_Window->getSize().x;
}

he::uint32 Window::getWindowHeight() const
{
    return m_Window->getSize().y;
}

void Window::setWindowTitle( const he::String& caption )
{
    m_Window->setTitle(caption);
    m_Titel = caption;
}

void Window::setWindowPosition( int x, int y )
{
    m_Window->setPosition(sf::Vector2i(x, y));
    m_WindowRect.x = x;
    m_WindowRect.y = y;
}

void Window::setWindowDimension( uint32 width, uint32 height )
{
    m_Window->setSize(sf::Vector2u(width, height));
    m_WindowRect.width = static_cast<int>(width);
    m_WindowRect.height = static_cast<int>(height);
}

void Window::setVSync( bool enable )
{
    setFlag(eFlags_VSyncEnabled, enable);
    m_Window->setVerticalSyncEnabled(enable);
}

void Window::setCursorVisible( bool visible )
{
    setFlag(eFlags_IsCursorVisible, visible);
    m_Window->setMouseCursorVisible(visible);
}

void Window::prepareForRendering()
{
    GRAPHICS->setActiveContext(&m_Context);
    if (checkFlag(eFlags_EnableOculusRift))
    {
        m_RenderTarget->clear(m_ClearColor);
    }
}

void Window::finishRendering()
{
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
    m_Window->display();
}

void Window::setFullscreen( bool fullscreen )
{
    if (checkFlag(eFlags_Fullscreen) != fullscreen)
    {
        setFlag(eFlags_Fullscreen, fullscreen);
        if (m_Window->isOpen())
        {
            close();
            open();
        }
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
    sf::Mouse::setPosition(sf::Vector2i(static_cast<int>(pos.x), static_cast<int>(pos.y)), *m_Window);
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

#ifdef HE_WINDOWS
void Window::setCursor( const io::MouseCursor cursor )
{
    LPCSTR cursorName(IDC_ARROW);
    switch(cursor)
    {
    case io::MouseCursor_Progress: 
        cursorName = IDC_APPSTARTING; break;

    case io::MouseCursor_Custom:
    case io::MouseCursor_ZoomIn:
    case io::MouseCursor_ZoomOut:
    case io::MouseCursor_Pointer:
    case io::MouseCursor_Copy:
    case io::MouseCursor_None:
    case io::MouseCursor_VerticalText:
    case io::MouseCursor_Cell:
    case io::MouseCursor_ContextMenu:
    case io::MouseCursor_Alias: 
        cursorName = IDC_ARROW; break;

    case io::MouseCursor_Cross: 
        cursorName = IDC_CROSS; break;

    case io::MouseCursor_Grab:
    case io::MouseCursor_Grabbing:
    case io::MouseCursor_Hand: 
        cursorName = IDC_HAND; break;

    case io::MouseCursor_Help: 
        cursorName = IDC_HELP; break;

    case io::MouseCursor_IBeam: 
        cursorName = IDC_IBEAM; break;

    case io::MouseCursor_NoDrop:
    case io::MouseCursor_NotAllowed: 
        cursorName = IDC_NO; break;

    case io::MouseCursor_Move: 
    case io::MouseCursor_MiddlePanning:
    case io::MouseCursor_EastPanning:
    case io::MouseCursor_WestPanning:
    case io::MouseCursor_NorthPanning:
    case io::MouseCursor_SouthPanning:
    case io::MouseCursor_NorthEastPanning:
    case io::MouseCursor_SouthWestPanning:
    case io::MouseCursor_NorthWestPanning:
    case io::MouseCursor_SouthEastPanning:
        cursorName = IDC_SIZEALL; break;

    case io::MouseCursor_NorthEastSouthWestResize: 
    case io::MouseCursor_NorthEastResize:
    case io::MouseCursor_SouthWestResize:
        cursorName = IDC_SIZENESW; break;

    case io::MouseCursor_NorthSouthResize: 
    case io::MouseCursor_NorthResize:
    case io::MouseCursor_RowResize:
    case io::MouseCursor_SouthResize:
        cursorName = IDC_SIZENS; break;

    case io::MouseCursor_NorthWestResize: 
    case io::MouseCursor_NorthWestSouthEastResize:
    case io::MouseCursor_SouthEastResize:
        cursorName = IDC_SIZENWSE; break;

    case io::MouseCursor_EastWestResize: 
    case io::MouseCursor_EastResize:
    case io::MouseCursor_ColumnResize:
    case io::MouseCursor_WestResize:
        cursorName = IDC_SIZEWE; break;

    case io::MouseCursor_Wait: 
        cursorName = IDC_WAIT; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknow mouse cursor %d", cursor);
    }

    m_Cursor = LoadCursor(NULL, cursorName);
    SetClassLongPtr(m_Window->getSystemHandle(), GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(m_Cursor));
}

#elif defined(HE_LINUX)
void Window::setCursor( const io::MouseCursor cursor )
{
    unsigned int shape(XC_left_ptr);
    switch(cursor)
    {
    case io::MouseCursor_Progress: 
        shape = XC_watch; break;

    case io::MouseCursor_Custom:
    case io::MouseCursor_ZoomIn:
    case io::MouseCursor_ZoomOut:
    case io::MouseCursor_Pointer:
    case io::MouseCursor_Copy:
    case io::MouseCursor_None:
    case io::MouseCursor_VerticalText:
    case io::MouseCursor_Cell:
    case io::MouseCursor_ContextMenu:
    case io::MouseCursor_Alias: 
        shape = XC_left_ptr; break;

    case io::MouseCursor_Cross: 
        shape = XC_tcross; break;

    case io::MouseCursor_Grab:
    case io::MouseCursor_Grabbing:
    case io::MouseCursor_Hand: 
        shape = XC_hand1; break;

    case io::MouseCursor_Help: 
        shape = XC_question_arrow; break;

    case io::MouseCursor_IBeam: 
        shape = XC_xterm; break;

    case io::MouseCursor_NoDrop:
    case io::MouseCursor_NotAllowed: 
        shape = XC_X_cursor; break;

    case io::MouseCursor_Move: 
    case io::MouseCursor_MiddlePanning:
    case io::MouseCursor_EastPanning:
    case io::MouseCursor_WestPanning:
    case io::MouseCursor_NorthPanning:
    case io::MouseCursor_SouthPanning:
    case io::MouseCursor_NorthEastPanning:
    case io::MouseCursor_SouthWestPanning:
    case io::MouseCursor_NorthWestPanning:
    case io::MouseCursor_SouthEastPanning:
        shape = XC_fleur; break;

    case io::MouseCursor_NorthEastResize:
        shape = XC_top_right_corner; break;

    case io::MouseCursor_SouthWestResize:
        shape = XC_bottom_left_corner; break;
 
    case io::MouseCursor_NorthResize:
        shape = XC_top_side; break;

    case io::MouseCursor_NorthSouthResize:
        shape = XC_sb_v_double_arrow; break;

    case io::MouseCursor_SouthResize:
        shape = XC_bottom_side; break;

    case io::MouseCursor_NorthWestResize: 
        shape = XC_top_left_corner; break;

    case io::MouseCursor_NorthEastSouthWestResize: 
    case io::MouseCursor_NorthWestSouthEastResize:
        shape = XC_sizing; break;

    case io::MouseCursor_SouthEastResize:
        shape = XC_bottom_right_corner; break;

    case io::MouseCursor_EastWestResize: 
        shape = XC_sb_h_double_arrow; break;

    case io::MouseCursor_EastResize:
        shape = XC_right_side; break;

    case io::MouseCursor_WestResize:
        shape = XC_left_side; break;

    case io::MouseCursor_RowResize:
        shape = XC_sb_v_double_arrow; break;
    case io::MouseCursor_ColumnResize:
        shape = XC_sb_h_double_arrow; break;

    case io::MouseCursor_Wait: 
        shape = XC_watch; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknow mouse cursor %d", cursor);
    }

    m_Display = XOpenDisplay(NULL);
    m_Cursor = XCreateFontCursor(m_Display, XC_xterm);
    XDefineCursor(m_Display, m_Window->getSystemHandle(), m_Cursor);
    XFlush(m_Display);
}
    
#elif defined(HE_MAC)
    
void Window::setCursor( const io::MouseCursor /*cursor*/ )
{
}

he::gfx::NativeWindowHandle Window::getNativeHandle() const
{
    return m_Window->getSystemHandle();
}

#endif
    
} } //end namespace
