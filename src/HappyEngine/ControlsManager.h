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
//Author: Bastian Damman 
//Created: 08/08/2011

#ifndef _CONTROLS_MANAGER_H_
#define _CONTROLS_MANAGER_H_
#pragma once

#include "Keyboard.h"
#include "Mouse.h"

namespace happyengine {
namespace io {

class ControlsManager
{
public:
	ControlsManager();
    virtual ~ControlsManager();

    void tick();

    const Keyboard* getKeyboard() const;
    const Mouse* getMouse() const;

private:

    Keyboard* m_pKeyboard;
    Mouse* m_pMouse;

    //Disable default copy constructor and default assignment operator
    ControlsManager(const ControlsManager&);
    ControlsManager& operator=(const ControlsManager&);
};

} } //end namespace

#endif
