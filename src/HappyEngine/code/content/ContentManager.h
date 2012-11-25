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

#ifndef _HE_CONTENT_MANAGER_H_
#define _HE_CONTENT_MANAGER_H_
#pragma once

#include "BufferLayout.h"
#include "RenderSettings.h"
#include "Material.h"

namespace he {
namespace gfx {
    class Model;
    class ModelMesh;
    class Texture2D;
    class TextureCube;
    class ShaderLayout;
}

namespace ct {
class ModelLoader;
class TextureLoader;
class PhysicsShapeLoader;
class FontLoader;
class ShaderLoader;
class MaterialLoader;

class ContentManager
{
public:
    ContentManager();
    virtual ~ContentManager();
    
    void tick(float dTime); //checks for new load operations, if true start thread
    void glThreadInvoke();  //needed for all of the gl operations

    gfx::Model* asyncLoadModel(const std::string& path, const gfx::BufferLayout& vertexLayout);
    gfx::ModelMesh* asyncLoadModelMesh(const std::string& modelPath, const std::string& meshName, const gfx::BufferLayout& vertexLayout);
    gfx::Model* loadModel(const std::string& path, const gfx::BufferLayout& vertexLayout);
    gfx::ModelMesh* loadModelMesh(const std::string& modelPath, const std::string& meshName, const gfx::BufferLayout& vertexLayout);
    gfx::ModelMesh* getFullscreenQuad();
    gfx::ModelMesh* getParticleQuad();

    const gfx::Texture2D* asyncLoadTexture2D(const std::string& path);
    const gfx::TextureCube* asyncLoadTextureCube(const std::string& path);
    const gfx::Texture2D* asyncMakeTexture2D(const Color& color);
    const gfx::Texture2D* loadTexture2D(const std::string& path);
    const gfx::TextureCube* loadTextureCube(const std::string& path);
    const gfx::Texture2D* makeTexture2D(const Color& color);
    
    ObjectHandle loadPhysicsConvex(const std::string& path);
    ObjectHandle loadPhysicsConcave(const std::string& path);

    gfx::Font* loadFont(const std::string& path, uint16 size, bool compress = true);
    gfx::Font* getDefaultFont(uint16 size = 12);

    ObjectHandle loadShader(const std::string& vsPath, const std::string& fsPath, const gfx::ShaderLayout& shaderLayout, const he::ObjectList<std::string>& outputs);
   
    ObjectHandle loadMaterial(const std::string& path);

    void setContentDir(const Path& path);
    void setTextureFolder(const std::string& folder);
    void setModelFolder(const std::string& folder);
    void setPhysicsFolder(const std::string& folder);
    void setFontFolder(const std::string& folder);
    void setShaderFolder(const std::string& folder);
    void setMaterialFolder(const std::string& folder);
    void setFxFolder(const std::string& folder);

    const Path& getContentDir() const;

    const Path& getTextureFolderPath() const;
    const Path& getModelFolderPath() const;
    const Path& getPhysicsFolderPath() const;
    const Path& getFontFolderPath() const;
    const Path& getShaderFolderPath() const;
    const Path& getMaterialFolderPath() const;
    const Path& getFxFolderPath() const;

    const std::string& getTextureFolder() const;
    const std::string& getModelFolder() const;
    const std::string& getPhysicsFolder() const;
    const std::string& getFontFolder() const;
    const std::string& getShaderFolder() const;
    const std::string& getMaterialFolder() const;
    const std::string& getFxFolder() const;

    void setRenderSettings(const gfx::RenderSettings& settings);

    bool isLoading() const;

private:

    ModelLoader* m_ModelLoader;
    TextureLoader* m_TextureLoader;
    PhysicsShapeLoader* m_PhysicsShapeLoader;
    FontLoader* m_FontLoader;
    ShaderLoader* m_ShaderLoader;
    MaterialLoader* m_MaterialLoader;

    Path m_ContentRootDir;
    std::string m_TextureFolder, m_ModelFolder, m_PhysicsFolder, m_FontFolder,
                m_ShaderFolder, m_MaterialFolder, m_FxFolder;
    Path m_TexturePath, m_ModelPath, m_PhysicsPath, m_FontPath,
                m_ShaderPath, m_MaterialPath, m_FxPath;
    
    gfx::ModelMesh* m_ParticleQuad;
    gfx::ModelMesh* m_FullscreenQuad;

    //Disable default copy constructor and default assignment operator
    ContentManager(const ContentManager&);
    ContentManager& operator=(const ContentManager&);
};

} } //end namespace

#endif
