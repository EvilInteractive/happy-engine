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

#include "HappyTypes.h"
#include "SDL_ttf.h"
#include "Texture2D.h"
#include "vec2.h"

#include <string>
#include "Color.h"

#include "boost/shared_ptr.hpp"
#include <vector>

namespace he {

namespace gui {
	class Text;
}

namespace gfx {

class Font
{
public:

	enum HAlignment
	{
		HAlignment_Left, 
		HAlignment_Center, 
		HAlignment_Right
	};
	enum VAlignment
	{
		VAlignment_Top, 
		VAlignment_Center, 
		VAlignment_Bottom
	};

	typedef boost::shared_ptr<Font> pointer;

	/* CONSTRUCTOR - DESTRUCTOR */
    explicit Font(TTF_Font* pFont);
    virtual ~Font();
    
	/* GETTERS */
    Texture2D::pointer createTextureText(const std::string& text, const Color& color,
										 bool bAntiAliased = true, vec2* sizeText = nullptr);
	Texture2D::pointer createTextureText(const gui::Text& text, const Color& color,
										 bool bAntiAliased = true);

	const std::string& getPath() const;

	uint getFontPixelHeight() const;
	uint getFontLineSpacing() const;
	uint getStringWidth(const std::string& string) const;

	/* SETTERS */
	void setPath(const std::string& path);

private:

	/* DATAMEMBERS */
    TTF_Font* m_pFont;

	std::string m_Path;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    Font(const Font&);
    Font& operator=(const Font&);
};

} } //end namespace

#endif
