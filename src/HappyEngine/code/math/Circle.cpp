//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
#include "HappyPCH.h" 

#include "Circle.h"
#include "HappyNew.h"
#include "MathFunctions.h"

namespace he {

namespace shapes {

Circle::Circle(const vec2& position, float radius): m_Position(position), m_Radius(radius)
{
}


Circle::~Circle()
{
}

const vec2& Circle::getPosition() const
{
    return m_Position;
}
float Circle::getRadius() const
{
    return m_Radius;
}

std::pair<float, float> Circle::fx(float x) const
{
    return std::make_pair(
        sqrt(sqr(m_Radius) - sqr(x - m_Position.x)) + m_Position.y,
        -sqrt(sqr(m_Radius) - sqr(x - m_Position.x)) + m_Position.y);
}
std::pair<float, float> Circle::fy(float y) const
{
    return std::make_pair(
        sqrt(sqr(m_Radius) - sqr(y - m_Position.y)) + m_Position.x,
        -sqrt(sqr(m_Radius) - sqr(y - m_Position.y)) + m_Position.x);
}
bool Circle::isOnCircle(const vec2& point) const
{
    return (fabs(sqr(point.x - m_Position.x) + sqr(point.y - m_Position.y) - sqr(m_Radius)) < 0.0001f);
}

void Circle::intersect(const Circle& other, he::PrimitiveList<vec2>& outIntersections) const
{
    float d(length(m_Position - other.getPosition()));

    if (d > m_Radius + other.m_Radius)
    {
        return;
    }
    else if (d < fabs(m_Radius - other.m_Radius))
    {
        return;
    }
    else
    {
        //line connecting 2 points = 
        const float& x1(m_Position.x), 
                     x2(other.m_Position.x), 
                     y1(m_Position.y), 
                     y2(other.m_Position.y), 
                     r1(m_Radius), 
                     r2(other.m_Radius);
        float d2(lengthSqr(other.m_Position - m_Position));

        float xPart1( (x2 + x1) / 2.0f + ((x2 - x1) * (sqr(r1) - sqr(r2))) / (2.0f * d2));
        float xPart2( ((y2 - y1) / (2.0f * d2)) * sqrtf( (sqr(r1 + r2) - d2) * (d2 - sqr(r2 - r1))));

        float yPart1( (y2 + y1) / 2.0f + ((y2 - y1) * (sqr(r1) - sqr(r2))) / (2.0f * d2));
        float yPart2( ((x2 - x1) / (2.0f * d2)) * sqrtf( (sqr(r1 + r2) - d2) * (d2 - sqr(r2 - r1))));

        outIntersections.add( vec2(xPart1 + xPart2, yPart1 - yPart2) );
        outIntersections.add( vec2(xPart1 - xPart2, yPart1 + yPart2) );
    }
}

} } //end namespace
