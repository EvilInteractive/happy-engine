//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
//Created: 22/10/2011
#include "StdAfx.h" 

#include "OpenGL.h"
#include "HappyNew.h"

namespace he {

//Clear
Color GL::m_ClearColor = Color(0.0f, 0.0f, 0.0f, 0.0f);

//Depth
bool GL::m_DepthRead = false, GL::m_DepthWrite = false;
DepthFunc GL::m_DepthFunc = DepthFunc_Less;

//Culling
bool GL::m_CullFrontFace = false;
bool GL::m_CullCWFrontFace = false;

//Binding
uint GL::m_BoundFbo = 0, GL::m_BoundVao = 0, GL::m_ActiveTex = 0;
uint GL::m_BoundTex2D[MAX_SAMPLERS] = {UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX } ;
//Blending
bool GL::m_BlendEnabled = false;
BlendFunc GL::m_BlendSrc = BlendFunc_One, GL::m_BlendDest = BlendFunc_Zero;
BlendEquation GL::m_BlendEquation = BlendEquation_Add;
Color GL::m_BlendColor = Color(0.0f, 0.0f, 0.0f, 0.0f);

//Scissor
bool GL::m_ScissorEnabled = false;
RectI GL::m_ScissorRect = RectI(0, 0, 0, 0);

//Clear
void GL::heClearColor(const Color& color)
{
   if (color != m_ClearColor)
   {
       glClearColor(color.r(), color.g(), color.b(), color.a());
       m_ClearColor = color;
   }
}

//Depth
void GL::heSetDepthRead(bool read)
{
    if (m_DepthRead != read)
    {
        m_DepthRead = read;
        if (read)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
    }
}
void GL::heSetDepthWrite(bool write)
{
    if (m_DepthWrite != write)
    {
        m_DepthWrite = write;
        glDepthMask(write);
    }
}
void GL::heSetDepthFunc(DepthFunc func)
{
    if (m_DepthFunc != func)
    {
        m_DepthFunc = func;
        glDepthFunc(func);
    }
}

//Culling
void GL::heSetCullFace(bool cullFrontFace)
{
    if (m_CullFrontFace != cullFrontFace)
    {
        m_CullFrontFace = cullFrontFace;
        glCullFace(cullFrontFace? GL_FRONT : GL_BACK);
    }
}
void GL::heSetWindingFrontFace(bool cw)
{
    if (m_CullCWFrontFace != cw)
    {
        m_CullCWFrontFace = cw;
        glFrontFace(cw? GL_CW : GL_CCW);
    }
}

//Binding
void GL::heBindFbo(uint fbo)
{
    if (m_BoundFbo != fbo)
    {
        m_BoundFbo = fbo;
        m_BoundVao = 0;
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }
}
void GL::heBindVao(uint vao)
{
   // if (m_BoundVao != vao)
    //{
        m_BoundVao = vao;
        glBindVertexArray(vao);
    //}
}
void GL::heBindTexture2D(uint samplerPos, uint tex)
{
    ASSERT(samplerPos < MAX_SAMPLERS, "samplerPos must be < MAX_SAMPLERS!");
    //if (m_BoundTex2D[samplerPos] != tex)
    //{
        //if (m_ActiveTex != samplerPos)
        //{
            m_ActiveTex = samplerPos;
            glActiveTexture(GL_TEXTURE0 + samplerPos);
        //}
        glBindTexture(GL_TEXTURE_2D, tex);
        m_BoundTex2D[samplerPos] = tex;
    //}
}

//Blending
void GL::heBlendEnabled(bool enabled)
{
    if (m_BlendEnabled != enabled)
    {
        m_BlendEnabled = enabled;
        if (enabled)
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);
    }
}
void GL::heBlendFunc(BlendFunc srcFactor, BlendFunc destFactor)
{
    if (srcFactor != m_BlendSrc || destFactor != m_BlendDest)
    {
        m_BlendSrc = srcFactor;
        m_BlendDest = destFactor;
        glBlendFunc(srcFactor, destFactor);
    }
}
void GL::heBlendColor(const Color& color)
{
    if (color != m_BlendColor)
    {
        m_BlendColor = color;
        glBlendColor(color.r(), color.g(), color.b(), color.a());
    }
}

//Scissor
void GL::heScissorEnabled(bool enabled)
{
    if (enabled != m_ScissorEnabled)
    {
        m_ScissorEnabled = enabled;
        if (enabled)
            glEnable(GL_SCISSOR_TEST);
        else
            glDisable(GL_SCISSOR_TEST);
    }
}
void GL::heScissorRect(const RectI& rect)
{
    if (rect != m_ScissorRect)
    {
        m_ScissorRect = rect;
        glScissor(rect.x, rect.y, rect.width, rect.height);
    }
}


} //end namespace