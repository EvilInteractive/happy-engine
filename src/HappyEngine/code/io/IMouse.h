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

namespace he {
namespace io {

class IMouse
{
public:
    virtual ~IMouse() {}
    
    virtual bool isButtonDown(MouseButton button) const = 0;
    virtual bool isButtonUp(MouseButton button) const = 0;
    virtual bool isButtonReleased(MouseButton button) const = 0; //true when it goes from down to up
    virtual bool isButtonPressed(MouseButton button) const = 0;  //true when it goes from up to down

    virtual int getScroll() const = 0;
    virtual const vec2& getPosition() const = 0;
    virtual vec2 getMove() const = 0;

    virtual void tick() = 0;

    event1<void, MouseButton> MouseButtonPressed;
    event1<void, MouseButton> MouseButtonReleased;
    event1<void, const vec2&> MouseMoved;
    event1<void, int>         MouseWheelMoved;
};

} } //end namespace

#endif
