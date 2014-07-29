//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
//Created: 14/04/2012

#include "Happy2DPCH.h" 

#include "MainGame.h"

#include "GraphicsEngine.h"
#include "CameraManager.h"
#include "ContentManager.h"

#include "FPSGraph.h"
#include "WebView.h"
#include "Renderer2D.h"
#include "Canvas2D.h"
#include "Font.h"

#include "View.h"
#include "Window.h"

namespace ht {

MainGame::MainGame(): m_FpsGraph(nullptr), m_Window(nullptr), m_View(nullptr), m_Renderer(nullptr), m_Font(nullptr), m_WebView(nullptr)
{
}


MainGame::~MainGame()
{
    if (m_WebView != nullptr)
        m_Renderer->removeWebView(m_WebView);

    m_Renderer->detachFromRender(m_FpsGraph);
    m_Renderer->detachFromRender(this);

    m_Font->release();
    delete m_FpsGraph;

    delete m_Renderer;
    GRAPHICS->removeView(m_View);
    GRAPHICS->removeWindow(m_Window);
}

void MainGame::init()
{
    m_View = GRAPHICS->createView();
    m_Window = GRAPHICS->createWindow();

    m_Window->setResizable(true);
    m_Window->setVSync(true);
    m_Window->setWindowDimension(1280, 720);
    m_Window->setWindowTitle("Happy2D");
    he::eventCallback0<void> quitHandler(boost::bind(&he::HappyEngine::quit, HAPPYENGINE));
    m_Window->Closed += quitHandler;
    m_Window->create();
}

void MainGame::load()
{
    he::gfx::RenderSettings settings;
    CONTENT->setRenderSettings(settings);

    m_Renderer = NEW he::gfx::Renderer2D;
    m_View->addRenderPlugin(m_Renderer);

    m_View->setWindow(m_Window);
    m_View->setRelativeViewport(he::RectF(0, 0, 1.0f, 1.0f));
    m_View->init(settings);

    m_WebView = m_Renderer->createWebViewRelative(he::RectF(0, 0, 1.0f, 1.0f), true);
    m_WebView->loadUrl("http://www.google.be");
    //m_WebView->loadUrl("http://www.sebastiaansprengers.be/snake/");
    //m_WebView->loadUrl("http://www.youtube.be");

    m_Renderer->attachToRender(this);

    m_FpsGraph = NEW he::tools::FPSGraph();
    m_FpsGraph->setType(he::tools::FPSGraph::Type_TextOnly);
    m_Renderer->attachToRender(m_FpsGraph);

    m_Font = CONTENT->loadFont("Ubuntu-Bold.ttf", 18);
}

void MainGame::tick( float dTime )
{
    he::ge::Game::tick(dTime);
    m_FpsGraph->tick(dTime);
}

void MainGame::draw2D( he::gui::Canvas2D* canvas )
{
    canvas->drawImage(m_Font->getTextureAtlas(), he::vec2(20, 20));
}

} //end namespace
