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
//Created: 13/06/2011
#include "HappyPCH.h"

#include "HeAssert.h"
#include <HappyMessageBox.h>

#pragma warning(disable:4127)
namespace he {
namespace err {
namespace details {

static std::unordered_map<int, std::unordered_set<const char*>> g_IgnoreAsserts;

void happyAssert(AssertType type, const char* file, const char* func, int line, const char* message)
{        
    char infoText[1000];
    sprintf(infoText, "** ASSERTION FAILURE! **\n"
        "Assert in function:\n    %s\n"
        "In file: \n    %s\n"
        "On line: %d\n\n"
        "Message:\n%s", func, file, line, message);
    
    bool ignore = false;
    std::unordered_map<int, std::unordered_set<const char*>>::const_iterator it(g_IgnoreAsserts.find(line));
    if (it != g_IgnoreAsserts.cend())
    {
        ignore = it->second.find(file) != it->second.cend();
    }
    if (ignore == false)
    {
        const HappyMessageBox::Button result(HappyMessageBox::showAssert(type == AssertType_Code? "Assert!" : "Art Assert!", 
            infoText, type == AssertType_Code? HappyMessageBox::Icon_ProgrammerAssert : HappyMessageBox::Icon_ArtAssert, 
            "Debug", "Skip", "Ignore"));
        if (result == HappyMessageBox::Button_Button1)
        {
            #ifdef _MSC_VER
            __debugbreak();
            #else
            __builtin_trap();
            #endif
        }
        else if (result == HappyMessageBox::Button_Button3)
        {
            g_IgnoreAsserts[line].insert(file);
        }
    }
}

} } } //end namespace
#pragma warning(default:4127)
