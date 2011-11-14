//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
#include "StdAfx.h" 

#include "Console.h"
#include "HappyNew.h"

#include "Exception.h"
#include <iostream>
#include <sstream>
#include "HappyEngine.h"
#include <algorithm>
#include "Exception.h"
#include "GraphicsEngine.h"
#include "ContentManager.h"
#include "ControlsManager.h"
#include "Happy2DRenderer.h"

#include "BoolTypeHandler.h"
#include "FloatTypeHandler.h"
#include "IntTypeHandler.h"
#include "UIntTypeHandler.h"
#include "Vec2TypeHandler.h"
#include "Vec3TypeHandler.h"
#include "Vec4TypeHandler.h"
#include "StringTypeHandler.h"

namespace he {
namespace tools {

/* CONSTRUCTOR - DESTRUCTOR */
Console::Console() :	m_Shortcut(io::Key_C),
						m_MaxMessages(0),
						m_bOpen(false),
						m_pTextBox(nullptr),
						m_MsgHistory(0),
						m_CmdHistoryPos(0),
						m_pScrollBar(nullptr),
						m_Help(nullptr)
{
	m_MsgColors[CMSG_TYPE_INFO] = Color(1.0f,1.0f,1.0f);
	m_MsgColors[CMSG_TYPE_WARNING] = Color(1.0f,0.8f,0.2f);
	m_MsgColors[CMSG_TYPE_ERROR] = Color(1.0f,0.3f,0.3f);
	m_MsgColors[CMSG_TYPE_ENGINE] = Color(0.2f,1.0f,0.2f);
	m_MsgColors[CMSG_TYPE_COMMAND] = Color(0.2f,0.6f,1.0f);

	m_HelpCommand = "type 'help' to see available commands...";

	m_ParseTypes[typeid(float).name()] = 'f';
	m_ParseTypes[typeid(std::string).name()] = 's';
	m_ParseTypes[typeid(int).name()] = 'd';
	m_ParseTypes[typeid(uint).name()] = 'u';
	m_ParseTypes[typeid(char).name()] = 'c';

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

	registerValue<bool>(&m_ShowMessageTypes[CMSG_TYPE_ENGINE], "c_show_msg_engine");
}
void Console::load()
{	
    m_pFont = CONTENT->loadFont("Ubuntu-Medium.ttf", 10);

	m_Help = new gui::Text(m_pFont);
	m_Help->addLine("******** HELP ********");
	m_Help->addLine("'listvars' (displays registered variables and their type)");
	m_Help->addLine("******** HELP ********");

    m_pTextBox = NEW gui::TextBox(
        RectF(0,200,static_cast<float>(GRAPHICS->getScreenWidth()), 20),
        "Enter command...", 10, "Ubuntu-Medium.ttf");

    m_pTextBox->setColors(	Color(0.3f,0.3f,0.3f,0.9f),
        Color(1.0f,1.0f,1.0f),
        Color(0.4f,0.4f,0.4f),
        Color(0.19f,0.19f,0.19f));

    m_CmdHistory.push_back("");

    m_pScrollBar = NEW gui::Scrollbar(
        vec2(static_cast<float>(GRAPHICS->getScreenWidth()) - 20.0f, 0.0f), vec2(20,200), 50.0f);

    m_pScrollBar->setBarPos(1.0f);

    m_MaxMessagesInWindow = static_cast<uint>(190 / m_pFont->getFontLineSpacing());
}

Console::~Console()
{
	std::for_each(m_TypeHandlers.begin(), m_TypeHandlers.end(), [&](std::pair<std::string, ITypeHandler*> p)
	{
		delete p.second;
		p.second = nullptr;
	});

	m_TypeHandlers.clear();

	delete m_pTextBox;
	delete m_pScrollBar;
	delete m_Help;
}

void Console::processCommand(const std::string& command)
{
	std::string s(command);

	// remove spaces
	s.erase(std::remove_if(s.begin(), s.end(), std::isspace), s.end());

	// console commands
	if (s == "help")
	{
		addMessage(m_pTextBox->getString(), CMSG_TYPE_COMMAND);

		displayHelp();
	}
	else if (s == "listvars")
	{
		addMessage(m_pTextBox->getString(), CMSG_TYPE_COMMAND);

		displayVars();
	}
	else if (s.find('=') != -1)
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
					addMessage(m_pTextBox->getString(), CMSG_TYPE_COMMAND);
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
	else
	{
		addMessage("the command '" + s + "' was not found!", CMSG_TYPE_ERROR);
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
		txt.addLine("!no registered values!");
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

/* GENERAL */
void Console::tick()
{
	if (CONTROLS->getKeyboard()->isKeyPressed(m_Shortcut) && !m_pTextBox->hasFocus())
	{
		m_bOpen =! m_bOpen;
		m_pTextBox->resetText();

		if (m_MsgHistory[m_MsgHistory.size() - 1].second != m_HelpCommand)
		{
			addMessage(m_HelpCommand, CMSG_TYPE_INFO);
		}
	}

	if (m_bOpen)
	{
		m_pTextBox->tick();

		if (m_MsgHistory.size() > m_MaxMessagesInWindow)
			m_pScrollBar->tick();

		if (CONTROLS->getKeyboard()->isKeyPressed(io::Key_Down))
		{
			m_pTextBox->setString(m_CmdHistory[m_CmdHistoryPos]);

			if (m_CmdHistoryPos > 0)
			{
				--m_CmdHistoryPos;
			}
			else
			{
				m_CmdHistoryPos = m_CmdHistory.size() - 1;
			}
		}
		else if (CONTROLS->getKeyboard()->isKeyPressed(io::Key_Up))
		{
			m_pTextBox->setString(m_CmdHistory[m_CmdHistoryPos]);

			if (m_CmdHistoryPos < m_CmdHistory.size() - 1)
			{
				++m_CmdHistoryPos;
			}
			else
			{
				m_CmdHistoryPos =0;
			}
		}

		if (m_pTextBox->entered())
		{
			processCommand(m_pTextBox->getString());;

			if (m_CmdHistory[m_CmdHistory.size() - 1] != m_pTextBox->getString())
			{
				m_CmdHistory.push_back(m_pTextBox->getString());
				m_CmdHistoryPos = m_CmdHistory.size() - 1;
			}

			m_pTextBox->resetText();

			m_pScrollBar->setBarPos(1.0f);
		}
	}
}

void Console::draw()
{
	if (m_bOpen)
	{
		GUI->setAntiAliasing(false);

		GUI->setColor(0.3f,0.3f,0.3f,0.9f);
		GUI->fillShape2D(gui::Rectangle2D(vec2(0,0),
										vec2(static_cast<float>(GRAPHICS->getScreenWidth()), 200)));

		GUI->setColor(0.19f,0.19f,0.19f);
		GUI->drawShape2D(gui::Rectangle2D(vec2(0,0),
										vec2(static_cast<float>(GRAPHICS->getScreenWidth()), 200)));

		m_pTextBox->draw();

		std::vector<std::pair<CMSG_TYPE, std::string> > msgHistory;

		std::for_each(m_MsgHistory.cbegin(), m_MsgHistory.cend(), [&] (std::pair<CMSG_TYPE, std::string> p)
		{
			if (m_ShowMessageTypes[p.first] == true)
				msgHistory.push_back(p);
		});

		uint startPos(0);

		if (msgHistory.size() > m_MaxMessagesInWindow)
		{
			startPos = static_cast<uint>((msgHistory.size() - 1 - m_MaxMessagesInWindow) * m_pScrollBar->getBarPos());
		}

		uint i(startPos);
		
		std::vector<std::pair<CMSG_TYPE, std::string> > msg;

		if (msgHistory.size() > m_MaxMessagesInWindow)
		{
			for (; i <= (startPos + m_MaxMessagesInWindow); ++i)
			{
				msg.push_back(msgHistory[i]);
			}
		}
		else
		{
			for (; i < msgHistory.size(); ++i)
			{
				msg.push_back(msgHistory[i]);
			}
		}

		gui::Text text(m_pFont);
		//text.setHorizontalAlignment(gui::Text::HAlignment_Left);
		//text.setVerticalAlignment(gui::Text::VAlignment_Top);

		uint k(0);
		std::for_each(msg.crbegin(), msg.crend(), [&](std::pair<CMSG_TYPE, std::string> p)
		{
			GUI->setColor(m_MsgColors[p.first]);

			text.clear();
			text.addLine(p.second);

			GUI->drawText(	text, RectF(5,5,
							static_cast<float>(GRAPHICS->getScreenWidth() - 10),
							190.0f - (k * m_pFont->getFontLineSpacing())));

			++k;
		});

		if (msgHistory.size() > m_MaxMessagesInWindow)
			m_pScrollBar->draw();
	}
}

void Console::addMessage(const gui::Text& msg, CMSG_TYPE type)
{
	std::for_each(msg.getText().cbegin(), msg.getText().cend(), [&](std::string str)
	{
		if (type == CMSG_TYPE_COMMAND)
			m_MsgHistory.push_back(std::pair<CMSG_TYPE, std::string>(type, "] " + str));
		else
			m_MsgHistory.push_back(std::pair<CMSG_TYPE, std::string>(type, str));
	});

	if (m_MsgHistory.size() > m_MaxMessages && m_MaxMessages != 0)
	{
		m_MsgHistory.erase(m_MsgHistory.begin());
	}
}

void Console::addMessage(const std::string& msg, CMSG_TYPE type)
{
	if (type == CMSG_TYPE_COMMAND)
		m_MsgHistory.push_back(std::pair<CMSG_TYPE, std::string>(type, "] " + msg));
	else
		m_MsgHistory.push_back(std::pair<CMSG_TYPE, std::string>(type, msg));

	if (m_MsgHistory.size() > m_MaxMessages && m_MaxMessages != 0)
	{
		m_MsgHistory.erase(m_MsgHistory.begin());
	}
}

void Console::addTypeHandler(ITypeHandler* typeHandler)
{
	if (m_TypeHandlers.find(typeHandler->getType()) == m_TypeHandlers.cend())
	{
		m_TypeHandlers[typeHandler->getType()] = typeHandler;
	}
	else
	{
		std::string s(typeHandler->getType());

		ASSERT(false, "Type handler for '" + s + "' already added!");
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

void Console::setMaxMsgHistory(uint maxMessages)
{
	m_MaxMessages = maxMessages;
}

void Console::toggleShowMessages(CMSG_TYPE type, bool show)
{
	m_ShowMessageTypes[type] = show;
}

} } //end namespace