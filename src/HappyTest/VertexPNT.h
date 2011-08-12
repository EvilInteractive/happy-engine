//HappyTest Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyTest.
//
//    HappyTest is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyTest is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyTest.  If not, see <http://www.gnu.org/licenses/>.
//
//Author:  Bastian Damman
//Created: 11/08/2011

#ifndef _VERTEX_PNT_H_
#define _VERTEX_PNT_H_
#pragma once

#include "Vector3.h"
#include "Vector2.h"

namespace happytest {

struct VertexPNT
{
public:
    happyengine::math::Vector3 position;
    happyengine::math::Vector2 texCoord;
    happyengine::math::Vector3 normal;

    VertexPNT();
	VertexPNT(const happyengine::math::Vector3& position,
              const happyengine::math::Vector2& texCoord,
              const happyengine::math::Vector3& normal);
    ~VertexPNT();
    //default assignment operator and copyconstructor are fine
};

} //end namespace

#endif
