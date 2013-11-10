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
//Created: 2013/11/10
#include "HappyPCH.h"
#include "GLContext.h"

#include "GraphicsEngine.h"
#include "Window.h"

#include <SDL2/SDL.h>

namespace he {
namespace gfx {


GLContext::GLContext()
: m_ID(UINT32_MAX)
, m_Window(nullptr)
, m_InternalContextPointer(nullptr)

// Clear
, m_ClearColor(0.2452f, 0.432413f, 0.1312f, 0.8783f)

// Misc
, m_Viewport(-1, -1, -1, -1)
, m_MaxMultiSamples(-1)

//Depth
, m_DepthWrite(false)
, m_DepthFunc(DepthFunc_Unassigned)

//Culling
, m_CullFrontFace(false)
, m_CullCWFrontFace(false)

//Binding
, m_ActiveTex(0)
//Blending
, m_BlendEnabled(false)
, m_BlendSrc(BlendFunc_Unassigned)
, m_BlendDest(BlendFunc_Unassigned)
, m_ColorBlendSrc(BlendFunc_Unassigned)
, m_ColorBlendDest(BlendFunc_Unassigned)
, m_AlphaBlendSrc(BlendFunc_Unassigned)
, m_AlphaBlendDest(BlendFunc_Unassigned)
, m_BlendEquation(BlendEquation_Unassigned)
, m_BlendColor(0.0f, 0.0f, 0.0f, 0.0f)

//Scissor
, m_ScissorEnabled(false)
, m_ScissorRect(0, 0, 0, 0)

//Line Smoothing
, m_LineSmoothEnabled(false)

// Texture
, m_MaxAnisotropicFilteringSupport(0.0f)
, m_SupportTextureCompression(false)
    
, m_DefaultVAO(UINT32_MAX)
{
    he_memset(m_BoundTex2D, 0xff, MAX_SAMPLERS * sizeof(uint32));
    he_memset(m_BoundTexCube, 0xff, MAX_SAMPLERS * sizeof(uint32));
    he_memset(m_BoundUbo, 0xff, MAX_UBO * sizeof(uint32));
}

bool GLContext::create(Window* const window)
{
    m_InternalContextPointer = SDL_GL_CreateContext(window->m_Window);
    if (m_InternalContextPointer != nullptr)
    {
        m_Window = window;
        GRAPHICS->setActiveContext(this);
        GL::init();
        
        glGenVertexArrays(1, &m_DefaultVAO);
        GL::heBindVao(m_DefaultVAO);
        
        return true;
    }
    else
    {
        return false;
    }
}
    
void GLContext::destroy()
{
    if (m_InternalContextPointer != nullptr)
    {
        glDeleteVertexArrays(1, &m_DefaultVAO);
        SDL_GL_DeleteContext(m_InternalContextPointer);
        m_InternalContextPointer = nullptr;
    }
}
    
void GLContext::makeCurrent()
{
    SDL_GL_MakeCurrent(m_Window->m_Window, m_InternalContextPointer);
}

} } //end namespace
