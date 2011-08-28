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

#include "Main.h"
#include "HappyCooker.h"

bool __stdcall initHappyCooker()
{
    happycooker::HappyCooker::getInstance();
    return true;
}
void __stdcall disposeHappyCooker()
{
    happycooker::HappyCooker::dispose();
}
bool __stdcall cookObjToBinObj(const char* inputPath, const char* outputPath)
{
    return happycooker::HappyCooker::getInstance()->cookObjToBinObj(inputPath, outputPath);
}
bool __stdcall cookObjToConvex(const char* inputPath, const char* outputPath)
{
    return happycooker::HappyCooker::getInstance()->cookObjToConvex(inputPath, outputPath);
}
