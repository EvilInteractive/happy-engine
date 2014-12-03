//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyCooker.
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
//Created: 21/08/2011

#ifndef _MAIN_H_
#define _MAIN_H_
#pragma once

#ifdef HE_WINDOWS
    #ifdef HappyCookerDll_EXPORTS
        #define HAPPYCOOKER_ENTRY extern "C" __declspec(dllexport) 
    #else
        #define HAPPYCOOKER_ENTRY __declspec(dllimport)
    #endif
#else
    #define HAPPYCOOKER_ENTRY
#endif
    
HAPPYCOOKER_ENTRY bool __stdcall initHappyCooker();
HAPPYCOOKER_ENTRY void __stdcall disposeHappyCooker();

HAPPYCOOKER_ENTRY void __stdcall setImportPath(const char* path);
HAPPYCOOKER_ENTRY void __stdcall setModelExportPath(const char* path);
HAPPYCOOKER_ENTRY void __stdcall setPhysicsExportPath(const char* path);

HAPPYCOOKER_ENTRY bool __stdcall cook(const char* inputFilename, const char* outputName);

HAPPYCOOKER_ENTRY void __stdcall setInfoCallback(bool (__stdcall *infoCallback)(const char*));


#endif
