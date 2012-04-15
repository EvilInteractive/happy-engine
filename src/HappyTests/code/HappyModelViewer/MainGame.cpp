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
//Created: 20/03/2012
#include "HappyTestsPCH.h" 

#include "MainGame.h"

#include "GraphicsEngine.h"
#include "CameraManager.h"

#include "Camera.h"
#include "FPSGraph.h"

namespace ht {

MainGame::MainGame(): m_pFPSGraph(nullptr)
{
}


MainGame::~MainGame()
{
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
    CAMERAMANAGER->addCamera("default", NEW he::gfx::Camera(GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight()));
    CAMERAMANAGER->setActiveCamera("default");

    m_pFPSGraph = NEW he::tools::FPSGraph();

}

void MainGame::tick( float dTime )
{
    he::game::Game::tick(dTime);

    m_pFPSGraph->tick(dTime);
}

void MainGame::drawGui()
{
    m_pFPSGraph->draw();

}

} //end namespace
