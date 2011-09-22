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

#include "Circle.h"
#include "HappyNew.h"
#include "MathFunctions.h"

namespace happyengine {
namespace math {
namespace shapes {

Circle::Circle(const math::Vector2& position, float radius): m_Position(position), m_Radius(radius)
{
}


Circle::~Circle()
{
}

std::pair<float, float> Circle::fx(float x) const
{
    return std::make_pair(
        sqrt(math::sqr(m_Radius) - math::sqr(x - m_Position.x)) + m_Position.y,
        -sqrt(math::sqr(m_Radius) - math::sqr(x - m_Position.x)) + m_Position.y);
}
std::pair<float, float> Circle::fy(float y) const
{
    return std::make_pair(
        sqrt(math::sqr(m_Radius) - math::sqr(y - m_Position.y)) + m_Position.x,
        -sqrt(math::sqr(m_Radius) - math::sqr(y - m_Position.y)) + m_Position.x);
}
bool Circle::isOnCircle(const math::Vector2& point) const
{
    return (fabs(math::sqr(point.x - m_Position.x) + math::sqr(point.y - m_Position.y) - math::sqr(m_Radius)) < 0.0001f);
}

std::pair<math::Vector2, math::Vector2> Circle::intersect(const Circle& other) const
{
    const float& xA(m_Position.x), 
                 xB(other.m_Position.x), 
                 yA(m_Position.y), 
                 yB(other.m_Position.y), 
                 rA(m_Radius), 
                 rB(other.m_Radius);

    float d2(math::sqr(xB - xA) + math::sqr(yB - yA));
    float K(0.25f * sqrt((math::sqr(rA + rB) - d2) * (d2 - math::sqr(rA - rB))));

    float x1(0.5f * (xB + xA) + 0.5f * (xB - xA) * (math::sqr(rA) - math::sqr(rB)) / d2 + 2.0f * (yB - yA) * K / d2);
    float x2(0.5f * (xB + xA) + 0.5f * (xB - xA) * (math::sqr(rA) - math::sqr(rB)) / d2 - 2.0f * (yB - yA) * K / d2);

    std::pair<float, float> y1s(fx(x1));
    std::pair<float, float> y2s(fx(x2));

    std::pair<math::Vector2, math::Vector2> ret(std::make_pair(math::Vector2(x1, y1s.first), math::Vector2(x2, y2s.first)));
    if (isOnCircle(ret.first) == false)
    {
        ret.first.y = y1s.second;
        //ASSERT(isOnCircle(ret.first), "Circle: no point intersects circle");
    }
    if (isOnCircle(ret.second) == false)
    {
        ret.second.y = y2s.second;
        //ASSERT(isOnCircle(ret.second), "Circle: no point intersects circle");
    }

    return ret;
}

} } } //end namespace