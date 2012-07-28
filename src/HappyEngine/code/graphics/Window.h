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

namespace he {
namespace gfx {

class Window
{
public:
    Window(Window* parent = nullptr);
    virtual ~Window();

    // Open/Close
    void open();
    void close();
    bool isOpen();
    void show();
    void hide();

    // Do
    void doEvents(float dTime);
    void prepareForRendering();
    void present();

    // Setters
    void setWindowTitle(const std::string& caption);
    void setWindowPosition(int x, int y);
    void setWindowDimension(uint width, uint height);
    void setVSync(bool enable);
    void setBackgroundColor(const Color& color);
    void setCursorVisible(bool visible);
    void setFullscreen(bool fullscreen);
    void setResizable(bool resizable);   // call before opening, or close and reopen

    // Getters
    void getWindowPosition(int& x, int& y) const;
    uint getWindowWidth() const;
    uint getWindowHeight() const;
    const std::vector<sf::Event>& getEvents() { return m_Events; }

    // Events
    event0<void> Resized;
    event0<void> Closed;
    event0<void> GainedFocus;
    event0<void> LostFocus;

private:
    sf::Window* m_Window;
    Window* m_Parent;

    RectI m_WindowRect;
    std::string m_Titel;
    Color m_ClearColor;
    bool m_VSyncEnabled;
    bool m_IsCursorVisible;
    bool m_Fullscreen;
    bool m_Resizeable; 

    std::vector<sf::Event> m_Events;

    //Disable default copy constructor and default assignment operator
    Window(const Window&);
    Window& operator=(const Window&);
};

} } //end namespace

#endif
