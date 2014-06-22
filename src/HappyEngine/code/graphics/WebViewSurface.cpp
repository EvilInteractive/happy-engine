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
//Created: 15/03/2013

#include "HappyPCH.h" 

#ifdef USE_WEB
#include "WebViewSurface.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESTRUCTOR */
WebViewSurface::WebViewSurface(uint16 width, uint16 height): m_Dirty(true),
                                                             m_Width(width),
                                                             m_Height(height),
                                                             m_BPP(4)
{
    m_Rowspan = m_Width * m_BPP;
    m_Buffer = static_cast<unsigned char*>(he_malloc(m_Rowspan * m_Height));
}

WebViewSurface::~WebViewSurface()
{
    he_free(m_Buffer);
}

/* AWESOMIUM SURFACE */
void WebViewSurface::Paint( unsigned char* srcBuffer,
                            int srcRowSpan,
                            const Awesomium::Rect& srcRect,
                            const Awesomium::Rect& destRect)
{
    if (srcBuffer == 0)
        return;

    for (int row (0); row < destRect.height; ++row)
    {
        he_memcpy(m_Buffer + (row + destRect.y) * m_Rowspan + (destRect.x * m_BPP),
                  srcBuffer + (row + srcRect.y) * srcRowSpan + (srcRect.x * 4),
                  destRect.width * m_BPP);
    }

    m_DirtyBounds.x = destRect.x;
    m_DirtyBounds.y = destRect.y;
    m_DirtyBounds.width = destRect.width;
    m_DirtyBounds.height = destRect.height;

    m_Dirty = true;
}

void WebViewSurface::Scroll(int dx, int dy, const Awesomium::Rect& clipRect)
{
    if (::abs(dx) >= clipRect.width || ::abs(dy) >= clipRect.height)
        return;

    if (dx < 0 && dy == 0)
    {
        unsigned char* tmpBuffer(static_cast<unsigned char*>(he_malloc((clipRect.width + dx) * 4)));

        for (uint32 i(0); i < (uint32)clipRect.height; ++i)
        {
            he_memcpy(tmpBuffer,
                      m_Buffer + (i + clipRect.y) * m_Rowspan + (clipRect.x - dx) * m_BPP,
                      (clipRect.width + dx) * 4);

            he_memcpy(m_Buffer + (i + clipRect.y) * m_Rowspan + (clipRect.x * m_BPP),
                      tmpBuffer, (clipRect.width + dx) * 4);
        }

        he_free(tmpBuffer);
    }
    else if (dx > 0 && dy == 0)
    {
        unsigned char* tmpBuffer(static_cast<unsigned char*>(he_malloc((clipRect.width - dx) * 4)));

        for (uint32 i(0); i < (uint32)clipRect.height; ++i)
        {
            he_memcpy(tmpBuffer,
                      m_Buffer + (i + clipRect.y) * m_Rowspan + (clipRect.x * m_BPP),
                      (clipRect.width - dx) * 4);
                
            he_memcpy(m_Buffer + (i + clipRect.y) * m_Rowspan + (clipRect.x + dx) * m_BPP,
                      tmpBuffer, (clipRect.width - dx) * 4);
        }

        he_free(tmpBuffer);
    }
    else if (dy < 0 && dx == 0)
    {
        for (uint32 i(0); i < (uint32)clipRect.height + dy; ++i)
        {
            he_memcpy(m_Buffer + (i + clipRect.y) * m_Rowspan + (clipRect.x * m_BPP),
                      m_Buffer + (i + clipRect.y - dy) * m_Rowspan + (clipRect.x * m_BPP),
                      clipRect.width * 4);
        }
    }
    else if (dy > 0 && dx == 0)
    {
        for (uint32 i(clipRect.height - 1); i >= (uint32)dy; --i)
        {
            he_memcpy(m_Buffer + (i + clipRect.y) * m_Rowspan + (clipRect.x * m_BPP),
                      m_Buffer + (i + clipRect.y - dy) * m_Rowspan + (clipRect.x * m_BPP),
                      clipRect.width * 4);
        }
    }

    m_DirtyBounds.x = clipRect.x;
    m_DirtyBounds.y = clipRect.y;
    m_DirtyBounds.width = clipRect.width;
    m_DirtyBounds.height = clipRect.height;

    m_Dirty = true;
}

/* GENERAL */
void WebViewSurface::copy(unsigned char* destination, bool flipY)
{
    if (m_Buffer == 0)
        return;

    if (flipY)
    {
        for (uint32 row(0); row < m_Height; ++row)
        {
            he_memcpy(destination + (row * m_Rowspan),
                      m_Buffer + ((m_Height - 1 - row) * m_Rowspan),
                      m_Width * m_BPP);
        }
    }
    else
    {
        he_memcpy(destination, m_Buffer, m_Width * m_Height * m_BPP);
    }
}

void WebViewSurface::copyDirty(unsigned char* destination, bool flipY)
{
    if (m_Buffer == 0 || (m_DirtyBounds.height == 0 && m_DirtyBounds.width == 0))
        return;

    if (flipY)
    {
        for (int row (0); row < m_DirtyBounds.height; ++row)
        {
            he_memcpy(destination + ((m_DirtyBounds.height - 1 - row) * m_DirtyBounds.width * m_BPP),
                      m_Buffer + (row + m_DirtyBounds.y) * m_Rowspan + (m_DirtyBounds.x * m_BPP),
                      m_DirtyBounds.width * m_BPP);
        }
    }
    else
    {
        for (int row (0); row < m_DirtyBounds.height; ++row)
        {
            he_memcpy(destination + (row * m_DirtyBounds.width * m_BPP),
                      m_Buffer + (row + m_DirtyBounds.y) * m_Rowspan + (m_DirtyBounds.x * m_BPP),
                      m_DirtyBounds.width * m_BPP);
        }
    }
}

/* GETTERS */
RectI WebViewSurface::getDirtyBounds(bool flipY) const
{
    if (flipY == false)
        return m_DirtyBounds;
    else
    {
        RectI bounds(m_DirtyBounds);
        bounds.y = m_Height - bounds.y - bounds.height;

        return bounds;
    }
}

}} //end namespace
#endif // USE_WEB
