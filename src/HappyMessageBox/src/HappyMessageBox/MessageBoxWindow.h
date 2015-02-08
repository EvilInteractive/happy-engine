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
//Created: 2013/03/16

#ifndef _HE_MessageBoxWindow_H_
#define _HE_MessageBoxWindow_H_
#pragma once

#include "HappyMessageBox.h"
#include <string>
#include <vector>

namespace sf {
    class Window;
}

namespace he {
namespace hmb {

class MessageBoxWindow
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    MessageBoxWindow();
    ~MessageBoxWindow();
    
    /* GENERAL */
    void init(const int width, const int height, const char* caption);
    void addButton(const char* button);
    void addText(const char* tabName, const char* text);
    void setIcon(const HappyMessageBox::Icon icon);
    void show();
        
    HappyMessageBox::Button getResult() { return m_Result; }
    
private:
    void tick();

    /* DATAMEMBERS */
    sf::Window* m_Window;

    std::string m_WebPage;
    std::string m_Title;

    std::vector<std::string> m_Buttons;
    std::vector<std::pair<std::string, std::string>> m_Messages;

    HappyMessageBox::Icon m_Icon;
    HappyMessageBox::Button m_Result;
    
    /* DEFAULT COPY & ASSIGNMENT */
    MessageBoxWindow(const MessageBoxWindow&);
    MessageBoxWindow& operator=(const MessageBoxWindow&);
};

} } //end namespace

#endif
