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

#include "FontLoader.h"
#include "SDL_ttf.h"

#include <iostream>

namespace happyengine {
namespace content {

FontLoader::FontLoader()
{
    TTF_Init();
}


FontLoader::~FontLoader()
{
}

bool FontLoader::load(const std::string& path, ushort size, graphics::Font::pointer& pOutFont)
{
    TTF_Font* pFont(TTF_OpenFont(path.c_str(), size));
    if (pFont == nullptr)
    {
        std::cout << "Error loading font: " << TTF_GetError() << "\n";
        return false;
    }
    else
    {
        pOutFont = graphics::Font::pointer(new graphics::Font(pFont));
        return true;
    }
}

} } //end namespace