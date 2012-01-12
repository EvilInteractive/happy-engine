//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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


#include "MainGame.h"

#include <vector>

#include "HappyPCH.h"

#include "MathConstants.h"
#include "HappyEngine.h"
#include "MathFunctions.h"
#include "GraphicsEngine.h"
#include "ContentManager.h"
#include "Console.h"
#include "ControlsManager.h"
#include "Happy2DRenderer.h"
#include "IniReader.h"
#include "FileNotFoundException.h"
#include "LightManager.h"
#include "CameraManager.h"
#include "Deferred3DRenderer.h"
#include "ExtraForward3DRenderer.h"
#include "FlyCamera.h"

namespace happysandbox {

MainGame::MainGame(QWidget* parent) :   HappyQtWidget(parent), m_pBaseGrid(nullptr),
                                        m_pSceneInfo(nullptr), m_pFxEditorBinding(NEW he::tools::HappyFxEditorBinding()),
                                        m_pTransFormTools(nullptr)
{
}


MainGame::~MainGame()
{
    CAMERAMANAGER->deleteAllCameras();

    delete m_pBaseGrid;
    delete m_pFxEditorBinding;
    delete m_pSceneInfo;
    delete m_pTransFormTools;
}

void MainGame::init()
{
    using namespace he;

    io::IniReader reader;

    try { reader.open("../data/settings.ini"); }
    catch (err::FileNotFoundException& e)
    { std::wcout << e.getMsg() << "\n"; }

    if (reader.isOpen())
    {
        vec2 windowDim(reader.readVector2(L"Window", L"dimension", vec2(1280, 720)));
        GRAPHICS->setScreenDimension(static_cast<int>(windowDim.x), static_cast<int>(windowDim.y));
        GRAPHICS->setWindowTitle(reader.readString(L"Window", L"title", "Test"));

        vec4 bColor(reader.readVector4(L"Background", L"color"));
        GRAPHICS->setBackgroundColor(Color(bColor.x, bColor.y, bColor.z, bColor.w));

        GRAPHICS->setVSync(reader.readBool(L"Graphics", L"vsync", true));
        GRAPHICS->toggleFullscreen(reader.readBool(L"Graphics", L"fullscreen"));
    }
}

void MainGame::load()
{
    using namespace he;

    GRAPHICS->setBackgroundColor(he::Color(0.0f,0.0f,0.0f,0.0f));

    /* LIGHT */
    GRAPHICS->getLightManager()->setAmbientLight(Color(0.9f, 1.0f, 1.0f, 1.0f), 0.3f);
    GRAPHICS->getLightManager()->setDirectionalLight(normalize(vec3(-0.5f, 2.0f, -1.0f)), Color(1.0f, 1.0f, 0.8f, 1.0f), 1.0f);

    /* GRID */
    m_pBaseGrid = NEW he::tools::Grid(he::vec3(0,0,0), 100, 1.0f);
    m_pBaseGrid->setColor(Color(0.2f,0.2f,0.2f), Color(0.6f,0.6f,0.6f));

    GRAPHICS->initPicking();

    /* CAMERA */
    FlyCamera* pCamera(NEW FlyCamera(GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight()));
    pCamera->lookAt(vec3(5, 2, 4), vec3::zero, vec3::up);
    pCamera->setLens(16.0f/9.0f,piOverFour,0.1f,1000.0f);
    CAMERAMANAGER->addCamera("default", pCamera);
    CAMERAMANAGER->setActiveCamera("default");

    /* SSAO */
    /*he::gfx::Deferred3DRenderer::SSAOSettings settings;

    settings.radius = 0.25f;
    settings.intensity = 4.0f;
    settings.scale = 2.0f;
    settings.bias = 0.03f;

    GRAPHICS->getDeferredRenderer()->setSSAOSettings(settings);*/

    /* GUI */
    GUI->setBlending(true);

    /* SCENEINFO */
    m_pSceneInfo = NEW SceneInfo();
    GAME->addToTickList(m_pSceneInfo);

    /* TRANSFORM TOOLS */
    m_pTransFormTools = NEW TransformTools();
    GAME->addToTickList(m_pTransFormTools);

    /* TEST */
    m_pTest = CONTENT->asyncLoadTexture("editor/light.png");
}

void MainGame::tick(float dTime)
{
    using namespace he;

    game::Game::tick(dTime); //tick all components
}

void MainGame::drawGui()
{

    /* TEMP */
    HE3DX->begin(CAMERAMANAGER->getActiveCamera());
        m_pBaseGrid->draw();
        m_pTransFormTools->draw();
        HE3DX->drawBillboard(m_pTest, he::vec3(-3.0f,5.0f,-2.0f));
    HE3DX->end();

    m_pSceneInfo->draw();
}

} //end namespace