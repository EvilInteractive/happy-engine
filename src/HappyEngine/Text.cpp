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

#include "Text.h"
#include "HappyNew.h"
#include <algorithm>

namespace happyengine {
namespace gui {

/* CONSTRUCTOR - DESTRUCTOR */
Text::Text(const graphics::Font::pointer& pFont, uint width, uint height) :	m_pFont(pFont),
																			m_Width(width),
																			m_Height(height)
{
}

Text::~Text()
{
}

void Text::calculateWidthHeight()
{
	m_Width = 0;
	m_Height = 0;

	std::for_each(m_Text.cbegin(), m_Text.cend(), [&](std::string str)
	{
		if (m_pFont->getStringWidth(str) > m_Width)
			m_Width = m_pFont->getStringWidth(str);
	});

	m_Height = (m_Text.size() * m_pFont->getFontPixelHeight()) + 
			((m_Text.size() - 1) * m_pFont->getFontLineSpacing());
}

/* GENERAL */
void Text::addLine(const std::string& string)
{
	m_Text.push_back(string);

	calculateWidthHeight();
}

/* SETTERS */
void Text::setLine(const std::string& string, uint lineNumber)
{
	if (lineNumber < m_Text.size())
	{
		m_Text[lineNumber] = string;

		calculateWidthHeight();
	}
}

void Text::setText(const std::vector<std::string> text)
{
	m_Text = text;

	calculateWidthHeight();
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

uint Text::getWidth() const
{
	return m_Width;
}

uint Text::getHeight() const
{
	return m_Height;
}

} } //end namespace