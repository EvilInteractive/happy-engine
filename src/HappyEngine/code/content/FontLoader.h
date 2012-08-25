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

#ifndef _HE_FONT_LOADER_H_
#define _HE_FONT_LOADER_H_
#pragma once

#include "Font.h"
#include "AssetContainer.h"

#include "ft2build.h"
#include FT_FREETYPE_H

namespace he {
namespace ct {

class FontLoader
{
public:

    /* CONSTRUCTOR - DESTRUCTOR*/
    FontLoader();
    virtual ~FontLoader();

    /* GENERAL */
    gfx::Font* load(const std::string& path, ushort size);

private:

    /* DATAMEMBERS */
    AssetContainer<ObjectHandle> m_AssetContainer;

    FT_Library m_FTLibrary;

    /* DEFAULT COPY & ASSIGNMENT */
    FontLoader(const FontLoader&);
    FontLoader& operator=(const FontLoader&);
};

} } //end namespace

#endif
