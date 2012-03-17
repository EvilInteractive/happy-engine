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

#include "Keyboard.h"
#include "SDL.h"
#include "HappyNew.h"
#include "boost/thread.hpp"

namespace he {
namespace io {

Keyboard::Keyboard(): m_NewKeyState(nullptr), m_CurrentKeyState(nullptr), m_PrevKeyState(nullptr), m_NumKeys(0)
{
    //Make sure that the prevkeystate is filled in to prevent array errors
    m_NewKeyState = SDL_GetKeyboardState(&m_NumKeys);
    m_CurrentKeyState = NEW byte[m_NumKeys];
    m_PrevKeyState = NEW byte[m_NumKeys];

    tick();
    tick();
}


Keyboard::~Keyboard()
{
    delete[] m_PrevKeyState;
    delete[] m_CurrentKeyState;
}
void Keyboard::tick()
{   
    std::swap(m_PrevKeyState, m_CurrentKeyState); 
    he_memcpy(m_CurrentKeyState, m_NewKeyState, m_NumKeys * sizeof(byte));
}

bool Keyboard::isKeyUp(Key key) const
{
    return m_CurrentKeyState[SDL_GetScancodeFromKey(key)] == 0;
}
bool Keyboard::isKeyDown(Key key) const
{
    return m_CurrentKeyState[SDL_GetScancodeFromKey(key)] != 0;
}

bool Keyboard::isKeyPressed(Key key) const
{
    return m_CurrentKeyState[SDL_GetScancodeFromKey(key)] != 0 && m_PrevKeyState[SDL_GetScancodeFromKey(key)] == 0;
}
bool Keyboard::isKeyPressed(KeyScanCode code) const
{
    HE_ASSERT(false, "not implemented");
    return m_CurrentKeyState[code] != 0 && m_PrevKeyState[code] == 0;
}
bool Keyboard::isKeyReleased(Key key) const
{
    return m_CurrentKeyState[SDL_GetScancodeFromKey(key)] == 0 && m_PrevKeyState[SDL_GetScancodeFromKey(key)] != 0;
}
bool Keyboard::isKeyReleased(KeyScanCode code) const
{
    HE_ASSERT(false, "not implemented");
    return m_CurrentKeyState[code] == 0 && m_PrevKeyState[code] != 0;
}

} } //end namespace