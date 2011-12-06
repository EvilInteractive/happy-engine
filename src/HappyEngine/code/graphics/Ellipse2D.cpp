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

#include "HappyPCH.h" 

#include "Ellipse2D.h"
#include "HappyNew.h"

namespace he {
namespace gui {

Ellipse2D::Ellipse2D(const vec2& centerPos, const vec2& size, uint interpolSteps) :	Shape2D()
{
	const float DEG2RAD = 3.14159f/180.0f;

	uint stepSize(360 / interpolSteps);

	for (uint i = 0; i < 360; i += stepSize)
	{
		float degInRad = i * DEG2RAD;
		m_Polygon.addPoint(vec2(vec2(centerPos.x + cosf(degInRad) * size.x/2,
										centerPos.y + sinf(degInRad) * size.y/2)));
	}
}

Ellipse2D::~Ellipse2D()
{
	
}

} } //end namespace