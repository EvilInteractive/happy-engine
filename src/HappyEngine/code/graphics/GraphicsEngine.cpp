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
#pragma warning(default:4100)
#include "GLContext.h"
#include <SFML/Window.hpp>

namespace he {
namespace gfx {

GraphicsEngine::GraphicsEngine(): m_ActiveWindow(nullptr), m_WebCore(nullptr), m_ActiveView(nullptr),
    m_DefaultContext(nullptr), m_DefaultSfContext(nullptr)
{
    for (uint32 i(0); i < MAX_OPENGL_CONTEXT; ++i)
    {
        m_FreeContexts.push(i);
    }
}


GraphicsEngine::~GraphicsEngine()
{
    delete m_DefaultSfContext;
}
void GraphicsEngine::destroy()
{
    HE_ASSERT(ViewFactory::getInstance()->isEmpty(), "View leak detected!");
    HE_ASSERT(SceneFactory::getInstance()->isEmpty(), "Scene leak detected!");
    HE_ASSERT(WindowFactory::getInstance()->isEmpty(), "Window leak detected!");
}

void GraphicsEngine::init()
{
    using namespace err;
    
    m_DefaultSfContext = NEW sf::Context();
    registerContext(&m_DefaultContext);
    GL::init();

    m_WebCore = Awesomium::WebCore::instance();
        
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
    LOG(LogType_ProgrammerAssert, "not implemented");
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

View* GraphicsEngine::createView()
{
    ViewFactory* factory(ViewFactory::getInstance());
    View* view(static_cast<View*>(factory->get(factory->create())));
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
    SceneFactory* const sceneFactory(SceneFactory::getInstance());
    m_Scenes.forEach([sceneFactory](const ObjectHandle& sceneHandle)
    {
        Scene* const scene(sceneFactory->get(sceneHandle));
        if (scene->getActive())
            scene->prepareForRendering();
    });
    WindowFactory* const windowFactory(WindowFactory::getInstance());
    m_Windows.forEach([this, windowFactory](const ObjectHandle& windowHandle)
    {
        Window* const window(windowFactory->get(windowHandle));
        if (window->isOpen())
        {
            const ObjectList<ObjectHandle>& viewList(window->getViews());
            if (viewList.empty() == false)
            {
                window->prepareForRendering();
                GraphicsEngine* const _this(this);
                ViewFactory* const viewFactory(ViewFactory::getInstance());
                viewList.forEach([_this, viewFactory](const ObjectHandle& viewHandle)
                {
                    View* const view(viewFactory->get(viewHandle));
                    _this->setActiveView(view);
                    view->draw();
                });

                window->present();
            }
        }
    });
}

void GraphicsEngine::tick( float /*dTime*/ )
{
    m_WebCore->Update();
}

bool GraphicsEngine::registerContext( GLContext* context )
{
    HE_IF_ASSERT(context->id == UINT32_MAX, "Context is already registered")
    if (m_FreeContexts.empty() == false)
    {
        context->id = m_FreeContexts.front();
        m_Contexts.add(context);
        m_FreeContexts.pop();
        setActiveContext(context);
        ContextCreated(context);
        return true;
    }
    LOG(LogType_ProgrammerAssert, "No free contexts available");
    return false;
}

void GraphicsEngine::unregisterContext( GLContext* context )
{
    HE_IF_ASSERT(context->id != UINT32_MAX, "Context has not been registered or is already unregistered")
    {
        m_FreeContexts.push(context->id);
        m_Contexts.remove(context);
        setActiveContext(context);
        ContextRemoved(context);
        context->window->m_Window->setActive(false);
        context->id = UINT32_MAX;
        setActiveContext(&m_DefaultContext);
    }
}

void GraphicsEngine::setActiveContext( GLContext* context )
{
    if (GL::s_CurrentContext != context)
    {
        if (context == &m_DefaultContext)
            m_DefaultSfContext->setActive(true);
        else
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
    LOG(LogType_ProgrammerAssert, "Unknown / Invalid shadow resolution");
    return 0;
}

} } //end namespace
