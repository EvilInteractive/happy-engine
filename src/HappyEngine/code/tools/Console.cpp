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
#include "Gui.h"

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

namespace he {
namespace tools {

/* CONSTRUCTOR - DESTRUCTOR */
Console::Console() :	m_Shortcut(io::Key_F1),
                        m_MaxMessages(0),
                        m_MaxMessagesInWindow(0),
                        m_IsOpen(false),
                        m_TextBox(nullptr),
                        m_MsgHistory(0),
                        m_CmdHistoryPos(0),
                        m_ScrollBar(nullptr),
                        m_Help(""),
                        m_Font(nullptr),
                        m_Renderer(nullptr),
                        m_ViewportSize(20,20),
                        m_Background(nullptr)
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

    addMessage(m_Help.c_str());

    m_ShowMessageTypes[CMSG_TYPE_COMMAND] = true;
    m_ShowMessageTypes[CMSG_TYPE_INFO] = true;
    m_ShowMessageTypes[CMSG_TYPE_WARNING] = true;
    m_ShowMessageTypes[CMSG_TYPE_ERROR] = true;
    m_ShowMessageTypes[CMSG_TYPE_ENGINE] = true;

    registerVar<bool>(&m_ShowMessageTypes[CMSG_TYPE_ENGINE], "c_show_msg_engine");

    // console commands
    registerCmd(std::bind(&he::tools::Console::displayHelp, this), "help");
    registerCmd(std::bind(&he::tools::Console::displayVars, this), "listvars");
    registerCmd(std::bind(&he::tools::Console::displayCmds, this), "listcmds");
}
void Console::load()
{
    // don't compress font because it is small and needs crispness
    m_Font = CONTENT->loadFont("DejaVuSansMono.ttf", 8, gui::Font::NO_COMPRESSION);
    if (m_Font != nullptr)
    {
        m_Text.setFont(m_Font);
        m_MaxMessagesInWindow = static_cast<uint32>(190 / m_Font->getLineSpacing());
    }
    m_Text.setHorizontalAlignment(gui::Text::HAlignment_Left);
    m_Text.setVerticalAlignment(gui::Text::VAlignment_Top);

    m_Help = "******** HELP ********\n"
             "'listvars' (displays registered variables and their type)\n"
             "'listcmds' (displays registered commands)\n"
             "******** HELP ********";

    m_ScrollBar = NEW gui::Scrollbar(
        vec2(1280-20, 0), vec2(20,200), 50.0f);

    m_TextBox = NEW gui::TextBox(
        RectF(0,200,1280, 20),
        "Enter command...", 8, "DejaVuSansMono.ttf");

    m_TextBox->setColors(
        Color(0.3f,0.3f,0.3f,0.9f),
        Color(1.0f,1.0f,1.0f),
        Color(0.8f,0.8f,0.8f),
        Color(0.19f,0.19f,0.19f));

    m_CmdHistory.add("");

    m_ScrollBar->setBarPos(1.0f);

    gui::SpriteCreator* const cr(GUI->getSpriteCreator());
    m_Background = cr->createSprite(vec2(1280.0f, 200.0f));
    renderBackground();
}

Console::~Console()
{
    std::for_each(m_TypeHandlers.begin(), m_TypeHandlers.end(), [&](std::pair<he::String, ITypeHandler*> p)
    {
        delete p.second;
        p.second = nullptr;
    });

    m_TypeHandlers.clear();

    if (m_Font != nullptr)
        m_Font->release();
    
    delete m_TextBox;
    delete m_ScrollBar;
}

void Console::processCommand(const he::String& command)
{
    he::String s(command);

    // remove spaces
    #if !GCC
    s.erase(std::remove_if(s.begin(), s.end(), isspace), s.end());
    #else // !FIX! will be fixed when work on linux resumed
    #error What if GCC?
    #endif

    if (s.find('=') != -1)
    {
        // get keyword (variable to change)
        he::String keyWord(s.substr(0, s.find('=')));

        if (m_ValueContainer.find(keyWord) != m_ValueContainer.end())
        {
            he::String values(s.substr(s.find('=') + 1, s.size() - 1));

            he::String type(m_ValueContainer[keyWord].type().name());
            type = type.substr(0, type.size() - 2);

            if (m_TypeHandlers.find(type) != m_TypeHandlers.cend())
            {
                if (m_TypeHandlers[type]->parse(values, m_ValueContainer[keyWord]))
                {
                    addMessage(m_TextBox->getString().c_str(), CMSG_TYPE_COMMAND);
                }
                else
                {
                    HE_ERROR("\"%s\" can't be assigned to '%s'!", values.c_str(), keyWord.c_str());
                }
            }
            else
            {
                HE_ERROR("no type handler specified for this keyword '%s' - type:%s!", keyWord.c_str(), type.c_str());
            }
        }
        else
        {
            HE_ERROR("the keyword '%s' was not found!", keyWord.c_str());
        }
    }
    else if (m_FunctionContainer.find(s) != m_FunctionContainer.end()) // check if it's a command
    {
        addMessage(m_TextBox->getString().c_str(), CMSG_TYPE_COMMAND);

        // execute command
        m_FunctionContainer[s]();
    }
    else
    {
        HE_ERROR("the command/variable '%s' was not found!", s.c_str());
    }
}

void Console::displayHelp()
{
    addMessage(m_Help.c_str(), CMSG_TYPE_INFO);
}

void Console::displayVars()
{
    std::stringstream stream;

    stream << "******** VARS ********\n";

    if (m_ValueContainer.empty())
    {
        stream << "!no registered variables!\n";
    }
    else
    {
        std::for_each(m_ValueContainer.cbegin(), m_ValueContainer.cend(), [&] (std::pair<he::String, boost::any> p)
        {
            he::String type(p.second.type().name());
            type = type.substr(0, type.size() - 2);

            stream << "'" << p.first << "' (" << type << ")\n";
        });
    }

    stream << "******** VARS ********";

    addMessage(stream.str().c_str(), CMSG_TYPE_INFO);
}

void Console::displayCmds()
{
    std::stringstream stream;

    stream << "******** CMDS ********\n";

    if (m_FunctionContainer.empty())
    {
        stream << "!no registered commands!\n";
    }
    else
    {
        std::for_each(m_FunctionContainer.cbegin(), m_FunctionContainer.cend(), [&] (std::pair<he::String, boost::function<void()> > p)
        {
            stream << "'" << p.first << "'\n";
        });
    }

    stream << "******** CMDS ********";

    addMessage(stream.str().c_str(), CMSG_TYPE_INFO);
}

/* GENERAL */
void Console::tick()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    if (CONTROLS->getKeyboard()->isKeyPressed(m_Shortcut))
    {
        m_IsOpen = !m_IsOpen;
        m_TextBox->resetText();

        he::String& s = m_MsgHistory[m_MsgHistory.size() - 1].second;

        if (s != he::String("&FFF").append(m_HelpCommand))
        {
            addMessage(m_HelpCommand.c_str(), CMSG_TYPE_INFO);
        }

        HE_IF_ASSERT(m_Renderer != nullptr, "Please set the renderer on CONSOLE!")
        {
            if (m_IsOpen)
                m_Renderer->attachToRender(this, 1);
            else
                m_Renderer->detachFromRender(this);
        }
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
                m_CmdHistoryPos = 0;
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

void Console::draw2D(gui::Canvas2D* canvas)
{
    canvas->drawSprite(m_Background, vec2(0,0));

    m_TextBox->draw(canvas);
    
    const size_t historySize(m_MsgHistory.size());
    uint32 startPos(static_cast<uint32>((historySize - m_MaxMessagesInWindow) * m_ScrollBar->getBarPos()));
    
    m_Text.clear();
    size_t maxMsg(startPos + m_MaxMessagesInWindow);

    for (size_t i(startPos); i < maxMsg; ++i)
    {
        m_Text.addTextExt("%s\n", m_MsgHistory[i].second.c_str());
    }

    canvas->fillText(m_Text, vec2(5, 5));
    
    if (m_MsgHistory.size() > m_MaxMessagesInWindow)
        m_ScrollBar->draw(canvas);
}

void Console::addMessage(const char* msg, CMSG_TYPE type)
{
    char buff[1024];
    
    Color col(m_MsgColors[type]);

    sprintf(buff, "&%c%c%c%s%s", 
        col.r16(), col.g16(), col.b16(), 
        type == CMSG_TYPE_COMMAND? "] " : "", msg);

    size_t size(strlen(buff));
    buff[size] = 0;

    size_t lineCharStart(0);
    for (size_t i(0); i < size; ++i)
    {
        const char character(buff[i]);

        if (character == '\n')
        {
            const size_t lineSize(i - lineCharStart);
            if (lineSize > 0)
            {
                m_MsgHistory.add(std::make_pair(type, he::String(buff, lineCharStart, lineSize)));
            }

            lineCharStart = i + 1;
        }
    }

    if (lineCharStart < size)
    {
        const size_t lineSize(size - lineCharStart);
        if (lineSize > 0)
        {
            m_MsgHistory.add(std::make_pair(type, he::String(buff, lineCharStart, lineSize)));
        }
    }

    if (m_MsgHistory.size() > m_MaxMessages && m_MaxMessages != 0)
    {
        m_MsgHistory.orderedRemoveAt(0);
    }
}

void Console::registerCmd(const boost::function<void()>& command, const he::String& cmdKey)
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

    addMessage(m_HelpCommand.c_str());
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

void Console::attachToRenderer(gfx::Renderer2D* renderer)
{
    HE_IF_ASSERT(m_Renderer == nullptr, "Console already attached to a renderer")
    {
        m_Renderer = renderer;

        gfx::View* view(renderer->getView());
        m_ResizeHandler = eventCallback0<void>(std::bind(&he::tools::Console::onResize, this));
        view->ViewportSizeChanged += m_ResizeHandler;

        m_ScrollBar->setPosition(vec2(static_cast<float>(view->getViewport().width) - 20.0f, 0.0f));
        m_TextBox->setSize(vec2(static_cast<float>(view->getViewport().width), 20));
        m_ScrollBar->setBarPos(1.0f);

        if (m_IsOpen)
        {
            m_Renderer->attachToRender(this, 1);
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

        gfx::View* view(m_Renderer->getView());
        view->ViewportSizeChanged -= m_ResizeHandler;

        m_Renderer = nullptr;
    }
}

void Console::onResize()
{
    m_ViewportSize.x = static_cast<float>(m_Renderer->getView()->getViewport().width);
    m_ViewportSize.y = static_cast<float>(m_Renderer->getView()->getViewport().height);

    m_TextBox->setSize(vec2(m_ViewportSize.x, 20.f));
    m_ScrollBar->setPosition(vec2(m_ViewportSize.x - 20, 0));

    m_Background->invalidate(vec2(m_ViewportSize.x, 200));

    renderBackground();
}

void Console::renderBackground()
{
    gui::SpriteCreator* const cr(GUI->getSpriteCreator());
    cr->setActiveSprite(m_Background);
    
    cr->rectangle(vec2(0,0), vec2(m_Background->getSize().x - 2, 200));
    cr->setColor(Color(0.2f,0.2f,0.2f,0.9f));
    cr->fill();
    cr->setColor(Color(0.19f,0.19f,0.19f));
    cr->stroke();
}

} } //end namespace
