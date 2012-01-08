//HappyTest Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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

#include "vec3.h"
#include "vec2.h"

namespace happytest {

struct VertexPNT
{
public:
    he::vec3 position;
    he::vec2 texCoord;
    he::vec3 normal;

    VertexPNT();
	VertexPNT(const he::vec3& position,
              const he::vec2& texCoord,
              const he::vec3& normal);
    ~VertexPNT();
    //default assignment operator and copyconstructor are fine
};

} //end namespace

#endif
