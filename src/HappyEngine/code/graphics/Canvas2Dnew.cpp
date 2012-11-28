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
namespace gui {

/* CONSTRUCTOR - DESTRUCTOR */
Canvas2Dnew::Canvas2Dnew(gfx::Renderer2D* parent,const RectF& relativeViewport) :
    m_Renderer2D(parent),
    m_BufferData(NEW gfx::Canvas2DBuffer()),
    m_CanvasDepth(0x7fff),
    m_RendererGL(nullptr),
    m_Color(Color(1.0f,1.0f,1.0f))
{
    m_Position = vec2(
        (parent->getView()->getViewport().x * relativeViewport.x),
        (parent->getView()->getViewport().y * relativeViewport.y));

    m_Size = vec2(
        (parent->getView()->getViewport().width * relativeViewport.width),
        (parent->getView()->getViewport().height * relativeViewport.height));
}

Canvas2Dnew::Canvas2Dnew(gfx::Renderer2D* parent, const RectI& absoluteViewport) :
    m_Renderer2D(parent),
    m_BufferData(NEW gfx::Canvas2DBuffer()),
    m_CanvasDepth(0x7fff),
    m_RendererGL(nullptr),
    m_Color(Color(1.0f,1.0f,1.0f))
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

    m_RendererGL = NEW gfx::Canvas2DRendererGL(m_BufferData, m_Renderer2D->getView()->getWindow()->getContext());
    m_RendererGL->init();
}

/* GETTERS */

gfx::Canvas2DBuffer* Canvas2Dnew::getCanvas2DBuffer() const
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

void Canvas2Dnew::setColor(const Color& color)
{
    m_Color = color;
}

/* DRAW */
void Canvas2Dnew::clear()
{
    m_BufferData->clear();
}
void Canvas2Dnew::draw()
{
    gfx::Texture2D* tex2D(ResourceFactory<gfx::Texture2D>::getInstance()->get(m_BufferData->renderTextureHandle));

    m_Renderer2D->drawTexture2DToScreen(
        tex2D,
        m_Position,
        true);
}

void Canvas2Dnew::fillText(const Text& text, const vec2& pos)
{
    m_RendererGL->setColor(m_Color);
    m_RendererGL->fillText(text, pos);
}

void Canvas2Dnew::drawImage(const gfx::Texture2D* tex2D, const vec2& pos,
                            const vec2& newDimensions,
                            const RectI& regionToDraw)
{
    m_RendererGL->drawImage(tex2D, pos, newDimensions, regionToDraw);
}

void Canvas2Dnew::drawSprite(const Sprite* sprite, const vec2& pos,
                             const vec2& size)
{
    m_RendererGL->drawSprite(sprite, pos, size);
}

/* INTERNAL */
void Canvas2Dnew::cleanup()
{
    delete m_BufferData;
    delete m_RendererGL;
}

}} //end namespace
