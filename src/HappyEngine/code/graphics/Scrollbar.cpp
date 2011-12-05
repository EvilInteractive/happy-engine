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
//Created: 12/10/2011

#include "StdAfx.h"

#include "Scrollbar.h"
#include "HappyNew.h"
#include "HappyEngine.h"
#include "ControlsManager.h"
#include "Happy2DRenderer.h"

namespace he {
namespace gui {

/* CONSTRUCTOR - DESTRUCTOR */
Scrollbar::Scrollbar(const vec2& pos, const vec2& size, float heightScrollbar) :	m_pHitregion(nullptr),
																					m_Pos(pos),
																					m_Size(size),
																					m_BarPos(0.0f),
																					m_bDragging(false),
																					m_PreviousMousePos(0,0),
																					m_ScrollbarHeight(heightScrollbar)
{
	m_pHitregion = NEW Hitregion(Hitregion::TYPE_RECTANGLE,
		vec2(pos.x + size.x/2, pos.y + (heightScrollbar/2)),
		vec2(size.x - 2, heightScrollbar));

	m_Colors["background"] = Color(0.6f,0.6f,0.6f);
	m_Colors["scrollbar"] = Color(0.5f,0.5f,0.5f);
	m_Colors["edge"] = Color(0.1f,0.1f,0.1f);
}

Scrollbar::~Scrollbar()
{
	delete m_pHitregion;
	m_pHitregion = nullptr;
}

/* GENERAL */
void Scrollbar::tick()
{
	if (CONTROLS->getMouse()->isButtonDown(io::MouseButton_Left))
	{
		if (m_pHitregion->hitTest(CONTROLS->getMouse()->getPosition()))
		{
			m_bDragging = true;
		}
	}
	else
	{
		m_bDragging = false;

		m_PreviousMousePos = CONTROLS->getMouse()->getPosition();
	}

	if (m_bDragging)
	{
		vec2 mouseMovement(0.0f,0.0f);

		if (CONTROLS->getMouse()->getPosition().y >= m_Pos.y && CONTROLS->getMouse()->getPosition().y <= m_Pos.y + m_Size.y)
			mouseMovement = CONTROLS->getMouse()->getPosition() - m_PreviousMousePos;

		m_PreviousMousePos = CONTROLS->getMouse()->getPosition();

		m_pHitregion->move(vec2(0, mouseMovement.y));

		if (m_pHitregion->getPosition().y - (m_ScrollbarHeight/2) < m_Pos.y)
			m_pHitregion->setPosition(vec2(m_pHitregion->getPosition().x, m_Pos.y + (m_ScrollbarHeight/2)));
		else if (m_pHitregion->getPosition().y + (m_ScrollbarHeight/2) > m_Pos.y + m_Size.y)
			m_pHitregion->setPosition(vec2(m_pHitregion->getPosition().x, m_Pos.y + m_Size.y - (m_ScrollbarHeight/2)));

		m_BarPos = (m_pHitregion->getPosition().y - m_Pos.y - (m_ScrollbarHeight/2)) / (m_Size.y - m_ScrollbarHeight);
	}

	if (m_BarPos < 0)
		m_BarPos = 0;
	else if (m_BarPos > 1.0f)
		m_BarPos = 1.0f;
}

void Scrollbar::draw()
{
	GUI->setAntiAliasing(false);

	GUI->setColor(m_Colors["background"]);
	GUI->fillShape2D(gui::Rectangle2D(vec2(m_Pos.x, m_Pos.y), vec2(m_Size.x, m_Size.y)), true);
	GUI->setColor(m_Colors["edge"]);
	GUI->drawShape2D(gui::Rectangle2D(vec2(m_Pos.x, m_Pos.y), vec2(m_Size.x, m_Size.y)), true);

	GUI->setColor(m_Colors["scrollbar"]);
	GUI->fillShape2D(gui::Rectangle2D(vec2(m_Pos.x + 1, m_pHitregion->getPosition().y - (m_ScrollbarHeight/2)),
									  vec2(m_Size.x - 2, m_ScrollbarHeight)));

	GUI->setColor(m_Colors["edge"]);
	GUI->drawShape2D(gui::Rectangle2D(vec2(m_Pos.x + 1, m_pHitregion->getPosition().y - (m_ScrollbarHeight/2)),
									  vec2(m_Size.x - 2, m_ScrollbarHeight)));
}

/* SETTERS */
void Scrollbar::setBarPos(float barPos)
{
	m_BarPos = barPos;

	float y((m_BarPos * (m_Size.y - m_ScrollbarHeight)) + (m_ScrollbarHeight/2) + m_Pos.y);

	m_pHitregion->setPosition(vec2(m_pHitregion->getPosition().x, y));
}

void Scrollbar::setColors(	const Color& backgroundColor,
							const Color& scrollbarColor,
							const Color& edgeColor)
{
	m_Colors["background"] = backgroundColor;
	m_Colors["scrollbar"] = scrollbarColor;
	m_Colors["edge"] = edgeColor;
}

/* GETTERS */
float Scrollbar::getBarPos() const
{
	return m_BarPos;
}

} } //end namespace
