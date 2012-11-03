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
//Created: 04/08/2011
//Changed: Sebastiaan Sprengers

#include "HappyPCH.h" 

#include "GraphicsEngine.h"

#include "Window.h"
#include "Scene.h"
#include "View.h"

#include "Light.h"

// warnings in awesomium lib
#pragma warning(disable:4100)
#include "Awesomium/WebCore.h"
#include "GLContext.h"
// warnings in awesomium lib
#pragma warning(default:4100)

namespace he {
namespace gfx {

GraphicsEngine::GraphicsEngine(): m_ActiveWindow(nullptr), m_WebCore(nullptr), m_ActiveView(nullptr),
    m_FallBackContext(nullptr), m_FallBackSfContext(nullptr)
{
    for (uint32 i(0); i < MAX_OPENGL_CONTEXT; ++i)
    {
        m_FreeContexts.push(i);
    }
}


GraphicsEngine::~GraphicsEngine()
{
    delete m_FallBackSfContext;
}
void GraphicsEngine::destroy()
{
    ViewFactory::getInstance()->destroyAll();
    SceneFactory::getInstance()->destroyAll();
    WindowFactory::getInstance()->destroyAll();

    if (m_WebCore != nullptr)
        Awesomium::WebCore::Shutdown();
}

void GraphicsEngine::init()
{
    using namespace err;
    
    m_FallBackSfContext = NEW sf::Context();
    m_FallBackSfContext->setActive(true);
    GL::s_CurrentContext = &m_FallBackContext;
    GL::init();

    m_WebCore = Awesomium::WebCore::Initialize(Awesomium::WebConfig());
        
    HE_INFO((char*)glGetString(GL_VENDOR));
    HE_INFO((char*)glGetString(GL_RENDERER));
    HE_INFO((char*)glGetString(GL_VERSION));
    HE_INFO((char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    HE_INFO("GL version %d.%d", major, minor);

    int doubleBuff;
    glGetIntegerv(GL_DOUBLEBUFFER, &doubleBuff);
    HE_INFO("Doubly buffered: %s", (doubleBuff == GL_TRUE)?"TRUE":"FALSE");

    int maxTexSize, maxRenderSize, maxRectSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderSize);
    glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE, &maxRectSize);
    HE_INFO("Max texture size: %d", maxTexSize);
    HE_INFO("Max render size: %d", maxRenderSize);
    HE_INFO("Max rect tex size: %d", maxRectSize);

    HE_INFO("Max anisotropic filtering support: %.1fx", GL::getMaxAnisotropicFilteringSupport());
}

Scene* GraphicsEngine::createScene()
{
    SceneFactory* factory(SceneFactory::getInstance());
    Scene* scene(factory->get(factory->create()));
    m_Scenes.add(scene->getHandle());
    return scene;
}

Scene* GraphicsEngine::getScene( SceneID /*id*/ )
{
    HE_ASSERT(false, "not implemented");
    return nullptr;
}

void GraphicsEngine::removeScene( Scene* scene )
{
    HE_IF_ASSERT(m_Scenes.contains(scene->getHandle()), "Scene does not exist in the scene list")
    {
        SceneFactory* factory(SceneFactory::getInstance());
        m_Scenes.remove(scene->getHandle());
        factory->destroyObject(scene->getHandle());
    }
}

View2D* GraphicsEngine::createView2D()
{
    ViewFactory* factory(ViewFactory::getInstance());
    View2D* view(static_cast<View2D*>(factory->get(factory->createView2D())));
    m_Views.add(view->getHandle());
    return view;
}

View3D* GraphicsEngine::createView3D()
{
    ViewFactory* factory(ViewFactory::getInstance());
    View3D* view(static_cast<View3D*>(factory->get(factory->createView3D())));
    m_Views.add(view->getHandle());
    return view;
}

void GraphicsEngine::removeView( View* view )
{
    HE_IF_ASSERT(m_Views.contains(view->getHandle()), "View does not exist in the view list")
    {
        ViewFactory* factory(ViewFactory::getInstance());
        m_Views.remove(view->getHandle());
        factory->destroyObject(view->getHandle());
    }
}

Window* GraphicsEngine::createWindow()
{
    WindowFactory* factory(WindowFactory::getInstance());
    Window* window(factory->get(factory->create()));
    m_Windows.add(window->getHandle());
    return window;
}

void GraphicsEngine::removeWindow( Window* window )
{
    HE_IF_ASSERT(m_Windows.contains(window->getHandle()), "Window does not exist in the window list")
    {
        WindowFactory* factory(WindowFactory::getInstance());
        m_Windows.remove(window->getHandle());
        factory->destroyObject(window->getHandle());
    }
}

void GraphicsEngine::draw()
{
    ViewFactory* factory(ViewFactory::getInstance());
    m_Views.forEach([factory](const ObjectHandle& view)
    {
        factory->get(view)->draw();
    });
}

void GraphicsEngine::tick( float /*dTime*/ )
{
    m_WebCore->Update();
}

bool GraphicsEngine::registerContext( GLContext* context )
{
    HE_IF_ASSERT(context->id == UINT_MAX, "Context is already registered")
    if (m_FreeContexts.empty() == false)
    {
        context->id = m_FreeContexts.front();
        m_Contexts.add(context);
        m_FreeContexts.pop();
        setActiveContext(context);
        ContextCreated(context);
        return true;
    }
    HE_ASSERT(false, "No free contexts available");
    return false;
}

void GraphicsEngine::unregisterContext( GLContext* context )
{
    HE_IF_ASSERT(context->id != UINT_MAX, "Context has not been registered or is already unregistered")
    {
        m_FreeContexts.push(context->id);
        m_Contexts.remove(context);
        setActiveContext(context);
        ContextRemoved(context);
        context->window->m_Window->setActive(false);
        context->id = UINT_MAX;
        m_FallBackSfContext->setActive(true);
        GL::s_CurrentContext = &m_FallBackContext;
    }
}

void GraphicsEngine::setActiveContext( GLContext* context )
{
    if (GL::s_CurrentContext != context)
    {
        context->window->m_Window->setActive(true);
        GL::s_CurrentContext = context;
    }
}

he::uint16 GraphicsEngine::getShadowMapSize( const ShadowResolution& resolution )
{
    switch (resolution)
    {
    case ShadowResolution_32: return 32;
    case ShadowResolution_64: return 64;
    case ShadowResolution_128: return 128;
    case ShadowResolution_256: return 256;
    case ShadowResolution_512: return 512;
    case ShadowResolution_1024: return 1024;
    }
    HE_ASSERT(false, "Unknown / Invalid shadow resolution");
    return 0;
}

} } //end namespace
