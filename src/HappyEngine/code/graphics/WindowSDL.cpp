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
#include "WindowSDL.h"

#include "ControlsManager.h"
#include "GLContextSDL.h"
#include "GraphicsEngine.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "SDL2/SDL.h"


namespace he {
namespace gfx {

WindowSDL::WindowSDL()
    : m_ID(0)
    , m_Context(nullptr)
    , m_Window(nullptr)
{
}

WindowSDL::~WindowSDL()
{
}

bool WindowSDL::create(const bool show)
{
    bool result(false);
    
    uint32 flags(SDL_WINDOW_OPENGL);
    if (checkFlag(eFlags_Resizeable))
        flags |= SDL_WINDOW_RESIZABLE;
    if (!show)
        flags |= SDL_WINDOW_HIDDEN;

    setFlag(Window::eFlags_IsVisible, show);
    
    he::gfx::GraphicsEngine* gfxEngine(GRAPHICS);
    if (gfxEngine->getSharedContext()) // First window == shared context
        gfxEngine->setActiveContext(gfxEngine->getSharedContext()); // always share with this one

    m_Window = SDL_CreateWindow(m_Titel.c_str(), 
        m_WindowRect.x == -1? SDL_WINDOWPOS_CENTERED : m_WindowRect.x, 
        m_WindowRect.y == -1? SDL_WINDOWPOS_CENTERED : m_WindowRect.y, 
        m_WindowRect.width, m_WindowRect.height, flags);
    m_Context = NEW GLContextSDL();
    if (m_Window && Window::create(show))
    {
        m_ID = SDL_GetWindowID(m_Window);
        SDL_GetWindowPosition(m_Window, &m_WindowRect.x, &m_WindowRect.y);
        CONTROLS->registerInputForWindow(getHandle(), NEW io::Keyboard(), NEW io::Mouse());
    }
    return result;
}
void WindowSDL::destroy()
{
    Window::destroy();
    delete m_Context;
    m_Context = nullptr;
    if (m_Window != nullptr)
    {
        CONTROLS->unregisterInputForWindow(getHandle());
        SDL_DestroyWindow(m_Window);
        m_Window = nullptr;
    }
}
bool WindowSDL::isOpen() const
{
    return m_Window != nullptr && checkFlag(eFlags_IsVisible);
}
    
void WindowSDL::show()
{
    if (checkFlag(eFlags_IsVisible) == false && m_Window)
    {
        SDL_ShowWindow(m_Window);
    }
    Window::show();
}
    
void WindowSDL::hide()
{
    if (checkFlag(eFlags_IsVisible) == true && m_Window)
    {
        SDL_HideWindow(m_Window);
    }
    Window::hide();
}

void WindowSDL::setWindowTitle( const he::String& caption )
{
    if (m_Window)
        SDL_SetWindowTitle(m_Window, caption.c_str());
    Window::setWindowTitle(caption);
}

void WindowSDL::setWindowPosition( int x, int y )
{
    if (m_Window)
        SDL_SetWindowPosition(m_Window, x, y);
    Window::setWindowPosition(x, y);
}

void WindowSDL::setWindowDimension( uint32 width, uint32 height )
{
    Window::setWindowDimension(width, height);
    if (m_Window)
        SDL_SetWindowSize(m_Window, m_WindowRect.width, m_WindowRect.height);
}

void WindowSDL::setVSync( bool enable )
{
    SDL_GL_SetSwapInterval(enable? 1 : 0);
    Window::setVSync(enable);
}

void WindowSDL::present()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    SDL_GL_SwapWindow(m_Window);
    Window::present();
}

void WindowSDL::setFullscreen( bool fullscreen )
{
    if (checkFlag(eFlags_Fullscreen) != fullscreen)
    {
        SDL_SetWindowFullscreen(m_Window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
    }
    Window::setFullscreen(fullscreen);
}

void WindowSDL::setMousePosition( const vec2& pos )
{
    if (m_Window)
        SDL_WarpMouseInWindow(m_Window, static_cast<int>(pos.x), static_cast<int>(pos.y));
}

GLContext* WindowSDL::getContext() const
{
    return m_Context;
}


} } //end namespace
