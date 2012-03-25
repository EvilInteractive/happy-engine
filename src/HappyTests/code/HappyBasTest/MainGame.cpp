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

#include "FlyCamera.h"
#include "FPSGraph.h"

#include "Entity.h"
#include "ModelComponent.h"
#include "ContentManager.h"

#include "LightManager.h"
#include "SkyBox.h"

#include "LightComponent.h"
#include "Random.h"

namespace ht {

MainGame::MainGame(): m_pFPSGraph(nullptr), m_pSkyBox(nullptr)
{
}


MainGame::~MainGame()
{
    delete m_pSkyBox;
    std::for_each(m_EntityList.cbegin(), m_EntityList.cend(), [&](he::game::Entity* entity)
    {
        delete entity;     
    });

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
    using namespace he;

    CAMERAMANAGER->addCamera("default", NEW FlyCamera(GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight()));
    CAMERAMANAGER->setActiveCamera("default");
    CAMERAMANAGER->getActiveCamera()->setLens(1280/720.0f, piOverTwo / 3.0f * 2.0f, 1.0f, 250.0f);

    m_pFPSGraph = NEW tools::FPSGraph();

    game::Entity* scene(NEW game::Entity());
    game::ModelComponent* modelComp(NEW game::ModelComponent());
    modelComp->setMaterial(CONTENT->loadMaterial("testSceneBas.material"));
    modelComp->setModelMesh(CONTENT->asyncLoadModelMesh("testSceneBas/testSceneBas.binobj", "M_Ground", modelComp->getMaterial().getCompatibleVertexLayout()));
    scene->addComponent(modelComp);
    m_EntityList.push_back(scene);

    #pragma region lights
    GRAPHICS->getLightManager()->setAmbientLight(Color(0.8f, 0.4f, 1.0f, 1.0f), 0.2f);
    GRAPHICS->getLightManager()->setDirectionalLight(normalize(vec3(-1.0f, 1.0f, 1.0f)), Color(1.0f, 1.0f, 0.9f, 1.0f), 0.5f);

    game::PointLightComponent* pTempPointLightComp(NEW game::PointLightComponent());
    scene->addComponent(pTempPointLightComp);
    pTempPointLightComp->setOffset(vec3(8.822f, 6.739f, -20.068f));
    pTempPointLightComp->setMultiplier(5.0f);
    pTempPointLightComp->setColor(Color((byte)126, 190, 255, 255));
    pTempPointLightComp->setAttenuation(0, 15);

    pTempPointLightComp = NEW game::PointLightComponent(); //pyramids
    scene->addComponent(pTempPointLightComp);
    pTempPointLightComp->setOffset(vec3(-41.428f, 23.483f, 46.274f));
    pTempPointLightComp->setMultiplier(4.0f);
    pTempPointLightComp->setColor(Color((byte)229, 174, 126, 255));
    pTempPointLightComp->setAttenuation(0, 42.2f);
    pTempPointLightComp->setBroken(true);

    pTempPointLightComp = NEW game::PointLightComponent(); //green
    scene->addComponent(pTempPointLightComp);
    pTempPointLightComp->setOffset(vec3(-37.718f, 23.483f, -17.693f));
    pTempPointLightComp->setMultiplier(6.0f);
    pTempPointLightComp->setColor(Color((byte)209, 243, 214, 255));
    pTempPointLightComp->setAttenuation(0, 39.6f);

    pTempPointLightComp = NEW game::PointLightComponent(); //small red
    scene->addComponent(pTempPointLightComp);
    pTempPointLightComp->setOffset(vec3(-20.946f, 3.02f, -13.92f));
    pTempPointLightComp->setMultiplier(6.0f);
    pTempPointLightComp->setColor(Color((byte)238, 132, 115, 255));
    pTempPointLightComp->setAttenuation(0, 11.9f);

    pTempPointLightComp = NEW game::PointLightComponent(); //whiteblue side
    scene->addComponent(pTempPointLightComp);
    pTempPointLightComp->setOffset(vec3(-18.697f, 5.963f, -63.635f));
    pTempPointLightComp->setMultiplier(4.0f);
    pTempPointLightComp->setColor(Color((byte)210, 223, 252, 255));
    pTempPointLightComp->setAttenuation(0, 25.798f);

    Random rand;
    for (int i(0); i < 5; ++i) //back
    {
        pTempPointLightComp = NEW game::PointLightComponent(); 
        scene->addComponent(pTempPointLightComp);
        pTempPointLightComp->setOffset(vec3(65.492f, 5.963f, -63.11f + 29.139f*i));
        pTempPointLightComp->setMultiplier(4.0f);
        pTempPointLightComp->setColor(Color((byte)246, 240, 194, 255));
        pTempPointLightComp->setAttenuation(0, 18.57f);
        if (rand.nextInt(0, 3) == 0)
            pTempPointLightComp->setBroken(true);
    }
    for (int i(0); i < 2; ++i) //back side
    {
        pTempPointLightComp = NEW game::PointLightComponent(); 
        scene->addComponent(pTempPointLightComp);
        pTempPointLightComp->setOffset(vec3(41.317f - 24.175f * i, 5.963f, 53.443f));
        pTempPointLightComp->setMultiplier(4.0f);
        pTempPointLightComp->setColor(Color((byte)246, 240, 194, 255));
        pTempPointLightComp->setAttenuation(0, 18.57f);
        if (rand.nextInt(0, 3) == 0)
            pTempPointLightComp->setBroken(true);
    }
    #pragma endregion

    m_pSkyBox = NEW gfx::SkyBox();
    m_pSkyBox->load("skybox/night/night.png");
    GRAPHICS->addToDrawList(m_pSkyBox);
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
