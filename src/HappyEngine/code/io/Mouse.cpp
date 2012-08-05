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
//Created: 08/08/2011
#include "HappyPCH.h" 

#include "Mouse.h"

namespace he {
namespace io {

Mouse::Mouse(): m_Position(0.0f, 0.0f), m_PrevPosition(0.0f, 0.0f), m_Scroll(0)
{
    he_memset(m_ButtonState, false, io::MouseButton_MAX);
    he_memset(m_PrevButtonState, false, io::MouseButton_MAX);

    eventCallback1<void, MouseButton> mouseButtonPressedHandler([&](MouseButton button)
    {
        m_ButtonState[button] = true;
    });
    eventCallback1<void, MouseButton> mouseButtonReleasedHandler([&](MouseButton button)
    {
        m_ButtonState[button] = false;
    });
    eventCallback1<void, const vec2&> mouseMovedHandler([&](const vec2& pos)
    {
        m_Position = pos;
    });
    eventCallback1<void, int> mouseWheelMovedHandler([&](int scroll)
    {
        m_Scroll = scroll;
    });

    MouseButtonPressed += mouseButtonPressedHandler;
    MouseButtonReleased += mouseButtonReleasedHandler;
    MouseMoved += mouseMovedHandler;
    MouseWheelMoved += mouseWheelMovedHandler;
}


Mouse::~Mouse()
{
}

void Mouse::tick()
{
    he_memcpy(m_PrevButtonState, m_ButtonState, io::MouseButton_MAX);
    m_PrevPosition = m_Position;
    m_Scroll = 0;
}


bool Mouse::isButtonDown(MouseButton button) const
{
    return m_ButtonState[button] == true;
}
bool Mouse::isButtonUp(MouseButton button) const
{
    return m_ButtonState[button] == false;
}
bool Mouse::isButtonReleased(MouseButton button) const
{
    return (m_ButtonState[button] == false && m_PrevButtonState[button] == true);
}
bool Mouse::isButtonPressed(MouseButton button) const
{
    return (m_ButtonState[button] == true && m_PrevButtonState[button] == false);
}

const vec2& Mouse::getPosition() const
{
    return m_Position;
}
int Mouse::getScroll() const
{
    return m_Scroll;
}
vec2 Mouse::getMove() const
{
    return (m_Position - m_PrevPosition);
}

} } //end namespace