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

#ifndef _HE_CONTENT_MANAGER_H_
#define _HE_CONTENT_MANAGER_H_
#pragma once

#include "VertexLayout.h"
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

class HAPPY_ENTRY ContentManager
{
public:
    ContentManager();
    virtual ~ContentManager();
    
    void tick(float dTime);
    void glThreadInvoke();  //needed for all of the gl operations

    void destroy();

    gfx::Model* asyncLoadModel(const he::String& path);
    gfx::ModelMesh* asyncLoadModelMesh(const he::String& modelPath, const he::String& meshName);
    gfx::Model* loadModel(const he::String& path);
    gfx::ModelMesh* loadModelMesh(const he::String& modelPath, const he::String& meshName);
    gfx::ModelMesh* getFullscreenQuad();
    gfx::ModelMesh* getParticleQuad();
    gfx::ModelMesh* getCenteredUnitQuad();

    const gfx::Texture2D* asyncLoadTexture2D(const he::String& path);
    const gfx::TextureCube* asyncLoadTextureCube(const he::String& path);
    const gfx::Texture2D* asyncMakeTexture2D(const Color& color);
    const gfx::TextureCube* asyncMakeTextureCube(const Color& color);
    const gfx::Texture2D* loadTexture2D(const he::String& path);
    const gfx::TextureCube* loadTextureCube(const he::String& path);
    const gfx::Texture2D* makeTexture2D(const Color& color);
    const gfx::TextureCube* makeTextureCube(const Color& color);
    
    ObjectHandle loadPhysicsConvex(const he::String& path);
    ObjectHandle loadPhysicsConcave(const he::String& path);

    gui::Font* loadFont(const he::String& path, uint16 size, uint8 options = 0);
    gui::Font* getDefaultFont(uint16 size = 12);

    gfx::Shader* loadShader(const he::String& vsAsset, const he::String& geomAsset, const he::String& fsAsset,
        const he::ObjectList<he::String>* const defines = nullptr, const he::ObjectList<he::String>* const outputLayout = nullptr);
   
    gfx::Material* loadMaterial(const he::String& asset);

    void setTextureFolder(const he::String& folder);
    void setModelFolder(const he::String& folder);
    void setPhysicsFolder(const he::String& folder);
    void setFontFolder(const he::String& folder);
    void setShaderFolder(const he::String& folder);
    void setMaterialFolder(const he::String& folder);
    void setFxFolder(const he::String& folder);

    const Path& getContentDir() const;

    const Path& getTextureFolderPath() const;
    const Path& getModelFolderPath() const;
    const Path& getPhysicsFolderPath() const;
    const Path& getFontFolderPath() const;
    const Path& getShaderFolderPath() const;
    const Path& getMaterialFolderPath() const;
    const Path& getFxFolderPath() const;

    const he::String& getTextureFolder() const;
    const he::String& getModelFolder() const;
    const he::String& getPhysicsFolder() const;
    const he::String& getFontFolder() const;
    const he::String& getShaderFolder() const;
    const he::String& getMaterialFolder() const;
    const he::String& getFxFolder() const;
    
    bool isLoading() const;

private:
    void loadTick();

    ModelLoader* m_ModelLoader;
    TextureLoader* m_TextureLoader;
    PhysicsShapeLoader* m_PhysicsShapeLoader;
    FontLoader* m_FontLoader;
    ShaderLoader* m_ShaderLoader;
    MaterialLoader* m_MaterialLoader;

    Path m_ContentRootDir;
    he::String m_TextureFolder, m_ModelFolder, m_PhysicsFolder, m_FontFolder,
                m_ShaderFolder, m_MaterialFolder, m_FxFolder;
    Path m_TexturePath, m_ModelPath, m_PhysicsPath, m_FontPath,
                m_ShaderPath, m_MaterialPath, m_FxPath;
    
    gfx::ModelMesh* m_ParticleQuad;
    gfx::ModelMesh* m_FullscreenQuad;

    bool m_LoadThreadRunning;
    he::Thread* m_LoadThread;

    //Disable default copy constructor and default assignment operator
    ContentManager(const ContentManager&);
    ContentManager& operator=(const ContentManager&);
};

} } //end namespace

#endif
