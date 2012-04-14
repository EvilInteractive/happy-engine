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

#define MOUSE_ARRAY_SIZE sf::Mouse::ButtonCount * sizeof(byte)

Mouse::Mouse(): m_Position(0.0f, 0.0f), m_ButtonState(0), m_PrevButtonState(0)
{
    m_ButtonState = static_cast<byte*>(he_malloc(MOUSE_ARRAY_SIZE));
    m_PrevButtonState = static_cast<byte*>(he_malloc(MOUSE_ARRAY_SIZE));
    he_memset(m_ButtonState, FALSE, MOUSE_ARRAY_SIZE);
    he_memset(m_PrevButtonState, FALSE, MOUSE_ARRAY_SIZE);
}


Mouse::~Mouse()
{
    he_free(m_ButtonState);
    he_free(m_PrevButtonState);
}

void Mouse::tick(byte* pMouseState, int scrollState)
{
    m_Scroll = scrollState;

    std::swap(m_ButtonState, m_PrevButtonState);
    he_memcpy(m_ButtonState, pMouseState, MOUSE_ARRAY_SIZE);
        
    sf::Vector2i vec(sf::Mouse::getPosition());

    m_Position.x = static_cast<float>(vec.x);
    m_Position.y = static_cast<float>(vec.y);
}

bool Mouse::isButtonDown(MouseButton button) const
{
    return m_ButtonState[button] == TRUE;
}
bool Mouse::isButtonUp(MouseButton button) const
{
    return m_ButtonState[button] == FALSE;
}
bool Mouse::isButtonReleased(MouseButton button) const
{
    return (m_ButtonState[button] == FALSE && m_PrevButtonState[button] == TRUE);
}
bool Mouse::isButtonPressed(MouseButton button) const
{
    return (m_ButtonState[button] == TRUE && m_PrevButtonState[button] == FALSE);
}

const vec2& Mouse::getPosition() const
{
    return m_Position;
}

int Mouse::getScroll() const
{
    return m_Scroll;
}

} } //end namespace