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
#include "HappyPongServerPCH.h" 

#include "MainGame.h"

#include "NetworkManager.h"

#include "Entity.h"

#include "Palet.h"
#include "Ball.h"
#include "Obstacle.h"
#include "NetworkObjectFactory.h"

#include "IniReader.h"
#include "IniWriter.h"

namespace hps {

MainGame::MainGame()
    : m_RestartTimer(0.0f)
    , m_RestartTime(2.0f)
    , m_Player1Id(UNASSIGNED_NETWORKID)
    , m_Player2Id(UNASSIGNED_NETWORKID)
    , m_Ball(nullptr)
{
}


MainGame::~MainGame()
{
    if (NETWORK->isConnected())
        NETWORK->disconnect();

    std::for_each(getObstacles().cbegin(), getObstacles().cend(), [&](const Obstacle* obstacle)
    {
        delete obstacle;
    });
    he::net::NetworkObjectFactory<Palet>::getInstance()->destroyAll();
    he::net::NetworkObjectFactory<Ball>::getInstance()->destroyAll();
}

void MainGame::init()
{
}

void MainGame::load()
{
    he::ushort port(0);

    he::io::IniReader reader;
    reader.open("net.cfg");
    if (reader.isOpen())
    {
        port = static_cast<he::ushort>(reader.readInt(L"Net", L"port"));
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
    NETWORK->setMaxConnections(2);
    NETWORK->host(port);
    NETWORK->ClientConnected += boost::bind(&hps::MainGame::createNewPaddle, this, _1);

    m_BoardDimension = he::vec2(85, 47);
    //m_Obstacles.push_back(NEW Obstacle());

    m_InputThread = boost::thread(boost::bind(&MainGame::inputThread, this));

    he::net::NetworkObjectFactory<Ball>* factory(he::net::NetworkObjectFactory<Ball>::getInstance());
    m_Ball = factory->get(factory->create());
}

void MainGame::inputThread()
{
    char buff[1000];

    for (;;)
    {
        std::cin.getline(buff, 1000);
        std::string command(buff);
        if (command == "stop")
        {
            HAPPYENGINE->quit();
            break;
        }
    }
}


void MainGame::tick( float dTime )
{
    he::ge::Game::tick(dTime);

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

void MainGame::createNewPaddle( const he::net::NetworkID& id )
{
    HE_IF_ASSERT(m_Player1Id == UNASSIGNED_NETWORKID || m_Player2Id == UNASSIGNED_NETWORKID, "2 players already connected!")
    {
        he::net::NetworkObjectFactory<Palet>* factory(he::net::NetworkObjectFactory<Palet>::getInstance());
        Palet* palet(factory->get(factory->create()));
        palet->setOwner(id);
        if (m_Player1Id == UNASSIGNED_NETWORKID)
        {
            m_Player1Id = id;
            palet->init(this, 0, false);
            HE_INFO("Palet created - player1");
        }
        else
        {
            m_Player2Id = id;
            palet->init(this, 1, false);
            HE_INFO("Palet created - player2");
        }
        m_Palets.push_back(palet);
    }
}
void MainGame::unregisterPaddle( const he::net::NetworkID& id )
{
    for (he::uint i(0); i < m_Palets.size(); ++i)
    {
        if (m_Palets[i]->getOwner() == id)
        {
            if (id == m_Player1Id)
                m_Player1Id = UNASSIGNED_NETWORKID;
            else
                m_Player2Id = UNASSIGNED_NETWORKID;
            m_Palets[i] = m_Palets.back();
            m_Palets.pop_back();
            break;
        }
    }
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

void MainGame::addPoint( he::byte /*player*/ )
{
    //m_Palets[player]->addPoint();
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

} //end namespace
