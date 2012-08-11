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
//Author:  Sebastiaan Sprengers
//Created: 14/04/2012

#include "Happy2DPCH.h" 

#include "MainGame.h"

#include "GraphicsEngine.h"
#include "CameraManager.h"
#include "ContentManager.h"

#include "Camera.h"
#include "FPSGraph.h"
#include "WebView.h"
#include "Renderer2D.h"
#include "Canvas2D.h"
#include "Font.h"

namespace ht {

MainGame::MainGame(): m_FpsGraph(nullptr), m_pWebView(nullptr), m_pCanvas(nullptr), m_pFont(nullptr)
{
}


MainGame::~MainGame()
{
    CAMERAMANAGER->deleteAllCameras();

    m_pFont->release();

    delete m_pWebView;
    delete m_pCanvas;
    delete m_FpsGraph;
}

void MainGame::init()
{
    GRAPHICS->setVSync(false);
    GRAPHICS->setScreenDimension(1280, 720);
    GRAPHICS->setViewport(he::RectI(0, 0, 1280, 720));
}

void MainGame::load()
{
    CAMERAMANAGER->addCamera("default", NEW he::gfx::CameraPerspective(GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight()));
    CAMERAMANAGER->setActiveCamera("default");

    //m_pWebView = GUI_NEW->createWebView(true);
    //m_pWebView->loadUrl("http://www.google.be");
    //m_pWebView->loadUrl("http://www.sebastiaansprengers.be/snake/");
    //m_pWebView->loadUrl("http://www.youtube.be");

    m_FpsGraph = NEW he::tools::FPSGraph();
    //m_FpsGraph->setType(1);

    m_pFont = CONTENT->loadFont("Ubuntu-Bold.ttf", 18);

    m_pCanvas = GUI->createCanvas();
}

void MainGame::tick( float dTime )
{
    he::ge::Game::tick(dTime);
    m_FpsGraph->tick(dTime);
}

void MainGame::drawGui()
{
    //m_pWebView->draw();

    //m_pCanvas->save();
    //m_pCanvas->scale(he::vec2(2,2));
    //m_pCanvas->translate(he::vec2(500,200));
    //m_pCanvas->setFillColor(he::Color(0.8f,0.2f,0.0f));
    //m_pCanvas->fillRect(he::vec2(450,250), he::vec2(100,100));
    //m_pCanvas->restore();

    //m_pCanvas->setFillColor(he::Color(0.2f,0.5f,0.8f));
    //m_pCanvas->fillText(he::gui::Text("Testing blie bloe bla Qede", m_pFont), he::vec2(250,300));
    
    //m_pCanvas->drawImage(m_pFont->getTextureAtlas(), he::vec2(0,0));//, he::vec2(20,20), he::RectF(320,5,20,20));
    m_pCanvas->drawImage(m_pFont->getTextureAtlas(), he::vec2(20, 20));

    m_pCanvas->draw();

    m_FpsGraph->draw();
}

} //end namespace
