//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
//Created: 10/11/2011

#ifndef _HE_IKEYBOARD_H_
#define _HE_IKEYBOARD_H_
#pragma once

#include "Keys.h"

namespace he {
namespace io {

class HAPPY_ENTRY IKeyboard
{
public:
    virtual ~IKeyboard() {}
    
    virtual bool isKeyUp(const Key key) const = 0;
    virtual bool isKeyDown(const Key key) const = 0;

    virtual bool isKeyPressed(const Key key) const = 0;           //true when state goes from up to down
    virtual bool isKeyReleased(const Key key) const = 0;          //true when state goes from down to up

    virtual bool isShortcutPressed(const Key key1, const Key key2) const = 0;
    virtual bool isShortcutPressed(const Key key1, const Key key2, const Key key3) const = 0;
    virtual bool isShortcutPressed(const Key key1, const Key key2, const Key key3, const Key key4) const = 0;
    
    virtual const he::String& getTextEntered() const = 0;

    virtual void tick() = 0;

    event1<void, Key> KeyPressed;
    event1<void, Key> KeyReleased;
    event1<void, const char*> TextEntered;
};

} } //end namespace

#endif
