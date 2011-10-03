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
//Extended:Sebastiaan Sprengers

#include "Font.h"
#include "SDL.h"
#include "GL/glew.h"
#include "Assert.h"
#include "HappyNew.h"

#include <algorithm>

namespace happyengine {
namespace graphics {

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
    ASSERT(pSurf != nullptr, "pSurf == nullPtr !");

	uint width(ceilToPowerOfTwo(pSurf->w)), 
        height(ceilToPowerOfTwo(pSurf->h));

    SDL_Surface* pP2Surf(SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000));

    SDL_SetSurfaceBlendMode(pSurf, SDL_BLENDMODE_NONE);
	SDL_BlitSurface(pSurf, 0, pP2Surf, 0);
    SDL_FreeSurface(pSurf);

    return pP2Surf;
}
Texture2D::pointer Font::createTextureText(const std::string& text, const Color& color,
											bool bAntiAliased, math::Vector2* sizeText)
{
    SDL_Color col;
    col.r = color.rByte();
    col.g = color.gByte();
    col.b = color.bByte();

	if (sizeText != nullptr)
	{
		int* textWidth = NEW int(0);
		int* textHeight = NEW int(0);

		TTF_SizeText(m_pFont, text.c_str(), textWidth, textHeight);

		sizeText->x = static_cast<float>(*textWidth);
		sizeText->y = static_cast<float>(*textHeight);

		delete textWidth;
		delete textHeight;
	}

    SDL_Surface* pSurf(convertNonP2ToP2Surface(
		TTF_RenderText_Blended(m_pFont, text.c_str(), col)));

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

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

    graphics::Texture2D::pointer tex2D(NEW graphics::Texture2D());
    tex2D->init(texID, pSurf->w, pSurf->h, GL_BGRA);
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

} } //end namespace