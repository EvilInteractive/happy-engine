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

#ifndef _HE_CONSOLE_H_
#define _HE_CONSOLE_H_
#pragma once

#include "boost\any.hpp"
#include <map>
#include <vector>
#include <utility>
#include <string>
#include "Keyboard.h"
#include "Color.h"
#include "Font.h"
#include "TextBox.h"
#include "Text.h"

namespace happyengine {
namespace tools {

class Console
{
public:

	enum MSG_TYPE
	{
		MSG_TYPE_INFO = 0,
		MSG_TYPE_WARNING = 1,
		MSG_TYPE_ERROR = 2,
		MSG_TYPE_ENGINE = 3,
		MSG_TYPE_COMMAND = 4
	};

	/* CONSTRUCTOR - DESTRUCTOR */
	Console();
    virtual ~Console();

	/* GENERAL */
	void tick();
	void draw();
	void addMessage(const gui::Text& msg, MSG_TYPE type = MSG_TYPE_INFO);
	void addMessage(const std::string& msg, MSG_TYPE type = MSG_TYPE_INFO);

	template <typename T>
	void registerValue(T* pValue, const std::string& valueKey)
	{
		if (m_ValueContainer.find(valueKey) != m_ValueContainer.end())
		{
			std::string str;
			str = "Value: '" + valueKey + "' already registered!";

			std::wstring wstr;
			wstr.assign(str.cbegin(), str.cend());

			throw (error::Exception(wstr));
		}
		else
		{
			m_ValueContainer[valueKey] = pValue;
		}
	}

	/* SETTERS */
	void setKeyboardShortcut(SDL_Scancode key);
	void setMessageColors(	const Color& infoColor = Color(1.0f,1.0f,1.0f),
							const Color& warningColor = Color(1.0f,1.0f,0.0f),
							const Color& errorColor = Color(1.0f,0.0f,0.0f),
							const Color& engineColor = Color(0.0f,1.0f,0.0f),
							const Color& commandColor = Color(0.0f,0.5f,1.0f));
	// * Sets the max messages to keep in console history, 0 == no limit. *
	void setMaxMsgHistory(uint maxMessages = 0);

private:

	void processCommand(const std::string& command);
	void displayHelp();
	void displayVars();

	/* DATAMEMBERS */
	std::map<std::string, boost::any> m_ValueContainer;
	std::map<MSG_TYPE, Color> m_MsgColors;
	std::vector<std::pair<MSG_TYPE, std::string> > m_MsgHistory;
	gui::Text::pointer m_Help;

	SDL_Scancode m_Shortcut;

	uint m_MaxMsg;

	bool m_bOpen;

	gui::TextBox* m_pTextBox;

	graphics::Font::pointer m_pFont;

    /* DEFAULT COPY & ASSIGNMENT */
    Console(const Console&);
    Console& operator=(const Console&);
};

} } //end namespace

#endif
