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

#ifndef _HE_KEYBOARD_H_
#define _HE_KEYBOARD_H_
#pragma once

#include "HappyTypes.h"
#include "Keys.h"
#include <vector>

namespace happyengine {
namespace io {

class Keyboard
{
public:
	Keyboard();
    virtual ~Keyboard();

    void tick();

    bool isKeyUp(Key key) const;
    bool isKeyDown(Key key) const;

    bool isKeyPressed(Key key) const;  //true when state goes from up to down
    bool isKeyReleased(Key key) const; //true when state goes from down to up

private:
    byte* m_KeyState;
    byte* m_PrevKeyState;

    int m_NumKeys, m_NumPrevKeys;

    //Disable default copy constructor and default assignment operator
    Keyboard(const Keyboard&);
    Keyboard& operator=(const Keyboard&);
};

} } //end namespace

#endif
