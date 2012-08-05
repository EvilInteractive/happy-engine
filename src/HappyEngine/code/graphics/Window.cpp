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
#include "HappyPCH.h" 

#include "Window.h"
#include "GraphicsEngine.h"
#include "ControlsManager.h"
#include "IKeyboard.h"
#include "IMouse.h"

namespace he {
namespace gfx {

Window::Window(Window* parent) 
  : m_Window(NEW sf::Window())
  , m_Parent(parent)
  , m_ClearColor(0, 0, 0)
  , m_WindowRect(-1, -1, 1280, 720)
  , m_Titel("")
  , m_VSyncEnabled(false)
  , m_IsCursorVisible(true)
  , m_Fullscreen(false)
  , m_Resizeable(true)
{
}


Window::~Window()
{
    delete m_Window;
}

void Window::open()
{
    HE_ASSERT(m_Parent != nullptr && m_Parent->isOpen(), "Parent window is not open!");
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 3;
    settings.minorVersion = 2;
    if (m_Parent == nullptr)
    {
        m_Window->create(sf::VideoMode(m_WindowRect.width, m_WindowRect.height, 32), m_Titel, 
            m_Fullscreen? sf::Style::Fullscreen : (m_Resizeable? sf::Style::Resize : sf::Style::Close), settings);
    }
    else
    {
        m_Window->create(m_Parent->m_Window->getSystemHandle(), settings);
        m_Window->setTitle(m_Titel);
        m_Window->setSize(sf::Vector2u(m_WindowRect.width, m_WindowRect.height));
    }
    m_Window->setKeyRepeatEnabled(false);
    m_Window->setFramerateLimit(0);
    setWindowPosition(m_WindowRect.x, m_WindowRect.y);
    setCursorVisible(m_IsCursorVisible);
    setVSync(m_VSyncEnabled);
}
void Window::close()
{
    m_Window->close();
}
bool Window::isOpen()
{
    return m_Window->isOpen();
}

void Window::show()
{
    m_Window->setVisible(true);
}

void Window::hide()
{
    m_Window->setVisible(false);
}

void Window::doEvents( float /*dTime*/ )
{
    if (isOpen() == false)
        return;

    sf::Event event;
    while (m_Window->pollEvent(event))
    {
        io::IMouse* mouse(CONTROLS->getMouse());
        io::IKeyboard* keyboard(CONTROLS->getKeyboard());
        bool hasFocus(GRAPHICS->getActiveWindow() == this);

        switch (event.type)
        {
        // Window
        case sf::Event::Closed:
            m_WindowRect.x = m_Window->getPosition().x;
            m_WindowRect.y = m_Window->getPosition().y;
            Closed();
            break;
        case sf::Event::Resized:
            m_WindowRect.width = static_cast<int>(m_Window->getSize().x);
            m_WindowRect.height = static_cast<int>(m_Window->getSize().y);
            Resized();
            break;
        case sf::Event::GainedFocus:
            GRAPHICS->setActiveWindow(this);
            GainedFocus();
            break;
        case sf::Event::LostFocus:
            LostFocus();
            break;

        // Mouse
        case sf::Event::MouseButtonPressed:
            if (hasFocus == true) 
                mouse->MouseButtonPressed(static_cast<io::MouseButton>(event.mouseButton.button));
            break;
        case sf::Event::MouseButtonReleased:
            if (hasFocus == true) 
                mouse->MouseButtonReleased(static_cast<io::MouseButton>(event.mouseButton.button));
            break;
        case sf::Event::MouseMoved:
            if (hasFocus == true) 
                mouse->MouseMoved(vec2((float)event.mouseButton.x, (float)event.mouseButton.y));
            break;
        case sf::Event::MouseWheelMoved:
            if (hasFocus == true) 
                mouse->MouseWheelMoved(event.mouseWheel.delta);
            break;


        // Keyboard
        case sf::Event::KeyPressed:
            if (hasFocus == true)
                keyboard->KeyPressed(static_cast<io::Key>(event.key.code));
            break;
        case sf::Event::KeyReleased:
            if (hasFocus == true)
                keyboard->KeyReleased(static_cast<io::Key>(event.key.code));
            break;
        case sf::Event::TextEntered:
            if (hasFocus == true)
                keyboard->TextCharEntered(static_cast<char>(event.text.unicode));
            break;
        }
    }
}

void Window::getWindowPosition( int& x, int& y ) const
{
    x = m_Window->getPosition().x;
    y = m_Window->getPosition().y;
}

he::uint Window::getWindowWidth() const
{
    return m_Window->getSize().x;
}

he::uint Window::getWindowHeight() const
{
    return m_Window->getSize().y;
}

void Window::setWindowTitle( const std::string& caption )
{
    m_Window->setTitle(caption);
    m_Titel = caption;
}

void Window::setWindowPosition( int x, int y )
{
    m_Window->setPosition(sf::Vector2i(x, y));
    m_WindowRect.x = x;
    m_WindowRect.y = y;
}

void Window::setWindowDimension( uint width, uint height )
{
    m_Window->setSize(sf::Vector2u(width, height));
    m_WindowRect.width = static_cast<int>(width);
    m_WindowRect.height = static_cast<int>(height);
}

void Window::setVSync( bool enable )
{
    m_VSyncEnabled = enable;
    m_Window->setVerticalSyncEnabled(enable);
}

void Window::setCursorVisible( bool visible )
{
    m_IsCursorVisible = visible;
    m_Window->setMouseCursorVisible(visible);
}

void Window::prepareForRendering()
{
    m_Window->setActive(true);
}

void Window::present()
{
    m_Window->display();
}

void Window::setFullscreen( bool fullscreen )
{
    if (m_Fullscreen != fullscreen)
    {
        m_Fullscreen = fullscreen;
        if (m_Window->isOpen())
        {
            close();
            open();
        }
    }
}



} } //end namespace
