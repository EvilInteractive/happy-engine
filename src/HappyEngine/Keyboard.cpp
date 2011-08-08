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

#include "Keyboard.h"
#include "SDL.h"

namespace happyengine {
namespace io {

    Keyboard::Keyboard(): m_KeyState(nullptr), m_PrevKeyState(nullptr), m_NumKeys(0), m_NumPrevKeys(0)
{
    //Make sure that the prevkeystate is filled in to prevent array errors
    tick();
    tick();
}


Keyboard::~Keyboard()
{
    delete[] m_PrevKeyState;
    delete[] m_KeyState;
}
void Keyboard::tick()
{
    int keys;
    byte* state;
    state = SDL_GetKeyboardState(&keys);

    byte* temp = m_PrevKeyState;
    m_PrevKeyState = m_KeyState;
    m_KeyState = temp;
    
    if (m_NumPrevKeys != keys)
    {
        delete[] m_KeyState;
        m_KeyState = new byte[keys];
        m_NumPrevKeys = m_NumKeys;
        m_NumKeys = keys;
    }
    memcpy(m_KeyState, state, m_NumKeys * sizeof(byte));
}

bool Keyboard::isKeyUp(Key key) const
{
    return m_KeyState[SDL_GetScancodeFromKey(key)] != 0;
}
bool Keyboard::isKeyDown(Key key) const
{
    return m_KeyState[SDL_GetScancodeFromKey(key)] == 0;
}

bool Keyboard::isKeyPressed(Key key) const
{
    return m_KeyState[SDL_GetScancodeFromKey(key)] != 0 && m_PrevKeyState[SDL_GetScancodeFromKey(key)] == 0;
}
bool Keyboard::isKeyReleased(Key key) const
{
    return m_KeyState[SDL_GetScancodeFromKey(key)] == 0 && m_PrevKeyState[SDL_GetScancodeFromKey(key)] != 0;
}

} } //end namespace