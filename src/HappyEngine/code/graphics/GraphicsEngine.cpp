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
//
//Author:  Bastian Damman
//Created: 04/08/2011
#include "StdAfx.h" 

#include "GraphicsEngine.h"

#include "OpenGL.h"
#include "ExternalError.h"

#include "HappyNew.h"

#include "IniReader.h"

namespace he {
namespace gfx {

GraphicsEngine::GraphicsEngine(): m_pMainWindow(nullptr), 
                                  m_ScreenRect(-1, -1, 1280, 720),
                                  m_Viewport(0, 0, 1280, 720),
                                  m_IsFullScreen(false),
                                  m_WindowTitle("HappyEngine"),
                                  m_VSyncEnabled(true),
                                  m_pDrawManager(NEW DrawManager())
{
}


GraphicsEngine::~GraphicsEngine()
{
    delete m_pDrawManager;
    delete m_pDeferred3DRenderer;
    SDL_GL_DeleteContext(m_GLContext);
    SDL_DestroyWindow(m_pMainWindow);
}
void GraphicsEngine::init()
{
    using namespace err;
    sdlHandleError(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3));
    sdlHandleError(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2));

    sdlHandleError(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
    sdlHandleError(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24));

    initWindow();
    glewExperimental = true;
    glHandleError(glewInit());

    setVSync(m_VSyncEnabled);
    setViewport(m_Viewport);

    setBackgroundColor(m_ClearColor);
    glClearDepth(1.0f);

    GL::heSetDepthRead(true);
    GL::heSetDepthWrite(true);
    GL::heSetDepthFunc(DepthFunc_LessOrEqual);
    GL::heSetWindingFrontFace(true);
    GL::heSetCullFace(false);
    glEnable(GL_CULL_FACE);


    io::IniReader reader;
    reader.open("gfxSettings.ini");

    DrawSettings settings;
    settings.setBloomEnabled(reader.readBool(L"GFX", L"bloom", false));
    settings.setShadowQuality((ShadowQuality)reader.readInt(L"GFX", L"shadowQuality", 1));

    m_pDeferred3DRenderer = NEW Deferred3DRenderer(settings);
    m_pDrawManager->init(settings);
}
void GraphicsEngine::initWindow()
{
    m_pMainWindow = SDL_CreateWindow(m_WindowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_ScreenRect.width, m_ScreenRect.height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    
    int x, y;
    SDL_GetWindowPosition(m_pMainWindow, &x, &y);
    if (m_ScreenRect.x == -1)
        m_ScreenRect.x = x;
    if (m_ScreenRect.y == -1)
        m_ScreenRect.y = y;

    SDL_SetWindowPosition(m_pMainWindow, m_ScreenRect.x, m_ScreenRect.y);
    SDL_SetWindowFullscreen(m_pMainWindow, static_cast<SDL_bool>(m_IsFullScreen));

    m_GLContext = SDL_GL_CreateContext(m_pMainWindow);
    err::sdlHandleError(SDL_GL_MakeCurrent(m_pMainWindow, m_GLContext));
}

void GraphicsEngine::setScreenPosition(int x, int y)
{
    m_ScreenRect.x = x;
    m_ScreenRect.y = y;
    if (m_pMainWindow != nullptr)
        SDL_SetWindowPosition(m_pMainWindow, x, y);
}
void GraphicsEngine::getScreenPosition(int& x, int& y) const
{
    x = m_ScreenRect.x;
    y = m_ScreenRect.y;
}
const RectI& GraphicsEngine::getScreenRect() const
{
    return m_ScreenRect;
}
void GraphicsEngine::setScreenDimension(uint width, uint height)
{
    m_ScreenRect.width = width;
    m_ScreenRect.height = height;
    if (m_pMainWindow != nullptr)
        SDL_SetWindowSize(m_pMainWindow, static_cast<int>(width), static_cast<int>(height));
}
uint GraphicsEngine::getScreenWidth() const
{
    return m_ScreenRect.width;
}
uint GraphicsEngine::getScreenHeight() const
{
    return m_ScreenRect.height;
}

void GraphicsEngine::toggleFullscreen(bool isFullscreen)
{
    m_IsFullScreen = isFullscreen;
    if (m_pMainWindow != nullptr)
        SDL_SetWindowFullscreen(m_pMainWindow, static_cast<SDL_bool>(isFullscreen));
}
void GraphicsEngine::setWindowTitle(const std::string& title)
{
    m_WindowTitle = title;
    if (m_pMainWindow != nullptr)
        SDL_SetWindowTitle(m_pMainWindow, title.c_str());
}

void GraphicsEngine::setViewport(const RectI& viewport)
{
    m_Viewport = viewport;
    glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
}
const RectI& GraphicsEngine::getViewport() const
{
    return m_Viewport;
}

void GraphicsEngine::setVSync(bool enable)
{
    if (m_pMainWindow != nullptr)
    {
        if (enable)
            err::sdlHandleError(SDL_GL_SetSwapInterval(1));
        else
            err::sdlHandleError(SDL_GL_SetSwapInterval(0));
        m_VSyncEnabled = enable;
    }
    else
    {
        m_VSyncEnabled = enable;
    }
}

void GraphicsEngine::setBackgroundColor(const Color& color)
{
    GL::heClearColor(color);
    m_ClearColor = color;
}
void GraphicsEngine::clearAll() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void GraphicsEngine::clearColor() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}
void GraphicsEngine::clearDepth() const
{
    glClear(GL_DEPTH_BUFFER_BIT);
}

void GraphicsEngine::begin(const Camera* pCamera)
{
    GL::reset();
    m_pCurrentCamera = pCamera;
    m_pDeferred3DRenderer->begin(pCamera);
    m_pDrawManager->draw(pCamera);
}
void GraphicsEngine::end()
{
    m_pDeferred3DRenderer->end();
}
void GraphicsEngine::draw(const ModelMesh::pointer& pModelMesh)
{
    GL::heBindVao(pModelMesh->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, pModelMesh->getNumIndices(), pModelMesh->getIndexType(), 0);
}
void GraphicsEngine::present() const
{    
    SDL_GL_SwapWindow(m_pMainWindow);
}
LightManager* GraphicsEngine::getLightManager() const
{
    return m_pDeferred3DRenderer->getLightManager();
}
const DrawSettings& GraphicsEngine::getSettings() const
{
    return m_Settings;
}

void GraphicsEngine::addToDrawList( const IDrawable* pDrawable )
{
    m_pDrawManager->addDrawable(pDrawable);
}

} } //end namespace