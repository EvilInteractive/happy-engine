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
#include "Canvas2D.h"
#include "Renderer2D.h"

#include "FlyCamera.h"
#include "FPSGraph.h"

#include "ContentManager.h"

#include "UIController.h"
#include "UIBind.h"

#include "Window.h"
#include "Scene.h"
#include "View.h"
#include "MessageBox.h"

//#include "boost/filesystem.hpp"

namespace hs {

MainGame::MainGame(): m_FPSGraph(nullptr),
                      m_UIController(nullptr),
                      m_UIBind(nullptr)
{
}

MainGame::~MainGame()
{
    delete m_FPSGraph;
    delete m_UIController;
    delete m_UIBind;

	CONSOLE->setView(nullptr);

	m_Scene->getCameraManager()->deleteAllCameras();

	GRAPHICS->removeScene(m_Scene);
	GRAPHICS->removeView(m_View);
	GRAPHICS->removeWindow(m_Window);
}

void MainGame::init()
{
    m_Scene = GRAPHICS->createScene();
    m_View = GRAPHICS->createView3D();
    m_Window = GRAPHICS->createWindow();

    m_Window->setResizable(true);
    m_Window->setVSync(true);
    m_Window->setWindowDimension(1280, 720);
    m_Window->setWindowTitle("Happy Sandbox");
    he::eventCallback0<void> quitHandler(boost::bind(&he::HappyEngine::quit, HAPPYENGINE));
    m_Window->Closed += quitHandler;
    m_Window->create();
}

void MainGame::load()
{
    using namespace he;
    
    he::gfx::RenderSettings settings;
    settings.enableDeferred = true;
    settings.enablePost = true;
	settings.skybox = "";

    settings.lightingSettings.enableLighting = true;
    settings.lightingSettings.enableNormalMap = true;
    settings.lightingSettings.enableShadows = false;
    settings.lightingSettings.enableSpecular = true;

    settings.shadowSettings.shadowMult = 2;

    settings.postSettings.shaderSettings.enableAO = false;
    settings.postSettings.shaderSettings.enableBloom = true;
    settings.postSettings.shaderSettings.enableDepthEdgeDetect = false;
    settings.postSettings.shaderSettings.enableFog = true;
    settings.postSettings.shaderSettings.enableHDR = true;
    settings.postSettings.shaderSettings.enableNormalEdgeDetect = false;
    settings.postSettings.shaderSettings.enableVignette = true;

    CONTENT->setRenderSettings(settings);

    m_View->setScene(m_Scene);
    m_View->setWindow(m_Window);
    m_View->setRelativeViewport(he::RectF(0, 0, 1.0f, 1.0f));
    m_View->init(settings);

    CONSOLE->setView(m_View);

	//m_View->get2DRenderer()->attachToRender(this);

    /* CAMERA */
    FlyCamera* flyCamera = NEW FlyCamera();
    m_Scene->getCameraManager()->addCamera("default", flyCamera);
    flyCamera->setLens(1280/720.0f, piOverTwo / 3.0f * 2.0f, 1.0f, 100.0f);
    flyCamera->lookAt(vec3(5, 2, 4), vec3::zero, vec3::up);
    m_View->setCamera("default");

    /* GUI */
    m_FPSGraph = NEW tools::FPSGraph();
	m_FPSGraph->setType(he::tools::FPSGraph::Type_TextOnly);

    m_UIController = NEW UIController();
    m_UIController->init(m_View);

	m_FPSGraph->setView(m_View);

    m_UIBind = NEW UIBind(m_UIController);

    // test
    he::eventCallback0<void> callbackTest([](){ CONSOLE->addMessage("testing gui", he::CMSG_TYPE_INFO);});
    m_UIBind->bindObjectMethodToCallback("HE", "test", callbackTest);

    m_UIController->load("main.html");
}

void MainGame::tick(float dTime)
{
	he::gfx::CameraPerspective* camera(m_View->getCamera());
    const he::vec3& position(camera->getPosition());

	m_UIController->updateSceneInfo(position);

    he::ge::Game::tick(dTime);
    m_FPSGraph->tick(dTime);
}

void MainGame::draw2D(he::gfx::Canvas2D* /*canvas*/)
{
	/*
	canvas->setFillColor(he::Color(1.0f,1.0f,1.0f));
	canvas->fillRect(he::vec2(200,200), he::vec2(50,50));*/

	//m_UIController->draw2D(canvas);
}

} //end namespace
