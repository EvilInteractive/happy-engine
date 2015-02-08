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

#include "MessageBoxWindow.h"

#include <sstream>

#include <SDL2/SDL.h>
#include <assert.h>

namespace he {
namespace hmb {
    
MessageBoxWindow::MessageBoxWindow()
: m_Result(HappyMessageBox::Button_None)
{
}

void MessageBoxWindow::init(const int /*width*/, const int /*height*/, const char* caption)
{
    // create MessageBoxWindow
    m_Title = caption;
}

MessageBoxWindow::~MessageBoxWindow()
{
}

void MessageBoxWindow::show()
{
    m_Result = HappyMessageBox::Button_None;
    
    SDL_MessageBoxData data;
    data.flags = 0;
    switch (m_Icon)
    {
        case HappyMessageBox::Icon_Info: data.flags |= SDL_MESSAGEBOX_INFORMATION; break;
        case HappyMessageBox::Icon_Warning: data.flags |= SDL_MESSAGEBOX_WARNING; break;
        case HappyMessageBox::Icon_Error: data.flags |= SDL_MESSAGEBOX_ERROR; break;
        case HappyMessageBox::Icon_Success: data.flags |= SDL_MESSAGEBOX_INFORMATION; break;
        case HappyMessageBox::Icon_ProgrammerAssert: data.flags |= SDL_MESSAGEBOX_ERROR; break;
        case HappyMessageBox::Icon_ArtAssert: data.flags |= SDL_MESSAGEBOX_WARNING; break;
    }
    data.window = nullptr;
    data.title = m_Title.c_str();
    
    std::stringstream msgStream;
    const size_t messageCount(m_Messages.size());
    for (size_t i(0); i < messageCount; ++i)
    {
        const std::pair<std::string, std::string>& pair(m_Messages[i]);
        msgStream << pair.first << ":\n";
        msgStream << pair.second << "\n\n";
    }
    std::string message(msgStream.str());
    data.message = message.c_str();

    const size_t buttonCount(m_Buttons.size());
    data.numbuttons = buttonCount;
    
    SDL_MessageBoxButtonData* buttons(static_cast<SDL_MessageBoxButtonData*>(malloc(sizeof(SDL_MessageBoxButtonData) * buttonCount)));
    for (size_t i(0); i < buttonCount; ++i)
    {
        SDL_MessageBoxButtonData& button(buttons[i]);
        button.buttonid = i;
        button.text = m_Buttons[i].c_str();
        button.flags = 0;
    }
    data.buttons = buttons;
    data.colorScheme = NULL;
    
    int button(0);
    if (SDL_ShowMessageBox(&data, &button) == 0)
    {
        m_Result = static_cast<HappyMessageBox::Button>(HappyMessageBox::Button_Button1 + button);
    }
    
    free(buttons);
}

void MessageBoxWindow::tick()
{
}
    
void MessageBoxWindow::addButton( const char* button )
{
    m_Buttons.push_back(button);
}

void MessageBoxWindow::addText( const char* tabName, const char* text )
{
    m_Messages.push_back(std::make_pair(tabName, text));
}

void MessageBoxWindow::setIcon( const HappyMessageBox::Icon icon )
{
    m_Icon = icon;
}

} } //end namespace
