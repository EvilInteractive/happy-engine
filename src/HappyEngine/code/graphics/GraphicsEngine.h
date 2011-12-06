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
#include "Deferred3DRenderer.h"
#include "IDrawable.h"

#include "SDL.h"
#include <string>

#include "DrawManager.h"
#include "DrawSettings.h"
#include "Camera.h"
#include "Picker.h"

namespace he {
namespace gfx {

class GraphicsEngine
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    GraphicsEngine();
    virtual ~GraphicsEngine();

    /* GENERAL */
    void init(bool useQt);
    // only init when needed, because it requires extra FBO & shader
    void initPicking();

    void clearAll() const;
    void clearColor() const;
    void clearDepth() const;

    void addToDrawList(const IDrawable* pDrawable);

    void begin();
    void end();
    void draw(const ModelMesh::pointer& pModelMesh);
    void present() const;

    void toggleFullscreen(bool isFullscreen);

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
    const DrawSettings& getSettings() const;
    Deferred3DRenderer* getDeferredRenderer() const;
    const std::vector<const IDrawable*>& getDrawList() const;

    uint pick(const vec2& screenPoint);

private:

    void initWindow();

    /* DATAMEMBERS */
    SDL_Window* m_pMainWindow;
    SDL_GLContext m_GLContext;

    RectI m_Viewport;
    RectI m_ScreenRect;

    bool m_IsFullScreen;
    bool m_VSyncEnabled;
    std::string m_WindowTitle;

    Color m_ClearColor;
    Deferred3DRenderer* m_pDeferred3DRenderer;

    DrawManager* m_pDrawManager;
    DrawSettings m_Settings;
    
    Picker* m_pPicker;

    /* DEFAULT COPY & ASSIGNMENT */
    GraphicsEngine(const GraphicsEngine&);
    GraphicsEngine& operator=(const GraphicsEngine&);
};

} } //end namespace

#endif
