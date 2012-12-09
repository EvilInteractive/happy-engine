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
//Created: 25/11/2012

#ifndef _HE_SPRITE_H_
#define _HE_SPRITE_H_
#pragma once

#include "Resource.h"

namespace he {
namespace gfx {
    class Texture2D;
}
namespace gui {
    class SpriteCreator;

class Sprite : public Resource<Sprite>
{
public:

    enum OPTIONS
    {
        OPTIONS_NONE =      0x00,
        UNIFORM_SCALE =     0x01,
        DYNAMIC_DRAW =      0x02
    };

    /* CONSTRUCTOR - DESTRUCTOR */
    Sprite();
    virtual ~Sprite();

    /* GENERAL */
    void init(const vec2& size, char flags = OPTIONS_NONE);
    void invalidate(const vec2& newSize = vec2(0.f,0.f));

    /* GETTERS */
    gfx::Texture2D* getRenderTexture() const;
    uint16 getID() const;
    vec2 getSize() const;
    RectF getCenter() const;
    char getFlags() const;

    /* SETTERS */
    void setCenter(const RectF& center);

private:

    /* STATIC */
    static uint16 s_SpriteCount;

    /* MEMBERS */
    gfx::Texture2D* m_Render;

    bool m_Rendered;

    vec2 m_Size;

    uint16 m_ID;

    RectF m_Center;

    char m_Flags;

    /* DEFAULT COPY & ASSIGNMENT */ 
    Sprite(const Sprite&);
    Sprite& operator=(const Sprite&);
};

}} //end namespace

#endif
