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
void happyAssert(AssertType type, const char* file, const char* func, int line, const char* message)
{        
    char infoText[1000];
    sprintf(infoText, "** ASSERTION FAILURE! **\n"
        "Assert in function:\n    %s\n"
        "In file: \n    %s\n"
        "On line: %d\n\n"
        "Message:\n%s", func, file, line, message);
    
    if (MessageBox::showExt(type == AssertType_Code? "Assert!" : "Art Assert!", 
        infoText, type == AssertType_Code? MessageBoxIcon_ProgrammerAssert : MessageBoxIcon_ArtAssert, 
        "Debug", "Skip", "Ignore") == MessageBoxButton_Button1)
    {
        #ifndef GCC
        __debugbreak();
        #else
        __builtin_trap();
        #endif
    }
}

} } } //end namespace
#pragma warning(default:4127)
