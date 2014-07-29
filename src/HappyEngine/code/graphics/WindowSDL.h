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
//Author:  Bastian Damman
//Created: 14/07/2012

#ifndef _HE_WindowSDL_H_
#define _HE_WindowSDL_H_
#pragma once

#include "Window.h"

struct SDL_Window;

namespace he {
namespace gfx {
class GLContextSDL;

class HAPPY_ENTRY WindowSDL : public Window
{
    friend class GLContextSDL;
public:
    WindowSDL();
    virtual ~WindowSDL();

    // From WindowBase
    bool create(const bool show);
    void destroy();

    bool isOpen() const;
    bool isVisible() const { return checkFlag(eFlags_IsVisible); }
    void show();
    void hide();

    // Do
    void present();

    // Setters
    void setWindowTitle(const he::String& caption);
    void setWindowPosition(int x, int y);
    void setWindowDimension(uint32 width, uint32 height);
    void setVSync(bool enable);
    void setFullscreen(bool fullscreen);
    void setMousePosition(const vec2& pos);

    GLContext* getContext() const;
    const he::FixedString& getType() const { return HEFS::strSDLWindow; }
    // ~

    uint32 getID() const { return m_ID; }

private:
    uint32 m_ID;
    GLContextSDL* m_Context;
    SDL_Window* m_Window;

    //Disable default copy constructor and default assignment operator
    WindowSDL(const WindowSDL&);
    WindowSDL& operator=(const WindowSDL&);
};

} } //end namespace

#endif
