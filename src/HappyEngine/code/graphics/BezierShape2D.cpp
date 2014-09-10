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
//Author:  Bastian Damman
//Created: 13/12/2012
#include "HappyPCH.h" 

#include "BezierShape2D.h"

#include "Gui.h"

namespace he {
namespace gui {

const vec2 marge(128, 4);

BezierShape2D::BezierShape2D():
    m_PositionBegin(0, 0), m_PositionEnd(0, 0)
{
}


BezierShape2D::~BezierShape2D()
{
}

void BezierShape2D::setPositionStart( const vec2& position )
{
    m_PositionBegin = position;
}

void BezierShape2D::setPositionEnd( const vec2& position )
{
    m_PositionEnd = position;
}

void BezierShape2D::setBeginTangent( const vec2& tangent )
{
    m_TangentBegin = tangent;
}

void BezierShape2D::setEndTangent( const vec2& tangent )
{
    m_TangentEnd = tangent;
}
void BezierShape2D::draw2D(gui::Canvas2D* const canvas, const mat33& transform)
{
    vec2 p0(transform * m_PositionBegin);
    vec2 p1(transform * m_PositionEnd);
    vec2 diff(p1 - p0);
    const vec2 myNormal(diff.x > 0? 1.0f : -1.0f, 0.0f);
    const vec2 myUp(0.0f, diff.y > 0? 1.0f : -1.0f);
    diff.x *= myNormal.x; // abs
    diff.y *= myUp.y;
    
    canvas->fillCurve(
        p0, 
        m_TangentBegin * diff.x / 2.0f,
        m_TangentEnd * diff.x / 2.0f,
        p1, 2.0f);
}

} } //end namespace
