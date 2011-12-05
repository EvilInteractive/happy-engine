//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
#include "StdAfx.h"

#include "HeAssert.h"
#include <iostream>

#pragma warning(disable:4127)
namespace he {
namespace err {
namespace details {
void happyAssert(bool isOk, const std::string& message, const char* file, int line)
{
    if (!isOk)
    {
        std::cout << "**ASSERTION FAILURE!**\n";
        std::cout << "*err in file " << file << "(" << line << ")\n";
        std::cout << "*" << message << "\n";
        #ifdef WINDOWS
        __debugbreak();
        #else
        __builtin_trap();
        #endif
    }
}
void happyAssert(bool isOk, const char* file, int line)
{
    if (!isOk)
    {
        std::cout << "**ASSERTION FAILURE!**\n";
        std::cout << "*err in file " << file << "(" << line << ")\n";
        #ifdef WINDOWS
        __debugbreak();
        #else
        __builtin_trap();
        #endif
    }
}
void happyAssert(int isOk, const char* file, int line)
{
    happyAssert(isOk != 0, file, line);
}
void happyAssert(void* isOk, const char* file, int line)
{
    happyAssert(isOk != nullptr, file, line);
}
void happyAssert(const std::string& message, const char* file, int line)
{
    std::cout << "**ASSERTION FAILURE!**\n";
    std::cout << "*err in file " << file << "(" << line << ")\n";
    std::cout << "*" << message << "\n";
    #ifdef WINDOWS
    __debugbreak();
    #else
    __builtin_trap();
    #endif
}
} } } //end namespace
#pragma warning(default:4127)
