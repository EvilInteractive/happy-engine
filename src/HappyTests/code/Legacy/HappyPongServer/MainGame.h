//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

#ifndef _HPS_MainGame_H_
#define _HPS_MainGame_H_
#pragma once

#include "Game.h"

namespace he {
    namespace ge {
        class Entity;
    }
}

namespace hps {
class Palet;
class Obstacle;
class Ball;
class MainGame : public he::ge::Game
{
public:
    MainGame();
    virtual ~MainGame();

    virtual void init();
    virtual void destroy() {}
    virtual void load();
    virtual void tick(float dTime);
    virtual void drawGui();

    const std::vector<Palet*>& getPalets() const;
    const std::vector<Obstacle*>& getObstacles() const;
    const he::vec2& getBoardDimension() const;
    Ball* getBall() const;

    void addPoint(he::uint8 player);
    void restart(bool timeout);


    void unregisterPaddle(const he::net::NetworkID& id);
private:
    void createNewPaddle(const he::net::NetworkID& id);
    void inputThread();

    float m_RestartTime;
    float m_RestartTimer;
    
    he::vec2 m_BoardDimension;
    std::vector<Palet*> m_Palets;
    std::vector<Obstacle*> m_Obstacles;
    Ball* m_Ball;
    
    he::net::NetworkID m_Player1Id, m_Player2Id;

    boost::thread m_InputThread;

    //Disable default copy constructor and default assignment operator
    MainGame(const MainGame&);
    MainGame& operator=(const MainGame&);
};

} //end namespace

#endif
