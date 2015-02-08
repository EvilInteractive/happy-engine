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
//Created: 04/08/2011
//Changed: Sebastiaan Sprengers

#include "HappyPCH.h" 

#include "GraphicsEngine.h"

#include "Window.h"
#include "WindowSDL.h"
#include "Scene.h"
#include "View.h"
#include "ContentManager.h"

#include "Light.h"
#include "ShaderUniformBufferManager.h"

// warnings in awesomium lib
#ifdef USE_WEB
#pragma warning(disable:4100)
#include "Awesomium/WebCore.h"
#pragma warning(default:4100)
#endif
#include "GLContext.h"
#include "WebViewSurfaceFactory.h"

#include <SDL2/SDL.h>

namespace he {
namespace gfx {

GraphicsEngine::GraphicsEngine()
    : m_ActiveWindow(ObjectHandle::unassigned)
    , m_ActiveView(nullptr)
    , m_OwnSharedContext(false)
#ifdef USE_WEB
    , m_WebCore(nullptr)
    , m_WebViewSurfaceFactory(nullptr)
#endif
    , m_SharedContext(nullptr)
    , m_UBOManager(nullptr)
{
    for (uint32 i(0); i < MAX_OPENGL_CONTEXT; ++i)
    {
        m_FreeContexts.push(i);
    }
}


GraphicsEngine::~GraphicsEngine()
{
#ifdef USE_WEB
    HEDelete(m_WebViewSurfaceFactory);
#endif
}
void GraphicsEngine::destroy()
{
    CONTENT->destroy();
    HEDelete(m_UBOManager);
    m_UBOManager = nullptr;
    if (m_OwnSharedContext)
    {
        removeWindow(m_SharedContext);
        m_SharedContext = nullptr;
    }
    SDL_Quit();
    HE_ASSERT(ViewFactory::getInstance()->isEmpty(), "View leak detected!");
    HE_ASSERT(SceneFactory::getInstance()->isEmpty(), "Scene leak detected!");
    HE_ASSERT(WindowFactory::getInstance()->isEmpty(), "Window leak detected!");
}

void GraphicsEngine::init(const bool supportWindowing, Window* const sharedContext)
{
    using namespace err;
    
    uint32 sdlFlags(SDL_INIT_GAMECONTROLLER);
    if (supportWindowing)
        sdlFlags |= SDL_INIT_VIDEO;
    SDL_Init(sdlFlags);

    if (supportWindowing)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    #ifdef HE_DEBUG
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    #endif
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 0); // Dont share the first one
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

        if (sharedContext == nullptr)
        {
            m_OwnSharedContext = true;
            Window* context = createWindow();
            context->setFullscreen(false);
            context->setResizable(false);
            context->setVSync(false);
            context->setWindowDimension(0, 0);
            context->create(false);
            m_SharedContext = context;
        }

        SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1); // Share from this point
    }
    if (sharedContext)
    {
        m_OwnSharedContext = false;
        m_SharedContext = sharedContext;
    }

    HE_ASSERT(supportWindowing || sharedContext, "GraphicsEngine is enabled, but not windowing. This can only work if a sharedContext is provided, which is not\nFATAL");
    setActiveContext(m_SharedContext->getContext());
    
    m_UBOManager = HENew(ShaderUniformBufferManager)();
    m_UBOManager->init();

#ifdef USE_WEB
    m_WebViewSurfaceFactory = HENew(WebViewSurfaceFactory)();
    m_WebCore = Awesomium::WebCore::instance();
    m_WebCore->set_surface_factory(m_WebViewSurfaceFactory);
#endif
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

Window*GraphicsEngine::createWindow()
{
    HE_ASSERT(m_OwnSharedContext, "You should not try to create a window from the engine when you supplied you own sharedContext!");
    Window* window(HENew(WindowSDL)());
    registerWindow(window);
    return window;
}

void GraphicsEngine::removeWindow( Window* window )
{
    if (unregisterWindow(window))
    {
        window->destroy();
        HEDelete(window);
    }
}

void GraphicsEngine::registerWindow(Window* window)
{
    WindowFactory* factory(WindowFactory::getInstance());
    factory->registerObject(window);
    m_Windows.add(window->getHandle());
}

bool GraphicsEngine::unregisterWindow(Window* window)
{
    if (m_Windows.contains(window->getHandle()))
    {
        WindowFactory* factory(WindowFactory::getInstance());
        m_Windows.remove(window->getHandle());
        factory->unregisterObject(window->getHandle());

        return true;
    }
    return false;
}

void GraphicsEngine::draw()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);

    PROFILER_BEGIN("Scenes");
    SceneFactory* const sceneFactory(SceneFactory::getInstance());
    m_Scenes.forEach([sceneFactory](const ObjectHandle& sceneHandle)
    {
        Scene* const scene(sceneFactory->get(sceneHandle));
        if (scene->getActive())
            scene->prepareForRendering();
    });
    PROFILER_END();
    
    PROFILER_BEGIN("Windows");
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
                PROFILER_BEGIN("Views");
                ViewFactory* const viewFactory(ViewFactory::getInstance());
                viewList.forEach([_this, viewFactory](const ObjectHandle& viewHandle)
                {
                    View* const view(viewFactory->get(viewHandle));
                    _this->setActiveView(view);
                    view->draw();
                });
                PROFILER_END();
                window->finishRendering();
                window->present();
            }
        }
    });
    PROFILER_END();
}

void GraphicsEngine::tick( float /*dTime*/ )
{
#ifdef USE_WEB
    m_WebCore->Update();
#endif
}

bool GraphicsEngine::registerContext( GLContext* context )
{
    HE_IF_ASSERT(context->getID() == UINT32_MAX, "Context is already registered")
    if (m_FreeContexts.empty() == false)
    {
        setActiveContext(context);
        
        if (m_Contexts.size() == 0)
        {
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
        
        context->setID(m_FreeContexts.front());
        m_Contexts.add(context);
        m_FreeContexts.pop();
        
        ContextCreated(context);
        return true;
    }
    LOG(LogType_ProgrammerAssert, "No free contexts available");
    return false;
}

void GraphicsEngine::unregisterContext( GLContext* context )
{
    HE_IF_ASSERT(context->getID() != UINT32_MAX, "Context has not been registered or is already unregistered")
    {
        m_FreeContexts.push(context->getID());
        m_Contexts.remove(context);
        setActiveContext(context);
        ContextRemoved(context);
        context->setID(UINT32_MAX);
        if (m_SharedContext->getContext() != context)
        {
            setActiveContext(m_SharedContext->getContext());
        }
        else
        {
            // Destroying shared context! free resources
            destroy();
        }
    }
}

void GraphicsEngine::setActiveContext( GLContext* context )
{
    if (GL::s_CurrentContext != context)
    {
        GL::s_CurrentContext = context;
        context->makeCurrent();
    }
}

GLContext* GraphicsEngine::getActiveContext() const
{
    return GL::s_CurrentContext;
}

GLContext* GraphicsEngine::getSharedContext() const
{
    if (m_SharedContext)
    {
        return m_SharedContext->getContext();
    }
    return nullptr;
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

void GraphicsEngine::setActiveWindow( Window* window )
{
    m_ActiveWindow = window->getHandle();
}


} } //end namespace
