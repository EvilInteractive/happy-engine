//HappyCooker Copyright (C) 2011  Bastian Damman
//
//This file is part of HappyCooker.
//
//    HappyCooker is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyCooker is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyCooker.  If not, see <http://www.gnu.org/licenses/>.
//
//Author:  Bastian Damman
//Created: 21/08/2011

#include "HappyCookerPCH.h"

#include "Main.h"
#include "HappyCooker.h"

#include <StaticDataManager.h>

bool __stdcall initHappyCooker()
{
    he::StaticDataManager::init();    
    happycooker::HappyCooker::getInstance();
    return true;
}
void __stdcall disposeHappyCooker()
{
    happycooker::HappyCooker::dispose();
    he::StaticDataManager::destroy();
}
bool __stdcall cook(const char* input, const char* output)
{
    return happycooker::HappyCooker::getInstance()->cook(input, output);
}

void __stdcall setInfoCallback(bool (__stdcall *infoCallback)(const char*))
{
    happycooker::HappyCooker::getInstance()->setInfoCallback(infoCallback);
    return;
}

void __stdcall setImportPath( const char* path )
{
    happycooker::HappyCooker::getInstance()->setImportPath(path);
}

void __stdcall setModelExportPath( const char* path )
{
    happycooker::HappyCooker::getInstance()->setModelExportPath(path);
}

void __stdcall setPhysicsExportPath( const char* path )
{
    happycooker::HappyCooker::getInstance()->setPhysicsExportPath(path);
}
