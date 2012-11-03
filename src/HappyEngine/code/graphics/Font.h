//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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

namespace he {

namespace gui {
    class Text;
}

namespace gfx {
    class Texture2D;

class Font : public Resource<Font>
{
public:

    struct CharData
    {
        RectF textureRegion;
        vec2 advance;
    };

    /* CONSTRUCTOR - DESTRUCTOR */
    Font();
    virtual ~Font();
    
    /* GENERAL */
    void init(FT_Library lib, FT_Face face, uint16 size);
    void preCache(bool extendedCharacters = false);

    /* GETTERS */
    uint32 getPixelHeight() const;
    uint32 getLineSpacing() const;
    uint32 getLineHeight() const;
    float getStringWidth(const std::string& string) const;

    int getKerning(char first, char second) const;

    Texture2D* getTextureAtlas() const;
    const CharData* getCharTextureData(uint8 chr) const;

    bool isPreCached() const;

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
    Texture2D* m_TextureAtlas;
    std::vector<CharData> m_CharTextureData;

    bool m_Init;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    Font(const Font&);
    Font& operator=(const Font&);
};

} } //end namespace

#endif
