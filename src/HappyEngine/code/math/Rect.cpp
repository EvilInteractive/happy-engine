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
#include "HappyPCH.h" 

#include "Rect.h"

namespace he {

RectF::RectF(): x(0.0f), y(0.0f), width(0.0f), height(0.0f)
{
}
RectF::RectF(float x_, float y_, float width_, float height_): x(x_), y(y_), width(width_), height(height_)
{
}
RectF::RectF(const vec2& pos, const vec2& size): x(pos.x), y(pos.y), width(size.x), height(size.y)
{
}
RectF::RectF(const RectI& rect): x(static_cast<float>(rect.x)), y(static_cast<float>(rect.y)), 
                                 width(static_cast<float>(rect.width)), height(static_cast<float>(rect.height))
{
}
RectF::~RectF()
{
}

bool RectF::operator==(const RectF& r) const
{
	return x == r.x && y == r.y && width == r.width && height == r.height;
}

bool RectF::operator!=(const RectF& r) const
{
	return !(*this == r);
}

RectI::RectI(): x(0), y(0), width(0), height(0)
{
}
RectI::RectI(int x_, int y_, int width_, int height_): x(x_), y(y_), width(width_), height(height_)
{
}
RectI::RectI(const RectF& rect): x(static_cast<int>(rect.x)), 
                                 y(static_cast<int>(rect.y)), 
                                 width(static_cast<int>(rect.width)), 
                                 height(static_cast<int>(rect.height))
{
}

RectI::RectI( const RectI& rect ): x(rect.x), 
                                   y(rect.y), 
                                   width(rect.width), 
                                   height(rect.height)
{
}

RectI::~RectI()
{
}
bool RectI::operator==(const RectI& r) const
{
    return x == r.x && y == r.y && width == r.width && height == r.height;
}

bool RectI::operator!=(const RectI& r) const
{
    return x != r.x || y != r.y || width != r.width || height != r.height;
}

RectI& RectI::operator=( const RectI& other )
{
    he_memcpy(this, &other, sizeof(RectI));
    return *this;
}

} //end namespace