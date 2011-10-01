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

const math::Vector2& Circle::getPosition() const
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

std::vector<Vector2> Circle::intersect(const Circle& other) const
{
    float d(math::length(m_Position - other.getPosition()));

	if (d > m_Radius + other.m_Radius)
	{
		return std::vector<Vector2>();
	}
	else if (d < fabs(m_Radius - other.m_Radius))
	{
		return std::vector<Vector2>();
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

		float xPart1( (x2 + x1) / 2.0f + ((x2 - x1) * (math::sqr(r1) - math::sqr(r2))) / (2.0f * d2));
		float xPart2( ((y2 - y1) / (2.0f * d2)) * sqrtf( (math::sqr(r1 + r2) - d2) * (d2 - math::sqr(r2 - r1))));

		float yPart1( (y2 + y1) / 2.0f + ((y2 - y1) * (math::sqr(r1) - math::sqr(r2))) / (2.0f * d2));
		float yPart2( ((x2 - x1) / (2.0f * d2)) * sqrtf( (math::sqr(r1 + r2) - d2) * (d2 - math::sqr(r2 - r1))));

		std::vector<Vector2> ret;
		ret.push_back( math::Vector2(xPart1 + xPart2, yPart1 - yPart2) );
		ret.push_back( math::Vector2(xPart1 - xPart2, yPart1 + yPart2) );
		
		return ret;
	}
}

} } } //end namespace