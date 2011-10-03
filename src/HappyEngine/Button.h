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

#ifndef _HE_BUTTON_H_
#define _HE_BUTTON_H_
#pragma once

#include "Vector2.h"
#include "HappyEngine.h"
#include "Texture2D.h"
#include <vector>
#include "Hitregion.h"
#include <string>
#include "Font.h"

namespace happyengine {
namespace gui {

class Button
{
public:

	enum TYPE
	{
		TYPE_NORMAL = 0,
		TYPE_SPRITE = 1,
		TYPE_SPRITESHEET = 2
	};

	enum STATE
	{
		STATE_NORMAL = 0,
		STATE_HOVER = 1,
		STATE_DOWN = 2,
		STATE_DISABLED = 3
	};

	enum ACTIVATION
	{
		ACTIVATION_MOUSE = 0,
		ACTIVATION_MANUAL = 1
	};

	/* CONSTRUCTOR - DESTRUCTOR */
	Button(TYPE type, const math::Vector2& centerPos, const math::Vector2& size);
    virtual ~Button();

	/* GENERAL */
	void tick();
	void draw();

	/* SETTERS */
	void setSpriteSheet(const graphics::Texture2D::pointer& pSpriteSheet);
	void setSprites(	const graphics::Texture2D::pointer& pNormalSprite,
						const graphics::Texture2D::pointer& pHoverSprite,
						const graphics::Texture2D::pointer& pDownSprite,
						const graphics::Texture2D::pointer& pDisabledSprite);
	void setState(STATE state);
	void setActivationMode(ACTIVATION activationMode);
	void setPosition(const math::Vector2& centerPos);
	void setText(const std::string& text, ushort fontSize = 12);

	/* GETTERS */
	bool isNormal() const;
	bool isHover() const;
	bool isDown() const;
	bool isDisabled() const;
	bool isClicked() const;

private:

	/* EXTRA */
	void drawColor();
	void drawSprites();
	void drawSpriteSheet();

	/* DATAMEMBERS */
	math::Vector2 m_Pos;
	math::Vector2 m_Size;

	bool m_bClicked;

	TYPE m_Type;
	STATE m_State;
	ACTIVATION m_ActivationType;

	std::vector<graphics::Texture2D::pointer> m_Sprites;
	graphics::Texture2D::pointer m_pSpriteSheet;

	Hitregion* m_pHitregion;

	std::string m_Text;
	graphics::Font::pointer m_pFont;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    Button(const Button&);
    Button& operator=(const Button&);
};

} } //end namespace

#endif
