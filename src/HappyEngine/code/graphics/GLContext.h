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
    GLContext();
    
    bool create(Window* const window);
    void destroy();
    void makeCurrent();

    inline const GLEWContextStruct& getGlewContext() const { return m_GLEWContext; }
    inline uint32 getID() const { return m_ID; }
    inline void setID(const uint32 id) { m_ID = id; }
    inline Window* getWindow() const { return m_Window; }
    
private:
    GLEWContextStruct m_GLEWContext;
    uint32 m_ID;
    Window* m_Window;
    void* m_InternalContextPointer;
    
    // Clear
    Color m_ClearColor;

    // Misc
    RectI m_Viewport;
    int m_MaxMultiSamples;

    // Depth
    bool m_DepthRead, m_DepthWrite;
    DepthFunc m_DepthFunc;

    // Culling
    bool m_CullFrontFace;
    bool m_CullCWFrontFace;

    // Binding
    uint32 m_BoundFbo, m_BoundVao;
    static const int MAX_UBO = 50;
    uint32 m_BoundUbo[MAX_UBO];
    static const int MAX_SAMPLERS = 31;
    uint32 m_BoundTex2D[MAX_SAMPLERS];
    uint32 m_BoundTexCube[MAX_SAMPLERS];
    uint32 m_ActiveTex;

    // Blending
    bool m_BlendEnabled;
    BlendFunc m_BlendSrc, m_BlendDest;
    BlendFunc m_ColorBlendSrc, m_ColorBlendDest,
              m_AlphaBlendSrc, m_AlphaBlendDest;
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
    
    uint32 m_DefaultVAO;

    GLContext(const GLContext&);
    GLContext& operator=(const GLContext&);
};

} } //end namespace

#endif
