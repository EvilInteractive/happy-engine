#include "StdAfx.h" 
#include "ContentManager.h"
#include "HappyNew.h"
#include "Vertex.h"

namespace he {
namespace ct {

ContentManager::ContentManager(): m_pModelLoader(NEW ModelLoader()), m_pTextureLoader(NEW TextureLoader()),
    m_pLineLoader(NEW LineLoader()), m_pPhysicsShapeLoader(NEW PhysicsShapeLoader()), m_pFontLoader(NEW FontLoader()),
    m_pShaderLoader(NEW ShaderLoader()), m_pEntityLoader(NEW EntityLoader()),

    m_ContentRootDir("../data/"),
    m_TextureFolder("textures/"), 
    m_ModelFolder("models/"), 
    m_LineFolder("lines/"), 
    m_PhysicsFolder("physics/"), 
    m_FontFolder("fonts/"),
    m_ShaderFolder("shaders/"), 
    m_EntityFolder("entities/")
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
    delete m_pEntityLoader;
}


void ContentManager::tick(float dTime) //checks for new load operations, if true start thread
{
    m_pModelLoader->tick(dTime);
    m_pTextureLoader->tick(dTime);
}
void ContentManager::glThreadInvoke()  //needed for all of the gl operations
{
    m_pModelLoader->glThreadInvoke();
    m_pTextureLoader->glThreadInvoke();
}

gfx::Model::pointer ContentManager::asyncLoadModel(const std::string& asset, const gfx::VertexLayout& vertexLayout)
{
    return m_pModelLoader->asyncLoadModel(m_ContentRootDir + m_ModelFolder + asset, vertexLayout);
}

gfx::Texture2D::pointer ContentManager::asyncLoadTexture(const std::string& asset)
{
    return m_pTextureLoader->asyncLoadTexture(m_ContentRootDir + m_TextureFolder + asset);
}
gfx::Texture2D::pointer ContentManager::asyncMakeTexture(const Color& color)
{
    return m_pTextureLoader->asyncMakeTexture(color);
}

gfx::Line::pointer ContentManager::loadLine(const std::string& asset)
{
    return m_pLineLoader->loadLine(m_ContentRootDir + m_LineFolder + asset);
}

const std::vector<px::shapes::IPhysicsShape::pointer>& ContentManager::loadPhysicsShape(const std::string& asset)
{
    return m_pPhysicsShapeLoader->load(m_ContentRootDir + m_PhysicsFolder + asset);
}

gfx::Font::pointer ContentManager::loadFont(const std::string& asset, ushort size, bool bold, bool italic)
{
    gfx::Font::pointer p;
    m_pFontLoader->load(m_ContentRootDir + m_FontFolder + asset, size, bold, italic, p);
    return p;
}

gfx::Shader::pointer ContentManager::loadShader(const std::string& vsAsset, const std::string& fsAsset, const gfx::ShaderLayout& shaderLayout, const std::vector<std::string>& outputs)
{
    return m_pShaderLoader->load(m_ContentRootDir + m_ShaderFolder + vsAsset, m_ContentRootDir + m_ShaderFolder + fsAsset, shaderLayout, outputs);
}


game::EntityDesc ContentManager::loadEntity(const std::string& asset)
{
    return m_pEntityLoader->load(m_ContentRootDir + m_EntityFolder + asset);
}


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
void ContentManager::setEntityFolder(const std::string& folder)
{
    m_EntityFolder = folder;
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
const std::string& ContentManager::getEntityFolder() const
{
    return m_EntityFolder;
}

gfx::ModelMesh::pointer ContentManager::getFullscreenQuad() const
{
    using namespace gfx;
    VertexLayout layout;
    layout.addElement(VertexElement(0, VertexElement::Type_Vector3, VertexElement::Usage_Position, 12, 0));

    gfx::ModelMesh::pointer pQuad(NEW gfx::ModelMesh("fullscreenQuad"));

    std::vector<VertexPos> vertices;
    vertices.push_back(VertexPos(vec3(-1, 1, 1.0f)));
    vertices.push_back(VertexPos(vec3(1, 1, 1.0f)));
    vertices.push_back(VertexPos(vec3(-1, -1, 1.0f)));
    vertices.push_back(VertexPos(vec3(1, -1, 1.0f)));

    std::vector<byte> indices;
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(1); indices.push_back(3); indices.push_back(2);

    pQuad->init();
    pQuad->setVertices(&vertices[0], 4, layout);
    pQuad->setIndices(&indices[0], 6, IndexStride_Byte);

    return pQuad;
}

} } //end namespace