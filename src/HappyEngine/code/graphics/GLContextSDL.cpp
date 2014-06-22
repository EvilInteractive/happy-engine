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
//Created: 2013/11/10
#include "HappyPCH.h"
#include "GLContextSDL.h"

#include "GraphicsEngine.h"
#include "Window.h"
#include "WindowSDL.h"

#include <SDL2/SDL.h>

namespace he {
namespace gfx {


GLContextSDL::GLContextSDL()
    : m_InternalContextPointer(nullptr)
{
}

bool GLContextSDL::create(Window* const window)
{
    HE_ASSERT(window && window->getType() == HEFS::strSDLWindow, "No window or wrong window type when creating context!");
    WindowSDL* sdlWindow(checked_cast<WindowSDL*>(window));
    m_Window = sdlWindow;
    m_InternalContextPointer = SDL_GL_CreateContext(sdlWindow->m_Window);
    if (m_InternalContextPointer != nullptr)
    {
        GLContext::create(window);
        return true;
    }
    else
    {
        return false;
    }
}
    
void GLContextSDL::destroy()
{
    if (m_InternalContextPointer != nullptr)
    {
        GLContext::destroy();
        SDL_GL_DeleteContext(m_InternalContextPointer);
        m_InternalContextPointer = nullptr;
    }
}
    
void GLContextSDL::makeCurrent()
{
    SDL_GL_MakeCurrent(m_Window->m_Window, m_InternalContextPointer);
}

} } //end namespace
