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

namespace he {
namespace io {
 
inline void getIndexShift(Key key, int& index, int& shift)
{
    index = key / 8;
    shift = key % 8;
}

Keyboard::Keyboard(): m_TextCharEntered(0)
{
    he_memset(&m_CurrentKeyState, 0, s_ArraySize);
    he_memset(&m_PrevKeyState, 0, s_ArraySize);

    eventCallback1<void, Key> keyPressedHandler([&](Key key)
    {
        int index(0);
        int shift(0);
        getIndexShift(key, index, shift);
        m_CurrentKeyState[index] |= 1 << shift;
    });
    eventCallback1<void, Key> keyReleasedHandler([&](Key key)
    {
        int index(0);
        int shift(0);
        getIndexShift(key, index, shift);
        m_CurrentKeyState[index] &= ~(1 << shift);
    });
    eventCallback1<void, char> textCharEnteredHandler([&](char chr)
    {
        m_TextCharEntered = chr;
    });

    KeyPressed += keyPressedHandler;
    KeyReleased += keyReleasedHandler;
    TextCharEntered += textCharEnteredHandler;
}


Keyboard::~Keyboard()
{
}

void Keyboard::tick()
{
    he_memcpy(m_PrevKeyState, m_CurrentKeyState, s_ArraySize);
    m_TextCharEntered = 0;
}

bool Keyboard::isKeyUp(Key key) const
{
    int index(0);
    int shift(0);
    getIndexShift(key, index, shift);
    return (m_CurrentKeyState[index] & (1 << shift)) == 0;
}
bool Keyboard::isKeyDown(Key key) const
{
    int index(0);
    int shift(0);
    getIndexShift(key, index, shift);
    return (m_CurrentKeyState[index] & (1 << shift)) != 0;
}

bool Keyboard::isKeyPressed(Key key) const
{
    int index(0);
    int shift(0);
    getIndexShift(key, index, shift);
    return ((m_CurrentKeyState[index] & (1 << shift)) != 0 && (m_PrevKeyState[index] & (1 << shift)) == 0);
}
bool Keyboard::isKeyReleased(Key key) const
{
    int index(0);
    int shift(0);
    getIndexShift(key, index, shift);
    return ((m_CurrentKeyState[index] & (1 << shift)) == 0 && (m_PrevKeyState[index] & (1 << shift)) != 0);
}

const char& Keyboard::getTextCharEntered() const
{
    return m_TextCharEntered;
}

} } //end namespace