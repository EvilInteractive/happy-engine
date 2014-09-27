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
//Extended:Sebastiaan Sprengers

#ifndef _HE_GRAPHICS_ENGINE_H_
#define _HE_GRAPHICS_ENGINE_H_
#pragma once

#include "SlotPContainer.h"

#ifdef USE_WEB
namespace Awesomium {
    class WebCore;
}
#endif

namespace sf {
    class Context;
namespace priv {
    class GlContext;
}
}

namespace he {
namespace gfx {

class Window;
class Scene;
class View;
class GLContext;
#ifdef USE_WEB
class WebViewSurfaceFactory;
#endif
class ShaderUniformBufferManager;

ENUM(ShadowResolution, uint8);

class HAPPY_ENTRY GraphicsEngine
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    GraphicsEngine();
    virtual ~GraphicsEngine();

    /* GENERAL */
    void init(const bool supportWindowing, he::gfx::Window* const sharedContexts);
    void destroy();
    void tick(float dTime);
    void draw();


    // View depends on window and scene!
    // Always delete view first!
    Scene* createScene();
    Scene* getScene(SceneID id);
    void removeScene(Scene* scene);

    View* createView();
    void removeView(View* view);

    Window* createWindow(); // Only works if no sharedContext was provided in init
    void removeWindow(Window* window);
    void registerWindow(Window* window);
    bool unregisterWindow(Window* window);

    bool registerContext(GLContext* context);
    void unregisterContext(GLContext* context);

    /* SETTERS */
    void setActiveWindow(Window* window); // Internal use
    void setActiveContext(GLContext* context);
    void setActiveView(View* view) { m_ActiveView = view; } // Internal use

    GLContext* getSharedContext() const;
    
    /* GETTERS */
    ObjectHandle getActiveWindow() const { return m_ActiveWindow; }
    GLContext* getActiveContext() const;
    const he::ObjectList<ObjectHandle>& getAllWindows() const { return m_Windows; }
    const he::PrimitiveList<GLContext*>& getContexts() const { return m_Contexts; } 

    static uint16 getShadowMapSize(const ShadowResolution& resolution);

    View* getActiveView() const { return m_ActiveView; }
    ShaderUniformBufferManager* getShaderUniformBufferManager() const { return m_UBOManager; }
    
#ifdef USE_WEB
    Awesomium::WebCore* getWebCore() const { return m_WebCore; }
#endif

    // Events
    he::event1<void, GLContext*> ContextCreated;
    he::event1<void, GLContext*> ContextRemoved;

private:
    /* DATAMEMBERS */
    he::ObjectList<ObjectHandle> m_Scenes;
    he::ObjectList<ObjectHandle> m_Views;
    he::ObjectList<ObjectHandle> m_Windows;

    ObjectHandle m_ActiveWindow;
    View* m_ActiveView;
#ifdef USE_WEB
    Awesomium::WebCore* m_WebCore;
    WebViewSurfaceFactory* m_WebViewSurfaceFactory;
#endif

    bool m_OwnSharedContext;
    Window* m_SharedContext;
    std::queue<uint32> m_FreeContexts;
    he::PrimitiveList<GLContext*> m_Contexts;

    ShaderUniformBufferManager* m_UBOManager;


    /* DEFAULT COPY & ASSIGNMENT */
    GraphicsEngine(const GraphicsEngine&);
    GraphicsEngine& operator=(const GraphicsEngine&);
};

} } //end namespace

#endif
