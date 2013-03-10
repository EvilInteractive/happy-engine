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
//Created: 2013/03/03

#include "HappyPluginTestPCH.h" 
#include "HappyPluginTestMain.h"

#include <StaticDataManager.h>
#include <GraphicsEngine.h>
#include <ContentManager.h>
#include <Window.h>
#include <Scene.h>
#include <View.h>
#include <DefaultRenderPipeline.h>
#include "FlyCamera.h"

ht::HappyPluginTestMain::HappyPluginTestMain()
    : m_View(nullptr)
    , m_Scene(nullptr)
    , m_RenderPipeline(nullptr)
    , m_Camera(nullptr)
{

}

ht::HappyPluginTestMain::~HappyPluginTestMain()
{

}

void ht::HappyPluginTestMain::init(he::gfx::Window* const window, const he::RectI& viewport)
{
    HE_INFO("HappyPluginTestMain init");
    he::gfx::GraphicsEngine* const graphicsEngine(GRAPHICS);
    m_View = graphicsEngine->createView();
    m_Scene = graphicsEngine->createScene();

    he::gfx::RenderSettings settings;
    CONTENT->setRenderSettings(settings);

    m_RenderPipeline = NEW he::ge::DefaultRenderPipeline();
    m_RenderPipeline->init(m_View, m_Scene, settings);

    m_Camera = NEW FlyCamera();

    m_View->setWindow(window);
    m_View->setAbsoluteViewport(viewport);
    m_View->init(settings);
    m_View->setCamera(m_Camera);
}

void ht::HappyPluginTestMain::terminate()
{
    he::gfx::GraphicsEngine* const graphicsEngine(GRAPHICS);
    graphicsEngine->removeView(m_View);
    m_View = nullptr;
    graphicsEngine->removeScene(m_Scene);
    m_Scene = nullptr;
    delete m_Camera;
    m_Camera = nullptr;

    delete m_RenderPipeline;
    m_RenderPipeline = nullptr;

    HE_INFO("HappyPluginTestMain terminated");
}

void ht::HappyPluginTestMain::onLoadLevel( const he::Path& /*path*/ )
{

}

void ht::HappyPluginTestMain::onUnloadLevel()
{

}

void ht::HappyPluginTestMain::onLevelLoaded()
{

}

void ht::HappyPluginTestMain::onStartGame()
{

}

void ht::HappyPluginTestMain::onStopGame()
{

}

void ht::HappyPluginTestMain::onPauseGame()
{

}

void ht::HappyPluginTestMain::onResumeGame()
{

}

void ht::HappyPluginTestMain::sdmInit( he::io::BinaryVisitor& /*visitor*/ )
{
    //he::StaticDataManager::visit(visitor);
}

void ht::HappyPluginTestMain::onResize( const he::RectI& newViewport )
{
    m_View->setAbsoluteViewport(newViewport);
}
