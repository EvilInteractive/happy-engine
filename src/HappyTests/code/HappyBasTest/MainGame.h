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
//Author:  
//Created: //

#ifndef _HT_MainGame_H_
#define _HT_MainGame_H_
#pragma once

#include "Game.h"
#include "IShapeDrawable.h"
#include "IDrawable2D.h"
#include "Random.h"
#include "Text.h"


namespace he {
    namespace tools {
        class FPSGraph;
    }
    namespace ge {
        class Entity;
    }
    namespace gfx {
        class Window;
        class Scene;
        class View3D;
        class ModelMesh;
        class CameraPerspective;
        class Texture2D;
        class SpotLight;
    }
}

namespace ht {
class FlyCamera;

class MainGame : public he::ge::Game, public he::gfx::IShapeDrawable, public he::gfx::IDrawable2D
{
public:
    MainGame();
    virtual ~MainGame();

    virtual void init();
    virtual void load();
    virtual void tick(float dTime);

    virtual void drawShapes(he::gfx::ShapeRenderer* renderer);
    virtual void draw2D(he::gfx::Canvas2D* renderer);

private:
    void fillDebugMeshes();

    he::tools::FPSGraph* m_FpsGraph;

    bool m_SpinShadows;

    he::gfx::Window* m_Window;
    he::gfx::View3D*   m_View;
    he::gfx::Scene*  m_Scene;

    he::gfx::Window* m_Window2;
    he::gfx::View3D*   m_View2;

    std::vector<he::ge::Entity*> m_EntityList;

    he::gfx::ModelMesh* m_DebugMesh;

    const he::gfx::Texture2D* m_TestTexture;

    static he::Random s_Random;
    struct MovingEntityRandomness
    {
        he::vec3 a;
        he::vec3 b;
        he::vec3 c;
    };
    std::vector<MovingEntityRandomness> m_MovingEntityRandomness;
    std::vector<he::ge::Entity*> m_MovingEntityList;
    float m_MovingEntityFase;
    bool m_ShowDebugMesh;
    
    he::gui::Text m_DebugText;

    he::gfx::SpotLight* m_DebugSpotLight;

    //Disable default copy constructor and default assignment operator
    MainGame(const MainGame&);
    MainGame& operator=(const MainGame&);
};

} //end namespace

#endif
