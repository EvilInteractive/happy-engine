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
#include "MessageBox.h"

#pragma warning(disable:4127)
namespace he {
namespace err {
namespace details {
bool happyAssert(bool isOk, const char* file, const char* func, int line, const char* message, ...)
{
    if (isOk == true)
        return true;
    
    LOG(tools::LogType_ProgrammerAssert, "**ASSERTION FAILURE!**");
    LOG(tools::LogType_ProgrammerAssert, "assert in function %s", func);
    LOG(tools::LogType_ProgrammerAssert, "", file, line);

    va_list argList;
    va_start(argList, message);
    char buff[1024];
    memset(buff, 0, 1024);
    vsnprintf(buff, 1024, message, argList);
    LOG(tools::LogType_ProgrammerAssert, message, argList);
    va_end(argList);

    char infoText[1000];
    sprintf(infoText, "** ASSERTION FAILURE! **\n"
        "Assert in function:\n    %s\n"
        "In file: \n    %s\n"
        "On line: %d\n\n"
        "Message:\n%s", func, file, line, buff);
    
    if (MessageBox::showExt("Assert!", 
        infoText, MessageBoxIcon_ProgrammerAssert, 
        "-  Debug  -", "-  Skip  -") == MessageBoxButton_Button1)
    {
        #ifndef GCC
        __debugbreak();
        #else
        __builtin_trap();
        #endif
    }
    
    return false;
}

} } } //end namespace
#pragma warning(default:4127)
