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
//Author:  Bastian Damman
//Created: 13/06/2011
#include "HappyPCH.h"

#include "HeAssert.h"
#include <iostream>

#pragma warning(disable:4127)
namespace he {
namespace err {
namespace details {
void happyAssert(bool isOk, const char* file, const char* func, int line, const char* message, const va_list& args)
{
    if (isOk == true)
        return;
    
    LOG(tools::LogType_ProgrammerAssert, "**ASSERTION FAILURE!**");
    LOG(tools::LogType_ProgrammerAssert, "assert in function %s", func);
    LOG(tools::LogType_ProgrammerAssert, "from file %s on line %d", file, line);
    LOG(tools::LogType_ProgrammerAssert, message, args);
    
    // TODO : Messagebox
    #ifndef GCC
    __debugbreak();
    #else
    __builtin_trap();
    #endif
    
}

void happyAssert(int isOk, const char* file, const char* func, int line)
{
    happyAssert(isOk != 0, file, func, line, "", 0);
}
void happyAssert(void* isOk, const char* file, const char* func, int line)
{
    happyAssert(isOk != nullptr, file, func, line, "", 0);
}

} } } //end namespace
#pragma warning(default:4127)
