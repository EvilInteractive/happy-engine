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
//Created: 30/09/2011

#ifndef _HE_MATERIAL_LOADER_H_
#define _HE_MATERIAL_LOADER_H_
#pragma once

#include "AssetContainer.h"
#include "RenderSettings.h"

namespace he {
namespace ct {

class MaterialLoader
{
public:
    MaterialLoader();
    virtual ~MaterialLoader();
    
    ObjectHandle load(const std::string& path);

    void setRenderSettings(const gfx::RenderSettings& settings);

private:
    AssetContainer<ObjectHandle> m_AssetContainer;

    gfx::RenderSettings m_RenderSettings;

    //Disable default copy constructor and default assignment operator
    MaterialLoader(const MaterialLoader&);
    MaterialLoader& operator=(const MaterialLoader&);
};

} } //end namespace

#endif
