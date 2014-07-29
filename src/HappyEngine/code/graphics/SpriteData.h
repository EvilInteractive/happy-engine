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
//
//Author:  Sebastiaan Sprengers
//Created: 16/12/20112

#ifndef _HE_SPRITEDATA_H_
#define _HE_SPRITEDATA_H_
#pragma once

#include <cairo/cairo.h>

namespace he {
namespace gfx {
    class Texture2D;

struct SpriteData
{
    SpriteData( const uint16& id,
                const vec2& size,
                Texture2D* tex2D,
                unsigned char* rBuff,
                cairo_surface_t* surf,
                cairo_t* cp) :
                    m_Id(id),
                    m_Size(size),
                    m_Texture2D(tex2D),
                    m_RenderBuffer(rBuff),
                    m_CairoSurface(surf),
                    m_CairoPaint(cp),
                    m_ReadyState(0x00)
    {}

        
    SpriteData(const SpriteData& sd)
    {
        this->m_Id = sd.m_Id;
        this->m_Size = sd.m_Size;
        this->m_DrawCalls = sd.m_DrawCalls;
        this->m_Texture2D = sd.m_Texture2D;
        this->m_RenderBuffer = sd.m_RenderBuffer;
        this->m_CairoSurface = sd.m_CairoSurface;
        this->m_CairoPaint = sd.m_CairoPaint;
        this->m_ReadyState = sd.m_ReadyState;
    }
    SpriteData& operator=(const SpriteData& sd)
    {
        this->m_Id = sd.m_Id;
        this->m_Size = sd.m_Size;
        this->m_DrawCalls = sd.m_DrawCalls;
        this->m_Texture2D = sd.m_Texture2D;
        this->m_RenderBuffer = sd.m_RenderBuffer;
        this->m_CairoSurface = sd.m_CairoSurface;
        this->m_CairoPaint = sd.m_CairoPaint;
        this->m_ReadyState = sd.m_ReadyState;

        return *this;
    }

    bool operator==(const SpriteData& sd)
    {
        return (this->m_Id == sd.m_Id);
    }

    uint16 m_Id;
    vec2 m_Size;
    std::queue<std::function<void()>> m_DrawCalls;
    Texture2D* m_Texture2D;
    unsigned char* m_RenderBuffer;
    cairo_surface_t* m_CairoSurface;
    cairo_t* m_CairoPaint;
    char m_ReadyState;
};

}} //end namespace

#endif
