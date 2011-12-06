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
//Created: 08/08/2011
#include "HappyPCH.h" 

#include "Mouse.h"

namespace he {
namespace io {

Mouse::Mouse(): m_Position(0.0f, 0.0f), m_ButtonState(0), m_PrevButtonState(0)
{
}


Mouse::~Mouse()
{
}

void Mouse::tick()
{
    int x, y;
    m_PrevButtonState = m_ButtonState;
    m_ButtonState = SDL_GetMouseState(&x, &y);
    m_Position.x = static_cast<float>(x);
    m_Position.y = static_cast<float>(y);
}

bool Mouse::isButtonDown(byte button) const
{
    return (m_ButtonState & button) == button;
}
bool Mouse::isButtonUp(byte button) const
{
    return (!m_ButtonState & button) == button;
}
bool Mouse::isButtonReleased(byte button) const
{
    return ((!m_ButtonState & button) == button) && ((m_PrevButtonState & button) == button);
}
bool Mouse::isButtonPressed(byte button) const
{
    return ((m_ButtonState & button) == button) && ((!m_PrevButtonState & button) == button);
}

vec2 Mouse::getPosition() const
{
    return m_Position;
}

} } //end namespace