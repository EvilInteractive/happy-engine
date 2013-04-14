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
//Created: 05/08/2012

#ifndef _HE_MouseButtons_H_
#define _HE_MouseButtons_H_
#pragma once

#include <SFML/Window/Mouse.hpp>

namespace he {
namespace io {

enum MouseButton
{
    MouseButton_Left = sf::Mouse::Left, 
    MouseButton_Right = sf::Mouse::Right, 
    MouseButton_Middle = sf::Mouse::Middle,
    MouseButton_X1 = sf::Mouse::XButton1, 
    MouseButton_X2 = sf::Mouse::XButton2,
    MouseButton_MAX = sf::Mouse::ButtonCount
};

} } //end namespace

#endif
