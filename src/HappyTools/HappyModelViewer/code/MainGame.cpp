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
//Created: 15/04/2012
#include "HappyModelViewerPCH.h" 

#include "MainGame.h"

#include "GraphicsEngine.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "ContentManager.h"

#include "FlyCamera.h"
#include "FPSGraph.h"

#include "Entity.h"
#include "ModelComponent.h"

namespace hmv {

MainGame::MainGame(const std::string& file): m_pFPSGraph(nullptr)
{
    std::stringstream stream;
    stream << "copy /Y \"";
    stream << file;
    stream << "\" \"";
    stream << CONTENT->getRootDir();
    stream << CONTENT->getModelFolder();
    stream << "temp.binobj\"";
    system(stream.str().c_str());
}


MainGame::~MainGame()
{
    std::for_each(m_Entities.cbegin(), m_Entities.cend(), [](he::game::Entity* entity)
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
    CAMERAMANAGER->addCamera("default", NEW FlyCamera(GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight()));
    CAMERAMANAGER->setActiveCamera("default");
    CAMERAMANAGER->getActiveCamera()->setLens(GRAPHICS->getScreenHeight() / (float)GRAPHICS->getScreenWidth(), he::piOverFour, 1.0f, 1000.0f);

    m_pFPSGraph = NEW he::tools::FPSGraph();
    m_pFPSGraph->setType(1);

    GRAPHICS->getLightManager()->setDirectionalLight(
        normalize(he::vec3(1, 5, 1)), he::Color(0.8f, 0.8f, 0.6f), 10.0f);
    GRAPHICS->getLightManager()->setAmbientLight(
        he::Color(0.6f, 0.8f, 0.8f), 5.0f);

    he::gfx::Material material(CONTENT->loadMaterial("default.material"));
    he::gfx::Model* model(CONTENT->asyncLoadModel("temp.binobj", material.getCompatibleVertexLayout()));
    model->callbackOnceIfLoaded([&, model, material]()
    {
        std::vector<he::game::Entity*>& entities(m_Entities);
        he::gfx::Material matRef(material);
        float maxRadius(0.0f);
        he::vec3 center;
        std::for_each(model->cbegin(), model->cend(), [&matRef, &entities, &maxRadius, &center](he::gfx::ModelMesh* mesh)
        {
            he::game::Entity* ent = NEW he::game::Entity();
            he::game::ModelComponent* modelComp(NEW he::game::ModelComponent());
            modelComp->setMaterial(matRef);
            modelComp->setModelMesh(mesh->getHandle());
            ent->addComponent(modelComp);
            entities.push_back(ent);
            if (maxRadius < mesh->getBoundingSphere().getRadius())
            {
                maxRadius = mesh->getBoundingSphere().getRadius();
                center = mesh->getBoundingSphere().getPosition();
            }
        });
        
        model->release();

        CAMERAMANAGER->getActiveCamera()->lookAt(center + he::vec3(-1, 1, 1) * maxRadius*1.5f, center, he::vec3::up);

        he::game::Entity* box = NEW he::game::Entity();
        he::game::ModelComponent* boxModelComp(NEW he::game::ModelComponent());
        boxModelComp->setMaterial(matRef);
        he::gfx::ModelMesh* boxMesh(CONTENT->asyncLoadModelMesh("openbox.binobj", "M_Box", matRef.getCompatibleVertexLayout()));
        boxModelComp->setModelMesh(boxMesh->getHandle());
        boxMesh->release();
        box->addComponent(boxModelComp);
        box->setWorldMatrix(he::mat44::createScale(maxRadius * 10.0f) * he::mat44::createTranslation(center));
        entities.push_back(box);
    });

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
