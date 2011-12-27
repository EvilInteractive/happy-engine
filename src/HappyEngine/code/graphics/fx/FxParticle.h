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
//Created: 27/11/2011

#ifndef _HE_FX_PARTICLE_H_
#define _HE_FX_PARTICLE_H_
#pragma once

#include "vec3.h"
#include "vec4.h"

namespace he {
namespace gfx {

//16byte aligned
struct FxParticle
{
    vec3    m_Position;    //+ 12  = 12
    float   m_Life;        //+  4  = 16
    vec3    m_Scale;       //+ 12  = 28
    float   m_Rotation;    //+  4  = 32
    vec3    m_Velocity;    //+ 12  = 44
    ushort  m_UvTileX;     //+  2  = 46
    ushort  m_UvTileY;     //+  2  = 48
    vec4    m_BlendColor;  //+ 16  = 64
};

} } //end namespace

#endif
