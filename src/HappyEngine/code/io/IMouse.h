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
//Created: 09/11/2011

#ifndef _HE_IMOUSE_H_
#define _HE_IMOUSE_H_
#pragma once

#include "vec2.h"
#include "HappyTypes.h"

namespace he {
namespace io {

enum MouseButton
{
    //MouseButton_None = 0,
    MouseButton_Left = sf::Mouse::Left, 
    MouseButton_Right = sf::Mouse::Right, 
    MouseButton_Middle = sf::Mouse::Middle,
    MouseButton_X1 = sf::Mouse::XButton1, 
    MouseButton_X2 = sf::Mouse::XButton2
};
class IMouse
{
public:
    virtual ~IMouse() {}

    virtual void tick(byte* mouseState, int scrollState, const vec2& mousePos) = 0;

    virtual bool isButtonDown(MouseButton button) const = 0;
    virtual bool isButtonUp(MouseButton button) const = 0;
    virtual bool isButtonReleased(MouseButton button) const = 0; //true when it goes from down to up
    virtual bool isButtonPressed(MouseButton button) const = 0;  //true when it goes from up to down

    virtual int getScroll() const = 0;
    virtual const vec2& getPosition() const = 0;
    virtual vec2 getMove() const = 0;

    virtual void addOnButtonPressedListener(boost::function<void(MouseButton)> callback) const = 0;
    virtual eventExt<void, MouseButton>& getOnButtonPressedListeners() = 0;
    virtual void addOnButtonReleasedListener(boost::function<void(MouseButton)> callback) const = 0;
    virtual eventExt<void, MouseButton>& getOnButtonReleasedListeners() = 0;
    virtual void addOnMouseMovedListener(boost::function<void(const vec2&)> callback) const = 0;
    virtual eventExt<void, const vec2&>& getOnMouseMovedListeners() = 0;
    virtual void addOnMouseWheelMovedListener(boost::function<void(int)> callback) const = 0;
    virtual eventExt<void, int>& getOnMouseWheelMovedListeners() = 0;
};

} } //end namespace

#endif
