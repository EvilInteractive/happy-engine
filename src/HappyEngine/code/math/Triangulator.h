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
//Created: 14/10/2011

#ifndef _HE_TRIANGULATOR_H_
#define _HE_TRINAGULATOR_H_
#pragma once

#include "vec2.h"
#include <vector>
#include "HappyTypes.h"

namespace he {

class Triangulator
{
public:

	static bool triangulatePolygon(const std::vector<vec2>& vertices, std::vector<uint>& indices);
	static float calculateArea(const std::vector<vec2>& vertices);
	static bool hitTestTriangle(const vec2& p1, const vec2& p2, const vec2& p3, const vec2& hitPoint);
    static bool isConvex(const std::vector<vec2>& vertices);

private:

	static bool snip(const std::vector<vec2>& contour, int u, int v, int w, int n, int* V);
    static void triangulateConvex(const std::vector<vec2>& vertices, std::vector<uint>& indices);
    static bool triangulateConcave(const std::vector<vec2>& vertices, std::vector<uint>& indices);
    static int sign(float x) { return x < 0 ? -1 : 1; }
};

} //end namespace

#endif
