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
#include "HappyBasTestPCH.h" 

#include "MainGame.h"

#include "GraphicsEngine.h"
#include "CameraManager.h"
#include "ControlsManager.h"

#include "ModelMesh.h"

#include "FlyCamera.h"
#include "FPSGraph.h"

#include "Entity.h"
#include "ModelComponent.h"
#include "ContentManager.h"

#include "LightManager.h"
#include "SkyBox.h"

#include "LightComponent.h"
#include "DirectionalLight.h"
#include "Random.h"

#include "Scene.h"
#include "View.h"
#include "Window.h"

#include "IKeyboard.h"


namespace ht {

MainGame::MainGame(): m_FpsGraph(nullptr), m_pSkyBox(nullptr), m_SpinShadows(false)
{
}


MainGame::~MainGame()
{
    delete m_pSkyBox;
    std::for_each(m_EntityList.cbegin(), m_EntityList.cend(), [&](he::ge::Entity* entity)
    {
        delete entity;     
    });

    GRAPHICS->removeView(m_View);
    GRAPHICS->removeScene(m_Scene);
    GRAPHICS->removeWindow(m_Window);

    delete m_FpsGraph;
}

void MainGame::init()
{
    m_View = GRAPHICS->createView();
    m_Scene = GRAPHICS->createScene();
    m_Window = GRAPHICS->createWindow();

    m_Window->setResizable(true);
    m_Window->setVSync(false);
    m_Window->setWindowDimension(1280, 720);
    m_Window->setWindowTitle("HappyBasTest");
    m_Window->open();
}

void MainGame::load()
{
    using namespace he;

    he::gfx::RenderSettings settings;
    settings.enableDeferred = true;
    settings.enablePost = true;

    settings.lightingSettings.enableLighting = true;
    settings.lightingSettings.enableNormalMap = true;
    settings.lightingSettings.enableShadows = true;
    settings.lightingSettings.enableSpecular = true;

    settings.postSettings.shaderSettings.enableAO = true;
    settings.postSettings.shaderSettings.enableBloom = true;
    settings.postSettings.shaderSettings.enableDepthEdgeDetect = false;
    settings.postSettings.shaderSettings.enableFog = false;
    settings.postSettings.shaderSettings.enableHDR = true;
    settings.postSettings.shaderSettings.enableNormalEdgeDetect = false;
    settings.postSettings.shaderSettings.enableVignette = true;

    m_View->setScene(m_Scene);
    m_View->setWindow(m_Window);
    m_View->setRelativeViewport(he::RectF(0, 0, 1.0f, 1.0f));
    m_View->init(settings);
    
    m_FlyCamera = NEW FlyCamera();
    m_Scene->getCameraManager()->addCamera("default", m_FlyCamera);
    m_Scene->getCameraManager()->setActiveCamera("default");
    m_FlyCamera->setLens(1280/720.0f, piOverTwo / 3.0f * 2.0f, 1.0f, 250.0f);

    m_FpsGraph = NEW tools::FPSGraph();
    m_FpsGraph->setView(m_View);
    m_FpsGraph->setType(tools::FPSGraph::Type_TextOnly);

    ge::Entity* scene(NEW ge::Entity());
    scene->init(m_Scene);
    ge::ModelComponent* modelComp(NEW ge::ModelComponent());
    he::ObjectHandle sceneMaterial(CONTENT->loadMaterial("testScene3.material"));
    modelComp->setMaterial(sceneMaterial);
    he::gfx::ModelMesh* mesh(CONTENT->asyncLoadModelMesh("testScene3.binobj", "M_Scene", modelComp->getMaterial()->getCompatibleVertexLayout()));
    //he::gfx::ModelMesh* mesh(CONTENT->asyncLoadModelMesh("testSceneBas/testSceneBas.binobj", "M_Ground", modelComp->getMaterial().getCompatibleVertexLayout()));
    //he::gfx::ModelMesh* mesh(CONTENT->asyncLoadModelMesh("testScene4.binobj", "Box008", modelComp->getMaterial().getCompatibleVertexLayout()));
    //he::gfx::ModelMesh* mesh(CONTENT->asyncLoadModelMesh("testScene5.binobj", "M_Terrain", modelComp->getMaterial().getCompatibleVertexLayout()));

    modelComp->setModelMesh(mesh->getHandle());
    //modelComp->setLocalTransform(he::mat44::createScale(100));
    scene->addComponent(modelComp);
    mesh->release();

    modelComp = NEW ge::ModelComponent();
    he::ObjectHandle sceneMaterial2(CONTENT->loadMaterial("testScene4.material"));
    modelComp->setMaterial(sceneMaterial2);
    mesh = CONTENT->asyncLoadModelMesh("testScene4.binobj", "Box008", modelComp->getMaterial()->getCompatibleVertexLayout());
    modelComp->setModelMesh(mesh->getHandle());
    modelComp->setLocalTransform(he::mat44::createTranslation(he::vec3(1, 1, 1)) * he::mat44::createRotation(vec3(0, 1, 0), he::pi) * he::mat44::createScale(100));
    scene->addComponent(modelComp);
    mesh->release();

    he::ResourceFactory<he::gfx::Material>::getInstance()->release(sceneMaterial);
    he::ResourceFactory<he::gfx::Material>::getInstance()->release(sceneMaterial2);

    m_EntityList.push_back(scene);

    #pragma region lights
    m_Scene->getLightManager()->setAmbientLight(Color(0.9f, 1.0f, 1.0f, 1.0f), 0.5f);
    m_Scene->getLightManager()->setDirectionalLight(normalize(vec3(-2.0f, 5.f, 1.0f)), Color(1.0f, 0.8f, 0.5f, 1.0f), 2.0f);

    ge::PointLightComponent* pTempPointLightComp(NEW ge::PointLightComponent());
    scene->addComponent(pTempPointLightComp);
    pTempPointLightComp->setOffset(vec3(8.822f, 6.739f, -20.068f));
    pTempPointLightComp->setMultiplier(50.0f);
    pTempPointLightComp->setColor(Color((he::byte)126, 190, 255, 255));
    pTempPointLightComp->setAttenuation(0, 15);

    #pragma endregion

    //m_pSkyBox = NEW gfx::SkyBox();
    //m_pSkyBox->load("skybox/day/day.png");
    //m_Scene->attachToScene(m_pSkyBox);
}

void MainGame::tick( float dTime )
{
    he::ge::Game::tick(dTime);

    if (CONTROLS->getKeyboard()->isKeyPressed(he::io::Key_Return))
        m_SpinShadows = !m_SpinShadows;

    if (m_SpinShadows)
    {
        const he::mat44 rot(he::mat44::createRotation(he::vec3::up, dTime / 4.0f));
        m_Scene->getLightManager()->getDirectionalLight()->setDirection(
            (rot * he::vec4(m_Scene->getLightManager()->getDirectionalLight()->getDirection(), 0)).xyz());
    }

    m_FpsGraph->tick(dTime);
}

} //end namespace
