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
//Created: 2014/10/19

#include "HappyPlugin2DTestPCH.h" 

#include "HappyPlugin2DTestMain.h"

PLUGIN_EXPORT he::pl::IPlugin* createPlugin()
{
    return NEW ht::HappyPlugin2DTestMain();
}
#ifdef HE_WINDOWS
BOOL WINAPI DllMain(HINSTANCE /*hinstDLL*/, DWORD fdwReason, LPVOID /*lpReserved*/ )
{
    switch( fdwReason ) 
    { 
    case DLL_PROCESS_ATTACH:
        std::cout << "Plugin HappyPlugin2DTest loaded!\n";
        break;
    case DLL_PROCESS_DETACH:
        std::cout << "Plugin HappyPlugin2DTest unloaded!\n";
        break;

    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}
#elif HE_MAC
__attribute__((constructor)) void DylibEntry()
{
    std::cout << "Plugin HappyPlugin2DTest loaded!\n";
}
__attribute__((destructor)) void DylibLeave()
{
    std::cout << "Plugin HappyPlugin2DTest unloaded!\n";
}
#endif
