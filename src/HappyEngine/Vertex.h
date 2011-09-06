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

#ifndef _VERTEX_H_
#define _VERTEX_H_
#pragma once

#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "VertexLayout.h"

namespace happyengine {
namespace graphics {

struct VertexPosCol
{
public:
    math::Vector3 position;
    math::Vector4 color;

	VertexPosCol();
	VertexPosCol(const math::Vector3& pos, const math::Vector4& col);
    ~VertexPosCol();
    //default copy constructor and assignment operator are fine
};

struct VertexPosCol2D
{
public:
    math::Vector2 position;
    math::Vector4 color;

	VertexPosCol2D();
	VertexPosCol2D(const math::Vector2& pos, const math::Vector4& col);
    ~VertexPosCol2D();
    //default copy constructor and assignment operator are fine
};

struct VertexPosTex
{
public:
    math::Vector3 position;
    math::Vector2 textureCoord;

	VertexPosTex();
	VertexPosTex(const math::Vector3& pos, const math::Vector2& tex);
    ~VertexPosTex();
    //default copy constructor and assignment operator are fine
};

struct VertexPosTex2D
{
public:
    math::Vector2 position;
    math::Vector2 textureCoord;

	VertexPosTex2D();
	VertexPosTex2D(const math::Vector2& pos, const math::Vector2& tex);
    ~VertexPosTex2D();
    //default copy constructor and assignment operator are fine
};

} } //end namespace

#endif
