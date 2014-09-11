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

#ifndef _HE_BezierShape2D_H_
#define _HE_BezierShape2D_H_
#pragma once

namespace he {
namespace gui {

class Sprite;

class HAPPY_ENTRY BezierShape2D
{
public:
    BezierShape2D();
    ~BezierShape2D();

    void setPositionStart(const vec2& position);
    void setPositionEnd(const vec2& position);

    void setBeginTangent(const vec2& tangent);
    void setEndTangent(const vec2& tangent);

    void draw2D(gui::Canvas2D* const canvas, const mat33& transform);

private:
    void updateShape();

    vec2 m_PositionBegin;
    vec2 m_PositionEnd;
    vec2 m_TangentBegin;
    vec2 m_TangentEnd;

    //Disable default copy constructor and default assignment operator
    BezierShape2D(const BezierShape2D&);
    BezierShape2D& operator=(const BezierShape2D&);
};

} } //end namespace

#endif
