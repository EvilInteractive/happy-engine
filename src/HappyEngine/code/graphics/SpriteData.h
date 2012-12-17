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
//Created: 16/12/20112

#ifndef _HE_SPRITEDATA_H_
#define _HE_SPRITEDATA_H_
#pragma once

#include "cairo\cairo.h"

namespace he {
namespace gfx {
    class Texture2D;

struct SpriteData
{
    SpriteData( const uint16& id,
                const vec2& size,
                Texture2D* tex2D,
                unsigned char* rBuff,
                _cairo_surface* surf,
                _cairo* cp) :
                    id(id),
                    size(size),
                    texture2D(tex2D),
                    renderBuffer(rBuff),
                    cairoSurface(surf),
                    cairoPaint(cp),
                    readyState(0x00)
    {}

        
    SpriteData(const SpriteData& sd)
    {
        this->id = sd.id;
        this->size = sd.size;
        this->drawCalls = sd.drawCalls;
        this->texture2D = sd.texture2D;
        this->renderBuffer = sd.renderBuffer;
        this->cairoSurface = sd.cairoSurface;
        this->cairoPaint = sd.cairoPaint;
        this->readyState = sd.readyState;
    }
    SpriteData& operator=(const SpriteData& sd)
    {
        this->id = sd.id;
        this->size = sd.size;
        this->drawCalls = sd.drawCalls;
        this->texture2D = sd.texture2D;
        this->renderBuffer = sd.renderBuffer;
        this->cairoSurface = sd.cairoSurface;
        this->cairoPaint = sd.cairoPaint;
        this->readyState = sd.readyState;

        return *this;
    }

    bool operator==(const SpriteData& sd)
    {
        return (this->id == sd.id);
    }

    uint16 id;
    vec2 size;
    std::queue<boost::function0<void> > drawCalls;
    Texture2D* texture2D;
    unsigned char* renderBuffer;
    _cairo_surface* cairoSurface;
    _cairo* cairoPaint;
    char readyState;
};

}} //end namespace

#endif
