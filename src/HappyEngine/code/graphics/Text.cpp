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
//Created: 09/10/2011
#include "StdAfx.h" 

#include "Text.h"
#include "HappyNew.h"
#include <algorithm>

namespace he {
namespace gui {

/* CONSTRUCTOR - DESTRUCTOR */
Text::Text(uint maxWidth, uint maxHeight) :	m_MaxWidth(maxWidth),
											m_MaxHeight(maxHeight)
{
}

Text::~Text()
{
}

/* GENERAL */
void Text::addLine(const std::string& string)
{
	m_Text.push_back(string);
}

/* SETTERS */
void Text::setLine(const std::string& string, uint lineNumber)
{
	if (lineNumber < m_Text.size())
	{
		m_Text[lineNumber] = string;
	}
}

/* GETTERS */
const std::string& Text::getLine(uint lineNumber) const
{
	return m_Text[lineNumber];
}

const std::vector<std::string>& Text::getText() const
{
	return m_Text;
}

bool Text::isEmpty() const
{
	return m_Text.empty();
}

uint Text::getMaxWidth() const
{
	return m_MaxWidth;
}

uint Text::getMaxHeight() const
{
	return m_MaxHeight;
}

} } //end namespace