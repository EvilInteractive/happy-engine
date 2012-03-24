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

Font::Font(TTF_Font* pFont):	m_pFont(pFont),
                                m_Path("")
{

}


Font::~Font()
{
    TTF_CloseFont(m_pFont);
}
inline uint round(float round)
{
    return static_cast<uint>(round + 0.5f);
}
uint ceilToPowerOfTwo(uint value)
{
    float logbase2 = log((float)value) / log(2.0f);
    return round(pow(2.0f, ceil(logbase2)));
}
SDL_Surface* convertNonP2ToP2Surface(SDL_Surface* pSurf)
{
    HE_ASSERT(pSurf != nullptr, "pSurf == nullPtr !");

    uint width(ceilToPowerOfTwo(pSurf->w)), 
        height(ceilToPowerOfTwo(pSurf->h));

    SDL_Surface* pP2Surf(nullptr);

    pP2Surf = SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

    SDL_SetSurfaceBlendMode(pSurf, SDL_BLENDMODE_NONE);
    SDL_BlitSurface(pSurf, 0, pP2Surf, 0);
    SDL_FreeSurface(pSurf);

    return pP2Surf;
}
const Texture2D* Font::createTextureText(const std::string& text, const Color& color,
                                         bool bAntiAliased, vec2* sizeText)
{
    SDL_Color col;
    col.r = color.rByte();
    col.g = color.gByte();
    col.b = color.bByte();

    if (sizeText != nullptr)
    {
        int h(0);
        int w(0);

        TTF_SizeText(m_pFont, text.c_str(), &w, &h);

        sizeText->x = static_cast<float>(w);
        sizeText->y = static_cast<float>(h);
    }

    SDL_Surface* pSurf(nullptr);

    pSurf = convertNonP2ToP2Surface(TTF_RenderText_Blended(m_pFont, text.c_str(), col));

    GLuint texID;
    glGenTextures(1, &texID);
    GL::heBindTexture2D(0, texID);

    if (bAntiAliased)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, pSurf->w, pSurf->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, pSurf->pixels);
    
    SDL_FreeSurface(pSurf);

    ObjectHandle handle(ResourceFactory<Texture2D>::getInstance()->create());
    gfx::Texture2D* tex2D(ResourceFactory<Texture2D>::getInstance()->get(handle));
    tex2D->setName("Font::tex2D: " + text);
    tex2D->init(texID, pSurf->w, pSurf->h, GL_BGRA);
    return tex2D;    
}

const Texture2D* Font::createTextureText(const gui::Text& text, const Color& color,
                                         bool bAntiAliased)
{
    SDL_Color col;
    col.r = color.rByte();
    col.g = color.gByte();
    col.b = color.bByte();

    uint width(0);
    uint height(0);

    std::for_each(text.getText().cbegin(), text.getText().cend(), [&](std::string str)
    {
            if (getStringWidth(str) > width)
                    width = getStringWidth(str);
    });

    height = (text.getText().size() * getFontPixelHeight()) + ((text.getText().size() - 1) * getFontLineSpacing());

    SDL_Surface* pSurf(SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000));

    SDL_Surface* pSTemp(nullptr);

    SDL_SetSurfaceBlendMode(pSurf, SDL_BLENDMODE_ADD);

    uint i(0);
    std::for_each(text.getText().cbegin(), text.getText().cend(), [&] (std::string str)
    {
        pSTemp = TTF_RenderText_Blended(m_pFont, str.c_str(), col);

        SDL_Rect r;
        r.x = 0;
        r.y = i * getFontLineSpacing();
        r.w = 0;
        r.h = 0;

        SDL_BlitSurface(pSTemp, 0, pSurf, &r);

        SDL_FreeSurface(pSTemp);

        ++i;
    });

    pSurf = convertNonP2ToP2Surface(pSurf);

    GLuint texID;
    glGenTextures(1, &texID);
    GL::heBindTexture2D(0, texID);

    if (bAntiAliased)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, pSurf->w, pSurf->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, pSurf->pixels);
    
    SDL_FreeSurface(pSurf);

    ObjectHandle handle(ResourceFactory<Texture2D>::getInstance()->create());
    gfx::Texture2D* tex2D(ResourceFactory<Texture2D>::getInstance()->get(handle));
    tex2D->init(texID, pSurf->w, pSurf->h, GL_BGRA);
    tex2D->setName("Font::tex2D: " + text.getLine(0));
    return tex2D;    
}

void Font::setPath(const std::string& path)
{
    m_Path = path;
}

const std::string& Font::getPath() const
{
    return m_Path;
}

uint Font::getFontPixelHeight() const
{
    return TTF_FontHeight(m_pFont);
}

uint Font::getFontLineSpacing() const
{
    return TTF_FontLineSkip(m_pFont);
}

uint Font::getStringWidth(const std::string& string) const
{
    int w(0);
    int h(0);

    TTF_SizeText(m_pFont, string.c_str(), &w, &h);

    return static_cast<uint>(w);
}

} } //end namespace