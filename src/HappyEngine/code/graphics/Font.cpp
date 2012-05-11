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

#include FT_GLYPH_H

namespace he {
namespace gfx {

Font::Font(FT_Library lib, FT_Face face, ushort size) :   m_FTLibrary(lib),
                                                          m_Face(face),
                                                          m_CharSize(size)
{

}

Font::~Font()
{
    FT_Done_Face(m_Face);
}

void Font::renderText(const std::string& text, const Color& color, Texture2D* tex)
{
    vec2 texSize;

    std::vector<byte*> glyphBuffers;
    std::vector<vec2> glyphSizes;
    std::vector<vec2> glyphAdvance;

    glyphBuffers.resize(text.size());
    glyphSizes.resize(text.size());
    glyphAdvance.resize(text.size());

    int maxHeight(0);

    for (uint i(0); i < text.size(); ++i)
    {
        // load character glyphs
        FT_ULong c(text[i]);
        FT_Load_Char(m_Face, c, FT_LOAD_TARGET_NORMAL);

        // render glyph
        FT_Glyph glyph;
        FT_Get_Glyph(m_Face->glyph, &glyph);
        //FT_Render_Glyph(m_Face->glyph, FT_RENDER_MODE_NORMAL); // normal -> 256 gray -> AA
        FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
        FT_BitmapGlyph bmpGlyph = (FT_BitmapGlyph)glyph;

        if (bmpGlyph->bitmap.rows > maxHeight)
            maxHeight = bmpGlyph->bitmap.rows;

        int width = nextP2(bmpGlyph->bitmap.width);
        int height = nextP2(bmpGlyph->bitmap.rows);

        texSize += vec2((float)width, (float)height);
        glyphSizes[i] = vec2((float)width, (float)height);

        glyphAdvance[i] = vec2(glyph->advance.x >> 16, bmpGlyph->top); // 1 / 64

        glyphBuffers[i] = NEW byte[2 * width * height];

        for (int j(0); j < height; ++j)
        {
            for (int h(0); h < width; ++h)
            {
                glyphBuffers[i][2 * (h + (j * width))] = glyphBuffers[i][2 * (h + (j * width)) + 1] =
                    (h >= bmpGlyph->bitmap.width || j >= bmpGlyph->bitmap.rows) ?
                    0 : bmpGlyph->bitmap.buffer[h + (bmpGlyph->bitmap.width * j)];
            }
        }

        FT_Done_Glyph(glyph);
    }

    GL::heBindTexture2D(tex->getID());
    tex->setData((uint)texSize.x, (uint)texSize.y, 
        gfx::Texture2D::TextureFormat_RGBA8, 0, 
        gfx::Texture2D::BufferLayout_RG, gfx::Texture2D::BufferType_Byte,
        gfx::Texture2D::WrapType_Repeat,  gfx::Texture2D::FilterType_Linear, false, false);
   
    FT_Vector kerning;
    vec2 penPos;

    for (uint i(0); i < text.size(); ++i)
    {
        penPos.y = maxHeight - glyphAdvance[i].y;

        glTexSubImage2D(GL_TEXTURE_2D, 0, penPos.x, penPos.y, (GLsizei)glyphSizes[i].x, (GLsizei)glyphSizes[i].y, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, glyphBuffers[i]);

        penPos.x += glyphAdvance[i].x;

        if (FT_HAS_KERNING(m_Face) && i < text.size() - 1)
        {
            FT_UInt index1(FT_Get_Char_Index(m_Face,text[i]));
            FT_UInt index2(FT_Get_Char_Index(m_Face,text[i + 1]));

            FT_Get_Kerning(m_Face, index1, index2, FT_KERNING_DEFAULT, &kerning);

            penPos.x += (kerning.x >> 6); // 1 / 64
        }
    }

    std::for_each(glyphBuffers.begin(), glyphBuffers.end(), [](byte* pBuffer)
    {
        delete pBuffer;
        pBuffer = nullptr;
    });
}

uint Font::getPixelHeight() const
{
    return m_CharSize;
}

uint Font::getLineSpacing() const
{
    //return 1;
    return m_Face->size->metrics.height >> 6;
}

uint Font::getStringWidth(const std::string& string) const
{
    std::vector<vec2> glyphSizes;
    std::vector<vec2> glyphAdvance;

    glyphSizes.resize(string.size());
    glyphAdvance.resize(string.size());

    int maxHeight(0);

    for (uint i(0); i < string.size(); ++i)
    {
        // load character glyphs
        FT_ULong c(string[i]);
        FT_Load_Char(m_Face, c, FT_LOAD_TARGET_NORMAL);

        // render glyph
        FT_Glyph glyph;
        FT_Get_Glyph(m_Face->glyph, &glyph);
        //FT_Render_Glyph(m_Face->glyph, FT_RENDER_MODE_NORMAL); // normal -> 256 gray -> AA
        FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
        FT_BitmapGlyph bmpGlyph = (FT_BitmapGlyph)glyph;

        if (bmpGlyph->bitmap.rows > maxHeight)
            maxHeight = bmpGlyph->bitmap.rows;

        glyphSizes[i] = vec2(bmpGlyph->bitmap.width, bmpGlyph->bitmap.rows);

        glyphAdvance[i] = vec2(glyph->advance.x >> 16, bmpGlyph->top); // 1 / 64

        FT_Done_Glyph(glyph);
    }

    FT_Vector kerning;
    vec2 penPos;

    for (uint i(0); i < string.size(); ++i)
    {
        penPos.y = maxHeight - glyphAdvance[i].y;
        penPos.x += glyphAdvance[i].x;

        if (FT_HAS_KERNING(m_Face) && i < string.size() - 1)
        {
            FT_UInt index1(FT_Get_Char_Index(m_Face, string[i]));
            FT_UInt index2(FT_Get_Char_Index(m_Face, string[i + 1]));

            FT_Get_Kerning(m_Face, index1, index2, FT_KERNING_DEFAULT, &kerning);

            penPos.x += (kerning.x >> 6); // 1 / 64
        }
    }

    return penPos.x;
}

/* EXTRA */
inline int Font::nextP2(int a) const
{
    int rval = 1;

    while (rval < a)
    {
        rval <<= 1; // rval *= 2;
    }

    return rval;
}

} } //end namespace