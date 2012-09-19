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
//Author:  Sebastiaan Sprengers
//Created: 10/07/2012

#include "HappySandBoxPCH.h" 

#include "MainGame.h"

#include "GraphicsEngine.h"
#include "CameraManager.h"
#include "ControlsManager.h"

#include "FlyCamera.h"
#include "FPSGraph.h"

#include "ContentManager.h"

#include "UIController.h"
#include "UIBind.h"

//#include "boost/filesystem.hpp"

namespace hs {

MainGame::MainGame(): m_FPSGraph(nullptr),
                      m_UIController(nullptr),
                      m_UIBind(nullptr)
{
}

MainGame::~MainGame()
{
    CAMERAMANAGER->deleteAllCameras();
    delete m_FPSGraph;
    delete m_UIController;
    delete m_UIBind;
}

void MainGame::init()
{
    GRAPHICS->setVSync(false);
    GRAPHICS->setScreenDimension(1280, 720);
    GRAPHICS->setViewport(he::RectI(0, 0, 1280, 720));
}

void MainGame::load()
{
    using namespace he;
    
    /* CAMERA */
    CAMERAMANAGER->addCamera("default", NEW FlyCamera(GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight()));
    CAMERAMANAGER->setActiveCamera("default");
    CAMERAMANAGER->getActiveCamera()->setLens(1280/720.0f, piOverTwo / 3.0f * 2.0f, 1.0f, 1000.0f);
    CAMERAMANAGER->getActiveCamera()->lookAt(vec3(5, 2, 4), vec3::zero, vec3::up);

    /* GUÏ */
    m_FPSGraph = NEW tools::FPSGraph();
    //m_pFPSGraph->setType(1);

    m_UIController = NEW UIController();
    m_UIController->init();

    m_UIBind = NEW UIBind(m_UIController);
    // test
    m_UIBind->bindObjectMethodToCallback("HE", "test", [](){ CONSOLE->addMessage("testing gui", he::CMSG_TYPE_INFO);});

    m_UIController->load("main.html");
}

void MainGame::tick(float dTime)
{
    he::ge::Game::tick(dTime);
    m_FPSGraph->tick(dTime);
}

void MainGame::drawGui()
{
    m_UIController->draw();
    m_FPSGraph->draw();
}

} //end namespace
