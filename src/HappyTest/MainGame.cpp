//HappyTest Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyTest.
//
//    HappyTest is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyTest is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyTest.  If not, see <http://www.gnu.org/licenses/>.

#include "MainGame.h"

#include <vector>
#include <set>
#include <list>
#include <sstream>
#include <iostream>

#include "mat44.h"
#include "vec3.h"
#include "MathConstants.h"
#include "HappyEngine.h"
#include "MathFunctions.h"
#include "NetworkManager.h"
#include "GraphicsEngine.h"
#include "ContentManager.h"
#include "Console.h"
#include "ControlsManager.h"
#include "Happy2DRenderer.h"
#include "PhysicsEngine.h"

#include "IniReader.h"
#include "FileNotFoundException.h"
#include "Circle.h"

#include "HappyNew.h"

#include "boost/timer.hpp"

#include "Random.h"

namespace happytest {

MainGame::MainGame() : m_pTestObject(nullptr), m_BackgroundIndex(0),
                       m_DrawTimer(0), m_UpdateTimer(0),       
					   m_pServer(nullptr), m_pClient(nullptr), m_pFPSGraph(NEW he::tools::FPSGraph()),
					   m_pCamera(nullptr), m_pGroundPlane(nullptr), m_pTestButton(nullptr), m_pAxis(nullptr),
					   m_pTextBox(nullptr), m_bTest(true), m_bTest2(true), m_Test3("You can edit this string via console")
{
    using namespace he;
    m_BackgroundColors[0] = Color((byte)10, (byte)130, (byte)131, (byte)255);
    m_BackgroundColors[1] = Color((byte)122, (byte)186, (byte)122, (byte)255);
    m_BackgroundColors[2] = Color((byte)255, (byte)127, (byte)80, (byte)255);
    m_BackgroundColors[3] = Color((byte)255, (byte)165, (byte)0, (byte)255);
    m_BackgroundColors[4] = Color((byte)30, (byte)144, (byte)255, (byte)255);
}


MainGame::~MainGame()
{
    delete m_pTestObject;
    std::for_each(m_Bullets.cbegin(), m_Bullets.cend(), [&](TestBullet* pBullet)
    {
        delete pBullet;
    });

    delete m_pServer;
    delete m_pClient;

	delete m_pFPSGraph;
	delete m_pCamera;

    delete m_pGroundPlane;
	delete m_pTestButton;
	delete m_pAxis;
	delete m_pTextBox;

    NETWORK->stop();
}

void MainGame::init()
{
    using namespace he;
    io::IniReader reader;
    try { reader.open("../data/settings.ini"); }
    catch (err::FileNotFoundException& e)
    { std::wcout << e.getMsg() << "\n"; }
    if (reader.isOpen())
    {
        vec2 windowDim(reader.readVector2(L"Window", L"dimension", vec2(1280, 720)));
        GRAPHICS->setScreenDimension(static_cast<int>(windowDim.x), static_cast<int>(windowDim.y));
        GRAPHICS->setWindowTitle(reader.readString(L"Window", L"title", "Test"));

        vec4 bColor(reader.readVector4(L"Background", L"color"));
        GRAPHICS->setBackgroundColor(Color(bColor.x, bColor.y, bColor.z, bColor.w));

        GRAPHICS->setVSync(reader.readBool(L"Graphics", L"vsync", true));
        GRAPHICS->toggleFullscreen(reader.readBool(L"Graphics", L"fullscreen"));
    }
}
void MainGame::load()
{
    using namespace he;

	m_SplashImage = CONTENT->asyncLoadTexture("happy_splash.png");

    PHYSICS->startSimulation();

	m_pCamera = NEW FlyCamera(GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight());
	m_pCamera->lookAt(vec3(-5, 5, -4), vec3(0, 0, 0), vec3(0, 1, 0));
	m_pCamera->setLens(16.0f/9.0f,piOverFour,1.0f,1000.0f);
	m_pCamera->setActive(true);
	//m_pCamera->controllable(false);

    GRAPHICS->getLightManager()->addPointLight(vec3(0, 2, 0), Color((byte)255, 50, 50, 255), 5.0f, 1, 10);
    //m_pSpotLight = GRAPHICS->getLightManager()->addSpotLight(vec3(-1, 0, -1), vec3(-1, 0, 0), Color((byte)255, 255, 200, 255), 3.0f, piOverFour, 1, 30);

    Random r;
    for (int i = 0; i < 200; ++i)
       GRAPHICS->getLightManager()->addSpotLight(vec3(r.nextFloat(0, -100), r.nextFloat(5, 20), r.nextFloat(0, 100)), vec3(0, -1, 0), Color((byte)255, 255, 200, 255), 1.0f, piOverTwo, 1, 20);
    //GRAPHICS->getLightManager()->addDirectionalLight(vec3(0, -1, 0), Color((byte)150, 200, 255, 255), 0.5f);
    GRAPHICS->getLightManager()->addAmbientLight(vec3::zero, Color(1.0f, 1.0f, 1.0f, 1.0f), 0.3f, 1000);
   
    m_pTestObject = NEW TestObject(CONTENT->loadEntity("car.entity"));
    m_pGroundPlane = NEW GroundPlane(CONTENT->loadEntity("groundPlane.entity")); 
	m_pAxis = NEW he::game::Entity(CONTENT->loadEntity("axis.entity"));
        
	m_TestImage = CONTENT->asyncLoadTexture("v8_vantage_color.png");

    m_pFont = CONTENT->loadFont("MODES.ttf", 32);

	m_pTestButton = NEW gui::Button(gui::Button::TYPE_NORMAL, vec2(50,600), vec2(60,20));
	m_pTestButton->setText("Button", 12);

	m_pTextBox = NEW gui::TextBox(RectF(50,650,200,20), "testing", 10);

	CONSOLE->registerValue<bool>(&m_bTest2, "draw_test");
	CONSOLE->registerValue<std::string>(&m_Test3, "test_string");
	CONSOLE->addMessage("warning test", CMSG_TYPE_WARNING);
}
void MainGame::tick(float dTime)
{
	m_pCamera->tick(dTime);

    if (CONTROLS->getKeyboard()->isKeyPressed(he::io::Key_Escape))
        HAPPYENGINE->quit();

    m_pTestObject->tick(dTime);
    
    if (m_pClient == nullptr && m_pServer == nullptr)
    {
        if (CONTROLS->getKeyboard()->isKeyPressed(he::io::Key_F11))
        {
            std::cout << "Starting server\n";
            m_pServer = NEW MyServer();
            m_pServer->start(30000, 16);
            NETWORK->start();
        }
        else if (CONTROLS->getKeyboard()->isKeyPressed(he::io::Key_F12))
        {
            std::cout << "Starting client\n";
            m_pClient = NEW MyClient();
            m_pClient->asyncConnect("78.21.63.179", 30000);
            NETWORK->start();
        }
    }

    std::for_each(m_Bullets.cbegin(), m_Bullets.cend(), [&dTime](TestBullet* pBullet)
    {
        pBullet->tick(dTime);
    });

    if (CONTROLS->getKeyboard()->isKeyPressed(he::io::Key_Space))
    {
        TestBullet* pBullet(NEW TestBullet(CONTENT->loadEntity("bullet.entity"), m_pCamera->getPosition(), m_pCamera->getLook() * 20));
        m_Bullets.push_back(pBullet);
        std::cout << m_Bullets.size() << "\n";
    }

    //m_pSpotLight->setPosition(m_pCamera->getPosition());
    //m_pSpotLight->setDirection(-he::normalize(m_pCamera->getLook()));

	m_pTestButton->tick();

	m_pTextBox->tick();

	m_pFPSGraph->tick(dTime, 0.5f);

	CONSOLE->tick();
}
void MainGame::draw(float /*dTime*/)
{
	using namespace he;
	using namespace gfx;

    /*if (CONTROLS->getKeyboard()->isKeyPressed(he::io::Key_Return))
    {
        ++m_BackgroundIndex;
        if (m_BackgroundIndex > 4)
            m_BackgroundIndex = 0;
        GRAPHICS->setBackgroundColor(m_BackgroundColors[m_BackgroundIndex]);
    }*/

    GRAPHICS->clearAll();

    GRAPHICS->begin(m_pCamera);
    GRAPHICS->draw(m_pTestObject);
		std::for_each(m_Bullets.cbegin(), m_Bullets.cend(), [&](TestBullet* pBullet)
		{
			GRAPHICS->draw(pBullet);
		});
    GRAPHICS->draw(m_pGroundPlane);
	GRAPHICS->draw(m_pAxis);
    GRAPHICS->end();

	// 2D test stuff
	HE2D->begin();

		// GUI elements need to be drawn inside HE2D renderer
		m_pTestButton->draw();
		m_pTextBox->draw();

		if (m_bTest2)
		{
			HE2D->setColor(1.0f,0.5f,0.0f);
			HE2D->setFontVerticalAlignment(Font::VAlignment_Center);
			HE2D->setFontHorizontalAlignment(Font::HAlignment_Center);

			HE2D->drawString(m_Test3, m_pFont, RectF(0,0,(float)GRAPHICS->getScreenWidth(),(float)GRAPHICS->getScreenHeight()));
		}

		m_pFPSGraph->draw();

		CONSOLE->draw();

	HE2D->end();
}

} //end namespace