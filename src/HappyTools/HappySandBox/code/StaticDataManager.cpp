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
//Created: 2013/04/06
#include "HappySandBoxPCH.h" 

#include "StaticDataManager.h"

#include "system/SelectionManager.h"
#include "system/GameStateMachine.h"
#include "system/UIManager.h"
#include "Sandbox.h"

namespace hs {

void StaticDataManager::init()
{
    Sandbox::sdmInit();
    GameStateMachine::sdmInit();
    UIManager::sdmInit();
    SelectionManger::sdmInit();
}

void StaticDataManager::destroy()
{
    Sandbox::sdmDestroy();
    GameStateMachine::sdmDestroy();
    UIManager::sdmDestroy();
    SelectionManger::sdmDestroy();
}

} //end namespace