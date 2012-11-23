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
#include "MathConstants.h"
#include "MathFunctions.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESTRUCTOR */
Canvas2DRendererCairo::Canvas2DRendererCairo(Canvas2DBuffer* canvasBuffer) :    m_CanvasBuffer(canvasBuffer),
                                                                                m_RenderTexture(nullptr),
                                                                                m_Cairo(nullptr),
                                                                                m_CairoSurface(nullptr),
                                                                                m_HandleDrawCalls(true),
																				m_SurfaceDirty(false)
{
	int w = static_cast<int>(canvasBuffer->size.x);
	int h = static_cast<int>(canvasBuffer->size.y);

	m_RenderBuffer = static_cast<unsigned char*>(he_calloc(4 * w * h, sizeof(unsigned char)));

	m_CairoSurface =
		cairo_image_surface_create_for_data(m_RenderBuffer, CAIRO_FORMAT_ARGB32, w, h, 4 * w);

    m_Cairo = cairo_create(m_CairoSurface);
    cairo_set_line_join(m_Cairo, CAIRO_LINE_JOIN_ROUND);

    m_DrawThread = boost::thread(&Canvas2DRendererCairo::handleDrawCalls, this);

	ObjectHandle handle(ResourceFactory<Texture2D>::getInstance()->create());
	m_RenderTexture = ResourceFactory<Texture2D>::getInstance()->get(handle);
	m_RenderTexture->init(TextureWrapType_Clamp, TextureFilterType_Nearest, TextureFormat_RGBA8, false);

    m_Size = vec2(canvasBuffer->size.x, canvasBuffer->size.y);
}

Canvas2DRendererCairo::~Canvas2DRendererCairo()
{
    // stop thread
    m_HandleDrawCalls = false;
    m_DrawThread.join();

	he_free(m_RenderBuffer);
    cairo_destroy(m_Cairo);
    cairo_surface_destroy(m_CairoSurface);

    m_RenderTexture->release();
}

/* GENERAL */
void Canvas2DRendererCairo::blit()
{
    boost::mutex::scoped_lock lock(m_CairoLock);

	m_RenderTexture->setData(
		static_cast<uint32>(m_CanvasBuffer->size.x),
		static_cast<uint32>(m_CanvasBuffer->size.y),
		m_RenderBuffer,
		TextureBufferLayout_BGRA,
		TextureBufferType_Byte);

	m_SurfaceDirty = false;
}

/* SETTERS */
void Canvas2DRendererCairo::setLineWidth(float width)
{
	HE_ASSERT(width > 0, "Linewidth can't be smaller or equal to zero!");

	boost::mutex::scoped_lock lock(m_QueueLock);

	// queue drawcall
	m_DrawCalls.push(boost::bind(
		&cairo_set_line_width,
		m_Cairo,
		static_cast<double>(width)));
}
void Canvas2DRendererCairo::setColor(const Color& col)
{
	boost::mutex::scoped_lock lock(m_QueueLock);

	if (col.a() == 1.0f)
	{
		// queue drawcall
		m_DrawCalls.push(boost::bind(
			&cairo_set_source_rgb,
			m_Cairo,
			static_cast<double>(col.r()),
			static_cast<double>(col.g()),
			static_cast<double>(col.b())));
	}
	else
	{
		// queue drawcall
		m_DrawCalls.push(boost::bind(
			&cairo_set_source_rgba,
			m_Cairo,
			static_cast<double>(col.r()),
			static_cast<double>(col.g()),
			static_cast<double>(col.b()),
			static_cast<double>(col.a())));
	}
}

/* GETTERS */
void Canvas2DRendererCairo::clear()
{
	boost::mutex::scoped_lock lock(m_QueueLock);

    m_DrawCalls.push(boost::bind(
		&cairo_save,
		m_Cairo));

    m_DrawCalls.push(boost::bind(
		&cairo_set_operator,
		m_Cairo,
        CAIRO_OPERATOR_CLEAR));

	m_DrawCalls.push(boost::bind(
		&cairo_paint,
		m_Cairo));

    m_DrawCalls.push(boost::bind(
        &cairo_restore,
		m_Cairo));

	m_SurfaceDirty = true;
}

const Texture2D* Canvas2DRendererCairo::getRenderTexture(bool blitIfDirty)
{
	if (m_SurfaceDirty == true &&
		blitIfDirty == true)
	{
		blit();
	}

	return m_RenderTexture;
}

bool Canvas2DRendererCairo::isRendering()
{
    boost::mutex::scoped_lock lock(m_QueueLock);

    if (m_DrawCalls.size() > 0)
        return true;
    else
        return false;
}

/* DRAW */
void Canvas2DRendererCairo::moveTo(const vec2& pos)
{
    boost::mutex::scoped_lock lock(m_QueueLock);

    // queue drawcall
    m_DrawCalls.push(boost::bind(
        &cairo_move_to,
        m_Cairo,
        static_cast<double>(pos.x),
        static_cast<double>(pos.y)));
}
void Canvas2DRendererCairo::lineTo(const vec2& pos)
{
    boost::mutex::scoped_lock lock(m_QueueLock);

    // queue drawcall
    m_DrawCalls.push(boost::bind(
        &cairo_line_to,
        m_Cairo,
        static_cast<double>(pos.x),
        static_cast<double>(pos.y)));
}

void Canvas2DRendererCairo::rectangle(const vec2& pos, const vec2& size)
{
	HE_ASSERT(size.x > 0 && size.y > 0, "Size of rectangle can't be negative!");

    boost::mutex::scoped_lock lock(m_QueueLock);

    // queue drawcall
    m_DrawCalls.push(boost::bind(
        &cairo_rectangle,
        m_Cairo,
        static_cast<double>(pos.x),
        static_cast<double>(pos.y),
        static_cast<double>(size.x),
        static_cast<double>(size.y)));
}
void Canvas2DRendererCairo::circle(const vec2& pos, float radius)
{
    boost::mutex::scoped_lock lock(m_QueueLock);

    // queue drawcall
    m_DrawCalls.push(boost::bind(
		&cairo_arc,
        m_Cairo,
        static_cast<double>(pos.x),
        static_cast<double>(pos.y),
        static_cast<double>(radius),
        0.0,
        static_cast<double>(twoPi)));
}
void Canvas2DRendererCairo::arc(const vec2& pos, float radius, float angle1, float angle2)
{
    boost::mutex::scoped_lock lock(m_QueueLock);

    // queue drawcall
    m_DrawCalls.push(boost::bind(
		&cairo_arc,
        m_Cairo,
        static_cast<double>(pos.x),
        static_cast<double>(pos.y),
        static_cast<double>(radius),
        static_cast<double>(normalizeAngle(angle1)),
        static_cast<double>(normalizeAngle(angle2))));
}
void Canvas2DRendererCairo::curveTo(const vec2& start, const vec2& middle, const vec2& end)
{
    boost::mutex::scoped_lock lock(m_QueueLock);

    // queue drawcall
    m_DrawCalls.push(boost::bind(
		&cairo_curve_to,
        m_Cairo,
        static_cast<double>(start.x),
        static_cast<double>(start.y),
        static_cast<double>(middle.x),
        static_cast<double>(middle.y),
        static_cast<double>(end.x),
        static_cast<double>(end.y)));
}
void Canvas2DRendererCairo::newPath()
{
    boost::mutex::scoped_lock lock(m_QueueLock);

    // queue drawcall
    m_DrawCalls.push(boost::bind(
        &cairo_new_path,
        m_Cairo));
}
void Canvas2DRendererCairo::closePath()
{
    boost::mutex::scoped_lock lock(m_QueueLock);

    // queue drawcall
    m_DrawCalls.push(boost::bind(
        &cairo_close_path,
        m_Cairo));
}
    
void Canvas2DRendererCairo::stroke()
{
	boost::mutex::scoped_lock lock(m_QueueLock);

    // queue drawcall
    m_DrawCalls.push(boost::bind(
		&cairo_stroke_preserve,
        m_Cairo));

	m_SurfaceDirty = true;
}
void Canvas2DRendererCairo::fill()
{
	boost::mutex::scoped_lock lock(m_QueueLock);

    // queue drawcall
    m_DrawCalls.push(boost::bind(
		&cairo_fill_preserve,
        m_Cairo));

	m_SurfaceDirty = true;
}
void Canvas2DRendererCairo::clip()
{
}

/* INTERNAL */
float Canvas2DRendererCairo::normalizeAngle(float a)
{
    return (toRadians(a) * -1.0f);
}
void Canvas2DRendererCairo::handleDrawCalls()
{
    boost::posix_time::milliseconds waitTime(boost::posix_time::milliseconds(1));
    bool empty(true);

    while (m_HandleDrawCalls)
    {
        m_QueueLock.lock();
            empty = m_DrawCalls.empty();
        m_QueueLock.unlock();

        if (!empty)
        {
            m_CairoLock.lock();
                m_QueueLock.lock();
                    // exec drawcall
                    m_DrawCalls.front()();
                m_QueueLock.unlock();
            m_CairoLock.unlock();

            m_QueueLock.lock();
                m_DrawCalls.pop();
            m_QueueLock.unlock();
        }
        else
            boost::this_thread::sleep(waitTime);
    }
}

}} //end namespace