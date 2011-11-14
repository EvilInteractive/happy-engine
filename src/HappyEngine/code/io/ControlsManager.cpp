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

#include "ControlsManager.h"
#include "HappyNew.h"
#include "Mouse.h"
#include "QtMouse.h"
#include "Keyboard.h"
#include "QtKeyboard.h"


namespace he {
namespace io {

ControlsManager::ControlsManager(bool qt): m_pMouse(nullptr), m_pKeyboard(nullptr)
{
    if (qt == false)
    {
        m_pMouse = NEW Mouse();
        m_pKeyboard = NEW Keyboard();
    }
#if HE_ENABLE_QT
    else
    {
        m_pMouse = NEW QtMouse();
        m_pKeyboard = NEW QtKeyboard();
    }
#endif
}


ControlsManager::~ControlsManager()
{
    delete m_pKeyboard;
    delete m_pMouse;
}

void ControlsManager::tick()
{
    m_pKeyboard->tick();
    m_pMouse->tick();
}

const IKeyboard* ControlsManager::getKeyboard() const
{
    return m_pKeyboard;
}
const IMouse* ControlsManager::getMouse() const
{
    return m_pMouse;
}

} } //end namespace