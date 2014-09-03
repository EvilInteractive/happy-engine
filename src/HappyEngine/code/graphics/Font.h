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
//Created: 07/05/2012

#ifndef _HE_FONT_H_
#define _HE_FONT_H_
#pragma once

#include "Resource.h"
#include "ft2build.h"
#include FT_FREETYPE_H

struct _cairo_font_face;
typedef _cairo_font_face cairo_font_face_t;

namespace he {

namespace gfx {
    class Texture2D;
}

namespace gui {
    class Text;

class Font : public Resource<Font>
{
public:

    struct CharData
    {
        RectF textureRegion;
        vec2 advance;
        float offset;
    };

    enum Options
    {
        NO_CACHE =          0x01,
        NO_COMPRESSION =    0x02,
    };

    /* CONSTRUCTOR - DESTRUCTOR */
    Font();
    virtual ~Font();
    
    /* GENERAL */
    void init(FT_Library lib, FT_Face face, uint16 size, uint8 options = 0);
    void preCache(bool extendedCharacters = false);

    /* GETTERS */
    uint32 getPixelHeight() const;
    uint32 getLineSpacing() const;
    uint32 getLineHeight() const;
    float getStringWidth(const he::String& string) const;
    float getStringWidth(const char* buff, const int len = -1) const;

    int getKerning(char first, char second) const;

    gfx::Texture2D* getTextureAtlas() const;
    inline const CharData& getCharTextureData(const uint8 chr) const
    {
        HE_ASSERT(m_Init, "Init Font before using!");
        HE_ASSERT(m_Cached, "Precache Font before using!");
        uint8 useChr(63);

        HE_IF_ART_ASSERT(chr <= 127 || m_ExtendedChars, "Font char %d out of range", chr)
        {
            useChr = chr;
        }

        return m_CharTextureData[useChr];
    }

    bool isPreCached() const;

    cairo_font_face_t* getCairoFontFace() const;

    uint32 getGlyphIndex(const char c) const;
    float getAdvance(const char c) const;
    float getOffset(const char c) const;

    /* SETTERS */

private:

    /* EXTRA */
    inline int nextP2(int a) const;

    /* DATAMEMBERS */
    FT_Library m_FTLibrary;
    FT_Face m_Face;

    uint16 m_CharSize;
    uint32 m_LineHeight;

    bool m_Cached;
    bool m_ExtendedChars;
    gfx::Texture2D* m_TextureAtlas;
    PrimitiveList<CharData> m_CharTextureData;

    bool m_Init;

    // needed for rendering with cairo to sprites
    cairo_font_face_t* m_CairoFontFace;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    Font(const Font&);
    Font& operator=(const Font&);
};

} } //end namespace

#endif
