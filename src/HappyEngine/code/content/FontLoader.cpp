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

FontLoader::FontLoader():   m_pAssetContainer(NEW AssetContainer<gfx::Font*>()),
                            m_pFTLibrary(NEW FT_Library())
{
    bool error(FT_Init_FreeType(m_pFTLibrary));
    HE_ASSERT(error == false,"Error creating freetype library!");
}


FontLoader::~FontLoader()
{
    FT_Done_FreeType(*m_pFTLibrary);
    delete m_pAssetContainer;
}

bool FontLoader::load(const std::string& path, ushort size, gfx::Font* pOutFont)
{
    std::stringstream stream;
    stream << path << size;

    if (m_pAssetContainer->isAssetPresent(stream.str()) == false)
    {
        FT_Face* pFace(nullptr);
        bool error(FT_New_Face(*m_pFTLibrary, path.c_str(), 0, pFace));

        if (error == true)
        {
            HE_ERROR("Error loading font: %s", path);
            HE_ASSERT(false, "Error loading font!");

            return false;
        }
        else
        {
            FT_Set_Char_Size(*pFace, size << 6, size << 6, 96, 96); // font size in 1/64 pixel

            pOutFont = NEW gfx::Font(m_pFTLibrary, pFace, size);
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