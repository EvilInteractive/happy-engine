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

#ifndef _HE_FONT_H_
#define _HE_FONT_H_
#pragma once

#include "HappyTypes.h"
#include "SDL_ttf.h"
#include "Texture2D.h"

#include <string>
#include "Color.h"

#include "boost/shared_ptr.hpp"

namespace happyengine {
namespace graphics {


enum FontHAlignment
{
    FontHAlignment_Left, 
    FontHAlignment_Center, 
    FontHAlignment_Right
};
enum FontVAlignment
{
    FontVAlignment_Top, 
    FontVAlignment_Center, 
    FontVAlignment_Bottom
};
class Font
{
public:
    explicit Font(TTF_Font* pFont);
    virtual ~Font();
    
    Texture2D::pointer createTextureText(const std::string& text, const Color& color, 
                                         FontHAlignment hAlignment, FontVAlignment vAlignment,
										 bool bAntiAliased = true);

    typedef boost::shared_ptr<Font> pointer;

private:

    TTF_Font* m_pFont;

    //Disable default copy constructor and default assignment operator
    Font(const Font&);
    Font& operator=(const Font&);
};

} } //end namespace

#endif
