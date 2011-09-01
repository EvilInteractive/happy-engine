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

#include "ControlsManager.h"
#include "HappyNew.h"

namespace happyengine {
namespace io {

ControlsManager::ControlsManager(): m_pKeyboard(NEW Keyboard()), m_pMouse(NEW Mouse())
{
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
void ControlsManager::sdlThreadInvoke()
{
    m_pKeyboard->sdlThreadInvoke();
    m_pMouse->sdlThreadInvoke();
}

const Keyboard* ControlsManager::getKeyboard() const
{
    return m_pKeyboard;
}
const Mouse* ControlsManager::getMouse() const
{
    return m_pMouse;
}

} } //end namespace