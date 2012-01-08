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

#ifndef _HE_POLYGON2D_H_
#define _HE_POLYGON2D_H_
#pragma once

#include "Shape2D.h"
#include "Polygon.h"
#include "vec2.h"

namespace he {
namespace gui {

class Polygon2D : public Shape2D
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */
	Polygon2D();
    virtual ~Polygon2D();

	/* GENERAL */
	void addPoint(const vec2& p);
	void clear();
	bool triangulate();

	/* GETTERS */
	bool isTriangulated() const;
	bool hitTest(const vec2& hitPoint) const;
	float getArea() const;
};

} } //end namespace

#endif
