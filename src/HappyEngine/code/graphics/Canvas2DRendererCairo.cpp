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
//Created: // 18/11/2012

#include "HappyPCH.h" 

#include "Canvas2DRendererCairo.h"
#include "Canvas2DBuffer.h"
#include "Texture2D.h"
#include "cairo\cairo.h"
#include "cairo\cairo-ft.h"
#include "Sprite.h"
#include "Text.h"
#include "Font.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESTRUCTOR */
Canvas2DRendererCairo::Canvas2DRendererCairo() : m_HandleDrawCalls(true)
{
    m_DrawThread = boost::thread(boost::bind(&Canvas2DRendererCairo::handleDrawCalls, this));
}

Canvas2DRendererCairo::~Canvas2DRendererCairo()
{
    // wait for thread to finish
    m_HandleDrawCalls = false;
    m_DrawThread.join();
}

/* GENERAL */
void Canvas2DRendererCairo::tick(float /*dTime*/)
{
}
void Canvas2DRendererCairo::glThreadInvoke()
{
    blit();
}

void Canvas2DRendererCairo::addNewSprite(he::gui::Sprite* sprite)
{
    finishSprite();

    uint16 id(sprite->getID());

    vec2 size(sprite->getSize());
    int w(static_cast<int>(size.x));
    int h(static_cast<int>(size.y));
    /*
    const bool isDynamic((sprite->getFlags() & gui::Sprite::DYNAMIC_DRAW) == gui::Sprite::DYNAMIC_DRAW);

    if (isDynamic)
    {
        size_t index(UINT32_MAX);

        if (m_DynamicSpriteList.find_if([&id](const SpriteData*spd) -> bool
        {
            if (spd->id == id)
                return true;
            else
                return false;
        }, index))
        {
            SpriteData& data(m_SpriteList.front());

            if (size == )
        }
    }*/

    unsigned char* rBuff(
        static_cast<unsigned char*>(he_calloc(
        4 * w * h,
        sizeof(unsigned char))));

    _cairo_surface* surf(
        cairo_image_surface_create_for_data(rBuff, CAIRO_FORMAT_ARGB32, w, h, 4 * w));

    _cairo* cp(cairo_create(surf));

    m_SpriteListLock.lock();

        m_SpriteList.push(SpriteData(
            id,
            size,
            sprite->getRenderTexture(),
            rBuff,
            surf,
            cp));

    m_SpriteListLock.unlock();

    clear();
    transformY();
}
void Canvas2DRendererCairo::finishSprite()
{
    boost::mutex::scoped_lock lock(m_SpriteListLock);

    if (m_SpriteList.empty() == false)
    {
        m_SpriteList.back().readyState |= SpriteReadyForRender;
    }
}

/* SETTERS */
void Canvas2DRendererCairo::setLineWidth(float width)
{
    HE_ASSERT(width > 0, "Linewidth can't be smaller or equal to zero!");

    boost::mutex::scoped_lock lock(m_SpriteListLock);

    SpriteData& sData = m_SpriteList.back();

    HE_ASSERT((sData.readyState &= SpriteReadyForRender) == false,
        "Sprite is already marked for rendering, can't draw!");

    sData.drawCalls.push(
        boost::bind(
        &cairo_set_line_width,
        sData.cairoPaint,
        static_cast<double>(width)));
}
void Canvas2DRendererCairo::setColor(const Color& col)
{
    boost::mutex::scoped_lock lock(m_SpriteListLock);

    SpriteData& sData = m_SpriteList.back();

    HE_ASSERT((sData.readyState &= SpriteReadyForRender) == false,
        "Sprite is already marked for rendering, can't draw!");

    if (col.a() == 1.0f)
    {
        sData.drawCalls.push(
            boost::bind(
            &cairo_set_source_rgb,
            sData.cairoPaint,
            static_cast<double>(col.r()),
            static_cast<double>(col.g()),
            static_cast<double>(col.b())));
    }
    else
    {
        sData.drawCalls.push(
            boost::bind(
            &cairo_set_source_rgba,
            sData.cairoPaint,
            static_cast<double>(col.r()),
            static_cast<double>(col.g()),
            static_cast<double>(col.b()),
            static_cast<double>(col.a())));
    }
}
void Canvas2DRendererCairo::setLineCap(gui::LINE_CAP cap)
{
    boost::mutex::scoped_lock lock(m_SpriteListLock);

    SpriteData& sData = m_SpriteList.back();

    HE_ASSERT((sData.readyState &= SpriteReadyForRender) == false,
        "Sprite is already marked for rendering, can't draw!");

    sData.drawCalls.push(
        boost::bind(
        &cairo_set_line_cap,
        sData.cairoPaint,
        static_cast<cairo_line_cap_t>(cap)));
}
void Canvas2DRendererCairo::setLineJoin(gui::LINE_JOIN join)
{
    boost::mutex::scoped_lock lock(m_SpriteListLock);

    SpriteData& sData = m_SpriteList.back();

    HE_ASSERT((sData.readyState &= SpriteReadyForRender) == false,
        "Sprite is already marked for rendering, can't draw!");

    sData.drawCalls.push(
        boost::bind(
        &cairo_set_line_join,
        sData.cairoPaint,
        static_cast<cairo_line_join_t>(join)));
}

/* DRAW */
void Canvas2DRendererCairo::clear()
{
    boost::mutex::scoped_lock lock(m_SpriteListLock);

    SpriteData& sData = m_SpriteList.back();

    HE_ASSERT((sData.readyState &= SpriteReadyForRender) == false,
        "Sprite is already marked for rendering, can't draw!");

    sData.drawCalls.push(
        boost::bind(
        &cairo_save,
        sData.cairoPaint));

    sData.drawCalls.push(
        boost::bind(
        &cairo_set_operator,
        sData.cairoPaint,
        CAIRO_OPERATOR_CLEAR));

    sData.drawCalls.push(
        boost::bind(
        &cairo_paint,
        sData.cairoPaint));

    sData.drawCalls.push(
        boost::bind(
        &cairo_restore,
        sData.cairoPaint));
}
void Canvas2DRendererCairo::moveTo(const vec2& pos)
{
    boost::mutex::scoped_lock lock(m_SpriteListLock);

    SpriteData& sData = m_SpriteList.back();

    HE_ASSERT((sData.readyState &= SpriteReadyForRender) == false,
        "Sprite is already marked for rendering, can't draw!");

    sData.drawCalls.push(
        boost::bind(
        &cairo_move_to,
        sData.cairoPaint,
        static_cast<double>(pos.x),
        static_cast<double>(pos.y)));
}
void Canvas2DRendererCairo::lineTo(const vec2& pos)
{
    boost::mutex::scoped_lock lock(m_SpriteListLock);

    SpriteData& sData = m_SpriteList.back();

    HE_ASSERT((sData.readyState &= SpriteReadyForRender) == false,
        "Sprite is already marked for rendering, can't draw!");

    sData.drawCalls.push(
        boost::bind(
        &cairo_line_to,
        sData.cairoPaint,
        static_cast<double>(pos.x),
        static_cast<double>(pos.y)));
}

void Canvas2DRendererCairo::rectangle(const vec2& pos, const vec2& size)
{
    HE_ASSERT(size.x > 0 && size.y > 0, "Size of rectangle can't be negative!");

    boost::mutex::scoped_lock lock(m_SpriteListLock);

    SpriteData& sData = m_SpriteList.back();

    HE_ASSERT((sData.readyState &= SpriteReadyForRender) == false,
        "Sprite is already marked for rendering, can't draw!");

    sData.drawCalls.push(
        boost::bind(
        &cairo_rectangle,
        sData.cairoPaint,
        static_cast<double>(pos.x),
        static_cast<double>(pos.y),
        static_cast<double>(size.x),
        static_cast<double>(size.y)));
}
void Canvas2DRendererCairo::arc(const vec2& pos, float radius, float angleRadStart, float angleRadEnd)
{
    boost::mutex::scoped_lock lock(m_SpriteListLock);

    SpriteData& sData = m_SpriteList.back();

    HE_ASSERT((sData.readyState &= SpriteReadyForRender) == false,
        "Sprite is already marked for rendering, can't draw!");

    sData.drawCalls.push(
        boost::bind(
        &cairo_arc,
        sData.cairoPaint,
        static_cast<double>(pos.x),
        static_cast<double>(pos.y),
        static_cast<double>(radius),
        static_cast<double>(normalizeAngle(angleRadStart)),
        static_cast<double>(normalizeAngle(angleRadEnd))));
}
void Canvas2DRendererCairo::curveTo(const vec2& controlP1, const vec2& controlP2, const vec2& end)
{
    boost::mutex::scoped_lock lock(m_SpriteListLock);

    SpriteData& sData = m_SpriteList.back();

    HE_ASSERT((sData.readyState &= SpriteReadyForRender) == false,
        "Sprite is already marked for rendering, can't draw!");

    sData.drawCalls.push(
        boost::bind(
        &cairo_curve_to,
        sData.cairoPaint,
        static_cast<double>(controlP1.x),
        static_cast<double>(controlP1.y),
        static_cast<double>(controlP2.x),
        static_cast<double>(controlP2.y),
        static_cast<double>(end.x),
        static_cast<double>(end.y)));
}
void Canvas2DRendererCairo::newPath()
{
    boost::mutex::scoped_lock lock(m_SpriteListLock);

    SpriteData& sData = m_SpriteList.back();

    HE_ASSERT((sData.readyState &= SpriteReadyForRender) == false,
        "Sprite is already marked for rendering, can't draw!");

    sData.drawCalls.push(
        boost::bind(
        &cairo_new_path,
        sData.cairoPaint));
}
void Canvas2DRendererCairo::closePath()
{
    boost::mutex::scoped_lock lock(m_SpriteListLock);

    SpriteData& sData = m_SpriteList.back();

    HE_ASSERT((sData.readyState &= SpriteReadyForRender) == false,
        "Sprite is already marked for rendering, can't draw!");

    sData.drawCalls.push(
        boost::bind(
        &cairo_close_path,
        sData.cairoPaint));
}

void Canvas2DRendererCairo::text(const gui::Text& text, const vec2& pos)
{
    boost::mutex::scoped_lock lock(m_SpriteListLock);

    SpriteData& sData = m_SpriteList.back();

    HE_ASSERT((sData.readyState &= SpriteReadyForRender) == false,
        "Sprite is already marked for rendering, can't draw!");

    sData.drawCalls.push(
        boost::bind(
        &Canvas2DRendererCairo::_text,
        this,
        text,
        pos,
        sData.cairoPaint));
}
    
void Canvas2DRendererCairo::stroke()
{
    boost::mutex::scoped_lock lock(m_SpriteListLock);

    SpriteData& sData = m_SpriteList.back();

    HE_ASSERT((sData.readyState &= SpriteReadyForRender) == false,
        "Sprite is already marked for rendering, can't draw!");

    sData.drawCalls.push(
        boost::bind(
        &cairo_stroke_preserve,
        sData.cairoPaint));
}
void Canvas2DRendererCairo::fill()
{
    boost::mutex::scoped_lock lock(m_SpriteListLock);

    SpriteData& sData = m_SpriteList.back();

    HE_ASSERT((sData.readyState &= SpriteReadyForRender) == false,
        "Sprite is already marked for rendering, can't draw!");

    sData.drawCalls.push(
        boost::bind(
        &cairo_fill_preserve,
        sData.cairoPaint));
}
void Canvas2DRendererCairo::clip()
{
}

/* INTERNAL */
void Canvas2DRendererCairo::blit()
{
    uint32 spriteToBlit(0);

    m_SpriteListBlitLock.lock();
        // check nr sprites to blit
        spriteToBlit = m_SpriteListBlit.size();

    m_SpriteListBlitLock.unlock();

    while (spriteToBlit > 0)
    {
        m_SpriteListBlitLock.lock();
            // get data from queue
            SpriteData data(m_SpriteListBlit.front());
            m_SpriteListBlit.pop();
        // make sure to stop blocking render thread
        m_SpriteListBlitLock.unlock();

        // blit
        data.texture2D->setData(
                static_cast<uint32>(data.size.x),
                static_cast<uint32>(data.size.y),
                data.renderBuffer,
                TextureBufferLayout_BGRA,
                TextureBufferType_Byte);

        // cleanup
        he_free(data.renderBuffer);
        cairo_destroy(data.cairoPaint);
        cairo_surface_destroy(data.cairoSurface);

        --spriteToBlit;
    }
}
float Canvas2DRendererCairo::normalizeAngle(float a)
{
    return (a * -1.0f);
}
void Canvas2DRendererCairo::handleDrawCalls()
{
    boost::posix_time::milliseconds waitTime = boost::posix_time::milliseconds(10);

    bool renderSprite(false);

    while (m_HandleDrawCalls)
    {
        uint32 spritesToRender(m_SpriteList.size());

        if (spritesToRender > 0)
        {
            while (spritesToRender > 0)
            {
                m_SpriteListLock.lock();

                    // check if needs to be rendered
                    SpriteData& data(m_SpriteList.front());

                m_SpriteListLock.unlock();
            
                renderSprite = 
                    (data.readyState & SpriteReadyForRender) == SpriteReadyForRender;

                if (renderSprite)
                {
                    // execute all the drawcalls for the sprite
                    while (data.drawCalls.empty() == false)
                    {
                        data.drawCalls.front()();
                        data.drawCalls.pop();
                    }

                    // set sprite ready for blitting
                    data.readyState |= SpriteReadyForBlit;

                    m_SpriteListBlitLock.lock();

                        // push spritedata to blitting queue
                        m_SpriteListBlit.push(data);

                    m_SpriteListBlitLock.unlock();
            
                    m_SpriteListLock.lock();
            
                        // pop it off the regular queue
                        m_SpriteList.pop();

                    m_SpriteListLock.unlock();
                }

                renderSprite = false;
                --spritesToRender;
            }
        }
        else
        {
            boost::this_thread::sleep(waitTime);
        }
    }
}

void Canvas2DRendererCairo::transformY()
{
    boost::mutex::scoped_lock lock(m_SpriteListLock);

    SpriteData& sData = m_SpriteList.back();

    HE_ASSERT((sData.readyState &= SpriteReadyForRender) == false,
        "Sprite is already marked for rendering, can't draw!");

    sData.drawCalls.push(
        boost::bind(
        &cairo_scale,
        sData.cairoPaint,
        1.0,
        -1.0));

    sData.drawCalls.push(
        boost::bind(
        &cairo_translate,
        sData.cairoPaint,
        0.0,
        static_cast<double>(-sData.size.y)));
}

void Canvas2DRendererCairo::_text(const gui::Text& text, const vec2& pos, cairo_t* cairoPaint)
{
    vec2 linePos(0,0);
    const bool hasBounds(text.hasBounds());
    const gui::Font* font(text.getFont());
    cairo_font_face_t* cairoFont(font->getCairoFontFace());
    const uint32 lineSpacing(font->getLineSpacing());
    const gui::Text::HAlignment hAl(text.getHorizontalAlignment());
    const gui::Text::VAlignment vAl(text.getVerticalAlignment());
    const char* fullText(text.getText());
    const size_t size(text.getTextSize());
    const vec2 bounds(hasBounds ? text.getBounds() : vec2(0,0));
    uint16 lines(0);

    cairo_glyph_t* cairoGlyphs((cairo_glyph_t*)he_malloc(size * sizeof(cairo_glyph_t)));
    size_t numGlyphs(0);

    size_t lineCharStart(0);

    for (uint32 i(0); i < size; ++i)
    {
        const char currentChar(fullText[i]);

        if (currentChar == '\n')
        {
            const size_t lineSize(i - lineCharStart);
            if (lineSize > 0)
            {
                vec2 glyphPos;
                float hoffset(0.0f);
                switch (hAl)
                {
                    case gui::Text::HAlignment_Left:
                        break;
                    case gui::Text::HAlignment_Center:
                        hoffset = -font->getStringWidth(fullText + lineCharStart, lineSize) / 2.0f;
                        break;
                    case gui::Text::HAlignment_Right:
                        hoffset = -font->getStringWidth(fullText + lineCharStart, lineSize);
                        break;
                }

                glyphPos.x += hoffset;

                for (uint32 i2(lineCharStart); i2 < lineCharStart + lineSize; ++i2)
                {
                    const gui::Font::CharData& charData = font->getCharTextureData(currentChar);

                    cairo_glyph_t& c(cairoGlyphs[i2]);

                    c.index = static_cast<int>(fullText[i2]);
                    c.x = static_cast<double>(glyphPos.x);
                    c.y = static_cast<double>(glyphPos.y);

                    glyphPos.x += charData.advance.x;

                    ++numGlyphs;
                }
            }

            lineCharStart = i + 1;

            linePos.y += lineSpacing;
            linePos.x = 0;

            ++lines;
        } 
    }

    if (lineCharStart < size)
    {
        const size_t lineSize(size - lineCharStart);
        if (lineSize > 0)
        {
            vec2 glyphPos;
            float hoffset(0.0f);
            switch (hAl)
            {
                case gui::Text::HAlignment_Left:
                    break;
                case gui::Text::HAlignment_Center:
                    hoffset = -font->getStringWidth(fullText + lineCharStart, lineSize) / 2.0f;
                    break;
                case gui::Text::HAlignment_Right:
                    hoffset = -font->getStringWidth(fullText + lineCharStart, lineSize);
                    break;
            }

            glyphPos.x += hoffset;

            for (uint32 i2(lineCharStart); i2 < lineCharStart + lineSize; ++i2)
            {
                const gui::Font::CharData& charData = font->getCharTextureData(fullText[i2]);

                cairo_glyph_t& c(cairoGlyphs[i2]);

                c.index = static_cast<int>(fullText[i2]);
                c.x = static_cast<double>(glyphPos.x);
                c.y = static_cast<double>(glyphPos.y);

                glyphPos.x += charData.advance.x;

                ++numGlyphs;
            }

            ++lines;
        }
    }

    vec2 offset(pos.x, pos.y);
    switch (vAl)
    {
        case gui::Text::VAlignment_Top:
            break;
        case gui::Text::VAlignment_Center:
            offset.y -= bounds.y / 2.0f - (lineSpacing * lines) / 2.0f;
            break;
        case gui::Text::VAlignment_Bottom:
            offset.y -= bounds.y + (lineSpacing * lines);
            break;
    }

    cairo_translate(cairoPaint, static_cast<double>(offset.x), static_cast<double>(offset.y));

    cairo_set_font_face(cairoPaint, cairoFont);
    cairo_set_font_size(cairoPaint, static_cast<double>(font->getPixelHeight()));

    // render glyphs
    //cairo_glyph_path(cairoPaint, cairoGlyphs, numGlyphs);

    cairo_translate(cairoPaint, static_cast<double>(-offset.x), static_cast<double>(-offset.y));

    he_free(cairoGlyphs);

    //cairo_glyph_free(cairoGlyphs);
}

}} //end namespace