//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#include "HappyEngine.h"

#include <iostream>

#include "boost/thread.hpp"
#include "Assert.h"

namespace happyengine {

HappyEngine::pointer HappyEngine::s_pHappyEngine = boost::shared_ptr<HappyEngine>(new HappyEngine());

inline void glHandleError(GLenum err)
{
    if (err != GL_NO_ERROR)
    {
        std::cout << "***GL error*** file: " << __FILE__ << " line: " << __LINE__ << "\n";
        std::cout << glewGetErrorString(err);
    }
}
inline void sdlHandleError(int err)
{
    if (err != 0)
    {
        std::cout << "***SDL error***" << __FILE__ << " line: " << __LINE__ << "\n";
        std::cout << SDL_GetError();
    }
}

HappyEngine::HappyEngine(): m_pGame(nullptr), m_Quit(false), m_Loaded(false)
{
}
HappyEngine::~HappyEngine()
{
}

void HappyEngine::quit()
{   
    //dispose/delete all sub engines here
    SDL_GL_DeleteContext(m_GLContext);
    SDL_DestroyWindow(m_pMainWindow);
    SDL_Quit();

    std::cout << "--Thank you for using HappyEngine--\n";
}
void HappyEngine::initSubEngines()
{
    //init all sub engines here
}

void HappyEngine::start(IGame* pGame)
{
    using namespace std;
    cout << "       ******************************       \n";
    cout << "  ****************        ****************  \n";
    cout << "*************  HappyEngine :D  *************\n";
    cout << "  ***************          ***************  \n";
    cout << "       ******************************       \n\n\n";

    m_pGame = pGame;

    //Init SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    //Init Game
    pGame->init();

    //Init other stuff
    initSubEngines();
    
    m_VertexQuad.push_back(VertexPosColor(8.0f, 8.0f, 0.5f, 
                                          0.0f, 0.0f, 1.0f));
    m_VertexQuad.push_back(VertexPosColor(128.0f, 8.0f, 0.5f, 
                                          1.0f, 0.0f, 1.0f));
    m_VertexQuad.push_back(VertexPosColor(8.0f, 128.0f, 0.5f, 
                                          0.0f, 1.0f, 0.0f));
    m_VertexQuad.push_back(VertexPosColor(128.0f, 128.0f, 0.5f, 
                                          1.0f, 1.0f, 1.0f));
    
    //Start draw thread
    boost::thread drawThread(boost::bind(&HappyEngine::drawThread, this));

    //Run main update loop
    Uint32 prevTicks = SDL_GetTicks();
    while (m_Quit == false)
    {
        Uint32 ticks = SDL_GetTicks();
        float dTime = (ticks - prevTicks) / 1000.0f;
        prevTicks = ticks;

        if (m_Loaded)
            m_pGame->tick(dTime);

        SDL_Delay(12);
    }
    
    drawThread.join();

    quit();
}
void HappyEngine::drawThread()
{
    initWindow();
    m_pGame->load();

    m_Loaded = true;
    Uint32 prevTicks = SDL_GetTicks();
    SDL_Event event;
    while (m_Quit == false)
    {
        Uint32 ticks = SDL_GetTicks();
        float dTime = (ticks - prevTicks) / 1000.0f;
        prevTicks = ticks;
        
        while (SDL_PollEvent(&event)) //Events are window related ==> need to be in the same thread
        {
            switch (event.type)
            {
                case SDL_QUIT: m_Quit = true; break;
            }
        }
        glViewport(0, 0, 1280, 720);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);                 

        m_pGame->draw(dTime);

        SDL_GL_SwapWindow(m_pMainWindow);
    }
}

void HappyEngine::initWindow()
{
    sdlHandleError(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2));
    sdlHandleError(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1));

    sdlHandleError(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
    sdlHandleError(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24));

    m_pMainWindow = SDL_CreateWindow("Happy Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    
    m_GLContext = SDL_GL_CreateContext(m_pMainWindow);
    if (SDL_GL_MakeCurrent(m_pMainWindow, m_GLContext) != 0)
        std::cout << SDL_GetError();

    glHandleError(glewInit());

    //VSync
    sdlHandleError(SDL_GL_SetSwapInterval(1));

    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
    glClearDepth(1.0f);
}

HappyEngine::pointer HappyEngine::getPointer()
{
    return s_pHappyEngine;
}

} //end namespace
