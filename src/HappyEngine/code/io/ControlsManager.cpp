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
#include "HappyPCH.h" 

#include "ControlsManager.h"
#include "HappyNew.h"
#include "Mouse.h"
#include "Keyboard.h"


namespace he {
namespace io {

ControlsManager::ControlsManager(): m_pMouse(nullptr), m_pKeyboard(nullptr), m_bLocked(false), m_pLockedObject(nullptr)
{
    m_pMouse = NEW Mouse();
    m_pKeyboard = NEW Keyboard();

    m_pKeys = NEW bool[sf::Keyboard::KeyCount];
    m_pButtons = NEW bool[sf::Mouse::ButtonCount];
}

ControlsManager::~ControlsManager()
{
    delete m_pKeyboard;
    delete m_pMouse;

    delete[] m_pKeys;
    delete[] m_pButtons;
}

void ControlsManager::tick()
{
    for (ushort i(0); i < sf::Keyboard::KeyCount; ++i)
    {
        m_pKeys[i] = false;
    }

    for (ushort i(0); i < sf::Mouse::ButtonCount; ++i)
    {
        m_pButtons[i] = false;
    }

    std::for_each(HAPPYENGINE->getEvents().cbegin(), HAPPYENGINE->getEvents().cend(), [&](sf::Event ev)
    {
        switch(ev.type)
        {
            case sf::Event::KeyPressed:
                m_pKeys[ev.key.code] = true;
                break;
            case sf::Event::MouseButtonPressed:
                m_pButtons[ev.mouseButton.button] = true;
                break;
        }
    });

    m_pKeyboard->tick(m_pKeys);
    m_pMouse->tick(m_pButtons);
}

const IKeyboard* ControlsManager::getKeyboard() const
{
    return m_pKeyboard;
}
const IMouse* ControlsManager::getMouse() const
{
    return m_pMouse;
}

bool ControlsManager::getFocus(void* pObject) const
{
    if (m_pLockedObject == nullptr)
    {
        if (!m_bLocked)
        {
            ControlsManager* _this = const_cast<ControlsManager*>(this);
            _this->m_bLocked = true;
            _this->m_pLockedObject = pObject;

            return true;
        }
        else
            return false;
    }
    else if (m_pLockedObject == pObject)
        return true;
    else
        return false;
}

void ControlsManager::returnFocus(void* pObject) const
{
    if (m_pLockedObject == pObject)
    {
        if (m_bLocked)
        {
            ControlsManager* _this = const_cast<ControlsManager*>(this);
            _this->m_bLocked = false;

            _this->m_pLockedObject = nullptr;
        }
    }
}

bool ControlsManager::hasFocus(void* pObject) const
{
    if (m_pLockedObject == pObject)
        return true;
    else
        return false;
}

} } //end namespace