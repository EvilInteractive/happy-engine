//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 2013/03/03

#include "HappyPluginTestPCH.h" 
#include "HappyPluginTestMain.h"

#include "StaticDataManager.h"

ht::HappyPluginTestMain::HappyPluginTestMain()
{

}

ht::HappyPluginTestMain::~HappyPluginTestMain()
{

}

void ht::HappyPluginTestMain::init()
{
    HE_INFO("HappyPluginTestMain init");
}

void ht::HappyPluginTestMain::terminate()
{
    HE_INFO("HappyPluginTestMain terminate");
}

void ht::HappyPluginTestMain::onLoadLevel( const he::Path& /*path*/ )
{

}

void ht::HappyPluginTestMain::onUnloadLevel()
{

}

void ht::HappyPluginTestMain::onLevelLoaded()
{

}

void ht::HappyPluginTestMain::onStartGame()
{

}

void ht::HappyPluginTestMain::onStopGame()
{

}

void ht::HappyPluginTestMain::onPauseGame()
{

}

void ht::HappyPluginTestMain::onResumeGame()
{

}

void ht::HappyPluginTestMain::sdmInit( he::io::BinaryVisitor& visitor )
{
    he::StaticDataManager::visit(visitor);
}
