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
//Created: 01/10/2011

#include "Button.h"
#include "HappyNew.h"
#include <algorithm>
#include "ContentManager.h"
#include "Rect.h"

namespace happyengine {
namespace gui {

/* CONSTRUCTOR - DESTRUCTOR */
Button::Button(TYPE type, const math::Vector2& centerPos, const math::Vector2& size) :	m_Type(type),
																						m_Pos(centerPos),
																						m_Size(size),
																						m_State(STATE_NORMAL),
																						m_ActivationType(ACTIVATION_MOUSE),
																						m_pHitregion(nullptr),
																						m_bClicked(false),
																						m_Text("")
{
	m_pHitregion = NEW Hitregion(Hitregion::TYPE_RECTANGLE, centerPos, size);
}

Button::~Button()
{
	delete m_pHitregion;
}

/* GENERAL */
void Button::tick()
{
	if (m_State != STATE_DISABLED)
	{
		m_bClicked = false;

		if (m_ActivationType == ACTIVATION_MOUSE)
		{
			if (m_pHitregion->hitTest(CONTROLS->getMouse()->getPosition()))
			{
				if (CONTROLS->getMouse()->isButtonDown(io::MouseButton_Left))
					m_State = STATE_DOWN;
				else
					m_State = STATE_HOVER;

				if (CONTROLS->getMouse()->isButtonReleased(io::MouseButton_Left))
					m_bClicked = true;
			}
			else
			{
				m_State = STATE_NORMAL;
			}
		}
	}
}

void Button::draw()
{
	switch (m_Type)
	{
		case TYPE_NORMAL:
		{
			drawColor();

			break;
		}
	}
}

/* SETTERS */
void Button::setSpriteSheet(const graphics::Texture2D::pointer& pSpriteSheet)
{
	m_pSpriteSheet = pSpriteSheet;
}

void Button::setSprites(	const graphics::Texture2D::pointer& pNormalSprite,
							const graphics::Texture2D::pointer& pHoverSprite,
							const graphics::Texture2D::pointer& pDownSprite,
							const graphics::Texture2D::pointer& pDisabledSprite)
{
	m_Sprites.push_back(pNormalSprite);
	m_Sprites.push_back(pHoverSprite);
	m_Sprites.push_back(pDownSprite);
	m_Sprites.push_back(pDisabledSprite);
}

void Button::setState(STATE state)
{
	m_State = state;
}

void Button::setActivationMode(ACTIVATION activationMode)
{
	m_ActivationType = activationMode;
}

void Button::setPosition(const math::Vector2& centerPos)
{
	m_Pos = centerPos;
}

void Button::setText(const std::string& text, ushort fontSize)
{
	m_Text = text;
	m_pFont = CONTENT->loadFont("Ubuntu-Regular.ttf", fontSize);
}

/* GETTERS */
bool Button::isNormal() const
{
	if (m_State == STATE_NORMAL) 
		return true;

	return false;
}

bool Button::isHover() const
{
	if (m_State == STATE_HOVER) 
		return true;

	return false;
}

bool Button::isDown() const
{
	if (m_State == STATE_DOWN) 
		return true;

	return false;
}

bool Button::isDisabled() const
{
	if (m_State == STATE_DISABLED) 
		return true;

	return false;
}

bool Button::isClicked() const
{
	return m_bClicked;
}

/* EXTRA */
void Button::drawColor()
{
	HE2D->resetTransformation();

	switch (m_State)
	{
		case STATE_NORMAL:
		{
			HE2D->setColor(220.0f/255,220.0f/255,220.0f/255);
			HE2D->fillRectangleInstanced(m_Pos - m_Size/2, m_Size);

			break;
		}

		case STATE_HOVER:
		{
			HE2D->setColor(180.0f/255,200.0f/255,220.0f/255);
			HE2D->fillRectangleInstanced(m_Pos - m_Size/2, m_Size);			

			break;
		}

		case STATE_DOWN:
		{
			HE2D->setColor(146.0f/255,162.0f/255,179.0f/255);
			HE2D->fillRectangleInstanced(m_Pos - m_Size/2, m_Size);

			break;
		}

		case STATE_DISABLED:
		{
			HE2D->setColor(150.0f/255,150.0f/255,150.0f/255);
			HE2D->fillRectangleInstanced(m_Pos - m_Size/2, m_Size);

			break;
		}
	}

	HE2D->setStrokeSize();
	HE2D->setColor(140.0f/255,140.0f/255,140.0f/255);
	HE2D->drawRectangleInstanced(m_Pos - m_Size/2, m_Size);

	if (m_pFont != nullptr)
	{
		HE2D->setColor(0.25f,0.25f,0.25f);
		HE2D->setFontHorizontalAlignment(graphics::Font::HAlignment_Center);
		HE2D->setFontVerticalAlignment(graphics::Font::VAlignment_Center);
		HE2D->drawString(m_Text, m_pFont, RectF(m_Pos.x - m_Size.x/2, m_Pos.y - m_Size.y/2, m_Size.x, m_Size.y));
	}
}

void Button::drawSprites()
{
}

void Button::drawSpriteSheet()
{
}

} } //end namespace