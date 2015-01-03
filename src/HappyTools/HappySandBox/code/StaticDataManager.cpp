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
//Created: 2013/04/06
#include "HappySandBoxPCH.h" 

#include "StaticDataManager.h"

#include "system/SelectionManager.h"
#include "system/GameStateMachine.h"
#include "system/UIManager.h"
#include "system/PluginManager.h"
#include "system/EditorPickingManager.h"
#include "system/Options.h"
#include "Sandbox.h"

#include "plugins/materialGenerator/system/MaterialGeneratorNodeFactory.h"
#include "plugins/materialGenerator/system/MaterialGeneratorNodeParam.h"

namespace hs {

void StaticDataManager::init()
{
    HSFS::sdmInit();
    Options::sdmInit();
    GameStateMachine::sdmInit();
    UIManager::sdmInit();
    SelectionManger::sdmInit();
    PluginManager::sdmInit();
    EditorPickingManager::sdmInit();
    MaterialGeneratorNodeFactory::sdmInit();
    MaterialGeneratorNodeParam::sdmInit();
}

void StaticDataManager::destroy()
{
    MaterialGeneratorNodeParam::sdmDestroy();
    MaterialGeneratorNodeFactory::sdmDestroy();
    GameStateMachine::sdmDestroy();
    UIManager::sdmDestroy();
    SelectionManger::sdmDestroy();
    PluginManager::sdmDestroy();
    EditorPickingManager::sdmDestroy();
    Options::sdmDestroy();
    HSFS::sdmDestroy();
}

} //end namespace