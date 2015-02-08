//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

#include "HappyPCH.h"

#include "Scrollbar.h"
#include "ControlsManager.h"
#include "IMouse.h"
#include "Gui.h"
#include "GraphicsEngine.h"

namespace he {
namespace gui {

/* CONSTRUCTOR - DESTRUCTOR */
Scrollbar::Scrollbar(const vec2& pos, const vec2& size, float heightScrollbar) :	m_Hitregion(nullptr),
                                                                                    m_Pos(pos),
                                                                                    m_Size(size),
                                                                                    m_BarPos(0.0f),
                                                                                    m_Dragging(false),
                                                                                    m_PreviousMousePos(0,0),
                                                                                    m_ScrollbarHeight(heightScrollbar)
{
    m_Hitregion = HENew(Hitregion)(
        vec2(pos.x + size.x/2, pos.y + (heightScrollbar/2)),
        vec2(size.x - 2, heightScrollbar));

    m_Colors["background"] = Color(0.6f,0.6f,0.6f);
    m_Colors["scrollbar"] = Color(0.4f,0.4f,0.4f);
    m_Colors["edge"] = Color(0.2f,0.2f,0.2f);

    SpriteCreator* const cr(GUI->getSpriteCreator());
    m_Sprites[0] = cr->createSprite(m_Size, Sprite::UNIFORM_SCALE);
    m_Sprites[1] = cr->createSprite(vec2(m_Size.x, heightScrollbar), Sprite::UNIFORM_SCALE);

    renderSprites();
}

Scrollbar::~Scrollbar()
{
    HEDelete(m_Hitregion);
    m_Hitregion = nullptr;
}

/* GENERAL */
void Scrollbar::tick()
{
    io::IMouse* mouse(CONTROLS->getMouse(GRAPHICS->getActiveWindow()));
    const vec2 mousePos(mouse->getPosition());
    const vec2 hitrectPos(m_Hitregion->getPosition());

    if (mouse->isButtonDown(io::MouseButton_Left))
    {
        if (m_Hitregion->hitTest(mousePos))
        {
            m_Dragging = true;
        }
    }
    else
    {
        m_Dragging = false;

        m_PreviousMousePos = mousePos;
    }

    if (m_Dragging)
    {
        vec2 mouseMovement(0.0f,0.0f);

        if (mousePos.y >= m_Pos.y && mousePos.y <= m_Pos.y + m_Size.y)
            mouseMovement = mousePos - m_PreviousMousePos;

        m_PreviousMousePos = mousePos;

        m_Hitregion->move(vec2(0, mouseMovement.y));

        if (hitrectPos.y - (m_ScrollbarHeight/2) < m_Pos.y)
            m_Hitregion->setPosition(vec2(hitrectPos.x, m_Pos.y + (m_ScrollbarHeight/2)));
        else if (hitrectPos.y + (m_ScrollbarHeight/2) > m_Pos.y + m_Size.y)
            m_Hitregion->setPosition(vec2(hitrectPos.x, m_Pos.y + m_Size.y - (m_ScrollbarHeight/2)));

        m_BarPos = (hitrectPos.y - m_Pos.y - (m_ScrollbarHeight/2)) / (m_Size.y - m_ScrollbarHeight);
    }

    if (m_BarPos < 0)
        m_BarPos = 0;
    else if (m_BarPos > 1.0f)
        m_BarPos = 1.0f;
}

void Scrollbar::draw(gui::Canvas2D* canvas)
{
   canvas->drawSprite(m_Sprites[0], m_Pos);
   canvas->drawSprite(m_Sprites[1], m_Hitregion->getPosition() - vec2(m_Size.x / 2.0f, m_Hitregion->getSize().y / 2.0f));
}

/* SETTERS */
void Scrollbar::setBarPos(float barPos)
{
    m_BarPos = barPos;

    float y((m_BarPos * (m_Size.y - m_ScrollbarHeight)) + (m_ScrollbarHeight/2) + m_Pos.y);

    m_Hitregion->setPosition(vec2(m_Hitregion->getPosition().x, y));
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

void Scrollbar::setPosition( const vec2& pos )
{
    m_Pos = pos;
    m_Hitregion->setPosition(vec2(m_Pos.x + m_Size.x / 2, m_Pos.y + (m_ScrollbarHeight / 2)));
    setBarPos(m_BarPos);
}

void Scrollbar::setSize( const vec2& size )
{
    m_Size = size;
    m_Hitregion->setSize(vec2(size.x - 2, m_ScrollbarHeight));
}

/* INTERNAL */
void Scrollbar::renderSprites()
{
    Sprite* const back(m_Sprites[0]);
    Sprite* const front(m_Sprites[1]);
    SpriteCreator* const cr(GUI->getSpriteCreator());

    const vec2 hitrectPos(m_Hitregion->getPosition());

    cr->setActiveSprite(back);
    cr->setColor(m_Colors["background"]);
    cr->rectangle(vec2(0, 0), vec2(m_Size.x, m_Size.y));
    cr->fill();
    cr->setColor(m_Colors["edge"]);
    cr->stroke();
    cr->renderSpriteAsync();

    cr->setActiveSprite(front);
    cr->setColor(m_Colors["scrollbar"]);
    cr->rectangle(vec2(1, hitrectPos.y - (m_ScrollbarHeight/2)),
                  vec2(m_Size.x - 2, m_ScrollbarHeight));
    cr->fill();
    cr->setColor(m_Colors["edge"]);
    cr->stroke();
    cr->renderSpriteAsync();
}

} } //end namespace
