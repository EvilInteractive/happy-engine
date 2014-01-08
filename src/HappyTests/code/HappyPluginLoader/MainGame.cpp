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
//Created: 09/03/2013

#include "HappyPluginLoaderPCH.h" 

#include "MainGame.h"

#include <Window.h>
#include <GraphicsEngine.h>
#include <PluginLoader.h>
#include <IPlugin.h>
#include <EntityManager.h>
#include <EngineEntityComponentFactory.h>
#include <FPSGraph.h>
#include <RenderSettings.h>
#include <Renderer2D.h>
#include <View.h>
#include <Text.h>
#include <ContentManager.h>
#include <GlobalSettings.h>

namespace ht {

MainGame::MainGame()
    : m_Window(nullptr)
    , m_PluginLoader(nullptr)
    , m_Plugin(nullptr)
    , m_Plugin2(nullptr)
    , m_View(nullptr)
    , m_DebugRenderer(nullptr)
    , m_FpsGraph(nullptr)
{
}


MainGame::~MainGame()
{
}

void MainGame::init()
{
    he::GlobalSettings* const globalSettings(he::GlobalSettings::getInstance());
    globalSettings->load(he::Path("settings.cfg"));
    globalSettings->save(he::Path("settings.cfg"));

    he::gfx::CameraSettings cameraSettings;
    cameraSettings.setRelativeViewport(he::RectF(0, 0, 1, 1));

    he::gfx::GraphicsEngine* const graphicsEngine(GRAPHICS);
    m_Window = graphicsEngine->createWindow();

    const bool oculus(globalSettings->getRenderSettings().stereoSetting == he::gfx::StereoSetting_OculusRift);

    m_Window->setResizable(true);
    m_Window->setVSync(false);
    m_Window->setWindowDimension(1280, 800);
    m_Window->setWindowTitle("lHappyPluginTest");
    m_Window->setFullscreen(false);
    m_Window->setOculusRiftEnabled(oculus);
    he::eventCallback0<void> quitHandler(boost::bind(&he::HappyEngine::quit, HAPPYENGINE));
    m_Window->Closed += quitHandler;
    m_Window->create();

    he::ge::EntityManager* const entityMan(he::ge::EntityManager::getInstance());
    entityMan->installComponentFactory(NEW he::ge::EngineEntityComponentFactory());
    entityMan->init();

    m_PluginLoader = NEW he::pl::PluginLoader();
    m_Plugin = m_PluginLoader->loadPlugin(he::Path(""), "HappyPluginTest");
    if (m_Plugin != nullptr)
    {
        m_Plugin->init(m_Window, he::RectF(0, 0, 1.0f, 1.0f));
        m_Plugin->onLoadLevel(he::Path(""));
        
        m_View = graphicsEngine->createView();
        m_View->setWindow(m_Window);
        m_DebugRenderer = NEW he::gfx::Renderer2D();
        m_View->addRenderPlugin(m_DebugRenderer);
        m_View->init(cameraSettings);
        m_View->setCamera(m_Plugin->getActiveCamera());
        
        PROFILER->attachToRenderer(m_DebugRenderer);
        CONSOLE->attachToRenderer(m_DebugRenderer);
        m_FpsGraph = NEW he::tools::FPSGraph(oculus? 3.0f : 1.0f);
        m_FpsGraph->setPos(he::vec2(5, 5));
        m_FpsGraph->setType(he::tools::FPSGraph::Type_Full);
        addToTickList(m_FpsGraph);
        m_DebugRenderer->attachToRender(m_FpsGraph);
        m_DebugRenderer->attachToRender(this);
    }
    else
    {
        HAPPYENGINE->quit();
    }
}

void MainGame::destroy()
{
    if (m_Plugin != nullptr)
    {
        m_Plugin->onUnloadLevel();
        m_Plugin->terminate();
        m_PluginLoader->unloadPlugin(m_Plugin);
        m_Plugin = nullptr;
    }

    if (m_Plugin2 != nullptr)
    {
        m_Plugin2->onUnloadLevel();
        m_Plugin2->terminate();
        m_PluginLoader->unloadPlugin(m_Plugin2);
        m_Plugin2 = nullptr;
    }

    delete m_PluginLoader;
    m_PluginLoader = nullptr;

    he::ge::EntityManager::getInstance()->destroy();

    PROFILER->detachFromRenderer();
    CONSOLE->detachFromRenderer();
    m_DebugRenderer->detachFromRender(m_FpsGraph);
    removeFromTickList(m_FpsGraph);
    delete m_FpsGraph;
    m_FpsGraph = nullptr;
    m_DebugRenderer->detachFromRender(this);

    he::gfx::GraphicsEngine* const graphicsEngine(GRAPHICS);
    graphicsEngine->removeView(m_View);
    m_View = nullptr;
    delete m_DebugRenderer;
    m_DebugRenderer = nullptr;
    graphicsEngine->removeWindow(m_Window);
    m_Window = nullptr;
}

void MainGame::draw2D( he::gui::Canvas2D* /*canvas*/ )
{
}

} //end namespace
