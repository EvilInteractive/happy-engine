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
//Author:  Sebastiaan Sprengers
//Created: 18/10/2011

#include "StdAfx.h" 

#include "RoundedRectangle2D.h"
#include "HappyNew.h"

namespace he {
namespace gui {

RoundedRectangle2D::RoundedRectangle2D(	const vec2& centerPos,
										const vec2& size,
										uint radius,
										uint interpolSteps) :	Shape2D()
{
	const float DEG2RAD = 3.14159f/180.0f;

	uint stepSize(360 / interpolSteps);

	for (uint i(180); i <= 270; i += stepSize)
	{
		float degInRad = i * DEG2RAD;
		m_Polygon.addPoint(vec2(vec2(centerPos.x + cosf(degInRad) * radius/2,
										centerPos.y + sinf(degInRad) * radius/2)));
	}
	for (uint i(270); i <= 360; i += stepSize)
	{
		float degInRad = i * DEG2RAD;
		m_Polygon.addPoint(vec2(vec2(centerPos.x + size.x + cosf(degInRad) * radius/2,
										centerPos.y + sinf(degInRad) * radius/2)));
	}
	for (uint i(0); i <= 90; i += stepSize)
	{
		float degInRad = i * DEG2RAD;
		m_Polygon.addPoint(vec2(vec2(centerPos.x + size.x + cosf(degInRad) * radius/2,
									centerPos.y + size.y + sinf(degInRad) * radius/2)));
	}
	for (uint i(90); i <= 180; i += stepSize)
	{
		float degInRad = i * DEG2RAD;
		m_Polygon.addPoint(vec2(vec2(centerPos.x + cosf(degInRad) * radius/2,
										centerPos.y + size.y + sinf(degInRad) * radius/2)));
	}
}

RoundedRectangle2D::~RoundedRectangle2D()
{
	
}

} } //end namespace