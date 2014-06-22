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
//Created: 08/08/2011
#include "HappyPCH.h" 

#include "Mouse.h"
#include "Window.h"

#include <SDL2/SDL_mouse.h>

namespace he {
namespace io {

Mouse::Mouse(): m_Position(0.0f, 0.0f), m_PrevPosition(0.0f, 0.0f), m_Scroll(0)
{
    he_memset(m_ButtonState, false, io::MouseButton_MAX);
    he_memset(m_PrevButtonState, false, io::MouseButton_MAX);

    eventCallback1<void, MouseButton> mouseButtonPressedHandler([&](MouseButton button)
    {
        m_ButtonState[button] = true;
    });
    eventCallback1<void, MouseButton> mouseButtonReleasedHandler([&](MouseButton button)
    {
        m_ButtonState[button] = false;
    });
    eventCallback1<void, const vec2&> mouseMovedHandler([&](const vec2& pos)
    {
        m_Position = pos;
    });
    eventCallback1<void, int> mouseWheelMovedHandler([&](int scroll)
    {
        m_Scroll = scroll;
    });

    MouseButtonPressed += mouseButtonPressedHandler;
    MouseButtonReleased += mouseButtonReleasedHandler;
    MouseMoved += mouseMovedHandler;
    MouseWheelMoved += mouseWheelMovedHandler;
}


Mouse::~Mouse()
{
    if (m_Cursor != nullptr)
    {
        SDL_SetCursor(SDL_GetDefaultCursor());
        SDL_FreeCursor(m_Cursor);
    }
}

void Mouse::tick()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    he_memcpy(m_PrevButtonState, m_ButtonState, io::MouseButton_MAX);
    m_PrevPosition = m_Position;
    m_Scroll = 0;
}


bool Mouse::isButtonDown(MouseButton button) const
{
    return m_ButtonState[button] == true;
}
bool Mouse::isButtonUp(MouseButton button) const
{
    return m_ButtonState[button] == false;
}
bool Mouse::isButtonReleased(MouseButton button) const
{
    return (m_ButtonState[button] == false && m_PrevButtonState[button] == true);
}
bool Mouse::isButtonPressed(MouseButton button) const
{
    return (m_ButtonState[button] == true && m_PrevButtonState[button] == false);
}

const vec2& Mouse::getPosition() const
{
    return m_Position;
}
int Mouse::getScroll() const
{
    return m_Scroll;
}
vec2 Mouse::getMove() const
{
    return (m_Position - m_PrevPosition);
}
    
void Mouse::setCursorVisible( const bool visible )
{
    SDL_ShowCursor(visible? 1 : 0);
}

void Mouse::setCursor( const MouseCursor cursor )
{
    SDL_SystemCursor cursorName(SDL_SYSTEM_CURSOR_ARROW);
    switch(cursor)
    {
        case io::MouseCursor_Progress:
            cursorName = SDL_SYSTEM_CURSOR_WAIT; break;
            
        case io::MouseCursor_Custom:
        case io::MouseCursor_ZoomIn:
        case io::MouseCursor_ZoomOut:
        case io::MouseCursor_Pointer:
        case io::MouseCursor_Copy:
        case io::MouseCursor_None:
        case io::MouseCursor_VerticalText:
        case io::MouseCursor_Cell:
        case io::MouseCursor_ContextMenu:
        case io::MouseCursor_Alias:
            cursorName = SDL_SYSTEM_CURSOR_ARROW; break;
            
        case io::MouseCursor_Cross:
            cursorName = SDL_SYSTEM_CURSOR_CROSSHAIR; break;
            
        case io::MouseCursor_Grab:
        case io::MouseCursor_Grabbing:
        case io::MouseCursor_Hand:
            cursorName = SDL_SYSTEM_CURSOR_HAND; break;
            
        case io::MouseCursor_Help:
            cursorName = SDL_SYSTEM_CURSOR_ARROW; break;
            
        case io::MouseCursor_IBeam:
            cursorName = SDL_SYSTEM_CURSOR_IBEAM; break;
            
        case io::MouseCursor_NoDrop:
        case io::MouseCursor_NotAllowed:
            cursorName = SDL_SYSTEM_CURSOR_NO; break;
            
        case io::MouseCursor_Move:
        case io::MouseCursor_MiddlePanning:
        case io::MouseCursor_EastPanning:
        case io::MouseCursor_WestPanning:
        case io::MouseCursor_NorthPanning:
        case io::MouseCursor_SouthPanning:
        case io::MouseCursor_NorthEastPanning:
        case io::MouseCursor_SouthWestPanning:
        case io::MouseCursor_NorthWestPanning:
        case io::MouseCursor_SouthEastPanning:
            cursorName = SDL_SYSTEM_CURSOR_SIZEALL; break;
            
        case io::MouseCursor_NorthEastSouthWestResize:
        case io::MouseCursor_NorthEastResize:
        case io::MouseCursor_SouthWestResize:
            cursorName = SDL_SYSTEM_CURSOR_SIZENESW; break;
            
        case io::MouseCursor_NorthSouthResize:
        case io::MouseCursor_NorthResize:
        case io::MouseCursor_RowResize:
        case io::MouseCursor_SouthResize:
            cursorName = SDL_SYSTEM_CURSOR_SIZENS; break;
            
        case io::MouseCursor_NorthWestResize:
        case io::MouseCursor_NorthWestSouthEastResize:
        case io::MouseCursor_SouthEastResize:
            cursorName = SDL_SYSTEM_CURSOR_SIZENWSE; break;
            
        case io::MouseCursor_EastWestResize:
        case io::MouseCursor_EastResize:
        case io::MouseCursor_ColumnResize:
        case io::MouseCursor_WestResize:
            cursorName = SDL_SYSTEM_CURSOR_SIZEWE; break;
            
        case io::MouseCursor_Wait:
            cursorName = SDL_SYSTEM_CURSOR_WAITARROW; break;
        default:
            LOG(LogType_ProgrammerAssert, "Unknow mouse cursor %d", cursor);
    }
    
    SDL_Cursor* oldCursor(m_Cursor);
    m_Cursor = SDL_CreateSystemCursor(cursorName);
    SDL_SetCursor(m_Cursor);
    if (oldCursor != nullptr)
        SDL_FreeCursor(oldCursor);
}
    
} } //end namespace