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

/* FORWARD DECLARATIONS */
struct _cairo_surface;
struct _cairo;

namespace he {
namespace gui {
    class Sprite;
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

    /* GETTERS */

    /* DRAW */
    void clear();
    void moveTo(const vec2& pos);
    void lineTo(const vec2& pos);

    void rectangle(const vec2& pos, const vec2& size);
    void circle(const vec2& pos, float radius);
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
                    _cairo* cp) :
                        id(id),
                        size(size),
                        texture2D(tex2D),
                        renderBuffer(rBuff),
                        cairoSurface(surf),
                        cairoPaint(cp),
                        readyState(0x00)
        {}

        uint16 id;
        vec2 size;
        std::queue<boost::function0<void> > drawCalls;
        Texture2D* texture2D;
        unsigned char* renderBuffer;
        _cairo_surface* cairoSurface;
        _cairo* cairoPaint;
        char readyState;
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
