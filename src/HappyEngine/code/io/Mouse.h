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

#ifndef _HE_MOUSE_H_
#define _HE_MOUSE_H_
#pragma once

#include "IMouse.h"

namespace he {
namespace io {

class Mouse : public IMouse
{
public:
    Mouse();
    virtual ~Mouse();

    virtual void tick(byte* mouseState, int scrollSate, const vec2& mousePos);

    virtual bool isButtonDown(MouseButton button) const;
    virtual bool isButtonUp(MouseButton button) const;
    virtual bool isButtonReleased(MouseButton button) const; //true when it goes from down to up
    virtual bool isButtonPressed(MouseButton button) const;  //true when it goes from up to down

    virtual int getScroll() const;

    virtual const vec2& getPosition() const;

    virtual const vec2& getMove() const;

private:

    vec2 m_Position;
    vec2 m_PrevPosition;

    byte* m_ButtonState;
    byte* m_PrevButtonState;

    int m_Scroll;

    //Disable default copy constructor and default assignment operator
    Mouse(const Mouse&);
    Mouse& operator=(const Mouse&);
};

} } //end namespace

#endif
