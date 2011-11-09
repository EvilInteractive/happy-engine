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
#include "StdAfx.h" 


#if HE_ENABLE_QT
#include "QtMouse.h"

#pragma warning(disable:4127)
#include <QMouseEvent>
#pragma warning(default:4127)

namespace he {
namespace io {

QtMouse::QtMouse(): m_Position(0.0f, 0.0f), m_ButtonState(0), m_NextButtonState(0), m_PrevButtonState(0)
{
}


QtMouse::~QtMouse()
{
}

void QtMouse::tick()
{
    m_PrevButtonState = m_ButtonState;
    m_ButtonState = m_NextButtonState;
}

bool QtMouse::isButtonDown(byte button) const
{
    return (m_ButtonState & button) == button;
}
bool QtMouse::isButtonUp(byte button) const
{
    return (!m_ButtonState & button) == button;
}
bool QtMouse::isButtonReleased(byte button) const
{
    return ((!m_ButtonState & button) == button) && ((m_PrevButtonState & button) == button);
}
bool QtMouse::isButtonPressed(byte button) const
{
    return ((m_ButtonState & button) == button) && ((!m_PrevButtonState & button) == button);
}

vec2 QtMouse::getPosition() const
{
    return m_Position;
}


MouseButton translateButton(Qt::MouseButton button)
{
    switch (button)
    {
    case Qt::LeftButton: return MouseButton_Left;
    case Qt::MiddleButton: return MouseButton_Middle;
    case Qt::RightButton: return MouseButton_Right;
    /*case Qt::QtMouseButton::XButton1: return QtMouseButton_X;
    case Qt::QtMouseButton::XButton2: return QtMouseButton_Left;*/
    }
    return MouseButton_None;
}
void QtMouse::mouseMoveEvent(QMouseEvent* event)
{
    m_Position.x = event->posF().x();
    m_Position.y = event->posF().y();
}
void QtMouse::mousePressEvent(QMouseEvent* event)
{
    m_NextButtonState |= translateButton(event->button());
}
void QtMouse::mouseReleaseEvent(QMouseEvent* event)
{
    m_NextButtonState &= ~translateButton(event->button());
}

} } //end namespace

#endif
