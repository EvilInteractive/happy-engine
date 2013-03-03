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

PLUGIN_EXPORT he::pl::IPlugin* createPlugin()
{
    return NEW ht::HappyPluginTestMain();
}

BOOL WINAPI DllMain(HINSTANCE /*hinstDLL*/, DWORD fdwReason, LPVOID /*lpReserved*/ )
{
    switch( fdwReason ) 
    { 
    case DLL_PROCESS_ATTACH:
        std::cout << "Plugin HappyPluginTestMain loaded!\n";
        break;
    case DLL_PROCESS_DETACH:
        std::cout << "Plugin HappyPluginTestMain unloaded!\n";
        break;

    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}
