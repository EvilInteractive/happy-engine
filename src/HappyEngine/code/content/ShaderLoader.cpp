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
//Author:  Bastian Damman
//Created: 30/09/2011
#include "HappyPCH.h" 

#include "ShaderLoader.h"
#include "HappyNew.h"
#include "IniReader.h"
#include "HappyEngine.h"
#include "BufferLayout.h"
#include "Shader.h"
#include "GlobalSettings.h"

namespace he {
namespace ct {

ShaderLoader::ShaderLoader()
{
}


ShaderLoader::~ShaderLoader()
{
    ResourceFactory<gfx::Shader>* factory(ResourceFactory<gfx::Shader>::getInstance());
    factory->garbageCollect();
}

ObjectHandle ShaderLoader::load(const he::String& vsPath, const he::String& fsPath, const gfx::ShaderLayout& shaderLayout, const he::ObjectList<he::String>& outputs)
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);

    ResourceFactory<gfx::Shader>* factory(ResourceFactory<gfx::Shader>::getInstance());

    he::String key(vsPath + fsPath);
    if (m_AssetContainer.isAssetPresent(key) && factory->isAlive(m_AssetContainer.getAsset(key)))
    {
        ObjectHandle shader(m_AssetContainer.getAsset(key));
        factory->instantiate(shader);
        return shader;
    }
    else
    {
        const gfx::RenderSettings& settings(GlobalSettings::getInstance()->getRenderSettings());
        gfx::Shader* shader(factory->get(factory->create()));
        std::set<he::String> defines;
        if (settings.lightingSettings.enableShadows)
            defines.insert("SHADOWS");
        if (settings.lightingSettings.enableSpecular)
            defines.insert("SPECULAR");
        if (settings.lightingSettings.enableNormalMap)
            defines.insert("NORMALMAP");

        shader->initFromFile(vsPath, fsPath, shaderLayout, defines, outputs);
        m_AssetContainer.addAsset(key, shader->getHandle());
        return shader->getHandle();
    }
}

} } //end namespace