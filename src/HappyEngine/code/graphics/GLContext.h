//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
//Created: 16/08/2012

#ifndef _HE_GLCONTEXT_H_
#define _HE_GLCONTEXT_H_
#pragma once

#include "GL/glew.h"
#include "OpenGL.h"
#include "Rect.h"
#include "Color.h"

namespace he {
namespace gfx {
class Window;
class GL;
    
class GLContext
{
friend GL;
public:
    GLContext(Window* window_)
        : id(UINT_MAX)
        , window(window_) 

        // Clear
        , m_ClearColor(0.2452f, 0.432413f, 0.1312f, 0.8783f)

            // Misc
        , m_Viewport(-1, -1, -1, -1)

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
        , m_BlendDest(BlendFunc_Unassigned)
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
    {
        he_memset(m_BoundTex2D, 0xff, MAX_SAMPLERS * sizeof(uint));
        he_memset(m_BoundUbo, 0xff, MAX_UBO * sizeof(uint));
    }

    GLEWContextStruct internalContext;
    unsigned int id;
    Window* window;
private:
    // Clear
    Color m_ClearColor;

    // Misc
    RectI m_Viewport;

    // Depth
    bool m_DepthRead, m_DepthWrite;
    DepthFunc m_DepthFunc;

    // Culling
    bool m_CullFrontFace;
    bool m_CullCWFrontFace;

    // Binding
    uint m_BoundFbo, m_BoundVao;
    static const int MAX_UBO = 50;
    uint m_BoundUbo[MAX_UBO];
    static const int MAX_SAMPLERS = 31;
    uint m_BoundTex2D[MAX_SAMPLERS];
    uint m_ActiveTex;

    // Blending
    bool m_BlendEnabled;
    BlendFunc m_BlendSrc, m_BlendDest;
    BlendEquation m_BlendEquation;
    Color m_BlendColor;

    // Scissor
    bool m_ScissorEnabled;
    RectI m_ScissorRect;

    // Line smoothing
    bool m_LineSmoothEnabled;

    // Texture
    float m_MaxAnisotropicFilteringSupport;
    bool  m_SupportTextureCompression;

    GLContext(const GLContext&);
    GLContext& operator=(const GLContext&);
};

} } //end namespace

#endif
