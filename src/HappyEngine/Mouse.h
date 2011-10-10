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

#ifndef _HE_MOUSE_H_
#define _HE_MOUSE_H_
#pragma once

#include "vec2.h"
#include "HappyTypes.h"
#include "SDL.h"

namespace he {
namespace io {

enum MouseButton
{
    MouseButton_Left = SDL_BUTTON_LMASK, 
    MouseButton_Right = SDL_BUTTON_RMASK, 
    MouseButton_Middle = SDL_BUTTON_MMASK, 
    MouseButton_ScrollUp = SDL_BUTTON_X1MASK, 
    MouseButton_ScrollDown = SDL_BUTTON_X2MASK
};
class Mouse
{
public:
	Mouse();
    virtual ~Mouse();

    void tick();

    bool isButtonDown(byte button) const;
    bool isButtonUp(byte button) const;
    bool isButtonReleased(byte button) const; //true when it goes from down to up
    bool isButtonPressed(byte button) const;  //true when it goes from up to down

    vec2 getPosition() const;

private:
    vec2 m_Position;
    byte m_ButtonState;
    byte m_PrevButtonState;

    //Disable default copy constructor and default assignment operator
    Mouse(const Mouse&);
    Mouse& operator=(const Mouse&);
};

} } //end namespace

#endif
