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
#include "HappyNew.h"
#include "boost/thread.hpp"

namespace he {
namespace io {

#define KEYBOARD_ARRAY_SIZE sizeof(byte) * sf::Keyboard::KeyCount

Keyboard::Keyboard(): m_CurrentKeyState(nullptr), m_PrevKeyState(nullptr)
{
    m_CurrentKeyState = static_cast<byte*>(he_malloc(KEYBOARD_ARRAY_SIZE));
    m_PrevKeyState = static_cast<byte*>(he_malloc(KEYBOARD_ARRAY_SIZE));
    he_memset(m_CurrentKeyState, FALSE, KEYBOARD_ARRAY_SIZE);
    he_memset(m_PrevKeyState, FALSE, KEYBOARD_ARRAY_SIZE);
}


Keyboard::~Keyboard()
{
    he_free(m_PrevKeyState);
    he_free(m_CurrentKeyState);
}
void Keyboard::tick(byte* pKeyState, const std::vector<char>& chars)
{
    std::swap(m_PrevKeyState, m_CurrentKeyState); 
    he_memcpy(m_CurrentKeyState, pKeyState, KEYBOARD_ARRAY_SIZE);

    m_TextEntered = chars;
}

bool Keyboard::isKeyUp(Key key) const
{
    return m_CurrentKeyState[key] == FALSE;
}
bool Keyboard::isKeyDown(Key key) const
{
    return m_CurrentKeyState[key] == TRUE;
}

bool Keyboard::isKeyPressed(Key key) const
{
    return (m_CurrentKeyState[key] == TRUE && m_PrevKeyState[key] == FALSE);
}
bool Keyboard::isKeyPressed(KeyScanCode code) const
{
    HE_ASSERT(false, "not implemented");
    return m_CurrentKeyState[code] == TRUE && m_PrevKeyState[code] == FALSE;
}
bool Keyboard::isKeyReleased(Key key) const
{
    return (m_CurrentKeyState[key] == FALSE && m_PrevKeyState[key] == TRUE);
}
bool Keyboard::isKeyReleased(KeyScanCode code) const
{
    HE_ASSERT(false, "not implemented");
    return m_CurrentKeyState[code] == FALSE && m_PrevKeyState[code] == TRUE;
}

void Keyboard::addOnKeyPressedListener(boost::function<void(Key)> callback) const
{
    Keyboard* _this = const_cast<Keyboard*>(this);
    _this->m_OnKeyPressedListeners += callback;
}

eventExt<void, Key>& Keyboard::getOnKeyPressedListeners()
{
    return m_OnKeyPressedListeners;
}

void Keyboard::addOnKeyReleasedListener(boost::function<void(Key)> callback) const
{
    Keyboard* _this = const_cast<Keyboard*>(this);
    _this->m_OnKeyReleasedListeners += callback;
}
eventExt<void, Key>& Keyboard::getOnKeyReleasedListeners()
{
    return m_OnKeyReleasedListeners;
}

const std::vector<char>& Keyboard::getTextEntered() const
{
    return m_TextEntered;
}

} } //end namespace