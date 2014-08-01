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

he::gfx::Shader* ShaderLoader::load(const he::String& vsPath, const he::String& fsPath, const he::ObjectList<he::String>* const defines /*= nullptr*/, const he::ObjectList<he::String>* const outputLayout /*= nullptr*/)
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);

    ResourceFactory<gfx::Shader>* factory(ResourceFactory<gfx::Shader>::getInstance());

    int32 hash(0);
    he::hash(hash, vsPath);
    he::hash(hash, fsPath);
    if (defines)
    {
        defines->forEach([&hash](const he::String& str)
        {
            he::hash(hash, str);
        });
    }
    if (outputLayout)
    {
        outputLayout->forEach([&hash](const he::String& str)
        {
            he::hash(hash, str);
        });
    }
    
    if (m_AssetContainer.isAssetPresent(hash) && factory->isAlive(m_AssetContainer.getAsset(hash)))
    {
        ObjectHandle shaderHandle(m_AssetContainer.getAsset(hash));
        he::gfx::Shader* const shader(factory->get(shaderHandle));
        shader->instantiate();
        return shader;
    }
    else
    {
        const gfx::RenderSettings& settings(GlobalSettings::getInstance()->getRenderSettings());
        gfx::Shader* shader(factory->get(factory->create()));
        he::ObjectList<he::String> allDefines;
        if (defines)
            allDefines.append(*defines);
        if (settings.lightingSettings.enableShadows)
            allDefines.add("SHADOWS");
        if (settings.lightingSettings.enableSpecular)
            allDefines.add("SPECULAR");
        if (settings.lightingSettings.enableNormalMap)
            allDefines.add("NORMALMAP");
        if (settings.postSettings.shaderSettings.enableHDR)
            allDefines.add("HDR");
        if (settings.postSettings.shaderSettings.enableBloom)
            allDefines.add("BLOOM");
        if (settings.postSettings.shaderSettings.enableAO)
            allDefines.add("AO");
        if (settings.postSettings.shaderSettings.enableDepthEdgeDetect)
            allDefines.add("DEPTH_EDGE");
        if (settings.postSettings.shaderSettings.enableNormalEdgeDetect)
            allDefines.add("NORMAL_EDGE");
        if (settings.postSettings.shaderSettings.enableFog)
            allDefines.add("FOG");
        if (settings.postSettings.shaderSettings.enableVignette)
            allDefines.add("VIGNETTE");

        const bool result(shader->initFromFile(vsPath, fsPath, &allDefines, outputLayout));
        m_AssetContainer.addAsset(hash, shader->getHandle());
        shader->setLoaded(result? eLoadResult_Success : eLoadResult_Failed);

        return shader;
    }
}

} } //end namespace
