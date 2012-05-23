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
#include "HappyPCH.h" 

#include "ShaderLoader.h"
#include "HappyNew.h"
#include "IniReader.h"
#include "HappyEngine.h"
#include "BufferLayout.h"

namespace he {
namespace ct {

ShaderLoader::ShaderLoader(): m_pAssetContainer(NEW AssetContainer<gfx::Shader::pointer>())
{
}


ShaderLoader::~ShaderLoader()
{
    delete m_pAssetContainer;
}

gfx::Shader::pointer ShaderLoader::load(const std::string& vsPath, const std::string& fsPath, const gfx::ShaderLayout& shaderLayout, const std::vector<std::string>& outputs)
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    std::string key(vsPath+fsPath);
    if (m_pAssetContainer->isAssetPresent(key))
    {
        return m_pAssetContainer->getAsset(key);
    }
    else
    {
        gfx::Shader::pointer pShader(NEW gfx::Shader());
        std::set<std::string> defines;
        if (m_RenderSettings.enableShadows)
            defines.insert("SHADOWS");
        if (m_RenderSettings.enableSpecular)
            defines.insert("SPECULAR");
        if (m_RenderSettings.enableNormalMap)
            defines.insert("NORMALMAP");

        pShader->initFromFile(vsPath, fsPath, shaderLayout, defines, outputs);
        m_pAssetContainer->addAsset(key, pShader);
        return pShader;
    }
}

void ShaderLoader::setRenderSettings( const gfx::RenderSettings& settings )
{
    m_RenderSettings = settings;
}

} } //end namespace