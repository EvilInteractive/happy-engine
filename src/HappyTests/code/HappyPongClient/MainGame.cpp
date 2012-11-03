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
//Author:  Bastian Damman
//Created: 30/03/2012
#include "HappyPongClientPCH.h" 

#include "MainGame.h"

#include "GraphicsEngine.h"
#include "CameraManager.h"
#include "NetworkManager.h"
#include "NetworkObjectFactory.h"

#include "CameraPerspective.h"
#include "FPSGraph.h"

#include "LightManager.h"
#include "ContentManager.h"

#include "Entity.h"
#include "ModelComponent.h"

#include "Palet.h"
#include "Ball.h"
#include "Obstacle.h"
#include "MessageBox.h"

#include "IniWriter.h"
#include "IniReader.h"

#include "Window.h"
#include "Scene.h"
#include "View.h"

namespace hpc {

MainGame::MainGame(): m_FPSGraph(nullptr), m_RestartTimer(0.0f), m_RestartTime(2.0f), m_Ball(nullptr)
{
}


MainGame::~MainGame()
{
    if (NETWORK->isConnected())
        NETWORK->disconnect();

    std::for_each(m_EntityList.cbegin(), m_EntityList.cend(), [&](he::ge::Entity* entity)
    {
        delete entity;     
    });
    he::net::NetworkObjectFactory<Palet>::getInstance()->destroyAll();
    he::net::NetworkObjectFactory<Ball>::getInstance()->destroyAll();

    delete m_FPSGraph;

    GRAPHICS->removeView(m_View);
    GRAPHICS->removeScene(m_Scene);
    GRAPHICS->removeWindow(m_Window);
}

void MainGame::init()
{
    m_View = GRAPHICS->createView3D();
    m_Scene = GRAPHICS->createScene();
    m_Window = GRAPHICS->createWindow();

    m_Window->setResizable(true);
    m_Window->setVSync(false);
    m_Window->setWindowDimension(1280, 720);
    m_Window->setWindowTitle("Happy pong - client");
    he::eventCallback0<void> quitHandler(boost::bind(&he::HappyEngine::quit, HAPPYENGINE));
    m_Window->Closed += quitHandler;
    m_Window->create();

}

void MainGame::load()
{
    he::gfx::RenderSettings settings;
    settings.enableDeferred = true;
    settings.enablePost = true;

    settings.lightingSettings.enableLighting = true;
    settings.lightingSettings.enableNormalMap = true;
    settings.lightingSettings.enableShadows = false;
    settings.lightingSettings.enableSpecular = true;

    settings.shadowSettings.shadowMult = 2;

    settings.postSettings.shaderSettings.enableAO = true;
    settings.postSettings.shaderSettings.enableBloom = true;
    settings.postSettings.shaderSettings.enableDepthEdgeDetect = false;
    settings.postSettings.shaderSettings.enableFog = false;
    settings.postSettings.shaderSettings.enableHDR = true;
    settings.postSettings.shaderSettings.enableNormalEdgeDetect = false;
    settings.postSettings.shaderSettings.enableVignette = true;

    CONTENT->setRenderSettings(settings);

    m_View->setScene(m_Scene);
    m_View->setWindow(m_Window);
    m_View->setRelativeViewport(he::RectF(0, 0, 1.0f, 1.0f));
    m_View->init(settings);

    he::uint16 port(0);
    std::string ip("");

    he::io::IniReader reader;
    reader.open("net.cfg");
    if (reader.isOpen())
    {
        port = static_cast<he::uint16>(reader.readInt(L"Net", L"port"));
        ip = reader.readString(L"Net", L"ip");
    }
    else
    {
        he::io::IniWriter writer;
        writer.open("net.cfg");
        writer.writeInt(L"Net", L"port", 3000);
        writer.writeString(L"Net", L"ip", "localhost");
        writer.close();
        HE_ERROR("Failed to load the net cfg file, please fill it in and restart");
        HAPPYENGINE->quit();
        return;
    }

    NETWORK->setSyncTimeout(1 / 60.0f);
    he::eventCallback0<void> connectionSuccesHandler(boost::bind(&MainGame::connectionSuccessful, this));
    he::eventCallback0<void> connectionFailedHandler(boost::bind(&MainGame::connectionFailed, this));
    he::eventCallback0<void> connectionLostHandler(boost::bind(&MainGame::connectionLost, this));
    NETWORK->ConnectionSuccessful += connectionSuccesHandler;
    NETWORK->ConnectionFailed += connectionFailedHandler;
    NETWORK->ConnectionLost += connectionLostHandler;
    NETWORK->join(ip, port);

    const he::RectI& viewport(m_View->getViewport());
    he::gfx::CameraPerspective* camera(NEW he::gfx::CameraPerspective());
    camera->setLens((float)viewport.width / viewport.height, he::piOverFour, 10.0f, 1000);
    camera->lookAt(he::vec3(0.010f, 67.5f, 0.01f), he::vec3::zero, he::vec3(0, 0, 1));
    m_Scene->getCameraManager()->addCamera("default", camera);
    m_View->setCamera("default");

    m_Scene->getLightManager()->setDirectionalLight(he::normalize(he::vec3(0.3f, 1.0f, 1.0f)), he::Color(1.0f, 1, 1), 0.75f);
    m_Scene->getLightManager()->setAmbientLight(he::Color(0.8f, 0.8f, 1), 0.25f);

    m_FPSGraph = NEW he::tools::FPSGraph();
    m_FPSGraph->setType(he::tools::FPSGraph::Type_ToConsole);
    m_FPSGraph->setView(m_View);

    m_BoardDimension = he::vec2(85, 47);

    he::ge::Entity* board(NEW he::ge::Entity());
    board->init(m_Scene);
    he::ge::ModelComponent* boardModel(NEW he::ge::ModelComponent());
    boardModel->setModelMeshAndMaterial("pong/board.material", "pong/board.binobj");
    board->setLocalScale(he::vec3(100, 100, 100));

    board->addComponent(boardModel);
    m_EntityList.push_back(board);
}

void MainGame::tick( float dTime )
{
    he::ge::Game::tick(dTime);

    PROFILER_BEGIN("Fps graph");
    m_FPSGraph->tick(dTime);
    PROFILER_END();

    if (m_RestartTimer > 0.0f)
    {
        m_RestartTimer -= dTime;
        if (m_RestartTimer <= 0.0f)
        {
            restart(false);
        }
    }
}

void MainGame::drawGui()
{
}

const std::vector<Palet*>& MainGame::getPalets() const
{
    return m_Palets;
}

const he::vec2& MainGame::getBoardDimension() const
{
    return m_BoardDimension;
}

const std::vector<Obstacle*>& MainGame::getObstacles() const
{
    return m_Obstacles;
}

void MainGame::addPoint( he::uint8 player )
{
    m_Palets[player]->addPoint();
}

void MainGame::restart( bool timeout )
{
    if (timeout == true)
    {
        m_RestartTimer = m_RestartTime;
    }
    else
    {
        m_Ball->reset();
    }
}

Ball* MainGame::getBall() const
{
    return m_Ball;
}

void MainGame::connectionSuccessful()
{
}

void MainGame::connectionFailed()
{
    if (he::MessageBox::showExt("Connection Failed", "No connection could be established", he::MessageBoxIcon_Warning, "Retry", "Quit") == he::MessageBoxButton_Button1)
    {
        NETWORK->join();
    }
    else
    {
        HAPPYENGINE->quit();
    }
}

void MainGame::connectionLost()
{
    he::MessageBox::showExt("Connection Lost", "Connection to the server lost", he::MessageBoxIcon_Warning, "Quit");
    HAPPYENGINE->quit();
}

void MainGame::setActiveBall( Ball* ball )
{
    m_Ball = ball;
}


} //end namespace
