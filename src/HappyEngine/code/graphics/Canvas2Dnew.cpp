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

#include "HappyPCH.h" 

#include "Canvas2Dnew.h"
#include "Canvas2DBuffer.h"
#include "Renderer2D.h"
#include "View.h"
#include "Window.h"
#include "Texture2D.h"
#include "Canvas2DRendererCairo.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESTRUCTOR */
Canvas2Dnew::Canvas2Dnew(Renderer2D* parent,const RectF& relativeViewport) :
	m_Renderer2D(parent),
		m_BufferData(NEW Canvas2DBuffer()),
	m_CanvasDepth(0x7fff),
	m_RenderTexture(nullptr),
    m_RendererCairo(nullptr),
    m_StrokeColor(Color(1.0f,1.0f,1.0f)),
    m_FillColor(Color(1.0f,1.0f,1.0f)),
    m_LineWidth(1.0f)
{
	m_Position = vec2(
		(parent->getView()->getViewport().x * relativeViewport.x),
		(parent->getView()->getViewport().y * relativeViewport.y));

	m_Size = vec2(
		(parent->getView()->getViewport().width * relativeViewport.width),
		(parent->getView()->getViewport().height * relativeViewport.height));
}

Canvas2Dnew::Canvas2Dnew(Renderer2D* parent, const RectI& absoluteViewport) :
	m_Renderer2D(parent),
	m_BufferData(NEW Canvas2DBuffer()),
	m_CanvasDepth(0x7fff),
	m_RenderTexture(nullptr),
    m_RendererCairo(nullptr),
    m_StrokeColor(Color(1.0f,1.0f,1.0f)),
    m_FillColor(Color(1.0f,1.0f,1.0f)),
    m_LineWidth(1.0f)
{
	m_Position = vec2(
		static_cast<float>(absoluteViewport.x),
		static_cast<float>(absoluteViewport.y));

	m_Size = vec2(
		static_cast<float>(absoluteViewport.width),
		static_cast<float>(absoluteViewport.height));
}

Canvas2Dnew::~Canvas2Dnew()
{
	cleanup();
}

/* GENERAL */
void Canvas2Dnew::init()
{
	m_BufferData->init(m_Renderer2D->getView()->getWindow()->getContext(), m_Size);

	m_RenderTexture = 
        ResourceFactory<Texture2D>::getInstance()->get(
        m_BufferData->renderTextureHandle);

    m_RendererCairo = NEW Canvas2DRendererCairo(m_BufferData);
}

/* GETTERS */

Canvas2DBuffer* Canvas2Dnew::getCanvas2DBuffer() const
{
    return m_BufferData;
}

const vec2& Canvas2Dnew::getPosition() const
{
    return m_Position;
}

const vec2& Canvas2Dnew::getSize() const
{
    return m_Size;
}
uint16 Canvas2Dnew::getCanvasDepth() const
{
    return m_CanvasDepth;
}

/* SETTERS */
void Canvas2Dnew::setCanvasDepth(uint16 depth)
{
    m_CanvasDepth = depth;
}
void Canvas2Dnew::setDefaultCanvasDepth()
{
    m_CanvasDepth = 0x7fff;
}
void Canvas2Dnew::setPosition(const vec2& position)
{
    m_Position = position;
}
void Canvas2Dnew::setSize(const vec2& size)
{
    m_Size = size;
}
void Canvas2Dnew::setFillColor(const Color& fillColor)
{
    if (m_FillColor != fillColor)
    {
        m_FillColor = fillColor;
    }

    m_RendererCairo->setColor(m_FillColor);
}
void Canvas2Dnew::setStrokeColor(const Color& strokeColor)
{
    if (m_StrokeColor != strokeColor)
    {
        m_StrokeColor = strokeColor;
    }

    m_RendererCairo->setColor(m_StrokeColor);
}

void Canvas2Dnew::setLineWidth(float width)
{
    if (m_LineWidth != width)
    {
        m_LineWidth = width;
        m_RendererCairo->setLineWidth(width);
    }
}

/* DRAW */
void Canvas2Dnew::clear()
{
    m_RendererCairo->clear();
}
void Canvas2Dnew::draw()
{
    // wait if rendering is not finished
    while (m_RendererCairo->isRendering()) {}

    m_Renderer2D->drawTexture2DToScreen(
        m_RendererCairo->getRenderTexture(),
        m_Position,
        true);
}
void Canvas2Dnew::moveTo(const vec2& pos)
{
    m_RendererCairo->moveTo(pos);
}
void Canvas2Dnew::lineTo(const vec2& pos)
{
    m_RendererCairo->lineTo(pos);
}
void Canvas2Dnew::rectangle(const vec2& pos, const vec2& size)
{
    m_RendererCairo->rectangle(pos, size);
}
void Canvas2Dnew::stroke()
{
    m_RendererCairo->stroke();
}
void Canvas2Dnew::fill()
{
    m_RendererCairo->fill();
}

/* INTERNAL */
void Canvas2Dnew::cleanup()
{
    //m_RenderTexture->release();
    delete m_BufferData;
    delete m_RendererCairo;
}

}} //end namespace
