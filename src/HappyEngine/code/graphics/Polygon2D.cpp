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

#include "Polygon2D.h"
#include "HappyNew.h"

namespace he {
namespace gui {

/* CONSTRUCTOR - DESTRUCTOR */
Polygon2D::Polygon2D() :	Shape2D()
{
}

Polygon2D::~Polygon2D()
{
	
}

/* GENERAL */
void Polygon2D::addPoint(const vec2& p)
{
	m_Polygon.addPoint(p);
}

void Polygon2D::clear()
{
	m_Polygon.clear();
}

bool Polygon2D::triangulate()
{
	return m_Polygon.triangulate();
}

/* GETTERS */
bool Polygon2D::isTriangulated() const
{
	return m_Polygon.isTriangulated();
}

bool Polygon2D::hitTest(const vec2& hitPoint) const
{
	return m_Polygon.hitTest(hitPoint);
}

float Polygon2D::getArea() const
{
	return m_Polygon.getArea();
}

} } //end namespace