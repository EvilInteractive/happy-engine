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
#include "HappyPCH.h"

#include "Console.h"

#include "GraphicsEngine.h"
#include "ContentManager.h"
#include "ControlsManager.h"
#include "Renderer2D.h"

#include "BoolTypeHandler.h"
#include "FloatTypeHandler.h"
#include "IntTypeHandler.h"
#include "UIntTypeHandler.h"
#include "Vec2TypeHandler.h"
#include "Vec3TypeHandler.h"
#include "Vec4TypeHandler.h"
#include "StringTypeHandler.h"

#include "Scrollbar.h"
#include "TextBox.h"
#include "View.h"
#include "Text.h"

namespace he {
namespace tools {

/* CONSTRUCTOR - DESTRUCTOR */
Console::Console() :	m_Shortcut(io::Key_C),
                        m_MaxMessages(0),
                        m_IsOpen(false),
                        m_TextBox(nullptr),
                        m_MsgHistory(0),
                        m_CmdHistoryPos(0),
                        m_ScrollBar(nullptr),
                        m_Help(nullptr),
                        m_Font(nullptr),
                        m_Renderer(nullptr)
{
    m_MsgColors[CMSG_TYPE_INFO] = Color(1.0f,1.0f,1.0f);
    m_MsgColors[CMSG_TYPE_WARNING] = Color(1.0f,0.9f,0.6f);
    m_MsgColors[CMSG_TYPE_ERROR] = Color(1.0f,0.6f,0.6f);
    m_MsgColors[CMSG_TYPE_ENGINE] = Color(0.6f,1.0f,0.6f);
    m_MsgColors[CMSG_TYPE_COMMAND] = Color(0.6f,0.9f,1.0f);

    m_HelpCommand = "type 'help' to see available commands...";

    addTypeHandler(NEW BoolTypeHandler());
    addTypeHandler(NEW FloatTypeHandler());
    addTypeHandler(NEW IntTypeHandler());
    addTypeHandler(NEW UIntTypeHandler());
    addTypeHandler(NEW Vec2TypeHandler());
    addTypeHandler(NEW Vec3TypeHandler());
    addTypeHandler(NEW Vec4TypeHandler());
    addTypeHandler(NEW StringTypeHandler());

    addMessage(m_HelpCommand);

    m_ShowMessageTypes[CMSG_TYPE_COMMAND] = true;
    m_ShowMessageTypes[CMSG_TYPE_INFO] = true;
    m_ShowMessageTypes[CMSG_TYPE_WARNING] = true;
    m_ShowMessageTypes[CMSG_TYPE_ERROR] = true;
    m_ShowMessageTypes[CMSG_TYPE_ENGINE] = true;

    registerVar<bool>(&m_ShowMessageTypes[CMSG_TYPE_ENGINE], "c_show_msg_engine");

	// console commands
	registerCmd(boost::bind(&he::tools::Console::displayHelp, this), "help");
	registerCmd(boost::bind(&he::tools::Console::displayVars, this), "listvars");
	registerCmd(boost::bind(&he::tools::Console::displayCmds, this), "listcmds");
}
void Console::load()
{
    m_Font = CONTENT->loadFont("Inconsolata.otf", 10);

    m_Help = NEW gui::Text(m_Font);
    m_Help->addLine("******** HELP ********");
    m_Help->addLine("'listvars' (displays registered variables and their type)");
    m_Help->addLine("'listcmds' (displays registered commands)");
    m_Help->addLine("******** HELP ********");

    m_ScrollBar = NEW gui::Scrollbar(
        vec2(1280-20, 0), vec2(20,200), 50.0f);

    m_TextBox = NEW gui::TextBox(
        RectF(0,200,1280, 20),
        "Enter command...", 10, "Inconsolata.otf");

    m_TextBox->setColors(
        Color(0.3f,0.3f,0.3f,0.9f),
        Color(1.0f,1.0f,1.0f),
        Color(0.8f,0.8f,0.8f),
        Color(0.19f,0.19f,0.19f));

    m_CmdHistory.add("");

    m_ScrollBar->setBarPos(1.0f);

    m_MaxMessagesInWindow = static_cast<uint32>(190 / m_Font->getLineSpacing());
}

Console::~Console()
{
    std::for_each(m_TypeHandlers.begin(), m_TypeHandlers.end(), [&](std::pair<std::string, ITypeHandler*> p)
    {
        delete p.second;
        p.second = nullptr;
    });

    m_TypeHandlers.clear();

    if (m_Font != nullptr)
        m_Font->release();
	
    delete m_TextBox;
    delete m_ScrollBar;
    delete m_Help;
}

void Console::processCommand(const std::string& command)
{
    std::string s(command);

    // remove spaces
    #if !GCC
    s.erase(std::remove_if(s.begin(), s.end(), std::isspace), s.end());
    #else // !FIX! will be fixed when work on linux resumed
    #error What if GCC?
    #endif

    if (s.find('=') != -1)
    {
        // get keyword (variable to change)
        std::string keyWord(s.substr(0, s.find('=')));

        if (m_ValueContainer.find(keyWord) != m_ValueContainer.end())
        {
            std::string values(command.substr(command.find('=') + 1, command.size() - 1));

            std::string type(m_ValueContainer[keyWord].type().name());
            type = type.substr(0, type.size() - 2);

            if (m_TypeHandlers.find(type) != m_TypeHandlers.cend())
            {
                if (m_TypeHandlers[type]->parse(values, m_ValueContainer[keyWord]))
                {
                    addMessage(m_TextBox->getString(), CMSG_TYPE_COMMAND);
                }
                else
                {
                    addMessage("\"" + values + "\" can't be assigned to '" + keyWord + "' !", CMSG_TYPE_ERROR);
                }
            }
            else
            {
                addMessage("no type handler specified for this keyword '" + keyWord + "' - type: " + type + "!", CMSG_TYPE_ERROR);
            }
        }
        else
        {
            addMessage("the keyword '" + keyWord + "' was not found!", CMSG_TYPE_ERROR);
        }
    }
    else if (m_FunctionContainer.find(s) != m_FunctionContainer.end()) // check if it's a command
    {
        addMessage(m_TextBox->getString(), CMSG_TYPE_COMMAND);

        // execute command
        m_FunctionContainer[s]();
    }
    else
    {
        addMessage("the command/variable '" + s + "' was not found!", CMSG_TYPE_ERROR);
    }
}

void Console::displayHelp()
{
    addMessage(*m_Help, CMSG_TYPE_INFO);
}

void Console::displayVars()
{
    gui::Text txt;

    txt.addLine("******** VARS ********");

    if (m_ValueContainer.empty())
    {
        txt.addLine("!no registered variables!");
    }
    else
    {
        std::for_each(m_ValueContainer.cbegin(), m_ValueContainer.cend(), [&] (std::pair<std::string, boost::any> p)
        {
            std::string type(p.second.type().name());
            type = type.substr(0, type.size() - 2);

            txt.addLine("'" + p.first + "' (" + type + ")");
        });
    }

    txt.addLine("******** VARS ********");

    addMessage(txt, CMSG_TYPE_INFO);
}

void Console::displayCmds()
{
    gui::Text txt;

    txt.addLine("******** CMDS ********");

    if (m_FunctionContainer.empty())
    {
        txt.addLine("!no registered commands!");
    }
    else
    {
        std::for_each(m_FunctionContainer.cbegin(), m_FunctionContainer.cend(), [&] (std::pair<std::string, boost::function<void()> > p)
        {
            txt.addLine("'" + p.first + "'");
        });
    }

    txt.addLine("******** CMDS ********");

    addMessage(txt, CMSG_TYPE_INFO);
}

/* GENERAL */
void Console::tick()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    if (CONTROLS->getKeyboard()->isKeyPressed(m_Shortcut) && !m_TextBox->hasFocus())
    {
        m_IsOpen = !m_IsOpen;
        m_TextBox->resetText();

        if (m_MsgHistory[m_MsgHistory.size() - 1].second != m_HelpCommand)
        {
            addMessage(m_HelpCommand, CMSG_TYPE_INFO);
        }

        if (m_IsOpen)
            m_Renderer->attachToRender(this);
        else
            m_Renderer->detachFromRender(this);
    }

    if (m_IsOpen)
    {
        m_TextBox->tick();

        if (m_MsgHistory.size() > m_MaxMessagesInWindow)
            m_ScrollBar->tick();

        if (CONTROLS->getKeyboard()->isKeyPressed(io::Key_Down))
        {
            m_TextBox->setString(m_CmdHistory[m_CmdHistoryPos]);

            if (m_CmdHistoryPos > 0)
            {
                --m_CmdHistoryPos;
            }
            else
            {
                m_CmdHistoryPos = (uint32)m_CmdHistory.size() - 1;
            }
        }
        else if (CONTROLS->getKeyboard()->isKeyPressed(io::Key_Up))
        {
            m_TextBox->setString(m_CmdHistory[m_CmdHistoryPos]);

            if (m_CmdHistoryPos < m_CmdHistory.size() - 1)
            {
                ++m_CmdHistoryPos;
            }
            else
            {
                m_CmdHistoryPos =0;
            }
        }

        if (m_TextBox->entered())
        {
            processCommand(m_TextBox->getString());;

            if (m_CmdHistory[m_CmdHistory.size() - 1] != m_TextBox->getString())
            {
                m_CmdHistory.add(m_TextBox->getString());
                m_CmdHistoryPos = (uint32)m_CmdHistory.size() - 1;
            }

            m_TextBox->resetText();

            m_ScrollBar->setBarPos(1.0f);
        }
    }
}

void Console::draw2D(gfx::Canvas2D* canvas)
{
    canvas->setBlendStyle(gfx::BlendStyle_Alpha);

    canvas->setDepth(-2000);

    canvas->setFillColor(Color(0.2f,0.2f,0.2f,0.9f));
    canvas->fillRect(vec2(0,0), vec2(canvas->getSize().x, 200));

    canvas->setStrokeColor(Color(0.19f,0.19f,0.19f));
    canvas->strokeRect(vec2(0,0), vec2(canvas->getSize().x, 200));

    m_TextBox->draw(canvas);

    he::ObjectList<std::pair<CMSG_TYPE, std::string>> msgHistory;

    m_MsgHistory.forEach([&](const std::pair<CMSG_TYPE, std::string>& p)
    {
        if (m_ShowMessageTypes[p.first] == true)
            msgHistory.add(p);
    });

    uint32 startPos(0);

    if (msgHistory.size() > m_MaxMessagesInWindow)
    {
        startPos = static_cast<uint32>((msgHistory.size() - 1 - m_MaxMessagesInWindow) * m_ScrollBar->getBarPos());
    }

    uint32 i(startPos);

    he::ObjectList<std::pair<CMSG_TYPE, std::string> > msg;

    if (msgHistory.size() > m_MaxMessagesInWindow)
    {
        for (; i <= (startPos + m_MaxMessagesInWindow); ++i)
        {
            msg.add(msgHistory[i]);
        }
    }
    else
    {
        for (; i < msgHistory.size(); ++i)
        {
            msg.add(msgHistory[i]);
        }
    }

    gui::Text text(m_Font);
    text.setHorizontalAlignment(gui::Text::HAlignment_Left);
    text.setVerticalAlignment(gui::Text::VAlignment_Bottom);

    uint32 k(0);
    msg.rForEach([&](const std::pair<CMSG_TYPE, std::string>& p)
    {
        canvas->setFillColor(m_MsgColors[p.first]);

        text.clear();
        text.addLine(p.second);

        canvas->fillText(text, vec2(5, 182.0f - (k * m_Font->getLineSpacing())));

        ++k;
    });

    if (msgHistory.size() > m_MaxMessagesInWindow)
        m_ScrollBar->draw(canvas);

    canvas->restoreDepth();
}

void Console::addMessage(const gui::Text& msg, CMSG_TYPE type)
{
    std::for_each(msg.getText().cbegin(), msg.getText().cend(), [&](std::string str)
    {
        if (type == CMSG_TYPE_COMMAND)
            m_MsgHistory.add(std::pair<CMSG_TYPE, std::string>(type, "] " + str));
        else
            m_MsgHistory.add(std::pair<CMSG_TYPE, std::string>(type, str));
    });

    if (m_MsgHistory.size() > m_MaxMessages && m_MaxMessages != 0)
    {
        m_MsgHistory.orderedRemoveAt(0);
    }
}

void Console::addMessage(const std::string& msg, CMSG_TYPE type)
{
    if (type == CMSG_TYPE_COMMAND)
        m_MsgHistory.add(std::pair<CMSG_TYPE, std::string>(type, "] " + msg));
    else
        m_MsgHistory.add(std::pair<CMSG_TYPE, std::string>(type, msg));

    if (m_MsgHistory.size() > m_MaxMessages && m_MaxMessages != 0)
    {
        m_MsgHistory.orderedRemoveAt(0);
    }
}

void Console::registerCmd(const boost::function<void()>& command, const std::string& cmdKey)
{
    HE_IF_ASSERT(m_FunctionContainer.find(cmdKey) == m_FunctionContainer.end(), "Command: '%s' already registered", cmdKey.c_str())
    {
        m_FunctionContainer[cmdKey] = command;
    }
}

void Console::addTypeHandler(ITypeHandler* typeHandler)
{
    HE_IF_ASSERT(m_TypeHandlers.find(typeHandler->getType()) == m_TypeHandlers.cend(), "Type handler for '%s' already added!", typeHandler->getType().c_str())
    {
        m_TypeHandlers[typeHandler->getType()] = typeHandler;
    }
}

void Console::flushMessageHistory()
{
    m_MsgHistory.clear();

    addMessage(m_HelpCommand);
}

/* SETTERS */
void Console::setKeyboardShortcut(io::Key key)
{
    m_Shortcut = key;
}

void Console::setMessageColors(	const Color& infoColor,
                                const Color& warningColor,
                                const Color& errorColor,
                                const Color& engineColor,
                                const Color& commandColor)
{
    m_MsgColors[CMSG_TYPE_INFO] = infoColor;
    m_MsgColors[CMSG_TYPE_WARNING] = warningColor;
    m_MsgColors[CMSG_TYPE_ERROR] = errorColor;
    m_MsgColors[CMSG_TYPE_ENGINE] = engineColor;
    m_MsgColors[CMSG_TYPE_COMMAND] = commandColor;
}

void Console::setMaxMsgHistory(uint32 maxMessages)
{
    m_MaxMessages = maxMessages;
}

void Console::toggleShowMessages(CMSG_TYPE type, bool show)
{
    m_ShowMessageTypes[type] = show;
}

void Console::attachToRenderer( gfx::Renderer2D* renderer )
{
    HE_IF_ASSERT(m_Renderer == nullptr, "Console already attached to a renderer")
    {
        m_Renderer = renderer;

        gfx::View* view(renderer->getView());
        m_ScrollBar->setPosition(vec2(static_cast<float>(view->getViewport().width) - 20.0f, 0.0f));
        m_TextBox->setSize(vec2(static_cast<float>(view->getViewport().width), 20));
        m_ScrollBar->setBarPos(1.0f);

        if (m_IsOpen)
        {
            m_Renderer->attachToRender(this);
        }
    }
}

void Console::detachFromRenderer()
{
    HE_IF_ASSERT(m_Renderer != nullptr, "Console not attached to a renderer")
    {
        if (m_IsOpen)
        {
            m_Renderer->detachFromRender(this);
        }
        m_Renderer = nullptr;
    }
}

} } //end namespace
