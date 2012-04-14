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

    virtual void tick(byte* pKeyState);

    virtual bool isKeyUp(Key key) const;
    virtual bool isKeyDown(Key key) const;

    virtual bool isKeyPressed(Key key) const;           //true when state goes from up to down
    virtual bool isKeyPressed(KeyScanCode code) const;
    virtual bool isKeyReleased(Key key) const;          //true when state goes from down to up
    virtual bool isKeyReleased(KeyScanCode code) const;


private:
    byte* m_CurrentKeyState;
    byte* m_PrevKeyState;

    //Disable default copy constructor and default assignment operator
    Keyboard(const Keyboard&);
    Keyboard& operator=(const Keyboard&);
};

} } //end namespace

#endif
