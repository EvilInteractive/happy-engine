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
//Created: 05/08/2012

#ifndef _HE_MouseButtons_H_
#define _HE_MouseButtons_H_
#pragma once

#include <SDL2/SDL_mouse.h>

namespace he {
namespace io {

enum MouseButton
{
    MouseButton_Left = SDL_BUTTON_LEFT,
    MouseButton_Right = SDL_BUTTON_RIGHT,
    MouseButton_Middle = SDL_BUTTON_MIDDLE,
    MouseButton_X1 = SDL_BUTTON_X1,
    MouseButton_X2 = SDL_BUTTON_X2,
    MouseButton_MAX
};

} } //end namespace

#endif
