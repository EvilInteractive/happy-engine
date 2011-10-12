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
						m_MaxMsg(0),
						m_bOpen(false),
						m_pTextBox(nullptr)
{
	m_MsgColors[MSG_TYPE_INFO] = Color(1.0f,1.0f,1.0f);
	m_MsgColors[MSG_TYPE_WARNING] = Color(1.0f,1.0f,0.2f);
	m_MsgColors[MSG_TYPE_ERROR] = Color(1.0f,0.3f,0.3f);
	m_MsgColors[MSG_TYPE_ENGINE] = Color(0.2f,1.0f,0.2f);
	m_MsgColors[MSG_TYPE_COMMAND] = Color(0.2f,0.6f,1.0f);

	m_pFont = CONTENT->loadFont("Ubuntu-Medium.ttf", 10);

	m_pTextBox = NEW gui::TextBox(
		RectF(0,200,static_cast<float>(GRAPHICS->getScreenWidth()), 20),
		"Enter command...", 10, "Ubuntu-Medium.ttf");

	m_pTextBox->setColors(	Color(0.3f,0.3f,0.3f,0.9f),
							Color(1.0f,1.0f,1.0f),
							Color(0.4f,0.4f,0.4f),
							Color(0.19f,0.19f,0.19f));

	m_Help = gui::Text::pointer(NEW gui::Text(m_pFont));
	m_Help->addLine("******** HELP ********");
	m_Help->addLine("'listvars' (displays registered variables and their type)");
	m_Help->addLine("******** HELP ********");

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

	addMessage(m_HelpCommand, MSG_TYPE_INFO);
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
}

void Console::processCommand(const std::string& command)
{
	std::string s(command);

	// remove spaces
	s.erase(std::remove_if(s.begin(), s.end(), std::isspace), s.end());

	// console commands
	if (s == "help")
	{
		addMessage(m_pTextBox->getString(), MSG_TYPE_COMMAND);

		displayHelp();
	}
	else if (s == "listvars")
	{
		addMessage(m_pTextBox->getString(), MSG_TYPE_COMMAND);

		displayVars();
	}
	else if (s.find('=') != -1)
	{
		// get keyword (variable to change)
		std::string keyWord(s.substr(0, s.find('=')));

		if (m_ValueContainer.find(keyWord) != m_ValueContainer.end())
		{
			size_t firstQ(s.find("\"", s.find('=')));
			size_t secondQ(s.find("\"", firstQ + 1));

			// check for correct usage of " "
			if (firstQ != -1 && secondQ != -1 && secondQ != firstQ + 1)
			{
				std::string values(s.substr(firstQ + 1, (secondQ - firstQ) - 1));

				std::string type(m_ValueContainer[keyWord].type().name());
				type = type.substr(0, type.size() - 2);

				if (m_TypeHandlers.find(type) != m_TypeHandlers.cend())
				{
					
					if (m_TypeHandlers[type]->parse(values, m_ValueContainer[keyWord]))
					{
						addMessage(m_pTextBox->getString(), MSG_TYPE_COMMAND);
					}
					else
					{
						addMessage("\"" + values + "\" can't be assigned to '" + keyWord + "'!", MSG_TYPE_ERROR);
					}
				}
				else
				{
					addMessage("no type handler specified for this keyword '" + keyWord + "' - type: " + type + "!", MSG_TYPE_ERROR);
				}
			}
			else
			{
				addMessage("please use '" + keyWord + " = \"...\"' to assign a value(s)!", MSG_TYPE_ERROR);
			}
		}
		else
		{
			addMessage("the keyword '" + keyWord + "' was not found!", MSG_TYPE_ERROR); 
		}
	}
	else
	{
		addMessage("the command '" + s + "' was not found!", MSG_TYPE_ERROR);
	}
}

void Console::displayHelp()
{
	addMessage(*(m_Help.get()), MSG_TYPE_INFO);
}

void Console::displayVars()
{
	gui::Text txt(m_pFont);

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

	addMessage(txt, MSG_TYPE_INFO);
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
			addMessage(m_HelpCommand, MSG_TYPE_INFO);
		}
	}

	if (m_bOpen)
	{
		m_pTextBox->tick();

		if (m_pTextBox->entered())
		{
			processCommand(m_pTextBox->getString());;
			m_pTextBox->resetText();
		}
	}
}

void Console::draw()
{
	if (m_bOpen)
	{
		HE2D->setColor(0.3f,0.3f,0.3f,0.9f);
		HE2D->fillRectangleInstanced(	vec2(0,0),
										vec2(static_cast<float>(GRAPHICS->getScreenWidth()), 200));

		HE2D->setStrokeSize();
		HE2D->setColor(0.19f,0.19f,0.19f);
		HE2D->drawRectangleInstanced(	vec2(0,0),
										vec2(static_cast<float>(GRAPHICS->getScreenWidth()), 200));

		HE2D->setAntiAliasing(false);

		m_pTextBox->draw();

		HE2D->setFontHorizontalAlignment(gfx::Font::HAlignment_Left);
		HE2D->setFontVerticalAlignment(gfx::Font::VAlignment_Bottom);

		uint i(0);
		std::for_each(m_MsgHistory.crbegin(), m_MsgHistory.crend(), [&](std::pair<MSG_TYPE, std::string> p)
		{
			HE2D->setColor(m_MsgColors[p.first]);
			HE2D->drawString(p.second, m_pFont, RectF(5,5,
							static_cast<float>(GRAPHICS->getScreenWidth() - 10),
							190.0f - (i * m_pFont->getFontLineSpacing())));

			++i;
		});
	}
}

void Console::addMessage(const gui::Text& msg, MSG_TYPE type)
{
	std::for_each(msg.getText().cbegin(), msg.getText().cend(), [&](std::string str)
	{
		if (type == MSG_TYPE_COMMAND)
			m_MsgHistory.push_back(std::pair<MSG_TYPE, std::string>(type, "] " + str));
		else
			m_MsgHistory.push_back(std::pair<MSG_TYPE, std::string>(type, str));
	});
}

void Console::addMessage(const std::string& msg, MSG_TYPE type)
{
	if (type == MSG_TYPE_COMMAND)
			m_MsgHistory.push_back(std::pair<MSG_TYPE, std::string>(type, "] " + msg));
		else
			m_MsgHistory.push_back(std::pair<MSG_TYPE, std::string>(type, msg));
}

void Console::addTypeHandler(ITypeHandler* typeHandler)
{
	if (m_TypeHandlers.find(typeHandler->getType()) == m_TypeHandlers.cend())
	{
		bool success(true);

		std::for_each(typeHandler->getInputTypes().cbegin(), typeHandler->getInputTypes().cend(), [&](std::string typeName)
		{
			if (m_ParseTypes.find(typeName) == m_ParseTypes.cend())
			{
				success = false;
			}
		});

		if (success)
		{
			m_TypeHandlers[typeHandler->getType()] = typeHandler;
		}
		else
		{
			throw(err::Exception(L"Input types of type handler are not basic types!"));
		}
	}
	else
	{
		std::string s(typeHandler->getType());

		std::wstring t;
		t.append(s.begin(), s.end());

		std::wstring str;
		str = L"Type handler for '" + t + L"' already added!";

		throw(err::Exception(str));
	}
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
	m_MsgColors[MSG_TYPE_INFO] = infoColor;
	m_MsgColors[MSG_TYPE_WARNING] = warningColor;
	m_MsgColors[MSG_TYPE_ERROR] = errorColor;
	m_MsgColors[MSG_TYPE_ENGINE] = engineColor;
	m_MsgColors[MSG_TYPE_COMMAND] = commandColor;
}

void Console::setMaxMsgHistory(uint maxMessages)
{
	m_MaxMsg = maxMessages;
}

} } //end namespace