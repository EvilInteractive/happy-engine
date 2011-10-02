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

#include "IGame.h"
#include "Deferred3DRenderer.h"
#include "TestObject.h"
#include "Color.h"
#include "TestBullet.h"
#include "Simple2DEffect.h"
#include "Texture2D.h"
#include "Font.h"
#include "FontLoader.h"
#include "FlyCamera.h"
#include "FPSGraph.h"
#include "GroundPlane.h"

#include <vector>

#include "MyServer.h"
#include "MyClient.h"
#include "GameTimer.h"

#include "Light.h"

#include "Entity.h"


namespace happytest {

class MainGame : public happyengine::IGame
{
public:
	MainGame();
    virtual ~MainGame();

    virtual void init();
    virtual void load();
    virtual void tick(float dTime);
    virtual void draw(float dTime);

private:
	happytest::FlyCamera* m_pCamera;

    TestObject* m_pTestObject;

    happyengine::Color m_BackgroundColors[5];
    happyengine::byte m_BackgroundIndex;
    
    float m_DrawTimer;
    float m_UpdateTimer;
    
    std::vector<TestBullet*> m_Bullets;

    MyServer* m_pServer;
    MyClient* m_pClient;

	happyengine::tools::FPSGraph* m_pFPSGraph;

	happyengine::graphics::Texture2D::pointer m_TestImage;
	happyengine::graphics::Texture2D::pointer m_SplashImage;
	happyengine::graphics::Font::pointer m_pFont;

    GroundPlane* m_pGroundPlane;
    happyengine::game::Entity* m_pAxis;

    happyengine::graphics::SpotLight::pointer m_pSpotLight;

	GameTimer m_SplashTimer;
	float m_SplashAlpha;

    //Disable default copy constructor and default assignment operator
    MainGame(const MainGame&);
    MainGame& operator=(const MainGame&);
};

} //end namespace

#endif
