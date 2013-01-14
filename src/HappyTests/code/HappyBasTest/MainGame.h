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
#include "Awesomium\JSValue.h"

namespace he {
    namespace tools {
        class FPSGraph;
        class MaterialGeneratorGraph;
    }
    namespace ge {
        class Entity;
        class DefaultRenderPipeline;
    }
    namespace gfx {
        class Window;
        class Scene;
        class View;
        class ModelMesh;
        class CameraPerspective;
        class Texture2D;
        class SpotLight;
        class WebView;
        class WebListener;
    }
    namespace sfx {
        class Sound2D;
    }
    namespace gui {
        class Sprite;
    }
}

namespace ht {
class FlyCamera;
class Player;

class MainGame : public he::ge::Game, public he::gfx::IShapeDrawable, public he::gfx::IDrawable2D
{
public:
    MainGame();
    virtual ~MainGame();

    virtual void init();
    virtual void destroy() {}
    virtual void load();
    virtual void tick(float dTime);

    virtual void drawShapes(he::gfx::ShapeRenderer* renderer);
    virtual void draw2D(he::gfx::Canvas2D* renderer);

    he::gfx::Scene* getScene() const { return m_Scene; }

    void updateToneMapData(const Awesomium::JSArray& args);

private:
    he::tools::FPSGraph* m_FpsGraph;

    bool m_SpinShadows;

    he::gfx::Window* m_Window;
    he::gfx::View*   m_View;
    he::ge::DefaultRenderPipeline* m_RenderPipeline;
    he::gfx::Scene*  m_Scene;

    he::gfx::Window* m_Window2;
    he::ge::DefaultRenderPipeline* m_RenderPipeline2;
    he::gfx::View*   m_View2;

    std::vector<he::ge::Entity*> m_EntityList;
    
    const he::gfx::Texture2D* m_TestTexture;

    he::sfx::Sound2D* m_BackgroundSound;

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
    
    he::gui::Text m_DebugText;

    he::gfx::SpotLight* m_DebugSpotLight;

    Player* m_Player;

    he::tools::MaterialGeneratorGraph* m_MaterialGenerator;

    he::Color m_Colors[16];
    he::Color m_ShuffeledColor[8];
    float m_ColorTimer;
    he::gui::Text m_BigText;

    //gui
    he::gfx::WebView* m_ToneMapGui;
    he::gfx::WebListener* m_ToneMapGuiListener;

    he::gui::Sprite* m_TestSprite;

    //Disable default copy constructor and default assignment operator
    MainGame(const MainGame&);
    MainGame& operator=(const MainGame&);
};

} //end namespace

#endif
