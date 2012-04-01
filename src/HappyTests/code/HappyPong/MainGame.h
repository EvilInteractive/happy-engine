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

#ifndef _HT_MainGame_H_
#define _HT_MainGame_H_
#pragma once

#include "Game.h"

namespace he {
    namespace tools {
        class FPSGraph;
    }
    namespace game {
        class Entity;
    }
}

namespace ht {
class Palet;
class Obstacle;
class Ball;
class MainGame : public he::game::Game
{
public:
    MainGame();
    virtual ~MainGame();

    virtual void init();
    virtual void load();
    virtual void tick(float dTime);
    virtual void drawGui();

    const std::vector<Palet*>& getPalets() const;
    const std::vector<Obstacle*>& getObstacles() const;
    const he::vec2& getBoardDimension() const;

    void addPoint(he::byte player);
    void restart(bool timeout);

private:
    float m_RestartTime;
    float m_RestartTimer;

    he::tools::FPSGraph* m_pFPSGraph;

    he::vec2 m_BoardDimension;
    std::vector<Palet*> m_Palets;
    std::vector<Obstacle*> m_Obstacles;
    Ball* m_Ball;


    std::vector<he::game::Entity*> m_EntityList;

    //Disable default copy constructor and default assignment operator
    MainGame(const MainGame&);
    MainGame& operator=(const MainGame&);
};

} //end namespace

#endif
