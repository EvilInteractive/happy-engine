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
//Created: 2013/03/16

#ifndef _HE_HappyMessageBox_H_
#define _HE_HappyMessageBox_H_
#pragma once

#ifdef HappyMessageBox_EXPORTS
#define HMB_ENTRY __declspec(dllexport)
#else
#define HMB_ENTRY __declspec(dllimport)
#endif

#include <string>

namespace he {
class HappyMessageBox
{
public:
    enum Button
    {
        Button_None,
        Button_Escape,
        Button_Button1, // Button_Button1 + i can be used to iterate buttons
        Button_Button2,
        Button_Button3,
        Button_Button4,
        Button_Button5,
        Button_Button6,
        Button_Button7,
        Button_Button8,
        Button_Button9
    };
    enum Icon
    {
        Icon_Info,
        Icon_Warning,
        Icon_Error,
        Icon_Success,
        Icon_ProgrammerAssert,
        Icon_ArtAssert
    };

    ~HappyMessageBox();

    static void init(const char* htmlPage, const int width, const int height);
    static void destroy();

    static Button showExt(const char* caption, const char* message, const Icon icon,
        const char* button1="Ok", const char* button2=0, const char* button3=0);
    static Button showAssert(const char* caption, const char* message, const Icon icon = Icon_ProgrammerAssert,
        const char* button1="Debug", const char* button2="Skip", const char* button3="Ignore");

private:
    HappyMessageBox();

    static int s_DefaultWidth;
    static int s_DefaultHeigth;
    static std::string s_HtmlPage;
    
    //Disable default copy constructor and default assignment operator
    HappyMessageBox(const HappyMessageBox&);
    HappyMessageBox& operator=(const HappyMessageBox&);
};

} //end namespace

#endif
