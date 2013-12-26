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

#ifndef _HE_KEYBOARD_H_
#define _HE_KEYBOARD_H_
#pragma once

#include "HappyTypes.h"
#include "Keys.h"
#include "IKeyboard.h"

namespace he {
namespace io {

class Keyboard : public IKeyboard
{
public:
    Keyboard();
    virtual ~Keyboard();
    
    virtual bool isKeyUp(const Key key) const;
    virtual bool isKeyDown(const Key key) const;

    virtual bool isKeyPressed(const Key key) const;           //true when state goes from up to down
    virtual bool isKeyReleased(const Key key) const;          //true when state goes from down to up

    virtual bool isShortcutPressed(const Key key1, const Key key2) const; // true when shortcut is pressed and one key went from up to down
    virtual bool isShortcutPressed(const Key key1, const Key key2, const Key key3) const;
    virtual bool isShortcutPressed(const Key key1, const Key key2, const Key key3, const Key key4) const;
    
    virtual const he::String& getTextEntered() const;

    virtual void tick();

private:
    const static int s_ArraySize = io::Key_MAX / 8 + 1;
    uint8 m_CurrentKeyState[s_ArraySize];
    uint8 m_PrevKeyState[s_ArraySize];

    he::String m_TextEntered;

    //Disable default copy constructor and default assignment operator
    Keyboard(const Keyboard&);
    Keyboard& operator=(const Keyboard&);
};

} } //end namespace

#endif
