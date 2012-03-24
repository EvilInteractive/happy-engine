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
//Created: 19/03/2012

#include "HappyPCH.h" 

#include "Canvas2D.h"

namespace he {
namespace gfx {

Canvas2D::Canvas2D(void* pRenderer)
{
}

Canvas2D::~Canvas2D()
{
}

/* GENERAL */
void translate(const vec2& translation)
{

}

void rotate(float rotation)
{

}

void scale(const vec2& scale)
{

}

void save()
{

}

void restore()
{

}

/* SETTERS */
void setStrokeColor(const Color& newColor)
{

}

void setFillColor(const Color& newColor)
{

}

void setLineWidth(float width)
{

}

void setGlobalAlpha(float alpha)
{

}

/* DRAW METHODS */
void strokeRect(const vec2& pos, const vec2& size)
{

}

void fillRect(const vec2& pos, const vec2& size)
{

}

void strokeArc(const vec2& pos, float radius, float startAngle, float endAngle, bool antiClockwise)
{

}

void fillArc(const vec2& pos, float radius, float startAngle, float endAngle, bool antiClockwise)
{

}

void beginPath()
{

}

void closePath()
{

}

void moveTo(const vec2& pos)
{

}

void lineTo(const vec2& pos)
{

}

void arcTo(float radius, float startAngle, float endAngle, bool antiClockwise)
{

}

void quadraticCurveTo(const vec2& cp, const vec2& pos)
{

}

void bezierCurveTo(const vec2& cp1, const vec2& cp2, const vec2& pos)
{

}

void fill()
{

}

void stroke()
{

}

void fillText(const gui::Text& txt, const vec2& pos)
{

}

void drawImage(	const Texture2D* tex2D, const vec2& pos,
				const vec2& newDimensions = vec2(0.0f,0.0f),
				const RectF& regionToDraw = RectF(0.0f,0.0f,0.0f,0.0f))
{

}

} }//end namespace