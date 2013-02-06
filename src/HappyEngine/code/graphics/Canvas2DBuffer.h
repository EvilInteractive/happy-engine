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
//Author:  Sebastiaan Sprengers
//Created: 17/11/2012

// This struct holds the buffers for canvas2d

#ifndef _HE_CANVAS2DBUFFER_H_
#define _HE_CANVAS2DBUFFER_H_
#pragma once

namespace he {
namespace gfx {

struct Canvas2DBuffer
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Canvas2DBuffer();
    ~Canvas2DBuffer();

    /* GENERAL */
    void init(GLContext* context, const vec2& size);
    void clear();
    void resize(const vec2& size);
    void store();

    /* DATA */
    ObjectHandle renderTextureHandle;
    uint32 frameBufferId;
    //uint32 depthRenderBufferId;
    GLContext* glContext;
    vec2 size;
};

}} //end namespace

#endif
