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

#include "Camera.h"
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

namespace hpc {

MainGame::MainGame(): m_pFPSGraph(nullptr), m_RestartTimer(0.0f), m_RestartTime(2.0f), m_Ball(nullptr)
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

    CAMERAMANAGER->deleteAllCameras();
    delete m_pFPSGraph;

}

void MainGame::init()
{
    GRAPHICS->setVSync(false);
    GRAPHICS->setScreenDimension(1280, 720);
    GRAPHICS->setViewport(he::RectI(0, 0, 1280, 720));
}

void MainGame::load()
{
    he::ushort port(0);
    std::string ip("");

    he::io::IniReader reader;
    reader.open("net.cfg");
    if (reader.isOpen())
    {
        port = static_cast<he::ushort>(reader.readInt(L"Net", L"port"));
        ip = reader.readString(L"Net", L"ip");
    }
    else
    {
        he::io::IniWriter writer;
        writer.open("net.cfg");
        writer.writeInt(L"Net", L"port", 30000);
        writer.writeString(L"Net", L"ip", "localhost");
        writer.close();
        HE_ERROR("Failed to load the net cfg file, please fill it in and restart");
        HAPPYENGINE->quit();
        return;
    }

    NETWORK->setSyncTimeout(1 / 60.0f);
    NETWORK->ConnectionSuccessful += boost::bind(&MainGame::connectionSuccessful, this);
    NETWORK->ConnectionFailed += boost::bind(&MainGame::connectionFailed, this);
    NETWORK->ConnectionLost += boost::bind(&MainGame::connectionLost, this);
    NETWORK->join(ip, port);

    he::gfx::Camera* camera(NEW he::gfx::Camera(GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight()));
    camera->setLens((float)GRAPHICS->getScreenHeight() / GRAPHICS->getScreenWidth(), he::piOverFour, 10.0f, 1000);
    camera->lookAt(he::vec3(0.010f, 67.5f, 0.01f), he::vec3::zero, he::vec3(0, 0, 1));
    CAMERAMANAGER->addCamera("default", camera);
    CAMERAMANAGER->setActiveCamera("default");

    GRAPHICS->getLightManager()->setDirectionalLight(he::normalize(he::vec3(0.3f, 1.0f, 1.0f)), he::Color(1, 1, 1), 0.75f);
    GRAPHICS->getLightManager()->setAmbientLight(he::Color(0.8f, 0.8f, 1), 0.25f);

    m_pFPSGraph = NEW he::tools::FPSGraph();
    m_pFPSGraph->setType(2);

    m_BoardDimension = he::vec2(85, 47);

    he::ge::Entity* board(NEW he::ge::Entity());
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
    m_pFPSGraph->tick(dTime);
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
    m_pFPSGraph->draw();

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

void MainGame::addPoint( he::byte player )
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
    if (he::MessageBox::show("Connection Failed", "No connection could be established", "-  Retry  -", "-  Quit  -") == he::MessageBoxButton_Button1)
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
    he::MessageBox::show("Connection Lost", "Connection to the server lost", "-  Quit  -");
    HAPPYENGINE->quit();
}

void MainGame::setActiveBall( Ball* ball )
{
    m_Ball = ball;
}


} //end namespace
