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
#include "HappyPCH.h" 

#include "FontLoader.h"

#include <iostream>
#include <sstream>

#include "HappyNew.h"
#include "HeAssert.h"

namespace he {
namespace ct {

FontLoader::FontLoader():   m_pAssetContainer(NEW AssetContainer<gfx::Font*>([](gfx::Font* pFont)
                            {
                                if (pFont)
                                {
                                    delete pFont;
                                    pFont = nullptr;
                                }
                            }))
{
    bool error(FT_Init_FreeType(&m_FTLibrary));
    HE_ASSERT(error == false,"Error creating freetype library!");
}


FontLoader::~FontLoader()
{
    delete m_pAssetContainer;
    FT_Done_FreeType(m_FTLibrary);
}

gfx::Font* FontLoader::load(const std::string& path, ushort size)
{
    std::stringstream stream;
    stream << path << size;

    if (m_pAssetContainer->isAssetPresent(stream.str()) == false)
    {
        FT_Face face;
        FT_Error error(FT_New_Face(m_FTLibrary, path.c_str(), 0, &face));

        if (error != 0)
        {
            HE_ERROR("Error loading font: %s", path.c_str());
            HE_ASSERT(false, "Error loading font!");
            return nullptr;
        }

        error = FT_Select_Charmap(face, FT_ENCODING_UNICODE);

        if (error != 0)
        {
            HE_ERROR("Error loading font charmap: %s", path.c_str());
            HE_ASSERT(false, "Error loading font!");
            return nullptr;
        }

        error = FT_Set_Char_Size(face, 0, size * 64, 96, 96); // font size in 1/64 pixel

        if (error != 0)
        {
            HE_ERROR("Error setting font size: %s", path.c_str());
            HE_ASSERT(false, "Error loading font!");
            return nullptr;
        }

        gfx::Font* pFont(NEW gfx::Font(m_FTLibrary, face, size));
        m_pAssetContainer->addAsset(stream.str(), pFont);

        return pFont;
    }
    else
    {
        return m_pAssetContainer->getAsset(stream.str());
        
        //return nullptr;
    }
}

} } //end namespace