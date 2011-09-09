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

#ifndef _HE_GRAPHICS_ENGINE_H_
#define _HE_GRAPHICS_ENGINE_H_
#pragma once

#include "Rect.h"
#include "Vector2.h"
#include "HappyTypes.h"
#include "Color.h"

#include "SDL.h"
#include <string>

namespace happyengine {
namespace graphics {

class GraphicsEngine
{
public:
	GraphicsEngine();
    virtual ~GraphicsEngine();

    void init();

    void setScreenPosition(int x, int y);
    void getScreenPosition(int& x, int& y) const;
    const RectI& getScreenRect() const;
    void setScreenDimension(uint width, uint height);
    uint getScreenWidth() const;
    uint getScreenHeight() const;

    void toggleFullscreen(bool isFullscreen);
    void setWindowTitle(const std::string& caption);

    void setViewport(const RectI& viewport);
    const RectI& getViewport() const;

    void setVSync(bool enable);

    void setBackgroundColor(const Color& color);
    void clearAll() const;
    void clearColor() const;
    void clearDepth() const;

    void present() const;

private:
    void initWindow();

    SDL_Window* m_pMainWindow;
    SDL_GLContext m_GLContext;

    RectI m_Viewport;
    RectI m_ScreenRect;

    bool m_IsFullScreen;
    bool m_VSyncEnabled;
    std::string m_WindowTitle;

    Color m_ClearColor;

    //Disable default copy constructor and default assignment operator
    GraphicsEngine(const GraphicsEngine&);
    GraphicsEngine& operator=(const GraphicsEngine&);
};

} } //end namespace

#endif
