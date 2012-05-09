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

#include "HappyPCH.h" 

#include "Font.h"
#include "Text.h"
#include "ResourceFactory.h"
#include "Texture2D.h"


namespace he {
namespace gfx {

Font::Font(FT_Library* pLib, FT_Face* pFace, ushort size) :  m_pFTLibrary(pLib),
                                                             m_pFace(pFace),
                                                             m_CharSize(size)
{
}

Font::~Font()
{
    FT_Done_Face(*m_pFace);
}

void Font::renderText(const std::string& text, const Color& color, Texture2D* tex)
{
    vec2 texSize;

    std::vector<byte*> glyphBuffers;
    std::vector<vec2> glyphSizes;

    glyphBuffers.reserve(text.size());
    glyphSizes.reserve(text.size());

    for (uint i(0); i < text.size(); ++i)
    {
        // load character glyphs
        FT_Load_Glyph(*m_pFace, FT_Get_Char_Index(*m_pFace, text[i]), FT_LOAD_DEFAULT);

        // render glyph
        FT_Render_Glyph((*m_pFace)->glyph, FT_RENDER_MODE_NORMAL); // normal -> 256 gray -> AA

        int width = nextP2((*m_pFace)->glyph->bitmap.width);
        int height = nextP2((*m_pFace)->glyph->bitmap.rows);

        texSize += vec2((float)width, (float)height);
        glyphSizes[i] = vec2((float)width, (float)height);

        glyphBuffers[i] = NEW byte[2 * width * height];

        for (int j(0); j < height; ++j)
        {
            for (int h(0); h < width; ++h)
            {
                glyphBuffers[i][2 * (h + (j * width))] = glyphBuffers[i][2 * (h + (j * width)) + 1] =
                    (h >= (*m_pFace)->glyph->bitmap.width || j >= (*m_pFace)->glyph->bitmap.rows) ?
                    0 : (*m_pFace)->glyph->bitmap.buffer[h + ((*m_pFace)->glyph->bitmap.width * j)];
            }
        }
    }

    GL::heBindTexture2D(tex->getID());
    tex->setData((uint)texSize.x, (uint)texSize.y, 
        gfx::Texture2D::TextureFormat_RGBA8, 0, 
        gfx::Texture2D::BufferLayout_LA, gfx::Texture2D::BufferType_Byte,
        gfx::Texture2D::WrapType_Clamp,  gfx::Texture2D::FilterType_Linear, false, false);
   
    int x(0);

    for (uint i(0); i < text.size(); ++i)
    {
        glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, (GLsizei)glyphSizes[i].x, (GLsizei)glyphSizes[i].y, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, glyphBuffers[i]);

        x += (int)glyphSizes[i].x;
    }
}

uint Font::getFontPixelHeight() const
{
    return 1;
}

uint Font::getFontLineSpacing() const
{
    return 1;
}

uint Font::getStringWidth(const std::string& string) const
{
    return 1;
}

/* EXTRA */
inline int Font::nextP2(int a)
{
    int rval = 1;

    while (rval < a)
    {
        rval <<= 1; // rval *= 2;
    }

    return rval;
}

} } //end namespace