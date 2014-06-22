//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

#ifndef _HE_VERTEX_H_
#define _HE_VERTEX_H_
#pragma once

#include "vec4.h"
#include "vec3.h"
#include "vec2.h"
#include "BufferLayout.h"

namespace he {
namespace gfx {

struct VertexPosCol
{
public:
    vec3 position;
    vec4 color;

    VertexPosCol();
    VertexPosCol(const vec3& pos, const vec4& col);
    ~VertexPosCol();
    //default copy constructor and assignment operator are fine
};

struct VertexPos
{
public:
    vec3 position;

    VertexPos();
    VertexPos(const vec3& pos);
    ~VertexPos();
    //default copy constructor and assignment operator are fine
};

struct VertexPos2D
{
public:
    vec2 position;

    VertexPos2D();
    VertexPos2D(const vec2& pos);
    ~VertexPos2D();
    //default copy constructor and assignment operator are fine
};

struct VertexPosCol2D
{
public:
    vec2 position;
    vec4 color;

    VertexPosCol2D();
    VertexPosCol2D(const vec2& pos, const vec4& col);
    ~VertexPosCol2D();
    //default copy constructor and assignment operator are fine
};

struct VertexPosTex
{
public:
    vec3 position;
    vec2 textureCoord;

    VertexPosTex();
    VertexPosTex(const vec3& pos, const vec2& tex);
    ~VertexPosTex();
    //default copy constructor and assignment operator are fine
};

struct VertexPosTex2D
{
public:
    vec2 position;
    vec2 textureCoord;

    VertexPosTex2D();
    VertexPosTex2D(const vec2& pos, const vec2& tex);
    ~VertexPosTex2D();
    //default copy constructor and assignment operator are fine
};
struct VertexText
{
public:
    vec2 position;
    vec2 textureCoord;
    vec4 color;

    VertexText() {}
    VertexText(const vec2& pos, const vec2& tex, const vec4& col): 
        position(pos), textureCoord(tex), color(col)  { }
    ~VertexText() {}
    //default copy constructor and assignment operator are fine
};

struct VertexPTNT
{
public:
    vec3 position;
    vec2 textureCoord;
    vec3 normal;
    vec3 tangent;

    VertexPTNT();
    VertexPTNT(const vec3& pos, const vec2& tex, const vec3& norm, const vec3& tan);
    ~VertexPTNT() {}
    //default copy constructor and assignment operator are fine
};

} } //end namespace

#endif
