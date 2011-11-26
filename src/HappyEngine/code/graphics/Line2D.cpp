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

#include "Line2D.h"
#include "HappyNew.h"

namespace he {
namespace gui {

/* CONSTRUCTOR - DESTRUCTOR */
Line2D::Line2D(const vec2& point1, const vec2& point2) :	Shape2D()
{
	m_Polygon.addPoint(point1); 
	m_Polygon.addPoint(point2); 
}

Line2D::~Line2D()
{
	
}

} } //end namespace