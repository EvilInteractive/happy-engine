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

#include "HappyPCH.h" 

#include "Font.h"
#include "Text.h"
#include "ResourceFactory.h"
#include "Texture2D.h"

#include FT_GLYPH_H
#include FT_BITMAP_H

namespace he {
namespace gfx {

Font::Font() :  m_FTLibrary(nullptr),
                m_Face(nullptr),
                m_CharSize(0),
                m_ExtendedChars(false),
                m_Cached(false),
                m_Init(false),
                m_LineHeight(0)
{
    
}

Font::~Font()
{
    if (m_Face != 0)
    {
        FT_Done_Face(m_Face);
    }

    m_TextureAtlas->release();
}

/* GENERAL */
void Font::init(FT_Library lib, FT_Face face, ushort size)
{
    m_FTLibrary = lib;
    m_Face = face;
    m_CharSize = size;

    ObjectHandle hnd = ResourceFactory<Texture2D>::getInstance()->create();
    m_TextureAtlas = ResourceFactory<Texture2D>::getInstance()->get(hnd);
    m_TextureAtlas->init(gfx::Texture2D::WrapType_Repeat,  gfx::Texture2D::FilterType_Nearest, 
        gfx::Texture2D::TextureFormat_Compressed_RGBA8_DXT5, false);
    m_TextureAtlas->setName(std::string("FontTextureAtlas: ") + getName());

    m_Init = true;

    /* Precache (create texture atlas) automatically.
    No reason not to, huge performance improvement */
    preCache();
}

void Font::preCache(bool extendedCharacters)
{
    HE_ASSERT(m_Init, "Init Font before using!");

    if (m_Cached)
        return;

    m_ExtendedChars = extendedCharacters;

    // normal chars -> ABC def 012 %=- (first 127 ASCII)
    // or with extended chars -> ιθη§ (256 ASCII)
    byte nrChars = (extendedCharacters == true) ? (byte)255 : (byte)128;
    vec2 texSize;

    std::vector<byte*> glyphBuffers;
    std::vector<vec2> glyphSizes;
    std::vector<float> glyphTop;

    glyphBuffers.resize(nrChars);
    glyphSizes.resize(nrChars);
    glyphTop.resize(nrChars);
    m_CharTextureData.resize(nrChars);

    int maxHeight(0), maxTop(0);
    int width(0), height(0);

    // render each char to buffers
    // note: first 32 chars are not significant
    for (byte chr(32); chr < nrChars; ++chr)
    {
        // load character glyphs
        FT_ULong c(chr);
        FT_Load_Char(m_Face, c, FT_LOAD_TARGET_NORMAL);

        // render glyph
        FT_Glyph glyph;
        FT_Get_Glyph(m_Face->glyph, &glyph);

        // normal -> 256 gray -> AA
        FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
        FT_BitmapGlyph bmpGlyph = (FT_BitmapGlyph)glyph;

        if (bmpGlyph->bitmap.rows > maxHeight)
            maxHeight = bmpGlyph->bitmap.rows;

        if (bmpGlyph->top > maxTop)
            maxTop = bmpGlyph->top;

        width = bmpGlyph->bitmap.width;
        height = bmpGlyph->bitmap.rows;

        texSize.x += (float)width;
        glyphSizes[chr] = vec2((float)width, (float)height);

        // 1 / 64 pixel -> weird format of freetype
        m_CharTextureData[chr].advance = vec2((float)(glyph->advance.x >> 16), (float)bmpGlyph->top); 
        glyphTop[chr] = (float)(height - bmpGlyph->top);

        // use RGBA instead of R (1 channel)
        // gpu's like 4 byte packing better
        glyphBuffers[chr] = NEW byte[width * height * 4];

        for (int h(0); h < height; ++h)
        {
            for (int w(0); w < width; ++w)
            {
                glyphBuffers[chr][4 * (w + (h * width))] = glyphBuffers[chr][(4 * (w + (h * width))) + 1] =
                glyphBuffers[chr][(4 * (w + (h * width))) + 2] = glyphBuffers[chr][(4 * (w + (h * width))) + 3] =
                    bmpGlyph->bitmap.buffer[w + (bmpGlyph->bitmap.width * (height - h - 1))];
            }
        }

        FT_Done_Glyph(glyph);
    }

    m_LineHeight = maxHeight;

    // power of 2 textures always work better
    texSize.x = (float)nextP2((int)texSize.x);
    texSize.y = (float)nextP2(maxHeight) * 2;

    vec2 penPos;

    // create final buffer for texture atlas
    byte* texBuffer = NEW byte[(uint)texSize.x * (uint)texSize.y * 4];

    // fill with 0
    for (uint i(0); i < (uint)texSize.x * (uint)texSize.y * 4; ++i)
        texBuffer[i] = 0;

    // put each glyph into buffer
    for (byte i(32); i < nrChars; ++i)
    {
        penPos.y = texSize.y - maxHeight - glyphTop[i];

        // MANUAL COPYING INTO 1 BUFFER
        for (int i2 = 0; i2 < (int)glyphSizes[i].y; ++i2)
        {
            // use the awesome he_memcpy for copying
            he_memcpy( texBuffer + (((int)penPos.y + i2) * (int)texSize.x + (int)penPos.x) * sizeof(byte) * 4, 
                       glyphBuffers[i] + (i2 * (int)glyphSizes[i].x) * sizeof(byte) * 4, (int)glyphSizes[i].x * sizeof(byte) * 4);
        }

        //glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)penPos.x, (GLint)penPos.y, (GLsizei)glyphSizes[i].x, (GLsizei)glyphSizes[i].y, GL_RGBA, GL_UNSIGNED_BYTE, glyphBuffers[i]);

        m_CharTextureData[i].textureRegion = RectF(penPos.x, 0, glyphSizes[i].x, texSize.y);

        penPos.x += glyphSizes[i].x;
    }

    // upload data to texture with compression, no noticeable quality difference
    //GL::heBindTexture2D(m_TextureAtlas->getID());
    m_TextureAtlas->setData((uint)texSize.x, (uint)texSize.y, 
        texBuffer, gfx::Texture2D::BufferLayout_RGBA, gfx::Texture2D::BufferType_Byte, 0);

    // delete CPU buffer, data is on GPU now
    delete texBuffer;

    std::for_each(glyphBuffers.begin(), glyphBuffers.end(), [](byte* pBuffer)
    {
        delete pBuffer;
        pBuffer = nullptr;
    });

    m_Cached = true;
    m_TextureAtlas->setLoadFinished();
}

uint Font::getPixelHeight() const
{
    HE_ASSERT(m_Init, "Init Font before using!");

    return m_CharSize;
}

uint Font::getLineSpacing() const
{
    HE_ASSERT(m_Init, "Init Font before using!");

    // 1 / 64 pixel -> weird format of freetype
    return m_Face->size->metrics.height >> 6;
}

uint Font::getLineHeight() const
{
    return m_LineHeight;
}

float Font::getStringWidth(const std::string& string) const
{
    HE_ASSERT(m_Init, "Init Font before using!");

    vec2 penPos;

    if (m_Cached)
    {
        for (uint i(0); i < string.size(); ++i)
        {
            penPos.x += m_CharTextureData[string[i]].advance.x;

            if (i < (string.size() - 1))
            {
                penPos.x += getKerning(string[i], string[i] + 1);
            }
        }
    }
    else
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

            // normal -> 256 gray -> AA
            FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
            FT_BitmapGlyph bmpGlyph = (FT_BitmapGlyph)glyph;

            if (bmpGlyph->bitmap.rows > maxHeight)
                maxHeight = bmpGlyph->bitmap.rows;

            glyphSizes[i] = vec2((float)bmpGlyph->bitmap.width, (float)bmpGlyph->bitmap.rows);

            glyphAdvance[i] = vec2((float)(glyph->advance.x >> 16), (float)bmpGlyph->top); // 1 / 64

            FT_Done_Glyph(glyph);
        }

        FT_Vector kerning;

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
    }

    return penPos.x;
}

int Font::getKerning(char first, char second) const
{
    HE_ASSERT(m_Init, "Init Font before using!");

    FT_Vector kerning;

    if (FT_HAS_KERNING(m_Face))
    {
        FT_UInt index1(FT_Get_Char_Index(m_Face, first));
        FT_UInt index2(FT_Get_Char_Index(m_Face, second));

        FT_Get_Kerning(m_Face, index1, index2, FT_KERNING_DEFAULT, &kerning);

        return (kerning.x / 64); // 1 / 64
    }
    else
    {
        return 0;
    }
}

Texture2D* Font::getTextureAtlas() const
{
    HE_ASSERT(m_Init, "Init Font before using!");
    HE_ASSERT(m_Cached, "Precache Font before using!");

    return m_TextureAtlas;
}

const Font::CharData* Font::getCharTextureData(byte chr) const
{
    HE_ASSERT(m_Init, "Init Font before using!");
    HE_ASSERT(m_Cached, "Precache Font before using!");

    if (!m_ExtendedChars && chr > 127)
    {
        return nullptr;
    }

    return &m_CharTextureData[chr];
}

bool Font::isPreCached() const
{
    return m_Cached;
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