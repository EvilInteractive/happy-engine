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

namespace ht {

MainGame::MainGame(): m_Window(nullptr), m_PluginLoader(nullptr), m_Plugin(nullptr)
{
}


MainGame::~MainGame()
{
}

void MainGame::init()
{
    m_Window = GRAPHICS->createWindow();

    m_Window->setResizable(true);
    m_Window->setVSync(true);
    m_Window->setWindowDimension(1280, 720);
    m_Window->setWindowTitle("HappyPluginTest");
    he::eventCallback0<void> quitHandler(boost::bind(&he::HappyEngine::quit, HAPPYENGINE));
    m_Window->Closed += quitHandler;
    m_Window->create();

    m_PluginLoader = NEW he::pl::PluginLoader();
    m_Plugin = m_PluginLoader->loadPlugin(he::Path("HappyPluginTest.dll"));
    if (m_Plugin != nullptr)
    {
        m_Plugin->init(m_Window, he::RectI(0, 0, m_Window->getWindowWidth(), m_Window->getWindowHeight()));

        he::eventCallback0<void> resizeHandler([this]()
        {
            m_Plugin->onResize(he::RectI(0, 0, m_Window->getWindowWidth(), m_Window->getWindowHeight()));
        });
        m_Window->Resized += resizeHandler;
    }
}

void MainGame::destroy()
{
    m_PluginLoader->unloadPlugin(m_Plugin);
    m_Plugin = nullptr;

    delete m_PluginLoader;
    m_PluginLoader = nullptr;

    GRAPHICS->removeWindow(m_Window);
    m_Window = nullptr;
}

} //end namespace
