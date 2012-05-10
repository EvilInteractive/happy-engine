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
//Author:  Bastian Damman
//Created: 12/08/2011
//Extended:Sebastiaan Sprengers

#ifndef _HE_FONT_H_
#define _HE_FONT_H_
#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

namespace he {

namespace gui {
    class Text;
}

namespace gfx {
class Texture2D;

class Font
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Font(FT_Library lib, FT_Face face, ushort size);
    virtual ~Font();
    
    /* GETTERS */
    void renderText(const std::string& text, const Color& color, Texture2D* tex);

    uint getPixelHeight() const;
    uint getLineSpacing() const;
    uint getStringWidth(const std::string& string) const;

    /* SETTERS */

private:

    /* EXTRA */
    inline int nextP2(int a) const;

    /* DATAMEMBERS */
    FT_Library m_FTLibrary;
    FT_Face m_Face;

    ushort m_CharSize;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    Font(const Font&);
    Font& operator=(const Font&);
};

} } //end namespace

#endif
