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

#ifndef _HE_Window_H_
#define _HE_Window_H_
#pragma once

#include "Singleton.h"

namespace he {
namespace gfx {
class Window;
class RenderTarget;
class Texture2D;

class WindowFactory: public ObjectFactory<Window, NoCreateObjectAllocator<Window>>, public Singleton<WindowFactory>
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
enum EFlags
{
    eFlags_VSyncEnabled = 1 << 0,
    eFlags_Fullscreen = 1 << 1,
    eFlags_Resizeable = 1 << 2,
    eFlags_IsVisible = 1 << 3,
    eFlags_EnableOculusRift = 1 << 4,
    eFlags_HasFocus = 1 << 5
};
struct OculusRiftBarrelDistorter;
public:
    Window();
    virtual ~Window();

    virtual bool create(const bool show);
    virtual void destroy();

    virtual bool isOpen() const = 0;
    virtual bool isVisible() const { return checkFlag(eFlags_IsVisible); }
    virtual void show();
    virtual void hide();

    // Do
    virtual void prepareForRendering();
    virtual void finishRendering();
    virtual void present();

    // Setters
    virtual void setWindowTitle(const he::String& caption);
    virtual void setWindowPosition(int x, int y);
    virtual void setWindowDimension(uint32 width, uint32 height);
    virtual void setVSync(bool enable);
    virtual void setFullscreen(bool fullscreen);
    virtual void setResizable(bool resizable);   // call before creating, or destroy and create
    virtual void setMousePosition(const vec2& pos);
    virtual void setOculusRiftEnabled(const bool enable);

    // Getters
    virtual GLContext* getContext() const = 0;
    const RenderTarget* getRenderTarget() const { return m_RenderTarget; }

    void getWindowPosition( int& x, int& y ) const { x = m_WindowRect.x; y = m_WindowRect.y; }
    he::uint32 getWindowWidth() const { return m_WindowRect.width; }
    he::uint32 getWindowHeight() const { return m_WindowRect.height; }

    virtual const he::FixedString& getType() const = 0;


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

protected:
    inline void raiseFlag(const EFlags flag) { m_Flags |= flag; }
    inline bool checkFlag(const EFlags flag) const { return (m_Flags & flag) != 0; }
    inline void clearFlag(const EFlags flag) { m_Flags &= ~flag; }
    inline void setFlag(const EFlags flag, const bool enable) { enable? raiseFlag(flag) : clearFlag(flag); }
    
    he::ObjectList<ObjectHandle> m_Views;
    RenderTarget* m_RenderTarget;

    // Oculus
    OculusRiftBarrelDistorter* m_OVRDistorter;

    RectI m_WindowRect;
    he::String m_Titel;
    Color m_ClearColor;
    uint8 m_Flags;

private:
    //Disable default copy constructor and default assignment operator
    Window(const Window&);
    Window& operator=(const Window&);
};

} } //end namespace

#endif
