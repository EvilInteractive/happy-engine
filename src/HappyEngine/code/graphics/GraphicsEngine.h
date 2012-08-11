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

namespace he {
namespace gfx {

class Window;
class Scene;
class View;

class GraphicsEngine
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    GraphicsEngine();
    virtual ~GraphicsEngine();

    /* GENERAL */
    void init();
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

    /* SETTERS */
    void setActiveWindow(Window* window) { m_ActiveWindow = window; } // Internal use
    void setActiveView(View* view) { m_ActiveView = view; } // Internal use
    
    /* GETTERS */
    Window* getActiveWindow() const { return m_ActiveWindow; }
    const std::vector<Window*>& getAllWindows() const { return m_Windows; }

    View* getActiveView() const { return m_ActiveView; }

    Awesomium::WebCore* getWebCore() const { return m_WebCore; }

private:

    /* DATAMEMBERS */
    SlotPContainer<Scene*>  m_Scenes;
    std::vector<View*>   m_Views;
    std::vector<Window*> m_Windows;

    Window* m_ActiveWindow;
    View* m_ActiveView;

    Awesomium::WebCore* m_WebCore;

    /* DEFAULT COPY & ASSIGNMENT */
    GraphicsEngine(const GraphicsEngine&);
    GraphicsEngine& operator=(const GraphicsEngine&);
};

} } //end namespace

#endif
