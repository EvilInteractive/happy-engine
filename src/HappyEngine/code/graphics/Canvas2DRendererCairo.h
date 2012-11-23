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
//Created: 18/11/2012

#ifndef _HE_CANVAS2DRENDERERCAIRO_H_
#define _HE_CANVAS2DRENDERERCAIRO_H_
#pragma once

#include <queue>
#include "boost\function.hpp"
#include "boost\thread.hpp"
#include "List.h"

/* FORWARD DECLARATIONS */
struct _cairo_surface;
struct _cairo;

namespace he {
namespace gfx {
    struct Canvas2DBuffer;
    class Texture2D;

class Canvas2DRendererCairo
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Canvas2DRendererCairo(Canvas2DBuffer* canvasBuffer);
    virtual ~Canvas2DRendererCairo();

    /* GENERAL */
    void blit();

    /* SETTERS */
    void setLineWidth(float width);
    void setColor(const Color& col);
	void setDirty(bool dirty = true);

    /* GETTERS */
	const Texture2D* getRenderTexture(bool blitIfDirty = true);
	bool isSurfaceDirty() const;
    bool isRendering();

    /* DRAW */
	void clear();

    void moveTo(const vec2& pos);
    void lineTo(const vec2& pos);

    void rectangle(const vec2& pos, const vec2& size);
    void circle(const vec2& pos, float radius);
    void arc(const vec2& pos, float radius, float angle1, float angle2);
    void curveTo(const vec2& start, const vec2& middle, const vec2& end);
    void newPath();
    void closePath();

    void stroke();
    void fill();
    void clip();

private:

    /* INTERNAL */
    float normalizeAngle(float a);
    void handleDrawCalls();

    /* MEMBERS */
    Canvas2DBuffer* m_CanvasBuffer;

    _cairo_surface* m_CairoSurface;
    _cairo* m_Cairo;

    //List<unsigned char*> m_RenderBuffers;
	unsigned char* m_RenderBuffer;
    Texture2D* m_RenderTexture;

    std::queue<boost::function0<void> > m_DrawCalls;
    boost::thread m_DrawThread;
    boost::mutex m_CairoLock;
    boost::mutex m_QueueLock;
    bool m_HandleDrawCalls;

	bool m_SurfaceDirty;

    vec2 m_Size;

    /* DEFAULT COPY & ASSIGNMENT */
    Canvas2DRendererCairo(const Canvas2DRendererCairo&);
    Canvas2DRendererCairo& operator=(const Canvas2DRendererCairo&);
};

}} //end namespace

#endif
