//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#include "Font.h"
#include "SDL.h"
#include "GL/glew.h"
#include "Assert.h"
#include "HappyNew.h"

namespace happyengine {
namespace graphics {

Font::Font(TTF_Font* pFont): m_pFont(pFont)
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
SDL_Surface* convertNonP2ToP2Surface(SDL_Surface* pSurf,
                                    FontHAlignment hAlignment, FontVAlignment vAlignment)
{
    ASSERT(pSurf != nullptr, "pSurf == nullPtr !");
    uint width(ceilToPowerOfTwo(pSurf->w)), 
        height(ceilToPowerOfTwo(pSurf->h));

    SDL_Surface* pP2Surf(SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000));
    SDL_Rect destRect;
	destRect.x = 0;
	destRect.y = 0;
    destRect.w = pSurf->w;
    destRect.h = pSurf->h;
    SDL_Rect srcRect;
	srcRect.x = 0;
	srcRect.y = 0;
    srcRect.w = pSurf->w; 
    srcRect.h = pSurf->h;
	
    switch (hAlignment)
    {
        case FontHAlignment_Left: destRect.x = 0; break;
        case FontHAlignment_Center: destRect.x = static_cast<int>(pP2Surf->w / 2.0f - destRect.w / 2.0f); break;
        case FontHAlignment_Right: destRect.x = pP2Surf->w - destRect.w; break;
        default: ASSERT("unkown font alignment");
    }
    switch (vAlignment)
    {
        case FontVAlignment_Top: destRect.y = 0; break;
        case FontVAlignment_Center: destRect.y = static_cast<int>(pP2Surf->h / 2.0f - destRect.h / 2.0f); break;
        case FontVAlignment_Bottom: destRect.y = pP2Surf->h - destRect.h; break;
        default: ASSERT("unkown font alignment");
    }

    SDL_SetSurfaceBlendMode(pSurf, SDL_BLENDMODE_NONE);
	SDL_BlitScaled(pSurf, &srcRect, pP2Surf, &destRect);
    SDL_FreeSurface(pSurf);

    return pP2Surf;
}
Texture2D::pointer Font::createTextureText(const std::string& text, const Color& color, 
                                           FontHAlignment hAlignment, FontVAlignment vAlignment)
{
    SDL_Color col;
    col.r = color.rByte();
    col.g = color.gByte();
    col.b = color.bByte();

    SDL_Surface* pSurf(convertNonP2ToP2Surface(
        TTF_RenderText_Blended(m_pFont, text.c_str(), col), 
        hAlignment, vAlignment));

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, pSurf->w, pSurf->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, pSurf->pixels);

    
    SDL_FreeSurface(pSurf);

    graphics::Texture2D::pointer tex2D(NEW graphics::Texture2D());
    tex2D->init(texID, pSurf->w, pSurf->h, GL_BGRA);
    return tex2D;    
}


} } //end namespace