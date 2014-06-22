//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

#include "FontLoader.h"

#include "ResourceFactory.h"

#define FACTORY ResourceFactory<gui::Font>::getInstance()

namespace he {
namespace ct {

/* CONSTRUCTOR - DESTRUCTOR*/
FontLoader::FontLoader()
{
    const FT_Error error(FT_Init_FreeType(&m_FTLibrary));
    HE_ASSERT(error == false,"Error creating freetype library!"); error;
}

FontLoader::~FontLoader()
{
    ResourceFactory<gui::Font>::getInstance()->garbageCollect();
    FT_Done_FreeType(m_FTLibrary);
}

/* GENERAL */
gui::Font* FontLoader::load(const he::String& path, uint16 size, uint8 options)
{
    std::stringstream stream;
    stream << path << size;

    if (m_AssetContainer.isAssetPresent(stream.str()) && FACTORY->isAlive(m_AssetContainer.getAsset(stream.str())))
    {
        ObjectHandle handle(m_AssetContainer.getAsset(stream.str()));
        FACTORY->instantiate(handle);       
        return FACTORY->get(handle);
    }
    else
    {
        ObjectHandle handle(FACTORY->create());

        FT_Face face;
        FT_Error error(FT_New_Face(m_FTLibrary, path.c_str(), 0, &face));

        if (error != 0)
        {
            LOG(LogType_ProgrammerAssert, "Error loading font: %s", path.c_str());
            return nullptr;
        }

        error = FT_Select_Charmap(face, FT_ENCODING_UNICODE);

        if (error != 0)
        {
            LOG(LogType_ProgrammerAssert, "Error loading font charmap: %s", path.c_str());
            return nullptr;
        }

        // font size in 1/64 pixel
        // 96 DPI -> should change later to adapt to screen DPI
        error = FT_Set_Char_Size(face, 0, size * 64, 96, 96);

        if (error != 0)
        {
            LOG(LogType_ProgrammerAssert, "Error setting font size: %s", path.c_str());
            return nullptr;
        }

        gui::Font* pFont = FACTORY->get(handle);
        pFont->setName(stream.str());
        pFont->init(m_FTLibrary, face, size, options);

        m_AssetContainer.addAsset(stream.str(), handle);

        return pFont;
    }
}

} } //end namespace