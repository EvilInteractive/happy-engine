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
//Created: 15/10/2011

#ifndef _HE_POLYGON_H_
#define _HE_POLYGON_H_
#pragma once

#include "vec2.h"
#include "HappyTypes.h"
#include <vector>

namespace he {

class Polygon
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */
	Polygon();
    virtual ~Polygon();

	/* GENERAL */
	void addPoint(const vec2& p);
	bool triangulate();
	void clear();

	/* GETTERS */
	const std::vector<vec2>& getVertices() const;
	const std::vector<uint>& getIndices() const;

	bool isTriangulated() const;
	bool hitTest(const vec2& hitPoint) const;

	float getArea() const;

	uint getVertexCount() const;
	uint getIndexCount() const;
	uint getTriangleCount() const;

	/* DEFAULT COPY & ASSIGNMENT */
    Polygon(const Polygon& p);
    Polygon& operator=(const Polygon& p);

private:

	/* DATAMEMBERS */
	std::vector<vec2> m_Vertices;
	std::vector<uint> m_Indices;
};

} //end namespace

#endif
