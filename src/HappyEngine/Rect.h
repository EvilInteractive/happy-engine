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
//Created: 05/08/2011

#ifndef _RECT_H_
#define _RECT_H_
#pragma once

#include "HappyTypes.h"

namespace happyengine {

struct RectI;
struct RectF
{
public:
    float x, y, width, height;

    RectF();
	RectF(float x, float y, float width, float height);
    explicit RectF(const RectI& rect);
    ~RectF();
    //Default copy constructor and assignment operator are fine
};
struct RectI
{
public:
    int x, y, width, height;

	RectI();
	RectI(int x, int y, int width, int height);
    explicit RectI(const RectF& rect);
    ~RectI();
    //Default copy constructor and assignment operator are fine
};

} //end namespace

#endif
