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
//Author:  Sebastiaan Sprengers
//Created: 18/10/2011

#include "HappyPCH.h" 

#include "Rectangle2D.h"
#include "HappyNew.h"

namespace he {
namespace gui {

/* CONSTRUCTOR - DESTRUCTOR */
Rectangle2D::Rectangle2D(const vec2& pos, const vec2& size) : Shape2D()
{
	m_Polygon.addPoint(vec2(pos.x, pos.y)); 
	m_Polygon.addPoint(vec2(pos.x + size.x, pos.y)); 
	m_Polygon.addPoint(vec2(pos.x + size.x, pos.y + size.y)); 
	m_Polygon.addPoint(vec2(pos.x, pos.y + size.y));
}

Rectangle2D::~Rectangle2D()
{
	
}

} } //end namespace