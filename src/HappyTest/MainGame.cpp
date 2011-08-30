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
#include <sstream>
#include <iostream>

#include "Matrix.h"
#include "Vector3.h"
#include "MathConstants.h"
#include "HappyEngine.h"

#include "IniReader.h"
#include "FileNotFoundException.h"

#include "HappyNew.h"

namespace happytest {

MainGame::MainGame() : m_pDeferred3DRenderer(nullptr), m_pTestObject(nullptr), m_BackgroundIndex(0),
                       m_DrawTimer(0), m_UpdateTimer(0), m_pDeferredPreEffect(NEW DeferredPreEffect()),                   
					   m_pServer(nullptr), m_pClient(nullptr), m_pSimple2DRenderer(nullptr), m_p2DEffect(nullptr)
{
    using namespace happyengine;
    m_BackgroundColors[0] = Color((byte)10, (byte)130, (byte)131, (byte)255);
    m_BackgroundColors[1] = Color((byte)122, (byte)186, (byte)122, (byte)255);
    m_BackgroundColors[2] = Color((byte)255, (byte)127, (byte)80, (byte)255);
    m_BackgroundColors[3] = Color((byte)255, (byte)165, (byte)0, (byte)255);
    m_BackgroundColors[4] = Color((byte)30, (byte)144, (byte)255, (byte)255);
}


MainGame::~MainGame()
{
    delete m_pDeferred3DRenderer;
	delete m_pSimple2DRenderer;
	delete m_p2DEffect;
    delete m_pTestObject;
    delete m_pDeferredPreEffect;
    std::for_each(m_Bullets.cbegin(), m_Bullets.cend(), [&](TestBullet* pBullet)
    {
        delete pBullet;
    });

    delete m_pServer;
    delete m_pClient;
    
    NETWORK->stop();
}

void MainGame::init()
{
    using namespace happyengine;
    io::IniReader reader;
    try { reader.open("../data/settings.ini"); }
    catch (error::FileNotFoundException& e)
    { std::wcout << e.getMsg() << "\n"; }
    if (reader.isOpen())
    {
        math::Vector2 windowDim(reader.readVector2(L"Window", L"dimension", math::Vector2(1280, 720)));
        GRAPHICS->setScreenDimension(static_cast<int>(windowDim.x), static_cast<int>(windowDim.y));
        GRAPHICS->setWindowTitle(reader.readString(L"Window", L"title", "Test"));

        math::Vector4 bColor(reader.readVector4(L"Background", L"color"));
        GRAPHICS->setBackgroundColor(Color(bColor.x, bColor.y, bColor.z, bColor.w));

        GRAPHICS->setVSync(reader.readBool(L"Graphics", L"vsync", true));
        GRAPHICS->toggleFullscreen(reader.readBool(L"Graphics", L"fullscreen"));
    }
}
void MainGame::load()
{
    using namespace happyengine;

    PHYSICS->startSimulation();

    m_pDeferred3DRenderer = NEW graphics::Deferred3DRenderer();
    m_pDeferred3DRenderer->getLightManager()->addPointLight(math::Vector3(-1, 0, -1), Color((byte)255, 50, 50, 255), 3.0f, 1, 10);
    m_pDeferred3DRenderer->getLightManager()->addDirectionalLight(math::Vector3(0, -1, 0), Color((byte)150, 200, 255, 255), 1.0f);
    m_pTestObject = NEW TestObject();
    m_pTestObject->load();

    TestBullet* pBullet(NEW TestBullet());
    pBullet->load();
    m_Bullets.push_back(pBullet);

    m_pDeferredPreEffect->load();

	m_pSimple2DRenderer = NEW graphics::Simple2DRenderer;
	m_p2DEffect = NEW graphics::Simple2DEffect;

	m_pSimple2DRenderer->initialize();
	m_p2DEffect->load();
}
void MainGame::tick(float dTime)
{
    m_UpdateTimer += dTime;
    if (m_UpdateTimer >= 1.0f)
    {
        std::cout << "update fps: " << 1.0f/dTime << "\n";
        m_UpdateTimer -= 1.0f;
    }

    if (CONTROLS->getKeyboard()->isKeyPressed(happyengine::io::Key_Escape))
        HAPPYENGINE->quit();
    m_pTestObject->tick(dTime);
    
    if (m_pClient == nullptr && m_pServer == nullptr)
    {
        if (CONTROLS->getKeyboard()->isKeyPressed(happyengine::io::Key_F11))
        {
            std::cout << "Starting server\n";
            m_pServer = NEW MyServer();
            m_pServer->start(30000, 16);
            NETWORK->start();
        }
        else if (CONTROLS->getKeyboard()->isKeyPressed(happyengine::io::Key_F12))
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
    if (CONTROLS->getKeyboard()->isKeyPressed(happyengine::io::Key_Space))
    {
        TestBullet* pBullet(NEW TestBullet());
        pBullet->load();
        m_Bullets.push_back(pBullet);
        std::cout << m_Bullets.size() << "\n";
    }
}
void MainGame::draw(float dTime)
{

    m_DrawTimer += dTime;
    if (m_DrawTimer >= 1.0f)
    {
        std::cout << "draw fps: " << 1.0f/dTime << "\n";
        m_DrawTimer -= 1.0f;
    }

    if (CONTROLS->getKeyboard()->isKeyPressed(happyengine::io::Key_Return))
    {
        ++m_BackgroundIndex;
        if (m_BackgroundIndex > 4)
            m_BackgroundIndex = 0;
        GRAPHICS->setBackgroundColor(m_BackgroundColors[m_BackgroundIndex]);
    }
    GRAPHICS->clearAll();

    m_pDeferred3DRenderer->begin();
    m_pDeferredPreEffect->begin();
    m_pTestObject->draw(m_pDeferred3DRenderer, m_pDeferredPreEffect, dTime);
    std::for_each(m_Bullets.cbegin(), m_Bullets.cend(), [&](TestBullet* pBullet)
    {
        pBullet->draw(m_pDeferred3DRenderer, m_pDeferredPreEffect, dTime);
    });
    m_pDeferredPreEffect->end();
    m_pDeferred3DRenderer->end(happyengine::math::Vector3(-5, 5, -4));

	m_pSimple2DRenderer->begin();
	m_p2DEffect->begin();

	m_pSimple2DRenderer->drawRectangle(50,50,100,100);

	m_p2DEffect->end();
	m_pSimple2DRenderer->end();
}

} //end namespace