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
    : m_OculusRiftBinding(HENew(OculusRiftBinding)())
    , m_Locked(false)
    , m_LockedObject(nullptr)
{
    //m_OculusRiftBinding->init();
}

ControlsManager::~ControlsManager()
{
    m_OculusRiftBinding->shutdown();
    HEDelete(m_OculusRiftBinding);
}

void ControlsManager::tick()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    m_Keyboard.forEach([](const ObjectHandle& /*key*/, IKeyboard* value)
    {
        value->tick();
    });
    m_Mouse.forEach([](const ObjectHandle& /*key*/, IMouse* value)
    {
        value->tick();
    });
}

IKeyboard* ControlsManager::getKeyboard(const ObjectHandle window) const
{
    IKeyboard** it = m_Keyboard.find(window);
    return it? *it : nullptr;
}
IMouse* ControlsManager::getMouse(const ObjectHandle window) const
{
    IMouse** it = m_Mouse.find(window);
    return it? *it : nullptr;
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
    IKeyboard** keyIt = m_Keyboard.find(window);
    if (keyIt)
    {
        HEDelete(*keyIt);
        m_Keyboard.remove(window);
    }
    IMouse** mouseIt = m_Mouse.find(window);
    if (mouseIt)
    {
        HEDelete(*mouseIt);
        m_Mouse.remove(window);
    }
}

} } //end namespace