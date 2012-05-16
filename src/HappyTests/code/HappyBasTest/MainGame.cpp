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
#include "ControlsManager.h"

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

MainGame::MainGame(): m_pFPSGraph(nullptr), m_pSkyBox(nullptr), m_SpinShadows(false)
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
    //m_pFPSGraph->setType(1);

    game::Entity* scene(NEW game::Entity());
    game::ModelComponent* modelComp(NEW game::ModelComponent());
    modelComp->setMaterial(CONTENT->loadMaterial("testScene3.material"));
    he::gfx::ModelMesh* mesh(CONTENT->asyncLoadModelMesh("testScene3.binobj", "M_Scene", modelComp->getMaterial().getCompatibleVertexLayout()));
    modelComp->setModelMesh(mesh->getHandle());
    mesh->release();
    scene->addComponent(modelComp);
    m_EntityList.push_back(scene);

    #pragma region lights
    GRAPHICS->getLightManager()->setAmbientLight(Color(0.6f, 1.0f, 1.0f, 1.0f), 2.0f);
    GRAPHICS->getLightManager()->setDirectionalLight(normalize(vec3(-2.0f, 5.f, 1.0f)), Color(1.0f, 0.8f, 0.5f, 1.0f), 20.0f);

    game::PointLightComponent* pTempPointLightComp(NEW game::PointLightComponent());
    scene->addComponent(pTempPointLightComp);
    pTempPointLightComp->setOffset(vec3(8.822f, 6.739f, -20.068f));
    pTempPointLightComp->setMultiplier(5.0f);
    pTempPointLightComp->setColor(Color((byte)126, 190, 255, 255));
    pTempPointLightComp->setAttenuation(0, 15);

    #pragma endregion

    m_pSkyBox = NEW gfx::SkyBox();
    m_pSkyBox->load("skybox/day/day.png");
    GRAPHICS->addToDrawList(m_pSkyBox);
}

void MainGame::tick( float dTime )
{
    he::game::Game::tick(dTime);

    if (CONTROLS->getKeyboard()->isKeyPressed(he::io::Key_Return))
        m_SpinShadows = !m_SpinShadows;

    if (m_SpinShadows)
    {
        const he::mat44 rot(he::mat44::createRotation(he::vec3::up, dTime / 4.0f));
        GRAPHICS->getLightManager()->getDirectionalLight()->setDirection(
            (rot * he::vec4(GRAPHICS->getLightManager()->getDirectionalLight()->getDirection(), 0)).xyz());
    }

    m_pFPSGraph->tick(dTime);
}

void MainGame::drawGui()
{
    m_pFPSGraph->draw();
}

} //end namespace
