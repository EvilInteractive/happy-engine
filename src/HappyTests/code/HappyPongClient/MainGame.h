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

#ifndef _HPC_MainGame_H_
#define _HPC_MainGame_H_
#pragma once

#include "Game.h"
#include "Singleton.h"

namespace he {
    namespace tools {
        class FPSGraph;
    }
    namespace ge {
        class Entity;
        class DefaultRenderPipeline;
    }
    namespace gfx {
        class Window;
        class Scene;
        class View;
    }
}

namespace hpc {
class Palet;
class Obstacle;
class Ball;
class MainGame : public he::ge::Game
{
public:
    MainGame();
    virtual ~MainGame();

    virtual void init();
    virtual void load();
    virtual void tick(float dTime);

    const std::vector<Palet*>& getPalets() const;
    const std::vector<Obstacle*>& getObstacles() const;
    const he::vec2& getBoardDimension() const;
    Ball* getBall() const;

    void addPoint(he::uint8 player);
    void restart(bool timeout);

    void setActiveBall(Ball* ball);

    he::gfx::Scene* getActiveScene() const { return m_Scene; }

private:
    void connectionSuccessful();
    void connectionFailed();
    void connectionLost();

    float m_RestartTime;
    float m_RestartTimer;

    he::tools::FPSGraph* m_FPSGraph;

    he::vec2 m_BoardDimension;
    std::vector<Palet*> m_Palets;
    std::vector<Obstacle*> m_Obstacles;
    Ball* m_Ball;

    he::gfx::Window* m_Window;
    he::gfx::View*   m_View;
    he::gfx::Scene*  m_Scene;
    he::ge::DefaultRenderPipeline* m_RenderPipeline;

    std::vector<he::ge::Entity*> m_EntityList;

    //Disable default copy constructor and default assignment operator
    MainGame(const MainGame&);
    MainGame& operator=(const MainGame&);
};

} //end namespace

#endif
