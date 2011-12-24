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
//Created: 30/09/2011

#ifndef _HE_SHADER_LOADER_H_
#define _HE_SHADER_LOADER_H_
#pragma once

#include "AssetContainer.h"
#include "Shader.h"
#include "RenderSettings.h"

namespace he {
namespace ct {

class ShaderLoader
{
public:
    ShaderLoader();
    virtual ~ShaderLoader();
    
    gfx::Shader::pointer load(const std::string& vsPath, const std::string& fsPath, const gfx::ShaderLayout& shaderLayout, const std::vector<std::string>& outputs);

    void setRenderSettings(const gfx::RenderSettings& settings);

private:
    AssetContainer<gfx::Shader::pointer>* m_pAssetContainer;

    gfx::RenderSettings m_RenderSettings;

    //Disable default copy constructor and default assignment operator
    ShaderLoader(const ShaderLoader&);
    ShaderLoader& operator=(const ShaderLoader&);
};

} } //end namespace

#endif
