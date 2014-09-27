//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

#include "Canvas2D.h"
#include "Canvas2DBuffer.h"
#include "Renderer2D.h"
#include "View.h"
#include "Window.h"
#include "Texture2D.h"
#include "Canvas2DRendererCairo.h"
#include "Canvas2DRendererGL.h"
#include "MathFunctions.h"
#include "Mesh2D.h"
#include "ContentManager.h"

namespace he {
namespace gui {

/* CONSTRUCTOR - DESTRUCTOR */
Canvas2D::Canvas2D(gfx::Renderer2D* parent,const RectF& relativeViewport) :
    m_Renderer2D(parent),
    m_BufferData(NEW gfx::Canvas2DBuffer()),
    m_RendererGL(nullptr),
    m_Color(Color(1.0f,1.0f,1.0f))
{
    m_Position = vec2(
        (parent->getView()->getViewport().x * relativeViewport.x),
        (parent->getView()->getViewport().y * relativeViewport.y));

    m_Size = vec2(
        (parent->getView()->getViewport().width * relativeViewport.width),
        (parent->getView()->getViewport().height * relativeViewport.height));

    he::eventCallback0<void> viewResizedHandler(std::bind(&Canvas2D::viewResized, this));
    parent->getView()->ViewportSizeChanged += viewResizedHandler;
}

Canvas2D::Canvas2D(gfx::Renderer2D* parent, const RectI& absoluteViewport) :
    m_Renderer2D(parent),
    m_BufferData(NEW gfx::Canvas2DBuffer()),
    m_RendererGL(nullptr),
    m_Color(Color(1.0f,1.0f,1.0f))
{
    m_Position = vec2(
        static_cast<float>(absoluteViewport.x),
        static_cast<float>(absoluteViewport.y));

    m_Size = vec2(
        static_cast<float>(absoluteViewport.width),
        static_cast<float>(absoluteViewport.height));

    he::eventCallback0<void> viewResizedHandler(std::bind(&Canvas2D::viewResized, this));
    parent->getView()->ViewportSizeChanged += viewResizedHandler;
}

Canvas2D::~Canvas2D()
{
    cleanup();
}

/* GENERAL */
void Canvas2D::init()
{
    m_BufferData->init(m_Renderer2D->getView()->getWindow()->getContext(), m_Size);

    m_RendererGL = NEW gfx::Canvas2DRendererGL(m_BufferData, m_Renderer2D->getView()->getWindow()->getContext());
    m_RendererGL->init();
}

/* GETTERS */
gfx::Canvas2DBuffer* Canvas2D::getCanvas2DBuffer() const
{
    return m_BufferData;
}

const vec2& Canvas2D::getPosition() const
{
    return m_Position;
}

const vec2& Canvas2D::getSize() const
{
    return m_Size;
}

/* SETTERS */
void Canvas2D::setPosition(const vec2& position)
{
    m_Position = position;
}

void Canvas2D::setSize(const vec2& size)
{
    m_Size = size;
    resize();
}

void Canvas2D::setColor(const Color& color)
{
    m_Color = color;
}

/* DRAW */
void Canvas2D::clear()
{
    m_BufferData->clear();
}

void Canvas2D::draw()
{
    gfx::Texture2D* const tex2D(ResourceFactory<gfx::Texture2D>::getInstance()->get(m_BufferData->m_RenderTextureHandle));
    m_RendererGL->blitImage(tex2D, m_Position, true);
}

void Canvas2D::draw3D( const gfx::ICamera* const camera )
{
    gfx::Texture2D* const tex2D(ResourceFactory<gfx::Texture2D>::getInstance()->get(m_BufferData->m_RenderTextureHandle));
    m_RendererGL->blitImage3D(tex2D, camera);
}

void Canvas2D::fillText(const Text& text, const vec2& pos)
{
    m_RendererGL->setColor(m_Color);
    m_RendererGL->fillText(text, pos);
}

void Canvas2D::drawImage(const gfx::Texture2D* tex2D, const vec2& pos,
                            const vec2& newDimensions,
                            const RectI& regionToDraw)
{
    m_RendererGL->drawImage(tex2D, pos, newDimensions, regionToDraw);
}

void Canvas2D::drawSprite(const Sprite* sprite, const vec2& pos,
                             const vec2& size)
{
    m_RendererGL->setColor(m_Color);
    m_RendererGL->drawSprite(sprite, pos, size);
}

void Canvas2D::blitImage(   const gfx::Texture2D* tex2D,
                            const vec2& pos,
                            bool useBlending,
                            const vec2& newDimensions,
                            const RectI& regionToDraw)
{
    m_RendererGL->blitImage(tex2D, pos, useBlending, newDimensions, regionToDraw);
}

void Canvas2D::strokeShape(gfx::Mesh2D* const shape)
{
    m_RendererGL->setColor(m_Color);
    m_RendererGL->strokeShape(shape);
}

void Canvas2D::fillShape(gfx::Mesh2D* const shape)
{
    m_RendererGL->setColor(m_Color);
    m_RendererGL->fillShape(shape);
}

void Canvas2D::strokeRect(const RectI& rect)
{
    m_RendererGL->setColor(m_Color);
    m_RendererGL->strokeRect(rect);
}

void Canvas2D::fillRect(const RectI& rect)
{
    m_RendererGL->setColor(m_Color);
    m_RendererGL->fillRect(rect);
}

void Canvas2D::drawLine(const vec2& p1, const vec2& p2)
{
    m_RendererGL->setColor(m_Color);
    m_RendererGL->drawLine(p1, p2);
}

void Canvas2D::fillCurve( const vec2& p1, const vec2& tan1, const vec2& tan2, const vec2& p2, const float radius )
{
    m_RendererGL->setColor(m_Color);
    m_RendererGL->fillCurve(p1, tan1, tan2, p2, radius);
}

/* INTERNAL */
void Canvas2D::cleanup()
{
    delete m_BufferData;
    delete m_RendererGL;
}

void Canvas2D::viewResized()
{
    m_Size.x = static_cast<float>(m_Renderer2D->getView()->getViewport().width);
    m_Size.y = static_cast<float>(m_Renderer2D->getView()->getViewport().height);

    resize();
}

void Canvas2D::resize()
{
    m_BufferData->resize(m_Size);
    m_RendererGL->resize();
}

const mat44& Canvas2D::getMatrix() const
{
    return m_RendererGL->getMatrix();
}

}} //end namespace
