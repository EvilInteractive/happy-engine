//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Author:  Sebastiaan Sprengers
//Created: 10/07/2012

#ifndef _HS_MainGame_H_
#define _HS_MainGame_H_
#pragma once

#include <Game.h>
#include <IDrawable2D.h>
#include <Singleton.h>

#include <qobject.h>
#include <qtimer.h>

namespace he {
    namespace tools {
        class FPSGraph;
    }
    namespace ge {
        class Entity;
    }
    namespace gfx {
        class Window;
        class View;
        class Scene;
    }
    namespace gui {
        class Canvas2D;
    }
    namespace pl {
        class IPlugin;
    }
}

class QGLWidget;

namespace hs {
class EntityManager;
class SandboxRenderPipeline;
class GLContextQT;
class MainWindow;

class Sandbox : public QObject, public he::ge::Game, public he::Singleton<Sandbox>
{
    Q_OBJECT
public:
    Sandbox();
    ~Sandbox();

    int run(int argc, char* args[]);

    void init();
    void destroy();
    void tick(float dTime);

    hs::EntityManager* getEntityManager() const { return m_EntityManager; }
    he::pl::IPlugin* getGamePlugin() const { return m_GamePlugin; }
    he::gfx::Window* getMainWindow() const;
    he::gfx::View* getMainView() const { return m_View; }
    hs::SandboxRenderPipeline* getRenderPipeline() { return m_RenderPipeline; }

    void setGamePlugin(he::pl::IPlugin* const plugin) { m_GamePlugin = plugin; }

    QGLWidget* getSharedWidget() const;

public slots:
    void quit();

private slots:
    void loop();

private:

    MainWindow* m_Window;
    he::gfx::View* m_View;
    SandboxRenderPipeline* m_RenderPipeline;        
    
    he::pl::IPlugin* m_GamePlugin;
    EntityManager* m_EntityManager;

    QTimer m_QtLoopTimer;

    bool m_IsExiting;

    //Disable default copy constructor and default assignment operator
    Sandbox(const Sandbox&);
    Sandbox& operator=(const Sandbox&);
};

} //end namespace

#endif
