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
//Created: 18/07/2012

#ifndef _HE_RenderTarget_H_
#define _HE_RenderTarget_H_
#pragma once

namespace he {
namespace gfx {
class Texture2D;

// RenderTarget is a class which encapsulates a fbo
// you can add texture targets and a depth target
// to enable a RenderTarget call prepareForRendering
// to draw to the backbuffer initialize an empty RenderTarget
class RenderTarget
{
public:
    RenderTarget(GLContext* context);
    virtual ~RenderTarget();

    // Add all targets then call init
    void addTextureTarget(const Texture2D* tex2D);
    void switchTextureTarget(uint index, const Texture2D* tex2D);
    void setDepthTarget(const Texture2D* tex2D);
    void setDepthTarget(uint id) { m_DepthBuffer = id; }
    void setDepthTarget();
    void init();

    void removeAllTargets();

    const Texture2D* getTextureTarget(uint index) const;
    const Texture2D* getDepthTarget() const;
    const uint getDepthBuffer() const { return m_DepthBuffer; }

    uint getWidth() const { return m_Width; }
    uint getHeight() const { return m_Height; }

    void setSize(uint width, uint height);

    void clear(const Color& color) const;

    void prepareForRendering() const;
    // used to disable targets while rendering
    // if we have 3 targets and pass down 2,0 in this function only 0 and 1 will be bound
    // if (2, 1) only 1 and 2 will be bound
    void prepareForRendering(uint numTextureTargets, uint offset = 0) const;

    const static int MAX_TEXTURE_TARGETS = 8;

private:
    uint m_FboId;
    uint m_DepthBuffer;
    const Texture2D* m_DepthTarget;

    std::vector<const Texture2D*> m_TextureTargets;

    uint m_DrawBuffers[MAX_TEXTURE_TARGETS];
    uint m_DrawBufferCount;

    uint m_Width, m_Height;
    GLContext* m_Context;

    //Disable default copy constructor and default assignment operator
    RenderTarget(const RenderTarget&);
    RenderTarget& operator=(const RenderTarget&);
};

} } //end namespace

#endif
