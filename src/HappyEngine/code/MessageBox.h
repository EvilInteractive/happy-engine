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
//Created: 02/01/2013

#ifndef _HE_MessageBox_H_
#define _HE_MessageBox_H_
#pragma once

#undef MessageBox

namespace he {
enum MessageBoxButton
{
    MessageBoxButton_None,
    MessageBoxButton_Escape,
    MessageBoxButton_Button1,
    MessageBoxButton_Button2,
    MessageBoxButton_Button3
};
enum MessageBoxIcon
{
    MessageBoxIcon_Info,
    MessageBoxIcon_Warning,
    MessageBoxIcon_Error,
    MessageBoxIcon_Success,
    MessageBoxIcon_ProgrammerAssert,
    MessageBoxIcon_ArtAssert
};
class MessageBox
{
public:
    virtual ~MessageBox();

    static MessageBoxButton showExt(const std::string& caption, const std::string& message, MessageBoxIcon icon,
        const std::string& button1="Ok", const std::string& button2="", const std::string& button3="");

private:
    MessageBox();
    
    //Disable default copy constructor and default assignment operator
    MessageBox(const MessageBox&);
    MessageBox& operator=(const MessageBox&);
};

} //end namespace

#endif
