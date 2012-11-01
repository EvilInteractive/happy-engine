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
//Created: 05/08/2011
//Extended:Sebastiaan Sprengers

#ifndef _HE_RECT_H_
#define _HE_RECT_H_
#pragma once

#include "HappyTypes.h"
#include "vec2.h"

namespace he {

struct RectI;
struct RectF
{
public:
    float x, y, width, height;

    RectF();
	RectF(float x, float y, float width, float height);
	RectF(const vec2& pos, const vec2& size);
    explicit RectF(const RectI& rect);
    ~RectF();
    //Default copy constructor and assignment operator are fine

	bool operator==(const RectF& r) const;
	bool operator!=(const RectF& r) const;
};
struct RectI
{
public:
    int x, y, width, height;

	RectI();
	RectI(int x, int y, int width, int height);
    explicit RectI(const RectF& rect);
    ~RectI();

    RectI(const RectI& rect);
    RectI& operator=(const RectI& other);

	bool operator==(const RectI& r) const;
	bool operator!=(const RectI& r) const;
};

} //end namespace

#endif
