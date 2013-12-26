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
//Created: 15/03/2013

#ifndef _HE_WEBVIEWSURFACE_H_
#define _HE_WEBVIEWSURFACE_H_
#pragma once

#ifdef USE_WEB
#include <Awesomium/Surface.h>

namespace he {
namespace gfx {

class WebViewSurface : public Awesomium::Surface
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    WebViewSurface(uint16 width, uint16 height);
    virtual ~WebViewSurface();

    /* AWESOMIUM SURFACE */
    virtual void Paint( unsigned char* srcBuffer,
                        int srcRowSpan,
                        const Awesomium::Rect& srcRect,
                        const Awesomium::Rect& destRect);
    virtual void Scroll(int dx, int dy, const Awesomium::Rect& clipRect);

    /* GENERAL */
    void copy(unsigned char* destination, bool flipY);
    void copyDirty(unsigned char* destination, bool flipY);

    /* GETTERS */
    int getWidth() const { return m_Width; }
    int getHeight() const { return m_Height; }
    int getSize() const { return m_Rowspan * m_Height; }
    bool isDirty() const { return m_Dirty; }
    unsigned char* getBuffer() const { return m_Buffer; }
    RectI getDirtyBounds(bool flipY = false) const;

    /* SETTERS */
    void setDirty(bool dirty) { m_Dirty = dirty; m_DirtyBounds = RectI(); }

private:

    /* MEMBERS */
    unsigned char* m_Buffer;

    uint8 m_BPP;
    uint16 m_Rowspan;
    uint16 m_Width;
    uint16 m_Height;

    bool m_Dirty;

    RectI m_DirtyBounds;

    /* DEFAULT COPY & ASSINGMENT */
    WebViewSurface(const WebViewSurface&);
    WebViewSurface& operator=(const WebViewSurface&);
};

}} //end namespace

#endif
#endif
