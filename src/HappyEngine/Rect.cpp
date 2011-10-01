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
//Extended:Sebastiaan Sprengers

#include "Rect.h"

namespace happyengine {

RectF::RectF(): x(0.0f), y(0.0f), width(0.0f), height(0.0f)
{
}
RectF::RectF(float x_, float y_, float width_, float height_): x(x_), y(y_), width(width_), height(height_)
{
}
RectF::RectF(const math::Vector2& pos, const math::Vector2& size): x(pos.x), y(pos.y), width(size.x), height(size.y)
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
	if (math::Vector2(x,y) == math::Vector2(r.x,r.y) &&
		math::Vector2(width,height) == math::Vector2(r.width,r.height))
		return true;
	else
		return false;
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
RectI::~RectI()
{
}

} //end namespace