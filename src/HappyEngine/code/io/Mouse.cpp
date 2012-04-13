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

Mouse::Mouse(): m_Position(0.0f, 0.0f), m_ButtonState(0), m_PrevButtonState(0)
{
    m_ButtonState = NEW bool[sf::Mouse::ButtonCount];
    m_PrevButtonState = NEW bool[sf::Mouse::ButtonCount];
}


Mouse::~Mouse()
{
    delete[] m_ButtonState;
    delete[] m_PrevButtonState;
}

void Mouse::tick(bool* pMouseState)
{
    m_PrevButtonState = m_ButtonState;

    m_ButtonState = pMouseState;
    
    sf::Vector2i vec = sf::Mouse::getPosition();

    m_Position.x = static_cast<float>(vec.x);
    m_Position.y = static_cast<float>(vec.y);
}

bool Mouse::isButtonDown(MouseButton button) const
{
    return m_ButtonState[button];
}
bool Mouse::isButtonUp(MouseButton button) const
{
    return !m_ButtonState[button];
}
bool Mouse::isButtonReleased(MouseButton button) const
{
    return (!m_ButtonState[button] && m_PrevButtonState[button]);
}
bool Mouse::isButtonPressed(MouseButton button) const
{
    return (m_ButtonState[button] && !m_PrevButtonState[button]);
}

vec2 Mouse::getPosition() const
{
    return m_Position;
}

} } //end namespace