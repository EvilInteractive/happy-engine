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
//Created: 10/11/2011

#ifndef _HE_IKEYBOARD_H_
#define _HE_IKEYBOARD_H_
#pragma once

#include "Keys.h"

namespace he {
namespace io {

class IKeyboard
{
public:
    virtual ~IKeyboard() {}

    virtual void tick(byte* pKeyState) = 0;

    virtual bool isKeyUp(Key key) const = 0;
    virtual bool isKeyDown(Key key) const = 0;

    virtual bool isKeyPressed(Key key) const = 0;           //true when state goes from up to down
    virtual bool isKeyPressed(KeyScanCode code) const = 0;
    virtual bool isKeyReleased(Key key) const = 0;          //true when state goes from down to up
    virtual bool isKeyReleased(KeyScanCode code) const = 0;
};

} } //end namespace

#endif
