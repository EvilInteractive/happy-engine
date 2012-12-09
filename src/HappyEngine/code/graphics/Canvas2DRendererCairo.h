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
#include "boost\thread.hpp"
#include "cairo\cairo.h"

/* FORWARD DECLARATIONS */
//struct _cairo_surface;
//struct _cairo;

namespace he {
namespace gui {
    class Sprite;

    enum LINE_JOIN
    {
        LINE_JOIN_MITER = CAIRO_LINE_JOIN_MITER,
        LINE_JOIN_ROUND = CAIRO_LINE_JOIN_ROUND,
        LINE_JOIN_BEVEL = CAIRO_LINE_JOIN_BEVEL
    };

    enum LINE_CAP
    {
        LINE_CAP_BUTT = CAIRO_LINE_CAP_BUTT,
        LINE_CAP_ROUND = CAIRO_LINE_CAP_ROUND,
        LINE_CAP_SQUARE = CAIRO_LINE_CAP_SQUARE
    };
}
namespace gfx {
    struct Canvas2DBuffer;
    class Texture2D;

class Canvas2DRendererCairo
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Canvas2DRendererCairo();
    virtual ~Canvas2DRendererCairo();

    /* GENERAL */
    void tick(float dTime);
    void glThreadInvoke();

    void addNewSprite(he::gui::Sprite* sprite);
    void finishSprite();

    /* SETTERS */
    void setLineWidth(float width);
    void setColor(const Color& col);
    void setLineCap(gui::LINE_CAP cap);
    void setLineJoin(gui::LINE_JOIN join);

    /* GETTERS */

    /* DRAW */
    void clear();
    void moveTo(const vec2& pos);
    void lineTo(const vec2& pos);

    void rectangle(const vec2& pos, const vec2& size);
    void arc(const vec2& pos, float radius, float angleRadStart, float angleRadEnd);
    void curveTo(const vec2& start, const vec2& middle, const vec2& end);
    void newPath();
    void closePath();

    void stroke();
    void fill();
    void clip();

private:

    /* INTERNAL */
    enum SpriteReadyState
    {
        SpriteReadyForRender =    0x01,
        SpriteReadyForBlit =      0x02
    };

    struct SpriteData
    {
        SpriteData( const uint16& id,
                    const vec2& size,
                    Texture2D* tex2D,
                    unsigned char* rBuff,
                    _cairo_surface* surf,
                    _cairo* cp,
                    bool keep) :
                        id(id),
                        size(size),
                        texture2D(tex2D),
                        renderBuffer(rBuff),
                        cairoSurface(surf),
                        cairoPaint(cp),
                        readyState(0x00),
                        keep(keep)
        {}

        uint16 id;
        vec2 size;
        std::queue<boost::function0<void> > drawCalls;
        Texture2D* texture2D;
        unsigned char* renderBuffer;
        _cairo_surface* cairoSurface;
        _cairo* cairoPaint;
        char readyState;
        bool keep;
    };

    void blit();
    float normalizeAngle(float a);
    void handleDrawCalls();
    void transformY();

    /* MEMBERS */
    std::queue<SpriteData> m_SpriteList;
    std::queue<SpriteData> m_SpriteListBlit;

    boost::thread m_DrawThread;
    boost::mutex m_SpriteListLock;
    boost::mutex m_SpriteListBlitLock;
    bool m_HandleDrawCalls;

    /* DEFAULT COPY & ASSIGNMENT */
    Canvas2DRendererCairo(const Canvas2DRendererCairo&);
    Canvas2DRendererCairo& operator=(const Canvas2DRendererCairo&);
};

}} //end namespace

#endif
