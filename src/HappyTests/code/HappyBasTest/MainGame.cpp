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
#include "Renderer2D.h"
#include "ShapeRenderer.h"

#define CONE_VERTICES 16

namespace ht {

MainGame::MainGame(): m_FpsGraph(nullptr), m_pSkyBox(nullptr), m_SpinShadows(false)
{
}


MainGame::~MainGame()
{
    m_View2->get2DRenderer()->detachFromRender(m_FpsGraph);
    delete m_FpsGraph;
    delete m_pSkyBox;
    std::for_each(m_EntityList.cbegin(), m_EntityList.cend(), [&](he::ge::Entity* entity)
    {
        delete entity;     
    });

    CONSOLE->setView(nullptr);
    GRAPHICS->removeView(m_View);
    GRAPHICS->removeView(m_View2);

    GRAPHICS->removeWindow(m_Window);
    GRAPHICS->removeWindow(m_Window2);

    m_Scene->getCameraManager()->deleteAllCameras();
    GRAPHICS->removeScene(m_Scene);
}

void MainGame::init()
{
    m_View = GRAPHICS->createView();
    m_Scene = GRAPHICS->createScene();
    m_Window = GRAPHICS->createWindow();

    m_View2 = GRAPHICS->createView();
    m_Window2 = GRAPHICS->createWindow();

    m_Window->setResizable(true);
    m_Window->setVSync(false);
    m_Window->setWindowDimension(1280, 720);
    m_Window->setWindowTitle("HappyBasTest");
    he::eventCallback0<void> quitHandler(boost::bind(&he::HappyEngine::quit, HAPPYENGINE));
    m_Window->Closed += quitHandler;
    m_Window->create();
}

void MainGame::load()
{
    using namespace he;

    #pragma region Window/View/Scene
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
    settings.postSettings.shaderSettings.enableFog = false;
    settings.postSettings.shaderSettings.enableHDR = false;
    settings.postSettings.shaderSettings.enableNormalEdgeDetect = false;
    settings.postSettings.shaderSettings.enableVignette = true;

    CONTENT->setRenderSettings(settings);

    m_View->setScene(m_Scene);
    m_View->setWindow(m_Window);
    m_View->setRelativeViewport(he::RectF(0, 0, 1.0f, 1.0f));
    m_View->init(settings);


    m_Window2->setResizable(true);
    m_Window2->setVSync(false);
    m_Window2->setWindowDimension(720, 720);
    m_Window2->setWindowTitle("HappyBasTest - 2");
    m_Window2->create();

    m_View2->setScene(m_Scene);
    m_View2->setWindow(m_Window2);
    m_View2->setRelativeViewport(he::RectF(0, 0, 1.0f, 1.0f));
    m_View2->init(settings);

    CONSOLE->setView(m_View2);
  
    FlyCamera* flyCamera = NEW FlyCamera();
    m_Scene->getCameraManager()->addCamera("default", flyCamera);
    flyCamera->setLens(1280/720.0f, piOverTwo / 3.0f * 2.0f, 1.0f, 100.0f);
    flyCamera->lookAt(vec3(), vec3(1, 0, 0), vec3(0, 1, 0));

    FlyCamera* flyCamera2 = NEW FlyCamera();
    m_Scene->getCameraManager()->addCamera("default2", flyCamera2);
    flyCamera2->setLens(1280/720.0f, piOverTwo / 3.0f * 2.0f, 1.0f, 1000.0f);
    flyCamera2->lookAt(vec3(), vec3(1, 0, 0), vec3(0, 1, 0));

    m_View->setCamera("default");
    m_View2->setCamera("default2");

    m_FpsGraph = NEW tools::FPSGraph();
    m_FpsGraph->setView(m_View);
    m_FpsGraph->setPos(vec2(8, 8));
    m_FpsGraph->setType(tools::FPSGraph::Type_ToConsole);
    m_View->get2DRenderer()->attachToRender(m_FpsGraph);

    m_View->getShapeRenderer()->attachToRenderer(this);
    m_View2->getShapeRenderer()->attachToRenderer(this);

    #pragma endregion

    #pragma region Scene
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

    he::ObjectHandle cubeMat(CONTENT->loadMaterial("cube.material"));
    mesh = CONTENT->asyncLoadModelMesh("cube.binobj", "M_Cube", modelComp->getMaterial()->getCompatibleVertexLayout());

    for (size_t x(0); x < 20; ++x)
    for (size_t y(0); y < 20; ++y)
    for (size_t z(0); z < 20; ++z)
    {
        modelComp = NEW ge::ModelComponent();
        modelComp->setMaterial(cubeMat);
        modelComp->setModelMesh(mesh->getHandle());
        modelComp->setLocalTransform(he::mat44::createTranslation(
            he::vec3(x * 2.0f + 1, 
                     y * 2.0f + 1, 
                     z * 2.0f + 1)) * he::mat44::createScale(1));
        scene->addComponent(modelComp);
    }
    mesh->release();

    he::ResourceFactory<he::gfx::Material>::getInstance()->release(sceneMaterial);
    he::ResourceFactory<he::gfx::Material>::getInstance()->release(sceneMaterial2);
    he::ResourceFactory<he::gfx::Material>::getInstance()->release(cubeMat);

    m_EntityList.push_back(scene);
    #pragma endregion

    #pragma region Lights
    //m_Scene->getLightManager()->setAmbientLight(Color(0.9f, 1.0f, 1.0f, 1.0f), 0.5f);
    //m_Scene->getLightManager()->setDirectionalLight(normalize(vec3(-2.0f, 5.f, 1.0f)), Color(1.0f, 0.8f, 0.5f, 1.0f), 2.0f);
    m_Scene->getLightManager()->setAmbientLight(Color(0.9f, 1.0f, 1.0f, 1.0f), 0.3f);
    m_Scene->getLightManager()->setDirectionalLight(normalize(vec3(-2.0f, 5.f, 1.0f)), Color(1.0f, 0.8f, 0.5f, 1.0f), 1.0f);

    mesh = CONTENT->asyncLoadModelMesh("cube.binobj", "M_Cube", modelComp->getMaterial()->getCompatibleVertexLayout());
    for (size_t i(0); i < 50; ++i)
    {
        vec3 direction(rand() / (float)RAND_MAX * 2.0f - 1.0f, rand() / (float)RAND_MAX * 2.0f - 1.0f, rand() / (float)RAND_MAX * 2.0f - 1.0f);
        direction = normalize(direction);
        float len(33.0f);
        vec3 pos(direction * len + vec3(20, 20, 20));

        ge::PointLightComponent* pTempPointLightComp(NEW ge::PointLightComponent());
        scene->addComponent(pTempPointLightComp);
        pTempPointLightComp->setOffset(pos);
        pTempPointLightComp->setMultiplier(0.5f);
        pTempPointLightComp->setColor(Color((he::byte)(rand()%255), 128, 255, 255));
        pTempPointLightComp->setAttenuation(0, 15);

        modelComp = NEW ge::ModelComponent();
        modelComp->setMaterial(cubeMat);
        modelComp->setModelMesh(mesh->getHandle());
        modelComp->setLocalTransform(he::mat44::createTranslation(pos) * he::mat44::createScale(1));
        scene->addComponent(modelComp);
    }
    mesh->release();
    #pragma endregion

    #pragma region Camera Debug Shape
    ResourceFactory<gfx::ModelMesh>* meshFactory(ResourceFactory<gfx::ModelMesh>::getInstance());
    gfx::BufferLayout debugCameraLayout;
    debugCameraLayout.addElement(gfx::BufferElement(0, gfx::BufferElement::Type_Vec3, gfx::BufferElement::Usage_Position, sizeof(vec3), 0));
    m_CameraFrustumMeshes.push_back(meshFactory->get(meshFactory->create()));

    std::vector<he::ushort> camerafrustumIndices;

    //Front
    camerafrustumIndices.push_back(0); camerafrustumIndices.push_back(1);
    camerafrustumIndices.push_back(0); camerafrustumIndices.push_back(2);
    camerafrustumIndices.push_back(1); camerafrustumIndices.push_back(3);
    camerafrustumIndices.push_back(2); camerafrustumIndices.push_back(3);

    //Back
    camerafrustumIndices.push_back(4); camerafrustumIndices.push_back(5);
    camerafrustumIndices.push_back(4); camerafrustumIndices.push_back(6);
    camerafrustumIndices.push_back(5); camerafrustumIndices.push_back(7);
    camerafrustumIndices.push_back(6); camerafrustumIndices.push_back(7);

    //Sides
    camerafrustumIndices.push_back(0); camerafrustumIndices.push_back(4);
    camerafrustumIndices.push_back(1); camerafrustumIndices.push_back(5);
    camerafrustumIndices.push_back(2); camerafrustumIndices.push_back(6);
    camerafrustumIndices.push_back(3); camerafrustumIndices.push_back(7);

    he::Cone::generateConeIndices<he::ushort>(CONE_VERTICES, 8, camerafrustumIndices);

    std::for_each(m_CameraFrustumMeshes.cbegin(), m_CameraFrustumMeshes.cend(), [&debugCameraLayout, &camerafrustumIndices](gfx::ModelMesh* mesh)
    {
        mesh->init(debugCameraLayout, he::gfx::MeshDrawMode_Lines);
        mesh->setVertices(nullptr, 0, he::gfx::MeshUsage_Dynamic);
        mesh->setIndices(camerafrustumIndices.data(), camerafrustumIndices.size(), he::gfx::IndexStride_UShort, he::gfx::MeshUsage_Static);
        mesh->setLoaded();
    });
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
    fillDebugCameraMeshes(m_View->getCamera(), m_CameraFrustumMeshes[0]);
    m_FpsGraph->tick(dTime);
}

void MainGame::fillDebugCameraMeshes(he::gfx::CameraPerspective* camera, he::gfx::ModelMesh* mesh)
{
    m_PointBuffer.clear();

    const he::Frustum& frustum(camera->getBound().getFrustum());
    frustum.generateFrustumPoints(m_PointBuffer);

    const he::Cone& cone(camera->getBound().getCone());
    cone.generateConeVertices(CONE_VERTICES, m_PointBuffer);

    mesh->setVertices(m_PointBuffer.data(), m_PointBuffer.size(), he::gfx::MeshUsage_Dynamic);

    m_PointBuffer.clear();
}

void MainGame::drawShapes( he::gfx::ShapeRenderer* renderer )
{
    std::for_each(m_CameraFrustumMeshes.cbegin(), m_CameraFrustumMeshes.cend(), [renderer](he::gfx::ModelMesh* mesh)
    {
        renderer->drawMeshColor(mesh, he::mat44::Identity, he::Color(1.0f, 0, 0, 1));
    });
}

} //end namespace
