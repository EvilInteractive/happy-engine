#include "ContentManager.h"
#include "HappyNew.h"

namespace happyengine {
namespace content {

ContentManager::ContentManager(): m_pModelLoader(NEW ModelLoader()), m_pTextureLoader(NEW TextureLoader()),
    m_pLineLoader(NEW LineLoader()), m_pPhysicsShapeLoader(NEW PhysicsShapeLoader()), m_pFontLoader(NEW FontLoader())
{
}

ContentManager::~ContentManager()
{
    delete m_pModelLoader;
    delete m_pTextureLoader;
    delete m_pLineLoader;
    delete m_pPhysicsShapeLoader;
    delete m_pFontLoader;
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

graphics::Model::pointer ContentManager::asyncLoadModel(const std::string& path, const graphics::VertexLayout& vertexLayout)
{
    return m_pModelLoader->asyncLoadModel(path, vertexLayout);
}

graphics::Texture2D::pointer ContentManager::asyncLoadTexture(const std::string& path)
{
    return m_pTextureLoader->asyncLoadTexture(path);
}

graphics::Line::pointer ContentManager::loadLine(const std::string& path)
{
    return m_pLineLoader->loadLine(path);
}

const std::vector<physics::shapes::IPhysicsShape::pointer>& ContentManager::loadPhysicsShape(const std::string& path)
{
    return m_pPhysicsShapeLoader->load(path);
}

graphics::Font::pointer ContentManager::loadFont(const std::string& path, ushort size)
{
    graphics::Font::pointer p;
    m_pFontLoader->load(path, size, p);
    return p;
}

} } //end namespace