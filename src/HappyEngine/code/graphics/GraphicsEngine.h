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
//Extended:Sebastiaan Sprengers

#ifndef _HE_GRAPHICS_ENGINE_H_
#define _HE_GRAPHICS_ENGINE_H_
#pragma once

#include "Rect.h"
#include "vec2.h"
#include "HappyTypes.h"
#include "Color.h"

#include <string>

#include "RenderSettings.h"
#include "ModelMesh.h"

struct SDL_Window;

namespace he {
namespace gfx {

class DrawManager;
class InstancingManager;
class Picker;
class IDrawable;
class Deferred3DRenderer;
class LightManager;

class GraphicsEngine
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    GraphicsEngine();
    virtual ~GraphicsEngine();

    /* GENERAL */
    void init(bool useQt);

    // only init picking when needed, because it requires extra FBO & shader
    void initPicking();

    void clearAll() const;
    void clearColor() const;
    void clearDepth() const;

    void addToDrawList(IDrawable* pDrawable);
    InstancingManager* getInstancingManager() const;

    void drawScene();
    void present() const;

    void toggleFullscreen(bool isFullscreen);

    uint pick(const vec2& screenPoint);

    /* SETTERS */
    void setWindowTitle(const std::string& caption);
    void setScreenPosition(int x, int y);
    void setScreenDimension(uint width, uint height);
    void setViewport(const RectI& viewport);
    void setVSync(bool enable);
    void setBackgroundColor(const Color& color);
    
    /* GETTERS */
    void getScreenPosition(int& x, int& y) const;
    const RectI& getScreenRect() const;
    uint getScreenWidth() const;
    uint getScreenHeight() const;
    const RectI& getViewport() const;
    LightManager* getLightManager() const;
    const RenderSettings& getSettings() const;
    const DrawManager* getDrawManager() const;

private:

    void initWindow();

    /* DATAMEMBERS */
    SDL_Window* m_pMainWindow;
    void* m_GLContext;

    RectI m_Viewport;
    RectI m_ScreenRect;

    bool m_IsFullScreen;
    bool m_VSyncEnabled;
    std::string m_WindowTitle;

    Color m_ClearColor;
    InstancingManager* m_pInstancingManager;

    DrawManager* m_pDrawManager;
    LightManager* m_pLightManager;

    RenderSettings m_Settings;
    
    Picker* m_pPicker;

    /* DEFAULT COPY & ASSIGNMENT */
    GraphicsEngine(const GraphicsEngine&);
    GraphicsEngine& operator=(const GraphicsEngine&);
};

} } //end namespace

#endif
