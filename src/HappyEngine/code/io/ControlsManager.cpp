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

#include "ControlsManager.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "OculusRiftBinding.h"

namespace he {
namespace io {

ControlsManager::ControlsManager()
    : m_OculusRiftBinding(NEW OculusRiftBinding())
    , m_Locked(false)
    , m_LockedObject(nullptr)
{
    //m_OculusRiftBinding->init();
}

ControlsManager::~ControlsManager()
{
    m_OculusRiftBinding->shutdown();
    delete m_OculusRiftBinding;
}

void ControlsManager::tick()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    m_Keyboard.forEach([](const std::pair<ObjectHandle, IKeyboard*>& pair)
    {
        pair.second->tick();
    });
    m_Mouse.forEach([](const std::pair<ObjectHandle, IMouse*>& pair)
    {
        pair.second->tick();
    });
}

IKeyboard* ControlsManager::getKeyboard(const ObjectHandle window) const
{
    TKeyboardMap::const_iterator it(m_Keyboard.find(window));
    return it != m_Keyboard.cend()? it->second : nullptr;
}
IMouse* ControlsManager::getMouse(const ObjectHandle window) const
{
    TMouseMap::const_iterator it(m_Mouse.find(window));
    return it != m_Mouse.cend()? it->second : nullptr;
}

bool ControlsManager::getFocus(void* object)
{
    if (m_LockedObject == nullptr)
    {
        if (!m_Locked)
        {
            m_Locked = true;
            m_LockedObject = object;
            return true;
        }
        else
            return false;
    }
    else if (m_LockedObject == object)
        return true;
    else
        return false;
}

void ControlsManager::returnFocus(void* object)
{
    if (m_LockedObject == object)
    {
        if (m_Locked)
        {
            m_Locked = false;
            m_LockedObject = nullptr;
        }
    }
}

bool ControlsManager::hasFocus(void* object) const
{
    if (m_LockedObject == object)
        return true;
    else
        return false;
}

void ControlsManager::registerInputForWindow( const ObjectHandle window, IKeyboard* keyboard, IMouse* mouse )
{
    m_Keyboard[window] = keyboard;
    m_Mouse[window] = mouse;
}

void ControlsManager::unregisterInputForWindow( const ObjectHandle window )
{
    TKeyboardMap::iterator keyIt(m_Keyboard.find(window));
    if (keyIt != m_Keyboard.end())
    {
        delete (*keyIt).second;
        m_Keyboard.erase(keyIt);
    }
    TMouseMap::iterator mouseIt(m_Mouse.find(window));
    if (mouseIt != m_Mouse.end())
    {
        delete (*mouseIt).second;
        m_Mouse.erase(mouseIt);
    }
}

} } //end namespace