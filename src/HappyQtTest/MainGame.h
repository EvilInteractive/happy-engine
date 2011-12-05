//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#ifndef _MAINGAME_H_
#define _MAINGAME_H_
#pragma once

#include "Game.h"
#include "Deferred3DRenderer.h"
#include "TestObject.h"
#include "Color.h"
#include "Simple2DEffect.h"
#include "Texture2D.h"
#include "Font.h"
#include "FontLoader.h"
#include "FlyCamera.h"
#include "FollowCamera.h"
#include "FPSGraph.h"
#include "Sound2D.h"
#include "Sound3D.h"

#include <vector>

#include "Light.h"
#include "Button.h"

#include "Entity.h"
#include "TextBox.h"
#include "Grid.h"

#include "PhysicsCarManager.h"

#include "Polygon2D.h"

#include "HappyFxEditorBinding.h"

#include "HappyQtWidget.h"

namespace happytest {

class MainGame : public he::gfx::HappyQtWidget
{
public:
    MainGame(/*qt::*/QWidget *parent = 0);
    virtual ~MainGame();

    virtual void init();
    virtual void load();
    virtual void tick(float dTime);
    virtual void draw();

private:
    happytest::FlyCamera* m_pFlyCamera;
    happytest::FollowCamera* m_pFollowCamera;
    he::gfx::Camera* m_pCurrentCamera;

    TestObject* m_pTestObject;

    he::Color m_BackgroundColors[5];
    he::byte m_BackgroundIndex;
    
    float m_DrawTimer;
    float m_UpdateTimer;

    bool m_bTest;
    bool m_bTest2;
    std::string m_Test3;
    
    std::vector<he::game::Entity*> m_Bullets;


    he::tools::FPSGraph* m_pFPSGraph;

    he::gfx::Texture2D::pointer m_TestImage;
    he::gfx::Texture2D::pointer m_SplashImage;
    he::gfx::Font::pointer m_pFont;

    he::game::Entity* m_pAxis;
    he::game::Entity* m_pScene;
    he::game::Entity* m_pSky;

    he::gfx::SpotLight::pointer m_pSpotLight;
    he::gfx::PointLight::pointer m_pCarLight;

    he::gui::Button* m_pTestButton;
    he::gui::Button* m_pTestButton2;
    he::gui::Button* m_pTestButton3;

    he::gui::TextBox* m_pTextBox;

    he::sfx::Sound2D* m_pTestSound2D;
    he::sfx::Sound3D* m_pTestSound3D;

    he::tools::Grid* m_pTestGrid;

    he::tools::HappyFxEditorBinding* m_pFxEditorBinding;

    he::vec3 m_PickPos;
    //bool m_bDrawPicking;

    he::px::DriveableSurfaceTyreFrictionTable* m_pFrictionTable;

    //Disable default copy constructor and default assignment operator
    MainGame(const MainGame&);
    MainGame& operator=(const MainGame&);
};

} //end namespace

#endif
