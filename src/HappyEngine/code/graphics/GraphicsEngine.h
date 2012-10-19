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
//Extended:Sebastiaan Sprengers

#ifndef _HE_GRAPHICS_ENGINE_H_
#define _HE_GRAPHICS_ENGINE_H_
#pragma once

#include "SlotPContainer.h"

namespace Awesomium {
    class WebCore;
}

namespace sf {
namespace priv {
    class GlContext;
}
}

namespace he {
namespace gfx {

class Window;
class Scene;
class View;

enum ShadowResolution;

class GraphicsEngine
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    GraphicsEngine();
    virtual ~GraphicsEngine();

    /* GENERAL */
    void init();
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

    Window* createWindow();
    void removeWindow(Window* window);
    bool registerContext(GLContext* context);
    void unregisterContext(GLContext* context);

    /* SETTERS */
    void setActiveWindow(Window* window) { m_ActiveWindow = window; } // Internal use
    void setActiveContext(GLContext* context);
    void setActiveView(View* view) { m_ActiveView = view; } // Internal use
    
    /* GETTERS */
    Window* getActiveWindow() const { return m_ActiveWindow; }
    const std::vector<ObjectHandle>& getAllWindows() const { return m_Windows; }
    const std::vector<GLContext*>& getContexts() const { return m_Contexts; } 

    ushort getShadowMapSize(const ShadowResolution& resolution);

    View* getActiveView() const { return m_ActiveView; }

    Awesomium::WebCore* getWebCore() const { return m_WebCore; }

    // Events
    he::event1<void, GLContext*> ContextCreated;
    he::event1<void, GLContext*> ContextRemoved;

private:
    GLContext m_FallBackContext;
    sf::Context* m_FallBackSfContext;

    /* DATAMEMBERS */
    std::vector<ObjectHandle> m_Scenes;
    std::vector<ObjectHandle> m_Views;
    std::vector<ObjectHandle> m_Windows;

    Window* m_ActiveWindow;
    View* m_ActiveView;

    Awesomium::WebCore* m_WebCore;

    std::queue<uint> m_FreeContexts;
    std::vector<GLContext*> m_Contexts;

    /* DEFAULT COPY & ASSIGNMENT */
    GraphicsEngine(const GraphicsEngine&);
    GraphicsEngine& operator=(const GraphicsEngine&);
};

} } //end namespace

#endif
