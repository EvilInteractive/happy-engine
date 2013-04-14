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

namespace he {
    namespace tools {
        class FPSGraph;
        class MaterialGeneratorGraph;
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

namespace hs {
class EntityManager;
class SandboxRenderPipeline;

class Sandbox : public he::ge::Game, public he::Singleton<Sandbox>
{
public:
    Sandbox();
    ~Sandbox();

    void init();
    void destroy();
    void tick(float dTime);

    void quit();

    hs::EntityManager* getEntityManager() const { return m_EntityManager; }
    he::pl::IPlugin* getGamePlugin() const { return m_GamePlugin; }
    he::tools::MaterialGeneratorGraph* getMaterialGenerator() const { return m_MaterialGenerator; }
    he::gfx::Window* getMainWindow() const { return m_Window; }
    he::gfx::View* getMainView() const { return m_View; }
    hs::SandboxRenderPipeline* getRenderPipeline() { return m_RenderPipeline; }

    void setGamePlugin(he::pl::IPlugin* const plugin) { m_GamePlugin = plugin; }

private:
    
    he::gfx::Window* m_Window;
    he::gfx::View* m_View;
    SandboxRenderPipeline* m_RenderPipeline;        
    
    he::pl::IPlugin* m_GamePlugin;
    EntityManager* m_EntityManager;

    he::tools::MaterialGeneratorGraph* m_MaterialGenerator;

    bool m_IsExiting;

    //Disable default copy constructor and default assignment operator
    Sandbox(const Sandbox&);
    Sandbox& operator=(const Sandbox&);
};

} //end namespace

#endif
