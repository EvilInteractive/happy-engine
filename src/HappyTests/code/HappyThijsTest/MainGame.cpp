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
//Author:  Thijs Morlion
//Created: 27/01/2013

#include "HappyThijsTestPCH.h" 

#include "MainGame.h"

#include "GraphicsEngine.h"
#include "CameraManager.h"
#include "ContentManager.h"

#include "FPSGraph.h"
#include "Renderer2D.h"
#include "Canvas2D.h"
#include "Font.h"

#include "View.h"
#include "WindowSDL.h"
#include "GlobalSettings.h"

namespace ht {

MainGame::MainGame()
	: m_FpsGraph(nullptr)
	, m_Window(nullptr)
	, m_View(nullptr)
	, m_Renderer(nullptr)
	, m_AStar(nullptr)
{
}


MainGame::~MainGame()
{
}

void MainGame::destroy()
{
    m_Renderer->detachFromRender(m_FpsGraph);
    m_Renderer->detachFromRender(this);
    CONSOLE->detachFromRenderer();
    PROFILER->detachFromRenderer();

    delete m_FpsGraph;

    delete m_Renderer;
    GRAPHICS->removeView(m_View);
    GRAPHICS->removeWindow(m_Window);    
}
void MainGame::init()
{
    he::GlobalSettings* const globalSettings(he::GlobalSettings::getInstance());
    globalSettings->load(he::Path("settings.cfg"));
    globalSettings->save(he::Path("settings.cfg"));

    m_View = GRAPHICS->createView();
    m_Window = GRAPHICS->createWindow();

    m_Window->setResizable(true);
    m_Window->setVSync(true);
    m_Window->setWindowDimension(1280, 720);
    m_Window->setWindowTitle("HappyThijsTest");
    he::eventCallback0<void> quitHandler(boost::bind(&he::HappyEngine::quit, HAPPYENGINE));
    m_Window->Closed += quitHandler;
    m_Window->create(true);

    he::gfx::CameraSettings cameraSettings;
    cameraSettings.setRelativeViewport(he::RectF(0, 0, 1.0f, 1.0f));

    m_Renderer = NEW he::gfx::Renderer2D;
    m_View->addRenderPlugin(m_Renderer);

    m_View->setWindow(m_Window);
    m_View->init(cameraSettings);

    m_Renderer->attachToRender(this);

    m_FpsGraph = NEW he::tools::FPSGraph();
    m_FpsGraph->setType(he::tools::FPSGraph::Type_TextOnly);
    m_Renderer->attachToRender(m_FpsGraph);

	m_AStar = NEW ht::AStar();
	m_AStar->init();
    
    CONSOLE->attachToRenderer(m_Renderer);
    PROFILER->attachToRenderer(m_Renderer);
}

void MainGame::tick( float dTime )
{
    he::ge::Game::tick(dTime);
    m_FpsGraph->tick(dTime);
}

void MainGame::draw2D( he::gui::Canvas2D* canvas )
{
	m_AStar->draw2D(canvas);
}

} //end namespace
