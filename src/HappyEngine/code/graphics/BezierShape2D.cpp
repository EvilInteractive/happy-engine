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
//Author:  Bastian Damman
//Created: 13/12/2012
#include "HappyPCH.h" 

#include "BezierShape2D.h"

#include "Gui.h"
#include "SpriteCreator.h"
#include "Renderer2D.h"
#include "Canvas2D.h"
#include "Canvas2Dnew.h"

namespace he {
namespace gui {

BezierShape2D::BezierShape2D():
    m_Sprite(nullptr), m_NeedsUpdate(false),
    m_PositionBegin(0, 0), m_PositionEnd(0, 0)
{
    gui::SpriteCreator* const cr(GUI->Sprites);
    m_Sprite = cr->createSprite(vec2(64, 64), gui::Sprite::DYNAMIC_DRAW | gui::Sprite::UNIFORM_SCALE);
}


BezierShape2D::~BezierShape2D()
{
    m_Sprite->release();
}

void BezierShape2D::setPositionStart( const vec2& position )
{
    if (m_PositionBegin != position)
    {
        m_PositionBegin = position;
        m_NeedsUpdate = true;
    }
}

void BezierShape2D::setPositionEnd( const vec2& position )
{
    if (m_PositionEnd != position)
    {
        m_PositionEnd = position;
        m_NeedsUpdate = true;
    }
}

void BezierShape2D::setBeginTangent( const vec2& tangent )
{
    if (m_TangentBegin != tangent)
    {
        m_TangentBegin = tangent;
        m_NeedsUpdate = true;
    }
}

void BezierShape2D::setEndTangent( const vec2& tangent )
{
    if (m_TangentEnd != tangent)
    {
        m_TangentEnd = tangent;
        m_NeedsUpdate = true;
    }
}
void BezierShape2D::draw2D( gfx::Canvas2D* const canvas, const mat33& transform )
{
    vec2 diff(m_PositionEnd - m_PositionBegin);
    const vec2 myNormal(diff.x > 0? 1.0f : -1.0f, 0.0f);
    const vec2 myUp(0.0f, diff.y > 0? 1.0f : -1.0f);

    const vec2 transformedPosition(transform * m_PositionBegin);

    Canvas2Dnew* const cvs(canvas->getRenderer2D()->getNewCanvas());
    cvs->drawSprite(m_Sprite, 
        transformedPosition - vec2(myNormal.x > 0 ? 0 : m_Sprite->getSize().x,
        myUp.y > 0 ? 0 : m_Sprite->getSize().y));

    if (m_NeedsUpdate)
    {
        updateShape();
        m_NeedsUpdate = false;
    }
}

void BezierShape2D::updateShape()
{
    const float lineWidth(3.0f);

    vec2 diff(m_PositionEnd - m_PositionBegin);
    const vec2 size(abs(diff.x) + lineWidth, abs(diff.y) + lineWidth);
    const vec2 myNormal(diff.x > 0? 1.0f : -1.0f, 0.0f);
    const vec2 myUp(0.0f, diff.y > 0? 1.0f : -1.0f);
    diff.x *= myNormal.x; // abs
    diff.y *= myUp.y;

    const vec2 beginPoint(size.x / 2.0f - myNormal.x * size.x / 2.0f, size.y / 2.0f - myUp.y * size.y / 2.0f + (lineWidth * myUp.y) / 2.0f);
    const vec2 endPoint(size.x / 2.0f + myNormal.x * size.x / 2.0f, size.y / 2.0f + myUp.y * size.y / 2.0f - (lineWidth * myUp.y) / 2.0f);

    m_Sprite->invalidate(size);

    gui::SpriteCreator* const cr(GUI->Sprites);
    cr->setActiveSprite(m_Sprite);
    cr->newPath();
    cr->moveTo(beginPoint);
    cr->curveTo(
        beginPoint + m_TangentBegin * diff.x / 2.0f,
        endPoint + m_TangentEnd * diff.x / 2.0f,
        endPoint);
    cr->setColor(Color(1.0f, 1.0f, 1.0f));
    cr->setLineWidth(3);
    cr->stroke();
    cr->renderSpriteAsync();
}

} } //end namespace
