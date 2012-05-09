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

    m_pKeys = static_cast<byte*>(he_malloc(sf::Keyboard::KeyCount * sizeof(byte)));
    m_pButtons = static_cast<byte*>(he_malloc(sf::Keyboard::KeyCount * sizeof(byte)));
    memset(m_pKeys, FALSE, sf::Keyboard::KeyCount * sizeof(byte));
    memset(m_pButtons, FALSE, sf::Keyboard::KeyCount * sizeof(byte));
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
    int scrollState(0);
    vec2 mousePos(-1.0f,-1.0f);
    std::vector<char> chars;

    std::for_each(HAPPYENGINE->getEvents().cbegin(), HAPPYENGINE->getEvents().cend(), [&](sf::Event ev)
    {
        switch(ev.type)
        {
            case sf::Event::KeyPressed:
                m_pKeyboard->getOnKeyPressedListeners()((Key)ev.key.code);
                m_pKeys[ev.key.code] = TRUE;
                break;
            case sf::Event::KeyReleased:
                m_pKeyboard->getOnKeyReleasedListeners()((Key)ev.key.code);
                m_pKeys[ev.key.code] = FALSE;
                break;
            case sf::Event::MouseButtonPressed:
                m_pMouse->getOnButtonPressedListeners()((MouseButton)ev.mouseButton.button);
                m_pButtons[ev.mouseButton.button] = TRUE;
                break;
            case sf::Event::MouseButtonReleased:
                m_pMouse->getOnButtonReleasedListeners()((MouseButton)ev.mouseButton.button);
                m_pButtons[ev.mouseButton.button] = FALSE;
                break;
            case sf::Event::MouseWheelMoved:
                m_pMouse->getOnMouseWheelMovedListeners()(ev.mouseWheel.delta);
                scrollState = ev.mouseWheel.delta;
                break;
            case sf::Event::MouseMoved:
                mousePos.x = static_cast<float>(ev.mouseMove.x);
                mousePos.y = static_cast<float>(ev.mouseMove.y);
                m_pMouse->getOnMouseMovedListeners()(mousePos);
                break;
            case sf::Event::TextEntered:
                // ASCII only
                if (ev.text.unicode < 128)
                {
                    chars.push_back((char)ev.text.unicode);
                }
                break;
        }
    });

    m_pKeyboard->tick(m_pKeys, chars);
    m_pMouse->tick(m_pButtons, scrollState, mousePos);
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