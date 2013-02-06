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
//Created: 25/11/2012

#include "HappyPCH.h" 

#include "SpriteCreator.h"

namespace he {
namespace gui {

SpriteCreator::SpriteCreator() : m_Renderer(NEW gfx::Canvas2DRendererCairo())
{
}

SpriteCreator::~SpriteCreator()
{
    delete m_Renderer;
    SpriteFactory::getInstance()->destroyAll();
}

/* GENERAL */
void SpriteCreator::tick(float dTime)
{
    m_Renderer->tick(dTime);
}
void SpriteCreator::glThreadInvoke()
{
    m_Renderer->glThreadInvoke();
}

Sprite* SpriteCreator::createSprite(const vec2& size, char flags)
{
    SpriteFactory* const factory(SpriteFactory::getInstance());
    ObjectHandle handle(factory->create());
    Sprite* sp(factory->get(handle));

    sp->init(size, flags);
    m_Renderer->addNewSprite(sp);

    return sp;
}
void SpriteCreator::removeSprite( Sprite* const sprite )
{
    SpriteFactory* const factory(SpriteFactory::getInstance());
    factory->destroyObject(sprite->getHandle());
}

void SpriteCreator::renderSpriteAsync()
{
    m_Renderer->finishSprite();
}

/* GETTERS */
    
/* SETTERS */
void SpriteCreator::setActiveSprite(Sprite* sprite)
{
    m_Renderer->addNewSprite(sprite);
}

void SpriteCreator::setColor(const Color& color)
{
    m_Renderer->setColor(color);
}
void SpriteCreator::setLineWidth(float width)
{
    m_Renderer->setLineWidth(width);
}

void SpriteCreator::setLineJoin(LINE_JOIN join)
{
    m_Renderer->setLineJoin(join);
}

void SpriteCreator::setLineCap(LINE_CAP cap)
{
    m_Renderer->setLineCap(cap);
}

/* DRAW */
void SpriteCreator::moveTo(const vec2& pos)
{
    m_Renderer->moveTo(pos);
}
void SpriteCreator::lineTo(const vec2& pos)
{
    m_Renderer->lineTo(pos);
}
void SpriteCreator::rectangle(const vec2& pos, const vec2& size)
{
    m_Renderer->rectangle(pos,size);
}
void SpriteCreator::roundedRectangle(const vec2& pos, const vec2& size, float radius)
{
    m_Renderer->newPath();
    m_Renderer->arc(he::vec2(pos.x + radius, pos.y + radius), radius, toRadians(180.0f), toRadians(90.0f));
    m_Renderer->arc(he::vec2(pos.x - radius + size.x, pos.y + radius), radius, toRadians(90.0f), toRadians(0.0f));
    m_Renderer->arc(he::vec2(pos.x - radius + size.x, pos.y - radius + size.y), radius, toRadians(0.0f), toRadians(-90.0f));
    m_Renderer->arc(he::vec2(pos.x + radius, pos.y - radius + size.y), radius, toRadians(-90.0f), toRadians(-180.0f));
    m_Renderer->closePath();
}
void SpriteCreator::circle(const vec2& pos, float radius)
{
    m_Renderer->arc(pos, radius, 0.0f, twoPi);
}
void SpriteCreator::arc(const vec2& pos, float radius, float angleRadStart, float angleRadEnd)
{
    m_Renderer->arc(pos,radius,angleRadStart,angleRadEnd);
}
void SpriteCreator::curveTo(const vec2& start, const vec2& middle, const vec2& end)
{
    m_Renderer->curveTo(start,middle,end);
}
void SpriteCreator::newPath()
{
    m_Renderer->newPath();
}
void SpriteCreator::closePath()
{
    m_Renderer->closePath();
}
void SpriteCreator::text(const gui::Text& text, const vec2& pos)
{
    m_Renderer->text(text, pos);
}
void SpriteCreator::stroke()
{
    m_Renderer->stroke();
}
void SpriteCreator::fill()
{
    m_Renderer->fill();
}

}} //end namespace
