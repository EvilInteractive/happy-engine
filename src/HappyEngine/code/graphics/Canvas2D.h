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
//Author:  Sebastiaan Sprengers
//Created: 17/11/2012

// This class is the canvas on which all 2d drawing is done.
// Most rendering operations are done by other classes linked to Canvas2D.

#ifndef _HE_CANVAS2DNEW_H_
#define _HE_CANVAS2DNEW_H_
#pragma once

#include "Text.h"

namespace he {
namespace gfx {
    struct Canvas2DBuffer;
    class Renderer2D;
    class Texture2D;
    class Canvas2DRendererGL;
    class Mesh2D;
}
namespace gui {
    class Sprite;

class HAPPY_ENTRY Canvas2D
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    // create canvas and attach to parent 2d renderer
    // relative in percentages from 0.0f to 1.0f, absolute in pixels
    Canvas2D(gfx::Renderer2D* parent, const RectF& relativeViewport);
    Canvas2D(gfx::Renderer2D* parent, const RectI& absoluteViewport);
    virtual ~Canvas2D();

    /* GENERAL */
    void init();
    // store render buffer to texture
    // this disables drawing to the canvas
    // and uploads buffer to gpu
    void storeBuffer();

    /* GETTERS */
    // return render buffer data
    gfx::Canvas2DBuffer* getCanvas2DBuffer() const;

    // return size of canvas in pixels
    const vec2& getPosition() const;
    const vec2& getSize() const;

    /* SETTERS */
    // sets position in pixels on screen
    void setPosition(const vec2& position);
    void setSize(const vec2& size);

    void setColor(const Color& color);

    /* DRAW */
    // clear buffer to rgba(0,0,0,0)
    void clear();
    // draw to screen
    void draw();
    void draw3D(const gfx::ICamera* const camera);

    // draw dynamic text, for static use sprite
    void fillText(const Text& text, const vec2& pos);
    // draw image
    void drawImage(const gfx::Texture2D* tex2D, const vec2& pos,
                   const vec2& size = vec2(),
                   const RectI& regionToDraw = RectI());
    void drawSprite(const Sprite* sprite, const vec2& pos,
                    const vec2& size = vec2());
    // blit to screen
    void blitImage( const gfx::Texture2D* tex2D, const vec2& pos = vec2(),
                    bool useBlending = true,
                    const vec2& newDimensions = vec2(),
                    const RectI& regionToDraw = RectI());
    void strokeShape(gfx::Mesh2D* const shape);
    void fillShape(gfx::Mesh2D* const shape);
    void strokeRect(const RectI& rect);
    void fillRect(const RectI& rect);
    void drawLine(const vec2& p1, const vec2& p2);

private:

    /* INTERNAL */
    // deletion
    void cleanup();
    // handler for resize
    void viewResized();
    // resize
    void resize();

    /* MEMBERS */
    gfx::Renderer2D* m_Renderer2D;
    gfx::Canvas2DBuffer* m_BufferData;

    gfx::Canvas2DRendererGL* m_RendererGL;

    vec2 m_Position;
    vec2 m_Size;

    Color m_Color;

    /* DEFAULT COPY & ASSIGNMENT */
    Canvas2D(const Canvas2D&);
    Canvas2D& operator=(const Canvas2D&);
};

}} //end namespace

#endif
