//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
//Created: 22/09/2011

#ifndef _HE_Circle_H_
#define _HE_Circle_H_
#pragma once

#include "Vector2.h"
#include <utility>

namespace happyengine {
namespace math {
namespace shapes {

class Circle
{
public:
    Circle(const math::Vector2& position, float radius);
    virtual ~Circle();

    std::pair<float, float> fx(float x) const;
    std::pair<float, float> fy(float y) const;
    std::pair<math::Vector2, math::Vector2> intersect(const Circle& other) const;

    bool isOnCircle(const math::Vector2& point) const;

private:

    math::Vector2 m_Position;
    float m_Radius;

    //Disable default copy constructor and default assignment operator
    Circle(const Circle&);
    Circle& operator=(const Circle&);
};

} } } //end namespace

#endif
