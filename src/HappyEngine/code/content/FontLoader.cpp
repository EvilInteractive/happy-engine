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
    HE_ASSERT(error == false,"Error creating freetype library!");;
}

FontLoader::~FontLoader()
{
    ResourceFactory<gui::Font>::getInstance()->garbageCollect();
    FT_Done_FreeType(m_FTLibrary);
}

/* GENERAL */
gui::Font* FontLoader::load(const he::String& path, uint16 size, uint8 options)
{
    m_TempStream.clear();
    m_TempStream.str("");
    m_TempStream << path << size;
    he::String assetName(m_TempStream.str());

    ResourceFactory<gui::Font>* const fontFactory(FACTORY);

    if (m_AssetContainer.isAssetPresent(assetName) && fontFactory->isAlive(m_AssetContainer.getAsset(assetName)))
    {
        const ObjectHandle handle(m_AssetContainer.getAsset(assetName));
        fontFactory->instantiate(handle);       
        return fontFactory->get(handle);
    }
    else
    {
        const ObjectHandle handle(fontFactory->create());

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

        gui::Font* const font(fontFactory->get(handle));
        font->setName(assetName);
        font->init(m_FTLibrary, face, size, options);
        font->setLoaded(eLoadResult_Success);

        m_AssetContainer.addAsset(assetName, handle);

        return font;
    }
}

} } //end namespace
