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

#include "HappyPCH.h" 

#include "Font.h"
#include "Text.h"
#include "ResourceFactory.h"
#include "Texture2D.h"

#include FT_GLYPH_H
#include FT_BITMAP_H

#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>

namespace he {
namespace gui {

Font::Font() :  m_FTLibrary(nullptr),
                m_Face(nullptr),
                m_CharSize(0),
                m_ExtendedChars(false),
                m_Cached(false),
                m_Init(false),
                m_LineHeight(0),
                m_TextureAtlas(nullptr),
                m_CairoFontFace(nullptr)
{
    
}

Font::~Font()
{
    cairo_font_face_destroy(m_CairoFontFace);

    if (m_Face != 0)
    {
        FT_Done_Face(m_Face);
    }

    m_TextureAtlas->release();
}

/* GENERAL */
void Font::init(FT_Library lib, FT_Face face, uint16 size, uint8 options)
{
    m_FTLibrary = lib;
    m_Face = face;
    m_CharSize = size;

    m_Init = true;

    m_CairoFontFace = cairo_ft_font_face_create_for_ft_face(m_Face, 0);

    if ((options & NO_CACHE) != NO_CACHE)
    {
        ObjectHandle hnd = ResourceFactory<gfx::Texture2D>::getInstance()->create();
        m_TextureAtlas = ResourceFactory<gfx::Texture2D>::getInstance()->get(hnd);
        m_TextureAtlas->init(
            gfx::TextureWrapType_Repeat,
            gfx::TextureFilterType_Nearest, 
            (options & NO_COMPRESSION) == NO_COMPRESSION ?
            gfx::TextureFormat_RGBA8 : gfx::TextureFormat_Compressed_RGBA8_DXT5,
            false);
        m_TextureAtlas->setName(he::String("FontTextureAtlas: ") + getName());

        preCache();
    }
}

void Font::preCache(bool extendedCharacters)
{
    HE_ASSERT(m_Init, "Init Font before using!");

    if (m_Cached)
        return;

    m_ExtendedChars = extendedCharacters;

    // normal chars -> ABC def 012 %=- (first 127 ASCII)
    // or with extended chars -> ιθη§ (256 ASCII)
    uint8 nrChars = (extendedCharacters == true) ? (uint8)255 : (uint8)128;
    vec2 texSize;

    he::PrimitiveList<uint8*> glyphBuffers;
    he::PrimitiveList<vec2> glyphSizes;
    he::PrimitiveList<float> glyphTop;

    glyphBuffers.resize(nrChars);
    glyphSizes.resize(nrChars);
    glyphTop.resize(nrChars);
    m_CharTextureData.resize(nrChars);

    int maxHeight(0), maxTop(0);
    int width(0), height(0);

    // render each char to buffers
    // note: first 32 chars are not significant
    for (uint8 chr(32); chr < nrChars; ++chr)
    {
        // load character glyphs
        FT_ULong c(chr);
        //FT_Load_Char(m_Face, c, FT_LOAD_TARGET_NORMAL);
        FT_UInt glyphIndex = FT_Get_Char_Index(m_Face, c);

        // render glyph
        FT_GlyphSlot& glyph = m_Face->glyph;
        //FT_Get_Glyph(m_Face->glyph, &glyph);
        FT_Load_Glyph(m_Face, glyphIndex, FT_LOAD_RENDER);

        // normal -> 256 gray -> AA
        //FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
        FT_Bitmap bmpGlyph = glyph->bitmap;

        if (bmpGlyph.rows > maxHeight)
            maxHeight = bmpGlyph.rows;

        if (glyph->bitmap_top > maxTop)
            maxTop = glyph->bitmap_top;

        width = bmpGlyph.width;
        height = bmpGlyph.rows;

        texSize.x += (float)width;
        glyphSizes[chr] = vec2((float)width, (float)height);

        // 1 / 64 pixel -> weird format of freetype
        m_CharTextureData[chr].advance = vec2(static_cast<float>(glyph->metrics.horiAdvance >> 6), static_cast<float>(glyph->bitmap_top)); 
        m_CharTextureData[chr].offset = static_cast<float>(glyph->metrics.horiBearingX >> 6);
        glyphTop[chr] = (float)(height - glyph->bitmap_top);

        // use RGBA instead of R (1 channel)
        // gpu's like 4 byte packing better
        glyphBuffers[chr] = NEW uint8[width * height * 4];

        for (int h(0); h < height; ++h)
        {
            for (int w(0); w < width; ++w)
            {
                glyphBuffers[chr][4 * (w + (h * width))] = glyphBuffers[chr][(4 * (w + (h * width))) + 1] =
                glyphBuffers[chr][(4 * (w + (h * width))) + 2] = glyphBuffers[chr][(4 * (w + (h * width))) + 3] =
                    bmpGlyph.buffer[w + (bmpGlyph.width * (height - h - 1))];
            }
        }

        //FT_Done_Glyph(glyph);
    }

    m_LineHeight = maxHeight;

    // power of 2 textures always work better
    texSize.x = (float)nextP2((int)texSize.x);
    texSize.y = (float)nextP2(maxHeight) * 2;

    vec2 penPos;

    // create final buffer for texture atlas
    uint8* texBuffer = NEW uint8[(uint32)texSize.x * (uint32)texSize.y * 4];

    // fill with 0
    for (uint32 i(0); i < (uint32)texSize.x * (uint32)texSize.y * 4; ++i)
        texBuffer[i] = 0;

    // put each glyph into buffer
    for (uint8 i(32); i < nrChars; ++i)
    {
        penPos.y = texSize.y - maxHeight - glyphTop[i];

        // MANUAL COPYING INTO 1 BUFFER
        for (int i2 = 0; i2 < (int)glyphSizes[i].y; ++i2)
        {
            // use the awesome he_memcpy for copying
            he_memcpy( texBuffer + (((int)penPos.y + i2) * (int)texSize.x + (int)penPos.x) * sizeof(uint8) * 4, 
                       glyphBuffers[i] + (i2 * (int)glyphSizes[i].x) * sizeof(uint8) * 4, (int)glyphSizes[i].x * sizeof(uint8) * 4);
        }

        //glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)penPos.x, (GLint)penPos.y, (GLsizei)glyphSizes[i].x, (GLsizei)glyphSizes[i].y, GL_RGBA, GL_UNSIGNED_BYTE, glyphBuffers[i]);

        m_CharTextureData[i].textureRegion = RectF(penPos.x, 0, glyphSizes[i].x, texSize.y);

        penPos.x += glyphSizes[i].x;
    }

    // upload data to texture with compression, no noticeable quality difference
    //GL::heBindTexture2D(m_TextureAtlas->getID());
    m_TextureAtlas->setData((uint32)texSize.x, (uint32)texSize.y, 
        texBuffer, gfx::TextureBufferLayout_RGBA, gfx::TextureBufferType_Byte, 0);

    // delete CPU buffer, data is on GPU now
    delete texBuffer;

    glyphBuffers.forEach([](uint8* pBuffer)
    {
        delete pBuffer;
        pBuffer = nullptr;
    });

    m_Cached = true;
    m_TextureAtlas->setLoaded(eLoadResult_Success);
}

uint32 Font::getPixelHeight() const
{
    HE_ASSERT(m_Init, "Init Font before using!");

    return m_CharSize;
}

uint32 Font::getLineSpacing() const
{
    HE_ASSERT(m_Init, "Init Font before using!");

    // 1 / 64 pixel -> weird format of freetype
    return m_Face->size->metrics.height >> 6;
}

uint32 Font::getLineHeight() const
{
    return m_LineHeight;
}

float Font::getStringWidth(const char* buff, const int len) const
{
    HE_ASSERT(m_Init, "Init Font before using!");
    HE_ASSERT(m_Cached == true, "Font must be prechached!");

    float width(0.0f);
    const size_t count(len == -1? strlen(buff) : len);

    for (uint32 i(0); i < count; ++i)
    {
        width += m_CharTextureData[buff[i]].advance.x;

        if (i < count - 1)
        {
            width += getKerning(buff[i], buff[i] + 1);
        }
    }

    return width;
}
float Font::getStringWidth(const he::String& string) const
{
    return getStringWidth(string.c_str(), string.size());
}

int Font::getKerning(char first, char second) const
{
    HE_ASSERT(m_Init, "Init Font before using!");

    FT_Vector kerning;

    int result(0);
    if (FT_HAS_KERNING(m_Face))
    {
        FT_UInt index1(FT_Get_Char_Index(m_Face, first));
        FT_UInt index2(FT_Get_Char_Index(m_Face, second));

        FT_Get_Kerning(m_Face, index1, index2, FT_KERNING_DEFAULT, &kerning);

        result = kerning.x / 64; // 1 / 64
    }
    return result;
}

gfx::Texture2D* Font::getTextureAtlas() const
{
    HE_ASSERT(m_Init, "Init Font before using!");
    HE_ASSERT(m_Cached, "Precache Font before using!");

    return m_TextureAtlas;
}

bool Font::isPreCached() const
{
    return m_Cached;
}

cairo_font_face_t* Font::getCairoFontFace() const
{
    return m_CairoFontFace;
}

uint32 Font::getGlyphIndex(const char c) const
{
    return static_cast<uint32>(FT_Get_Char_Index(m_Face, c));
}

float Font::getAdvance(const char c) const
{
    FT_Load_Glyph(m_Face, FT_Get_Char_Index(m_Face, c), FT_LOAD_DEFAULT);

    return static_cast<float>(m_Face->glyph->metrics.horiAdvance >> 6);
}

float Font::getOffset(const char c) const
{
    FT_Load_Glyph(m_Face, FT_Get_Char_Index(m_Face, c), FT_LOAD_DEFAULT);

    return static_cast<float>(m_Face->glyph->metrics.horiBearingX >> 6);
}

/* EXTRA */
inline int Font::nextP2(int a) const
{
    HE_ASSERT(m_Init, "Init Font before using!");

    int rval = 1;

    while (rval < a)
    {
        rval <<= 1; // rval *= 2;
    }

    return rval;
}

} } //end namespace