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

#include "TextBox.h"
#include "HappyNew.h"
#include "SDL.h"
#include <algorithm>
#include "HappyEngine.h"

namespace he {
namespace gui {

/* CONSTRUCTOR - DESTRUCTOR */
TextBox::TextBox(RectF posSize,
				 const std::string& defaultString,
				 ushort fontSize,
				 const std::string& customFont) :	m_Rect(posSize),
													m_DefaultString(defaultString),
													m_bActive(true),
													m_String(""),
													m_bHasFocus(false),
													m_pHitrect(nullptr),
													m_CursorPos(0),
													m_Cursor("|"),
													m_bEntered(false),
													m_bKeyDown(false)
{
	m_pFont = CONTENT->loadFont(customFont, fontSize);

	m_pHitrect = NEW gui::Hitregion(
		gui::Hitregion::TYPE_RECTANGLE,
		vec2(posSize.x + (posSize.width/2.0f), posSize.y + (posSize.height/2.0f)),
		vec2(posSize.width, posSize.height));

	m_Colors["background"] = Color(0.9f,0.9f,0.9f);
	m_Colors["text"] = Color(0.2f,0.2f,0.2f);
	m_Colors["focus"] = Color(0.0f,0.75f,1.0f);
	m_Colors["edge"] = Color(0.1f,0.1f,0.1f);

	m_BlinkTimer.restart();
}

TextBox::~TextBox()
{
	delete m_pHitrect;
}

/* GENERAL */
void TextBox::tick()
{
	if (CONTROLS->getMouse()->isButtonPressed(io::MouseButton_Left))
	{
		if (m_pHitrect->hitTest(CONTROLS->getMouse()->getPosition()))
		{
			m_bHasFocus = true;
			SDL_StartTextInput();
		}
		else
		{
			m_bHasFocus = false;
			SDL_StopTextInput();
		}
	}

	if (m_bHasFocus && m_bActive)
	{
		if (CONTROLS->getKeyboard()->isKeyPressed(io::Key_Left))
		{
			if (m_CursorPos > 0)
			{
				--m_CursorPos;
			}
		}
		else if (CONTROLS->getKeyboard()->isKeyPressed(io::Key_Right))
		{
			if (m_CursorPos < m_String.size())
			{
				++m_CursorPos;
			}
		}
		// check for backspace
		else if (CONTROLS->getKeyboard()->isKeyPressed(io::Key_Backspace))
		{
			// remove char before cursor
			if (m_String.size() > 0)
			{
				m_String.erase(m_CursorPos - 1, 1);

				--m_CursorPos;
			}
		}
		else if (CONTROLS->getKeyboard()->isKeyPressed(io::Key_Delete))
		{
			// remove char after cursor
			if (m_String.size() > 0 && m_CursorPos < m_String.size())
			{
				m_String.erase(m_CursorPos, 1);
			}
		}
		else
		{
			std::for_each(HAPPYENGINE->getSDLEvents().cbegin(), HAPPYENGINE->getSDLEvents().cend(), [&](SDL_Event sdlEvent)
			{
				if (sdlEvent.type == SDL_TEXTINPUT)
				{
					m_String.insert(m_CursorPos, sdlEvent.text.text);

					++m_CursorPos;
				}
			});
		}

		if (CONTROLS->getKeyboard()->isKeyPressed(io::Key_Return) ||
			CONTROLS->getKeyboard()->isKeyPressed(io::Key_Return2))
		{
			m_bEntered = true;
		}
		else
		{
			m_bEntered = false;
		}
	}
}

void TextBox::draw()
{
	HE2D->setStrokeSize();

	HE2D->setFontHorizontalAlignment(gfx::Font::HAlignment_Left);
	HE2D->setFontVerticalAlignment(gfx::Font::VAlignment_Center);

	RectF textRect(m_Rect.x + 4, m_Rect.y + 4, m_Rect.width - 8, m_Rect.height - 8);

	if (m_bActive)
	{
		HE2D->setColor(m_Colors["background"]);
		HE2D->fillRectangleInstanced(vec2(m_Rect.x, m_Rect.y), vec2(m_Rect.width, m_Rect.height));
			
		HE2D->setColor(m_Colors["edge"]);
		HE2D->drawRectangleInstanced(vec2(m_Rect.x, m_Rect.y), vec2(m_Rect.width, m_Rect.height));

		if (m_bHasFocus)
		{
			HE2D->setColor(m_Colors["focus"]);
			HE2D->drawRectangleInstanced(	vec2(m_Rect.x + 1 , m_Rect.y + 1),
											vec2(m_Rect.width - 2, m_Rect.height - 2));
		}

		HE2D->setColor(m_Colors["text"]);

		if (m_String == "")
		{
			if (!m_bHasFocus)
			{
				HE2D->drawString(m_DefaultString, m_pFont, textRect);
			}
		}
		else
		{
			HE2D->drawString(m_String, m_pFont, textRect);
		}

		if (m_bHasFocus)
		{
			if (static_cast<int>(m_BlinkTimer.elapsed() * 100) % 100 < 50)
			{
				std::string cursorText(m_String.substr(0, m_CursorPos));

				RectF cursorRect(textRect);

				if (cursorText != "")
				{
					uint cursorX(m_pFont->getStringWidth(cursorText));
					cursorX -= 1;

					cursorRect.x += cursorX;
				}

				HE2D->drawString(m_Cursor, m_pFont, cursorRect);
			}
		}
	}
	else
	{
		HE2D->setColor(Color(0.3f,0.3f,0.3f));
		HE2D->fillRectangleInstanced(vec2(m_Rect.x, m_Rect.y), vec2(m_Rect.width, m_Rect.height));

		HE2D->setColor(Color(0.1f,0.1f,0.1f));
		HE2D->drawRectangleInstanced(vec2(m_Rect.x, m_Rect.y), vec2(m_Rect.width, m_Rect.height));

		HE2D->setColor(Color(0.5f,0.5f,0.5f));
		
		if (m_String == "")
		{
			if (!m_bHasFocus)
			{
				HE2D->drawString(m_DefaultString, m_pFont, textRect);
			}
		}
		else
		{
			HE2D->drawString(m_String, m_pFont, textRect);
		}
	}
}

void TextBox::resetText()
{
	m_String.clear();
	m_CursorPos = 0;
}

/* GETTERS */
const std::string& TextBox::getString() const
{
	return m_String;
}

bool TextBox::hasFocus() const
{
	return m_bHasFocus;
}

bool TextBox::entered() const
{
	return m_bEntered;
}

/* SETTERS */
void TextBox::setActive(bool isActive)
{
	m_bActive = isActive;
}

void TextBox::setFocus(bool focus)
{
	m_bHasFocus = focus;
}

void TextBox::setColors(	const Color& backgroundColor,
							const Color& textColor,
							const Color& focusColor,
							const Color& edgeColor)
{
	m_Colors["background"] = backgroundColor;
	m_Colors["text"] = textColor;
	m_Colors["focus"] = focusColor;
	m_Colors["edge"] = edgeColor;
}

} } //end namespace