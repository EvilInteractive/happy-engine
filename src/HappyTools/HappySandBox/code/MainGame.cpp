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
#include "DefaultRenderPipeline.h"
#include "Entity.h"
#include "ModelComponent.h"
#include "StaticPhysicsComponent.h"
#include "PhysicsConvexShape.h"
#include "PhysicsConcaveShape.h"
#include "PhysicsMaterial.h"
#include "LightManager.h"

#include "PluginLoader.h"
#include "EntityManager.h"
#include <EntityManager.h>

//#include "boost/filesystem.hpp"

namespace hs {

MainGame::MainGame(): m_FPSGraph(nullptr),
                      m_UIController(nullptr),
                      m_UIBind(nullptr),
                      m_RenderPipeline(nullptr),
                      m_Scene(nullptr),
                      m_View(nullptr),
                      m_Window(nullptr),
                      m_GamePlugin(nullptr),
                      m_EntityManager(nullptr),
                      m_TestScene(nullptr)
{
}

MainGame::~MainGame()
{
    m_RenderPipeline->get2DRenderer()->detachFromRender(m_FPSGraph);
    m_RenderPipeline->get2DRenderer()->detachFromRender(this);
    CONSOLE->detachFromRenderer();
    PROFILER->detachFromRenderer();

    delete m_FPSGraph;
    delete m_UIController;
    delete m_UIBind;

    m_Scene->getCameraManager()->deleteAllCameras();

    delete m_RenderPipeline;
    GRAPHICS->removeScene(m_Scene);
    GRAPHICS->removeView(m_View);
    GRAPHICS->removeWindow(m_Window);
}

void MainGame::destroy()
{
    he::ge::EntityManager::getInstance()->destroyEntity(m_TestScene);
    delete m_EntityManager;
    m_EntityManager = nullptr;

    HAPPYENGINE->getPluginLoader()->unloadPlugin(m_GamePlugin);
    m_GamePlugin = nullptr;
}

void MainGame::init()
{
    m_View = GRAPHICS->createView();
    m_Window = GRAPHICS->createWindow();

    m_Window->setResizable(true);
    m_Window->setVSync(false);
    m_Window->setWindowDimension(1280, 720);
    m_Window->setWindowTitle("Happy Sandbox");
    he::eventCallback0<void> quitHandler(boost::bind(&he::HappyEngine::quit, HAPPYENGINE));
    m_Window->Closed += quitHandler;
    m_Window->create();

    m_GamePlugin = HAPPYENGINE->getPluginLoader()->loadPlugin(he::Path("HappyPluginTest.dll"));

    using namespace he;

    he::gfx::RenderSettings settings;
    settings.enableDeferred = true;
    settings.enablePost = true;

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
    settings.postSettings.hdrSettings.exposureSpeed = 0.5f;

    CONTENT->setRenderSettings(settings);

    m_Scene = GRAPHICS->createScene();
    m_View->setWindow(m_Window);
    m_View->setRelativeViewport(he::RectF(0, 0, 1.0f, 1.0f));

    m_RenderPipeline = NEW he::ge::DefaultRenderPipeline();
    m_RenderPipeline->init(m_View, m_Scene, settings);

    m_View->init(settings);

    /* CAMERA */
    FlyCamera* flyCamera = NEW FlyCamera();
    m_Scene->getCameraManager()->addCamera("default", flyCamera);
    flyCamera->setLens(1280/720.0f, piOverTwo / 3.0f * 2.0f, 1.0f, 10000.0f);
    flyCamera->lookAt(vec3(5, 2, 4), vec3::zero, vec3::up);
    m_View->setCamera(flyCamera);

    /* GUI */
    m_FPSGraph = NEW tools::FPSGraph();
    m_FPSGraph->setType(he::tools::FPSGraph::Type_TextOnly);
    m_FPSGraph->setPos(he::vec2(910,35));
    m_RenderPipeline->get2DRenderer()->attachToRender(m_FPSGraph);

    m_EntityManager = NEW EntityManager(m_Scene);

    m_UIController = NEW UIController();
    m_UIController->init(m_RenderPipeline->get2DRenderer());

    m_UIBind = NEW UIBind(m_UIController, m_EntityManager);
    m_UIBind->setup();

    CONSOLE->attachToRenderer(m_RenderPipeline->get2DRenderer());
    PROFILER->attachToRenderer(m_RenderPipeline->get2DRenderer());
    m_RenderPipeline->get2DRenderer()->attachToRender(this);

    // test
    he::eventCallback1<void, const Awesomium::JSArray&> callbackTest([](const Awesomium::JSArray& /*args*/){ CONSOLE->addMessage("testing gui", he::CMSG_TYPE_INFO);});
    m_UIBind->bindObjectMethodToCallback("HE", "test", callbackTest);

    m_UIController->load("main.html");

    // test 3D
    he::ge::EntityManager* const entityMan(he::ge::EntityManager::getInstance());
    m_TestScene = entityMan->createEmptyEntity();
    m_TestScene->setScene(m_Scene);
    ge::ModelComponent* modelComp(static_cast<ge::ModelComponent*>(
        entityMan->createComponent(he::HEFS::strModelComponent)));
    m_TestScene->addComponent(modelComp);
    modelComp->setModelMeshAndMaterial("testSceneBas.material", "testPlatformer/scene.binobj");    
    //m_EntityList.push_back(scene);
    ge::StaticPhysicsComponent* physicsComp(static_cast<ge::StaticPhysicsComponent*>(
        entityMan->createComponent(he::HEFS::strStaticPhysicsComponent)));
    m_TestScene->addComponent(physicsComp);
    px::PhysicsConvexShape convexSceneShape("testPlatformer/scene.pxcv");
    px::PhysicsConcaveShape concaveSceneShape("testPlatformer/scene.pxcc");
    physicsComp->addShape(&convexSceneShape, px::PhysicsMaterial(1.2f, 1.0f, 0.1f));
    physicsComp->addShape(&concaveSceneShape, px::PhysicsMaterial(1.2f, 1.0f, 0.1f));
    m_TestScene->activate();

    m_Scene->getLightManager()->setAmbientLight(Color(1.0f, 1.0f, 1.0f, 0.8f), 0.5f);
    m_Scene->getLightManager()->setDirectionalLight(normalize(vec3(-4.0f, 5.f, 1.0f)), Color(1.0f, 0.9f, 0.8f, 1.0f), 1.0f);
}

void MainGame::tick(float dTime)
{
    he::gfx::CameraPerspective* camera(m_View->getCamera());
    const he::vec3& position(camera->getPosition());

    m_UIController->updateSceneInfo(position);

    he::ge::Game::tick(dTime);
    m_FPSGraph->tick(dTime);
}

void MainGame::draw2D(he::gui::Canvas2D* /*canvas*/)
{
    /*
    canvas->setFillColor(he::Color(1.0f,1.0f,1.0f));
    canvas->fillRect(he::vec2(200,200), he::vec2(50,50));*/

    //m_UIController->draw2D(canvas);
}

} //end namespace
