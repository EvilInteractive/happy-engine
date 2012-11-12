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

#include <typeinfo>
#include "IDrawable2D.h"

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

namespace gfx {
    class Font;
    class Renderer2D;
}

namespace tools {
class ITypeHandler;

class Console : public gfx::IDrawable2D
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Console();
    virtual ~Console();

    /* GENERAL */
    void load();
    void tick();

    void attachToRenderer(gfx::Renderer2D* renderer);
    void detachFromRenderer();

    virtual void draw2D(gfx::Canvas2D* canvas); // auto called

    void addMessage(const gui::Text& msg, CMSG_TYPE type = CMSG_TYPE_INFO);
    void addMessage(const std::string& msg, CMSG_TYPE type = CMSG_TYPE_INFO);

    template <typename T>
    void registerVar(T* pVar, const std::string& varKey)
    {
        const char* type(typeid(T).name());
        HE_IF_ASSERT(m_TypeHandlers.find(type) != m_TypeHandlers.cend(), "Type handler for '%s'not specified!", type)
        {
            HE_IF_ASSERT(m_ValueContainer.find(varKey) == m_ValueContainer.end(), "Variable: '%s' already registered!", varKey.c_str())
            {
                m_ValueContainer[varKey] = pVar;
            }
        }
    }

    void registerCmd(const boost::function<void()>& command, const std::string& cmdKey);

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
    void setMaxMsgHistory(uint32 maxMessages = 0);
    void toggleShowMessages(CMSG_TYPE type, bool show);

private:

    void processCommand(const std::string& command);
    void displayHelp();
    void displayVars();
    void displayCmds();
	void onResize();

    /* DATAMEMBERS */
    std::map<std::string, boost::any> m_ValueContainer;
    std::map<std::string, boost::function<void()> > m_FunctionContainer;
    std::map<CMSG_TYPE, Color> m_MsgColors;
    std::map<std::string, ITypeHandler*> m_TypeHandlers;
    he::ObjectList<std::pair<CMSG_TYPE, std::string> > m_MsgHistory;
    he::ObjectList<std::string> m_CmdHistory;
    std::map<CMSG_TYPE, bool> m_ShowMessageTypes;

    io::Key m_Shortcut;

    uint32 m_MaxMessages;
    uint32 m_CmdHistoryPos;
    uint32 m_MaxMessagesInWindow;

    bool m_IsOpen;

    gui::TextBox* m_TextBox;
    gui::Text* m_Help;
    gui::Scrollbar* m_ScrollBar;

    std::string m_HelpCommand;

    gfx::Font* m_Font;
    gfx::Renderer2D* m_Renderer;

    vec2 m_ViewportSize;
    eventCallback0<void> m_ResizeHandler;

    /* DEFAULT COPY & ASSIGNMENT */
    Console(const Console&);
    Console& operator=(const Console&);
};

} } //end namespace

#endif
