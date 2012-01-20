//TheGreatEscape Copyright (C) 2011 Evil Interactive, Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyFxTest.
//
// 
//Author: Bastian Damman
// 

#include "HappyFxTestPCH.h"

#include "MainGame.h"
#include "GraphicsEngine.h"
#include "Happy2DRenderer.h"
#include "Console.h"

#include "CameraManager.h"
#include "Camera.h"
#include "GraphicsEngine.h"
#include "LightManager.h"
#include "ModelComponent.h"
#include "ContentManager.h"
#include "FlyCamera.h"

namespace happyfxtest {
using namespace he;

MainGame::MainGame(): m_pFpsGraph(NEW tools::FPSGraph()), m_pSkyBox(NEW gfx::SkyBox()),
                      m_pGround(NEW game::Entity()), m_pFxEditorBinding(NEW tools::HappyFxEditorBinding())
{
}


MainGame::~MainGame()
{
    CAMERAMANAGER->deleteAllCameras();
    delete m_pFpsGraph;
    delete m_pSkyBox;
    delete m_pGround;
    delete m_pFxEditorBinding;
}

void MainGame::init()
{
    GRAPHICS->setVSync(false);
}

void MainGame::load()
{
    FlyCamera* pFlyCamera = NEW FlyCamera(GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight());
    pFlyCamera->lookAt(vec3(-5, 5, -4), vec3(0, 0, 0), vec3(0, 1, 0));
    pFlyCamera->setLens(16.0f/9.0f,piOverFour,1.0f,400.0f);
    CAMERAMANAGER->addCamera("fly", pFlyCamera);
    CAMERAMANAGER->setActiveCamera("fly");

    GRAPHICS->getLightManager()->setAmbientLight(Color(0.8f, 0.4f, 1.0f, 1.0f), 0.2f);
    GRAPHICS->getLightManager()->setDirectionalLight(normalize(vec3(-1.0f, 1.0f, 1.0f)), Color(1.0f, 1.0f, 0.9f, 1.0f), 0.5f);

    m_pSkyBox->load("skybox/night/night.png");
    GRAPHICS->addToDrawList(m_pSkyBox);

    game::ModelComponent* pGroundModelComp(NEW game::ModelComponent());
    pGroundModelComp->setMaterial(CONTENT->loadMaterial("box.material"));
    pGroundModelComp->setModelMesh(CONTENT->asyncLoadModelMesh("box.binobj", "ChamferBox001", pGroundModelComp->getMaterial().getCompatibleVertexLayout()));
    m_pGround->addComponent(pGroundModelComp);

    m_pFxEditorBinding->init();
}

void MainGame::tick( float dTime )
{
    game::Game::tick(dTime);

    m_pFpsGraph->tick(dTime);
}

void MainGame::drawGui()
{
    m_pFpsGraph->draw();
}


} //end namespace