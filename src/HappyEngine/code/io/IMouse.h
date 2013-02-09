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
namespace gfx {
    class Window;
}
namespace io {

ENUM(MouseCursor, uint8)
{
    MouseCursor_Pointer,
    MouseCursor_Cross,
    MouseCursor_Hand,
    MouseCursor_IBeam,
    MouseCursor_Wait,
    MouseCursor_Help,

    MouseCursor_EastResize,
    MouseCursor_NorthResize,
    MouseCursor_NorthEastResize,
    MouseCursor_NorthWestResize,
    MouseCursor_SouthResize,
    MouseCursor_SouthEastResize,
    MouseCursor_SouthWestResize,
    MouseCursor_WestResize,
    MouseCursor_NorthSouthResize,
    MouseCursor_EastWestResize,
    MouseCursor_NorthEastSouthWestResize,
    MouseCursor_NorthWestSouthEastResize,
    MouseCursor_ColumnResize,
    MouseCursor_RowResize,

    MouseCursor_MiddlePanning,
    MouseCursor_EastPanning,
    MouseCursor_NorthPanning,
    MouseCursor_NorthEastPanning,
    MouseCursor_NorthWestPanning,
    MouseCursor_SouthPanning,
    MouseCursor_SouthEastPanning,
    MouseCursor_SouthWestPanning,
    MouseCursor_WestPanning,

    MouseCursor_Move,
    MouseCursor_VerticalText,
    MouseCursor_Cell,
    MouseCursor_ContextMenu,
    MouseCursor_Alias,
    MouseCursor_Progress,
    MouseCursor_NoDrop,
    MouseCursor_Copy,
    MouseCursor_None,
    MouseCursor_NotAllowed,

    MouseCursor_ZoomIn,
    MouseCursor_ZoomOut,

    MouseCursor_Grab,
    MouseCursor_Grabbing,

    MouseCursor_Custom
};

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
