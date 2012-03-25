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
//Author:  Sebastiaan Sprengers
//Created: 08/10/2011

#ifndef _HE_CONSOLE_H_
#define _HE_CONSOLE_H_
#pragma once

#include "Font.h"
#include <typeinfo>

namespace he {

enum CMSG_TYPE
{
    CMSG_TYPE_INFO = 0,
    CMSG_TYPE_WARNING = 1,
    CMSG_TYPE_ERROR = 2,
    CMSG_TYPE_ENGINE = 3,
    CMSG_TYPE_COMMAND = 4
};

namespace gui {
    class TextBox;
    class Text;
    class Scrollbar;
}

namespace tools {
class ITypeHandler;
class Console
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Console();
    virtual ~Console();

    /* GENERAL */
    void load();
    void tick();
    void draw();

    void addMessage(const gui::Text& msg, CMSG_TYPE type = CMSG_TYPE_INFO);
    void addMessage(const std::string& msg, CMSG_TYPE type = CMSG_TYPE_INFO);

    template <typename T>
    void registerVar(T* pVar, const std::string& varKey)
    {
        if (m_TypeHandlers.find(typeid(T).name()) != m_TypeHandlers.cend())
        {
            if (m_ValueContainer.find(varKey) != m_ValueContainer.end())
            {
                std::stringstream str;
                str << "Variable: '" << varKey << "' already registered!";

                HE_ASSERT(false, str.str().c_str());
            }
            else
            {
                m_ValueContainer[varKey] = pVar;
            }
        }
        else
        {
            std::stringstream str;
            str << "Type handler for '" << typeid(T).name() << "'not specfied!";

            HE_ASSERT(false, str.str().c_str());
        }
    }

    void registerCmd(boost::function<void()> command, const std::string& cmdKey);

    void addTypeHandler(ITypeHandler* typeHandler);
    void flushMessageHistory();


    /* SETTERS */
    void setKeyboardShortcut(io::Key key);
    void setMessageColors(	const Color& infoColor = Color(1.0f,1.0f,1.0f),
                            const Color& warningColor = Color(1.0f,1.0f,0.0f),
                            const Color& errorColor = Color(1.0f,0.0f,0.0f),
                            const Color& engineColor = Color(0.0f,1.0f,0.0f),
                            const Color& commandColor = Color(0.0f,0.5f,1.0f));
    // * Sets the max messages to keep in console history, 0 == no limit. *
    void setMaxMsgHistory(uint maxMessages = 0);
    void toggleShowMessages(CMSG_TYPE type, bool show);

private:

    void processCommand(const std::string& command);
    void displayHelp();
    void displayVars();
    void displayCmds();

    /* DATAMEMBERS */
    std::map<std::string, boost::any> m_ValueContainer;
    std::map<std::string, boost::function<void()> > m_FunctionContainer;
    std::map<CMSG_TYPE, Color> m_MsgColors;
    std::map<std::string, ITypeHandler*> m_TypeHandlers;
    std::vector<std::pair<CMSG_TYPE, std::string> > m_MsgHistory;
    std::vector<std::string> m_CmdHistory;
    std::map<CMSG_TYPE, bool> m_ShowMessageTypes;

    io::Key m_Shortcut;

    uint m_MaxMessages;
    uint m_CmdHistoryPos;
    uint m_MaxMessagesInWindow;

    bool m_bOpen;

    gui::TextBox* m_pTextBox;
    gui::Text* m_Help;
    gui::Scrollbar* m_pScrollBar;

    std::string m_HelpCommand;

    gfx::Font::pointer m_pFont;

    /* DEFAULT COPY & ASSIGNMENT */
    Console(const Console&);
    Console& operator=(const Console&);
};

} } //end namespace

#endif
