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
#include "Canvas2DRendererGL.h"
#include "MathFunctions.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESTRUCTOR */
Canvas2Dnew::Canvas2Dnew(Renderer2D* parent,const RectF& relativeViewport) :
    m_Renderer2D(parent),
    m_BufferData(NEW Canvas2DBuffer()),
    m_CanvasDepth(0x7fff),
    m_RenderTexture(nullptr),
    m_RendererCairo(nullptr),
    m_RendererGL(nullptr),
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
    m_RendererGL(nullptr),
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

    ResourceFactory<Texture2D>::getInstance()->instantiate(m_BufferData->renderTextureHandle);
    m_RenderTexture = ResourceFactory<Texture2D>::getInstance()->get(m_BufferData->renderTextureHandle);

    m_RendererCairo = NEW Canvas2DRendererCairo(m_BufferData);
    m_RendererGL = NEW Canvas2DRendererGL(m_BufferData, m_Renderer2D->getView()->getWindow()->getContext());
    m_RendererGL->init();
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
}
void Canvas2Dnew::setStrokeColor(const Color& strokeColor)
{
    if (m_StrokeColor != strokeColor)
    {
        m_StrokeColor = strokeColor;
    }
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
    m_RendererGL->clear();
}
void Canvas2Dnew::draw()
{
    // wait if rendering is not finished
    while (m_RendererCairo->isRendering()) {}

    m_Renderer2D->drawTexture2DToScreen(
        m_RendererCairo->getRenderTexture(),
        m_Position,
        true,
        he::vec2(
        (float)m_RendererCairo->getRenderTexture()->getWidth(),
        -(float)m_RendererCairo->getRenderTexture()->getHeight()));

    m_Renderer2D->drawTexture2DToScreen(
        m_RendererGL->getRenderTexture(),
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
void Canvas2Dnew::roundedRectangle(const vec2& pos, const vec2& size, float radius)
{
    m_RendererCairo->newPath();
    m_RendererCairo->arc(he::vec2(pos.x + radius, pos.y + radius), radius, toRadians(180.0f), toRadians(90.0f));
    m_RendererCairo->arc(he::vec2(pos.x - radius + size.x, pos.y + radius), radius, toRadians(90.0f), toRadians(0.0f));
    m_RendererCairo->arc(he::vec2(pos.x - radius + size.x, pos.y - radius + size.y), radius, toRadians(0.0f), toRadians(-90.0f));
    m_RendererCairo->arc(he::vec2(pos.x + radius, pos.y - radius + size.y), radius, toRadians(-90.0f), toRadians(-180.0f));
    m_RendererCairo->closePath();
}
void Canvas2Dnew::circle(const vec2& pos, float radius)
{
    m_RendererCairo->circle(pos, radius);
}
void Canvas2Dnew::arc(const vec2& pos, float radius, float angleRadStart, float angleRadEnd)
{
    m_RendererCairo->arc(pos, radius, angleRadStart, angleRadEnd);
}
void Canvas2Dnew::curveTo(const vec2& start, const vec2& middle, const vec2& end)
{
    m_RendererCairo->curveTo(start, middle, end);
}
void Canvas2Dnew::newPath()
{
    m_RendererCairo->newPath();
}
void Canvas2Dnew::closePath()
{
    m_RendererCairo->closePath();
}

void Canvas2Dnew::stroke()
{
    m_RendererCairo->setColor(m_StrokeColor);
    m_RendererCairo->stroke();
}
void Canvas2Dnew::fill()
{
    m_RendererCairo->setColor(m_FillColor);
    m_RendererCairo->fill();
}

void Canvas2Dnew::fillText(const gui::Text& text, const vec2& pos)
{
    m_RendererGL->setFillColor(m_FillColor);
    m_RendererGL->fillText(text, pos);
}

void Canvas2Dnew::drawImage(const Texture2D* tex2D, const vec2& pos,
                            const vec2& newDimensions,
                            const RectI& regionToDraw)
{
    m_RendererGL->drawImage(tex2D, pos, newDimensions, regionToDraw);
}

/* INTERNAL */
void Canvas2Dnew::cleanup()
{
    m_RenderTexture->release();
    delete m_BufferData;
    delete m_RendererCairo;
    delete m_RendererGL;
}

}} //end namespace
