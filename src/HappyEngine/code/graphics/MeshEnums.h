//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 2014/03/12

#ifndef _HE_MeshEnums_H_
#define _HE_MeshEnums_H_
#pragma once

namespace he {
namespace gfx {

ENUM(IndexStride, uint8)
{
    IndexStride_UShort = sizeof(uint16),
    IndexStride_UInt = sizeof(uint32)
};

enum MeshUsage
{
    MeshUsage_Static  =  GL_STATIC_DRAW,    // Update rarely to never
    MeshUsage_Stream  =  GL_STREAM_DRAW,    // Update frequently
    MeshUsage_Dynamic =  GL_DYNAMIC_DRAW    // Update every frame
};

enum MeshDrawMode
{
    MeshDrawMode_Points     =   GL_POINTS,
    MeshDrawMode_Lines      =   GL_LINES,
    MeshDrawMode_LineLoop   =   GL_LINE_LOOP,
    MeshDrawMode_Triangles  =   GL_TRIANGLES
};

} } //end namespace

#endif
