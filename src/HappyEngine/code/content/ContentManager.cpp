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

#include "HappyPCH.h" 

#include "ContentManager.h"

#include "Vertex.h"
#include "ModelMesh.h"

#include "TextureLoader.h"
#include "ModelLoader.h"
#include "LineLoader.h"
#include "PhysicsShapeLoader.h"
#include "MaterialLoader.h"
#include "LineLoader.h"
#include "FxLoader.h"
#include "FontLoader.h"
#include "ShaderLoader.h"

namespace he {
namespace ct {

ContentManager::ContentManager(): 
    m_pModelLoader(NEW ModelLoader()), 
    m_pTextureLoader(NEW TextureLoader()),
    m_pLineLoader(NEW LineLoader()), 
    m_pPhysicsShapeLoader(NEW PhysicsShapeLoader()), 
    m_pFontLoader(NEW FontLoader()),
    m_pShaderLoader(NEW ShaderLoader()), 
    m_pMaterialLoader(NEW MaterialLoader()), 
    m_pFxLoader(NEW FxLoader()),

    m_ContentRootDir("../data/"),
    m_TextureFolder("textures/"), 
    m_ModelFolder("models/"), 
    m_LineFolder("lines/"), 
    m_PhysicsFolder("physics/"), 
    m_FontFolder("fonts/"),
    m_ShaderFolder("shaders/"), 
    m_MaterialFolder("materials/"),
    m_FxFolder("fx/"),

    m_ParticleQuad(nullptr),
    m_FullscreenQuad(nullptr)
{
}

ContentManager::~ContentManager()
{
    delete m_pModelLoader;
    delete m_pTextureLoader;
    delete m_pLineLoader;
    delete m_pPhysicsShapeLoader;
    delete m_pFontLoader;
    delete m_pShaderLoader;
    delete m_pMaterialLoader;
    delete m_pFxLoader;

    if (m_ParticleQuad != nullptr)
        m_ParticleQuad->release();
    if (m_FullscreenQuad != nullptr)
        m_FullscreenQuad->release();
}


void ContentManager::tick(float dTime) //checks for new load operations, if true start thread
{
    PROFILER_BEGIN("Model Loader loop");
    m_pModelLoader->tick(dTime);
    PROFILER_END();

    PROFILER_BEGIN("Texture Loader loop");
    m_pTextureLoader->tick(dTime);
    PROFILER_END();
}
void ContentManager::glThreadInvoke()  //needed for all of the gl operations
{
    m_pModelLoader->glThreadInvoke();
    m_pTextureLoader->glThreadInvoke();
}

//////////////////////////////////////////////////////////////////////////
gfx::Model* ContentManager::asyncLoadModel(const std::string& asset, const gfx::BufferLayout& vertexLayout)
{
    return m_pModelLoader->asyncLoadModel(m_ContentRootDir + m_ModelFolder + asset, vertexLayout);
}
gfx::ModelMesh* ContentManager::asyncLoadModelMesh( const std::string& asset, const std::string& meshName, const gfx::BufferLayout& vertexLayout )
{
    return m_pModelLoader->asyncLoadModelMesh(m_ContentRootDir + m_ModelFolder + asset, meshName, vertexLayout);
}
gfx::Model* ContentManager::loadModel(const std::string& path, const gfx::BufferLayout& vertexLayout)
{
    return m_pModelLoader->loadModel(m_ContentRootDir + m_ModelFolder + path, vertexLayout);
}
gfx::ModelMesh* ContentManager::loadModelMesh(const std::string& path, const std::string& meshName, const gfx::BufferLayout& vertexLayout)
{
    return m_pModelLoader->loadModelMesh(m_ContentRootDir + m_ModelFolder + path, meshName, vertexLayout);
}

//////////////////////////////////////////////////////////////////////////
const gfx::Texture2D* ContentManager::asyncLoadTexture(const std::string& asset, bool storePixelsInTexture)
{
    return m_pTextureLoader->asyncLoadTexture(m_ContentRootDir + m_TextureFolder + asset, storePixelsInTexture);
}
const gfx::Texture2D* ContentManager::asyncMakeTexture(const Color& color)
{
    return m_pTextureLoader->asyncMakeTexture(color);
}
const gfx::Texture2D* ContentManager::loadTexture(const std::string& path)
{
    return m_pTextureLoader->loadTexture(m_ContentRootDir + m_TextureFolder + path);
}
const gfx::Texture2D* ContentManager::makeTexture(const Color& color)
{
    return m_pTextureLoader->makeTexture(color);
}

//////////////////////////////////////////////////////////////////////////
gfx::Line::pointer ContentManager::loadLine(const std::string& asset)
{
    return m_pLineLoader->loadLine(m_ContentRootDir + m_LineFolder + asset);
}

//////////////////////////////////////////////////////////////////////////
const std::vector<px::PhysicsConvexMesh::pointer>& ContentManager::loadPhysicsConvex(const std::string& asset)
{
    return m_pPhysicsShapeLoader->loadConvex(m_ContentRootDir + m_PhysicsFolder + asset);
}
const std::vector<px::PhysicsConcaveMesh::pointer>& ContentManager::loadPhysicsConcave(const std::string& asset)
{
    return m_pPhysicsShapeLoader->loadConcave(m_ContentRootDir + m_PhysicsFolder + asset);
}

//////////////////////////////////////////////////////////////////////////
gfx::Font::pointer ContentManager::loadFont(const std::string& asset, ushort size, bool bold, bool italic)
{
    gfx::Font::pointer p;
    m_pFontLoader->load(m_ContentRootDir + m_FontFolder + asset, size, bold, italic, p);
    return p;
}

const gfx::Font::pointer& ContentManager::getDefaultFont(ushort size)
{
    /*if (m_pDefaultFont == nullptr)
    {*/
    return m_pDefaultFont = loadFont("Ubuntu-Bold.ttf", size);
    //}
}

//////////////////////////////////////////////////////////////////////////
gfx::Shader::pointer ContentManager::loadShader(const std::string& vsAsset, const std::string& fsAsset, const gfx::ShaderLayout& shaderLayout, const std::vector<std::string>& outputs)
{
    return m_pShaderLoader->load(m_ContentRootDir + m_ShaderFolder + vsAsset, m_ContentRootDir + m_ShaderFolder + fsAsset, shaderLayout, outputs);
}

//////////////////////////////////////////////////////////////////////////
gfx::Material ContentManager::loadMaterial(const std::string& asset)
{
    return m_pMaterialLoader->load(m_ContentRootDir + m_MaterialFolder + asset);
}

//////////////////////////////////////////////////////////////////////////
uint ContentManager::loadFx( const std::string& path )
{
    return m_pFxLoader->load(m_ContentRootDir + m_FxFolder + path);
}

//////////////////////////////////////////////////////////////////////////
void ContentManager::setRootDir(const std::string& root)
{
    m_ContentRootDir = root;
}
void ContentManager::setTextureFolder(const std::string& folder)
{
    m_TextureFolder = folder;
}
void ContentManager::setModelFolder(const std::string& folder)
{
    m_ModelFolder = folder;
}
void ContentManager::setLineFolder(const std::string& folder)
{
    m_LineFolder = folder;
}
void ContentManager::setPhysicsFolder(const std::string& folder)
{
    m_PhysicsFolder = folder;
}
void ContentManager::setFontFolder(const std::string& folder)
{
    m_FontFolder = folder;
}
void ContentManager::setShaderFolder(const std::string& folder)
{
    m_ShaderFolder = folder;
}
void ContentManager::setMaterialFolder(const std::string& folder)
{
    m_MaterialFolder = folder;
}
void ContentManager::setFxFolder( const std::string& folder )
{
    m_FxFolder = folder;
}
const std::string& ContentManager::getRootDir() const
{
    return m_ContentRootDir;
}
const std::string& ContentManager::getTextureFolder() const
{
    return m_TextureFolder;
}
const std::string& ContentManager::getModelFolder() const
{
    return m_ModelFolder;
}
const std::string& ContentManager::getLineFolder() const
{
    return m_LineFolder;
}
const std::string& ContentManager::getPhysicsFolder() const
{
    return m_PhysicsFolder;
}
const std::string& ContentManager::getFontFolder() const
{
    return m_FontFolder;
}
const std::string& ContentManager::getShaderFolder() const
{
    return m_ShaderFolder;
}
const std::string& ContentManager::getMaterialFolder() const
{
    return m_MaterialFolder;
}
const std::string& ContentManager::getFxFolder() const
{
    return m_FxFolder;
}

//////////////////////////////////////////////////////////////////////////
gfx::ModelMesh* ContentManager::getFullscreenQuad()
{
    using namespace gfx;
    if (m_FullscreenQuad == nullptr)
    {
        BufferLayout layout;
        layout.addElement(BufferElement(0, BufferElement::Type_Vec3, BufferElement::Usage_Position, 12, 0));

        ObjectHandle handle(ResourceFactory<ModelMesh>::getInstance()->create());
        m_FullscreenQuad = ResourceFactory<ModelMesh>::getInstance()->get(handle);
        m_FullscreenQuad->setName("Full screen quad");

        std::vector<VertexPos> vertices;
        vertices.push_back(VertexPos(vec3(-1, 1, 1.0f)));
        vertices.push_back(VertexPos(vec3(1, 1, 1.0f)));
        vertices.push_back(VertexPos(vec3(-1, -1, 1.0f)));
        vertices.push_back(VertexPos(vec3(1, -1, 1.0f)));

        std::vector<byte> indices;
        indices.push_back(0); indices.push_back(1); indices.push_back(2);
        indices.push_back(1); indices.push_back(3); indices.push_back(2);

        m_FullscreenQuad->init();
        m_FullscreenQuad->setVertices(&vertices[0], 4, layout);
        m_FullscreenQuad->setIndices(&indices[0], 6, IndexStride_Byte);
    }

    ResourceFactory<ModelMesh>::getInstance()->instantiate(m_FullscreenQuad->getHandle());
    return m_FullscreenQuad;
}

void ContentManager::setRenderSettings( const gfx::RenderSettings& settings )
{
    m_pMaterialLoader->setRenderSettings(settings);
    m_pShaderLoader->setRenderSettings(settings);
}

bool ContentManager::isLoading() const
{
    if (m_pModelLoader->isLoading() || m_pTextureLoader->isLoading())
        return true;
    else
        return false;
}

//////////////////////////////////////////////////////////////////////////
///  Particle Quad
gfx::ModelMesh* ContentManager::getParticleQuad()
{
    if (m_ParticleQuad == nullptr)
    {    
        using namespace gfx;
        BufferLayout layout;
        layout.addElement(BufferElement(0, BufferElement::Type_Vec3, BufferElement::Usage_Position, 12, 0));

        std::vector<VertexPos> vertices;
        vertices.push_back(VertexPos(vec3(-1, 1, 0.0f)));
        vertices.push_back(VertexPos(vec3(1, 1, 0.0f)));
        vertices.push_back(VertexPos(vec3(-1, -1, 0.0f)));
        vertices.push_back(VertexPos(vec3(1, -1, 0.0f)));

        std::vector<byte> indices;
        indices.push_back(0); indices.push_back(1); indices.push_back(2);
        indices.push_back(1); indices.push_back(3); indices.push_back(2);

        m_ParticleQuad->init();
        m_ParticleQuad->setVertices(&vertices[0], 4, layout);
        m_ParticleQuad->setIndices(&indices[0], 6, IndexStride_Byte);
    }

    ResourceFactory<gfx::ModelMesh>::getInstance()->instantiate(m_ParticleQuad->getHandle());
    return m_ParticleQuad;
}
//////////////////////////////////////////////////////////////////////////


} } //end namespace