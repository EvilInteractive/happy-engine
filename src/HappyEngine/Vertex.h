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

#include "Vector3.h"
#include "VertexLayout.h"

namespace happyengine {
namespace graphics {

struct VertexPosCol
{
public:
    math::Vector3 position;
    math::Vector3 color;

	VertexPosCol();
	VertexPosCol(const math::Vector3& pos, const math::Vector3& col);
    ~VertexPosCol();
	VertexPosCol(const VertexPosCol& other);
	VertexPosCol& operator=(const VertexPosCol& other);
};

} } //end namespace

#endif
