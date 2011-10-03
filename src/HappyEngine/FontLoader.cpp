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
#include <sstream>

#include "HappyNew.h"

namespace happyengine {
namespace content {

FontLoader::FontLoader(): m_pAssetContainer(NEW AssetContainer<graphics::Font::pointer>())
{
    TTF_Init();
}


FontLoader::~FontLoader()
{
    delete m_pAssetContainer;
}

bool FontLoader::load(const std::string& path, ushort size, bool bold, bool italic, graphics::Font::pointer& pOutFont)
{
    std::stringstream stream;
    stream << path << size;

    if (m_pAssetContainer->isAssetPresent(stream.str()) == false)
    {
        TTF_Font* pFont(TTF_OpenFont(path.c_str(), size));
        if (pFont == nullptr)
        {
            std::cout << "Error loading font: " << TTF_GetError() << "\n";
            return false;
        }
        else
        {
			if (bold && italic)
			{
				TTF_SetFontStyle(pFont, TTF_STYLE_BOLD | TTF_STYLE_ITALIC);
			}
			else if (bold && !italic)
			{
				TTF_SetFontStyle(pFont, TTF_STYLE_BOLD);
			}
			else if (!bold && italic)
			{
				TTF_SetFontStyle(pFont, TTF_STYLE_ITALIC);
			}

            pOutFont = graphics::Font::pointer(NEW graphics::Font(pFont));
            m_pAssetContainer->addAsset(stream.str(), pOutFont);
            return true;
        }
    }
    else
    {
        pOutFont = m_pAssetContainer->getAsset(stream.str());
        return true;
    }
}

} } //end namespace