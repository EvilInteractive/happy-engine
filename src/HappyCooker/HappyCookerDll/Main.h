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

#ifdef HAPPYCOOKER_DLL
#define EXPORT extern "C" __declspec(dllexport) 
#else
#define EXPORT __declspec(dllimport)
#endif
    
EXPORT bool __stdcall initHappyCooker();
EXPORT void __stdcall disposeHappyCooker();
EXPORT bool __stdcall cookToBinObj(const char* inputPath, const char* outputPath);
EXPORT bool __stdcall cookLineToBinObj(const char* inputPath, const char* outputPath);
EXPORT bool __stdcall cookToConvex(const char* inputPath, const char* outputPath);
EXPORT void __stdcall setInfoCallback(bool (__stdcall *infoCallback)(const char*));


#endif
