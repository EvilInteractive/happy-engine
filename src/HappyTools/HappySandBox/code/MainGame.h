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

#include "Game.h"
#include "IDrawable2D.h"

namespace he {
    namespace tools {
        class FPSGraph;
    }
    namespace ge {
        class DefaultRenderPipeline;
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
}

namespace hs {
    class UIController;
    class UIBind;

class MainGame : public he::ge::Game, public he::gfx::IDrawable2D
{
public:
    MainGame();
    virtual ~MainGame();

    virtual void init();
    virtual void destroy() {}
    virtual void load();
    virtual void tick(float dTime);
    virtual void draw2D(he::gui::Canvas2D* canvas);

private:

    he::tools::FPSGraph* m_FPSGraph;

    he::gfx::Window* m_Window;
    he::gfx::Scene*  m_Scene;
    he::gfx::View* m_View;
    he::ge::DefaultRenderPipeline* m_RenderPipeline;

    UIController* m_UIController;
    UIBind* m_UIBind;

    he::ge::Entity* m_TestScene;

    //Disable default copy constructor and default assignment operator
    MainGame(const MainGame&);
    MainGame& operator=(const MainGame&);
};

} //end namespace

#endif
