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
//
//Author:  Bastian Damman
//Created: 04/08/2011
#include "HappyPCH.h" 

#include "GraphicsEngine.h"
#include "SDL.h"

#include "OpenGL.h"
#include "ExternalError.h"

#include "HappyNew.h"

#include "IniReader.h"
#include "Happy2DRenderer.h"
#include "CameraManager.h"

#include "Picker.h"
#include "InstancingManager.h"

#include "DrawManager.h"
#include "LightManager.h"

#include "ContentManager.h"
#include "FileNotFoundException.h"

namespace he {
namespace gfx {

GraphicsEngine::GraphicsEngine(): m_pMainWindow(nullptr), 
                                  m_ScreenRect(-1, -1, 1280, 720),
                                  m_Viewport(0, 0, 1280, 720),
                                  m_IsFullScreen(false),
                                  m_WindowTitle("HappyEngine"),
                                  m_VSyncEnabled(true),
                                  m_pDrawManager(NEW DrawManager()),
                                  m_pPicker(NEW Picker()),
                                  m_pInstancingManager(NEW InstancingManager()),
                                  m_pLightManager(NEW LightManager())
{
}


GraphicsEngine::~GraphicsEngine()
{
    delete m_pDrawManager;
    delete m_pPicker;
    delete m_pInstancingManager;
    delete m_pLightManager;
    SDL_GL_DeleteContext(m_GLContext);
    SDL_DestroyWindow(m_pMainWindow);
}
void GraphicsEngine::init(bool useQt)
{
    using namespace err;
    if (useQt == false)
    {
        sdlHandleError(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3));
        sdlHandleError(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2));

        sdlHandleError(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
        sdlHandleError(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24));

        initWindow();
    }
    glewExperimental = true;
    glHandleError(glewInit());

    HE_INFO(std::string((char*)glGetString(GL_VENDOR)));
    HE_INFO(std::string((char*)glGetString(GL_RENDERER)));
    HE_INFO(std::string((char*)glGetString(GL_VERSION)));
    HE_INFO(std::string((char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));

    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    char sMajor[5];                     char sMinor[5];
    sprintf(sMajor, "%d", major);       sprintf(sMinor, "%d", minor);   
    HE_INFO("GL version " + std::string(sMajor) + "." + std::string(sMinor));

    int doubleBuff;
    glGetIntegerv(GL_DOUBLEBUFFER, &doubleBuff);
    HE_INFO(std::string("Doubly buffered: ") + ((doubleBuff == GL_TRUE)?"TRUE":"FALSE"));

    int maxTexSize, maxRenderSize, maxRectSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderSize);
    glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE, &maxRectSize);
    char sMaxTexSize[10];                     char sMaxRenderSize[10];                       char sMaxRectSize[10];
    sprintf(sMaxTexSize, "%d", maxTexSize);   sprintf(sMaxRenderSize, "%d", maxRenderSize);  sprintf(sMaxRectSize, "%d", maxRectSize);
    HE_INFO("Max texture size: " + std::string(sMaxTexSize));
    HE_INFO("Max render size: " + std::string(sMaxRenderSize));
    HE_INFO("Max rect tex size: " + std::string(sMaxRectSize));


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
    try { reader.open("settings.ini"); }
    catch (const err::FileNotFoundException& /*e*/)
    { HE_ERROR("settings.ini not found!, using defaults"); }

    if (reader.isOpen())
    {
        m_Settings.enableBloom = reader.readBool(L"GFX", L"bloom", false);
        m_Settings.enableSSAO = reader.readBool(L"GFX", L"ssao", false);
        m_Settings.shadowMult = static_cast<byte>(clamp(reader.readInt(L"GFX", L"shadowQuality", 2), 0, 3));
        if (m_Settings.shadowMult == 0)
            m_Settings.enableShadows = false;
        else
            m_Settings.enableShadows = true;

        m_Settings.enableDeferred = reader.readBool(L"GFX", L"deferred", true);
        m_Settings.enableHDR = reader.readBool(L"GFX", L"hdr", true);

        m_Settings.enableLighting = reader.readBool(L"GFX", L"lighting", true);
        m_Settings.enableSpecular = reader.readBool(L"GFX", L"specular", true);
        m_Settings.enableNormalMap = reader.readBool(L"GFX", L"normalMap", true);

        m_Settings.enableNormalEdgeDetect = reader.readBool(L"GFX", L"normalEdge", false);
        m_Settings.enableDepthEdgeDetect = reader.readBool(L"GFX", L"depthEdge", true);
        m_Settings.enableVignette = reader.readBool(L"GFX", L"vignette", true);
        m_Settings.enableFog = reader.readBool(L"GFX", L"fog", true);

        m_Settings.exposureSpeed = reader.readFloat(L"GFX", L"exposureSpeed", 1.0f);
    }
    else
    {
        m_Settings.enableBloom = false;
        m_Settings.enableSSAO = false;
        m_Settings.shadowMult = 1;
        if (m_Settings.shadowMult == 0)
            m_Settings.enableShadows = false;
        else
            m_Settings.enableShadows = true;

        m_Settings.enableDeferred = true;
        m_Settings.enableHDR = true;

        m_Settings.enableLighting = true;
        m_Settings.enableSpecular = true;
        m_Settings.enableNormalMap = true;

        m_Settings.enableNormalEdgeDetect = false;
        m_Settings.enableDepthEdgeDetect = true;
        m_Settings.enableVignette = true;
        m_Settings.enableFog = true;

        m_Settings.exposureSpeed = 1.0f;
    }

    m_Settings.ssaoSettings.radius = 0.2f;
    m_Settings.ssaoSettings.intensity = 4.0f;
    m_Settings.ssaoSettings.scale = 2.0f;
    m_Settings.ssaoSettings.bias = 0.05f;
    m_Settings.ssaoSettings.minIterations = 4;
    m_Settings.ssaoSettings.maxIterations = 8;
    m_Settings.ssaoSettings.passes = 1;

    CONTENT->setRenderSettings(m_Settings);

    m_pDrawManager->init(m_Settings);
}
void GraphicsEngine::initWindow()
{
    m_pMainWindow = SDL_CreateWindow(m_WindowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_ScreenRect.width, m_ScreenRect.height, /*SDL_WINDOW_SHOWN |*/ SDL_WINDOW_OPENGL | SDL_RESIZABLE);
    
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
    if (m_pDrawManager != nullptr)
        m_pDrawManager->onScreenResized();
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
    GL::heBindFbo(0);
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
void GraphicsEngine::drawScene()
{
    GL::reset();
    clearAll();
    m_pDrawManager->draw();
}
void GraphicsEngine::present() const
{    
    PROFILER_BEGIN("GraphicsEngine::present");
    SDL_GL_SwapWindow(m_pMainWindow);
    PROFILER_END();
}
LightManager* GraphicsEngine::getLightManager() const
{
    return m_pLightManager;
}
const RenderSettings& GraphicsEngine::getSettings() const
{
    return m_Settings;
}

void GraphicsEngine::addToDrawList( IDrawable* pDrawable )
{
    m_pDrawManager->addDrawable(pDrawable);
}

void GraphicsEngine::initPicking()
{
    m_pPicker->initialize();
}
uint GraphicsEngine::pick(const vec2& screenPoint)
{
    return m_pPicker->pick(screenPoint);
}

InstancingManager* GraphicsEngine::getInstancingManager() const
{
    return m_pInstancingManager;
}

const DrawManager* GraphicsEngine::getDrawManager() const
{
    return m_pDrawManager;
}

} } //end namespace