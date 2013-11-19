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

inline void getIndexShift(const Key key, int& index, int& shift)
{
    index = key / 8;
    shift = key % 8;
}

inline uint8 getPressed(const Key key, const uint8* const keys)
{
    int index(0);
    int shift(0);
    getIndexShift(key, index, shift);
    return keys[index] & (1 << shift);
}

Keyboard::Keyboard(): m_TextEntered("")
{
    he_memset(&m_CurrentKeyState, 0, s_ArraySize);
    he_memset(&m_PrevKeyState, 0, s_ArraySize);

    eventCallback1<void, Key> keyPressedHandler([&](Key key)
    {
        int index(0);
        int shift(0);
        getIndexShift(key, index, shift);
        m_CurrentKeyState[index] |= 1 << shift;

        Key mergeKey(Key_MAX);
        switch (key)
        {
            case Key_Lctrl:
            case Key_Rctrl:
            {
                mergeKey = Key_Ctrl;          
            } break;
            case Key_Lalt:
            case Key_Ralt:
            {
                mergeKey = Key_Alt;          
            } break;
            case Key_Lshift:
            case Key_Rshift:
            {
                mergeKey = Key_Shift;          
            } break;
            default: break;
        }
        if (mergeKey != Key_MAX)
        {
            getIndexShift(mergeKey, index, shift);
            m_CurrentKeyState[index] |= 1 << shift;
        }
    });
    eventCallback1<void, Key> keyReleasedHandler([&](Key key)
    {
        int index(0);
        int shift(0);
        getIndexShift(key, index, shift);
        m_CurrentKeyState[index] &= ~(1 << shift);

        Key mergeKey(Key_MAX);
        bool releaseMergeKey(true);
        switch (key)
        {
        case Key_Lctrl: mergeKey = Key_Ctrl; releaseMergeKey = getPressed(Key_Rctrl, m_CurrentKeyState) == 0; break;
        case Key_Rctrl: mergeKey = Key_Ctrl; releaseMergeKey = getPressed(Key_Lctrl, m_CurrentKeyState) == 0; break;

        case Key_Lalt: mergeKey = Key_Alt; releaseMergeKey = getPressed(Key_Ralt, m_CurrentKeyState) == 0; break;
        case Key_Ralt: mergeKey = Key_Alt; releaseMergeKey = getPressed(Key_Lalt, m_CurrentKeyState) == 0; break;

        case Key_Lshift: mergeKey = Key_Shift; releaseMergeKey = getPressed(Key_Rshift, m_CurrentKeyState) == 0; break;
        case Key_Rshift: mergeKey = Key_Shift; releaseMergeKey = getPressed(Key_Lshift, m_CurrentKeyState) == 0; break;
        default: break;
        }
        if (mergeKey != Key_MAX && releaseMergeKey)
        {
            getIndexShift(mergeKey, index, shift);
            m_CurrentKeyState[index] &= ~(1 << shift);
        }
    });
    eventCallback1<void, const char*> textCharEnteredHandler([&](const char* chr)
    {
        m_TextEntered = chr;
    });

    KeyPressed += keyPressedHandler;
    KeyReleased += keyReleasedHandler;
    TextEntered += textCharEnteredHandler;
}


Keyboard::~Keyboard()
{
}

void Keyboard::tick()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    he_memcpy(m_PrevKeyState, m_CurrentKeyState, s_ArraySize);
    m_TextEntered.clear();
}

bool Keyboard::isKeyUp(const Key key) const
{
    int index(0);
    int shift(0);
    getIndexShift(key, index, shift);
    return (m_CurrentKeyState[index] & (1 << shift)) == 0;
}
bool Keyboard::isKeyDown(const Key key) const
{
    return getPressed(key, m_CurrentKeyState) != 0;
}

bool Keyboard::isKeyPressed(const Key key) const
{
    return getPressed(key, m_CurrentKeyState) != 0 && getPressed(key, m_PrevKeyState) == 0;
}
bool Keyboard::isKeyReleased(const Key key) const
{
    return getPressed(key, m_CurrentKeyState) == 0 && getPressed(key, m_PrevKeyState) != 0;
}

const he::String& Keyboard::getTextEntered() const
{
    return m_TextEntered;
}

bool Keyboard::isShortcutPressed( const Key key1, const Key key2 ) const
{
    return (getPressed(key1, m_CurrentKeyState) != 0) && (getPressed(key2, m_CurrentKeyState) != 0) &&
          ((getPressed(key1, m_PrevKeyState) == 0) || (getPressed(key2, m_PrevKeyState) == 0));
}

bool Keyboard::isShortcutPressed( const Key key1, const Key key2, const Key key3 ) const
{
    return (getPressed(key1, m_CurrentKeyState) != 0) && (getPressed(key2, m_CurrentKeyState) != 0) &&
            (getPressed(key3, m_CurrentKeyState) != 0) &&
        ((getPressed(key1, m_PrevKeyState) == 0) || (getPressed(key2, m_PrevKeyState) == 0) ||
          getPressed(key3, m_PrevKeyState) == 0);
}

bool Keyboard::isShortcutPressed( const Key key1, const Key key2, const Key key3, const Key key4 ) const
{
    return (getPressed(key1, m_CurrentKeyState) != 0) && (getPressed(key2, m_CurrentKeyState) != 0) &&
        (getPressed(key3, m_CurrentKeyState) != 0) && (getPressed(key4, m_CurrentKeyState) != 0) &&
        ((getPressed(key1, m_PrevKeyState) == 0) || (getPressed(key2, m_PrevKeyState) == 0) ||
        (getPressed(key3, m_PrevKeyState) == 0) || (getPressed(key4, m_PrevKeyState) == 0));
}

} } //end namespace