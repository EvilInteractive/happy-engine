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
//Created: 14/07/2012

#ifndef _HE_Window_H_
#define _HE_Window_H_
#pragma once

#include "Singleton.h"

struct SDL_Window;

namespace he {
namespace io {
    ENUM(MouseCursor, uint8);
}
namespace gfx {
class Window;
class RenderTarget;
class Texture2D;

class WindowFactory: public ObjectFactory<Window>, public Singleton<WindowFactory>
{
    friend Singleton;
    WindowFactory() { init(1, 2, "WindowFactory"); }
    virtual ~WindowFactory() { }
};

#if defined(HE_WINDOWS)
typedef HWND__* NativeWindowHandle;
#elif defined(HE_LINUX)
typedef unsigned long NativeWindowHandle;
#elif defined(HE_MAC)
typedef void* NativeWindowHandle;
#endif

class HAPPY_ENTRY Window
{
DECLARE_OBJECT(Window)
friend class GraphicsEngine;
friend class GLContext;
enum EFlags
{
    eFlags_VSyncEnabled = 1 << 0,
    eFlags_Fullscreen = 1 << 1,
    eFlags_Resizeable = 1 << 2,
    eFlags_IsVisible = 1 << 3,
    eFlags_EnableOculusRift = 1 << 4
};
struct OculusRiftBarrelDistorter;
public:
    Window();
    virtual ~Window();

    // Open/Close
    bool create();
    void destroy();
    bool isOpen();
    
    bool isVisible() const { return checkFlag(eFlags_IsVisible); }
    void show();
    void hide();

    // Do
    void prepareForRendering();
    void finishRendering();
    void present();

    // Setters
    void setWindowTitle(const he::String& caption);
    void setWindowPosition(int x, int y);
    void setWindowDimension(uint32 width, uint32 height);
    void setVSync(bool enable);
    void setFullscreen(bool fullscreen);
    void setResizable(bool resizable);   // call before creating, or destroy and create
    void setMousePosition(const vec2& pos);
    void setOculusRiftEnabled(const bool enable);

    // Getters
    uint32 getID() const { return m_ID; }
    void getWindowPosition(int& x, int& y) const;
    uint32 getWindowWidth() const;
    uint32 getWindowHeight() const;
    GLContext* getContext() { return &m_Context; }  
    NativeWindowHandle getNativeHandle() const;
    const RenderTarget* getRenderTarget() const { return m_RenderTarget; }

    // Views
    void addViewAtBegin(const ObjectHandle& view);
    void addViewAtEnd(const ObjectHandle& view);
    void removeView(const ObjectHandle& view);
    const he::ObjectList<ObjectHandle>& getViews() const { return m_Views; }

    // Events
    event2<void, int32, int32> Moved; // x, y
    event2<void, int32, int32> Resized; // width, height
    event0<void> Closed;
    event0<void> GainedFocus;
    event0<void> LostFocus;

private:
    inline void raiseFlag(const EFlags flag) { m_Flags |= flag; }
    inline bool checkFlag(const EFlags flag) const { return (m_Flags & flag) != 0; }
    inline void clearFlag(const EFlags flag) { m_Flags &= ~flag; }
    inline void setFlag(const EFlags flag, const bool enable) { enable? raiseFlag(flag) : clearFlag(flag); }

    uint32 m_ID;
    
    he::ObjectList<ObjectHandle> m_Views;
    RenderTarget* m_RenderTarget;

    // Oculus
    OculusRiftBarrelDistorter* m_OVRDistorter;

    SDL_Window* m_Window;
    Window* m_Parent;

    RectI m_WindowRect;
    he::String m_Titel;
    Color m_ClearColor;
    uint8 m_Flags;

    GLContext m_Context;

    //Disable default copy constructor and default assignment operator
    Window(const Window&);
    Window& operator=(const Window&);
};

} } //end namespace

#endif
