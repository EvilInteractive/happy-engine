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
//Author: Bastian Damman 
//Created: 08/08/2011

#ifndef _HE_CONTROLS_MANAGER_H_
#define _HE_CONTROLS_MANAGER_H_
#pragma once

#include <Map.h>

namespace he {
namespace io {
class IMouse;
class IKeyboard;
class OculusRiftBinding;

class HAPPY_ENTRY ControlsManager
{
public:
    ControlsManager();
    virtual ~ControlsManager();

    void tick();

    IKeyboard* getKeyboard(const ObjectHandle window) const;
    IMouse* getMouse(const ObjectHandle window) const;
    OculusRiftBinding* getOculusRiftBinding() const { return m_OculusRiftBinding; }

    bool getFocus(void* object);
    void returnFocus(void* object);
    bool hasFocus(void* object) const;

    void registerInputForWindow(const ObjectHandle window, IKeyboard* keyboard, IMouse* mouse); // Takes ownership of keyboard and mouse
    void unregisterInputForWindow(const ObjectHandle window);

private:

    typedef he::Map<ObjectHandle, IKeyboard*, ObjectHandle> TKeyboardMap;
    typedef he::Map<ObjectHandle, IMouse*, ObjectHandle> TMouseMap;

    TKeyboardMap m_Keyboard;
    TMouseMap m_Mouse;
    OculusRiftBinding* m_OculusRiftBinding;

    bool m_Locked;
    void* m_LockedObject;

    //Disable default copy constructor and default assignment operator
    ControlsManager(const ControlsManager&);
    ControlsManager& operator=(const ControlsManager&);
};

} } //end namespace

#endif
