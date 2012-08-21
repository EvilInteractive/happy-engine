\//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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

#include "Awesomium/WebCore.h"
#include "GLContext.h"


namespace he {
namespace gfx {

GraphicsEngine::GraphicsEngine(): m_ActiveWindow(nullptr), m_WebCore(nullptr), m_ActiveView(nullptr)
{
    for (uint i(0); i < MAX_OPENGL_CONTEXT; ++i)
    {
        m_FreeContexts.push(i);
    }
}


GraphicsEngine::~GraphicsEngine()
{
    std::for_each(m_Views.cbegin(), m_Views.cend(), [](View* view)
    {
        delete view;
    });
    std::for_each(m_Windows.cbegin(), m_Windows.cend(), [](Window* window)
    {
        delete window;
    });
    he::for_each(m_Scenes.cbegin(), m_Scenes.cend(), [](Scene* scene)
    {
        delete scene;
    });

    Awesomium::WebCore::Shutdown();
}
void GraphicsEngine::init()
{
    using namespace err;

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
    Scene* scene(NEW Scene());
    scene->setId(m_Scenes.insert(scene));
    return scene;
}

Scene* GraphicsEngine::getScene( SceneID id )
{
    return m_Scenes[id];
}

void GraphicsEngine::removeScene( Scene* scene )
{
    delete m_Scenes.remove(scene->getId());
}

View* GraphicsEngine::createView()
{
    View* view(NEW View());
    m_Views.push_back(view);
    return view;
}

void GraphicsEngine::removeView( View* view )
{
    HE_IF_ASSERT(std::find(m_Views.cbegin(), m_Views.cend(), view) != m_Views.cend(), "View does not exist in the view list")
    {
        *std::find(m_Views.begin(), m_Views.end(), view) = m_Views.back();
        m_Views.pop_back();
        delete view;
    }
}

Window* GraphicsEngine::createWindow()
{
    Window* window(NEW Window());
    m_Windows.push_back(window);
    return window;
}

void GraphicsEngine::removeWindow( Window* window )
{
    HE_IF_ASSERT(std::find(m_Windows.cbegin(), m_Windows.cend(), window) != m_Windows.cend(), "Window does not exist in the window list")
    {
        *std::find(m_Windows.begin(), m_Windows.end(), window) = m_Windows.back();
        m_Windows.pop_back();
        delete window;
    }
}

void GraphicsEngine::draw()
{
    std::for_each(m_Views.cbegin(), m_Views.cend(), [](View* view)
    {
        view->draw();
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
        m_Contexts.push_back(context);
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
        *std::find(m_Contexts.begin(), m_Contexts.end(), context) = m_Contexts.back();
        m_Contexts.pop_back();
        setActiveContext(context);
        ContextRemoved(context);
        context->id = UINT_MAX;
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

} } //end namespace
