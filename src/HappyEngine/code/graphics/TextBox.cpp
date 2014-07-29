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
//Created: 08/10/2011
#include "HappyPCH.h" 

#include "TextBox.h"
#include "ContentManager.h"
#include "ControlsManager.h"
#include "IMouse.h"
#include "Hitregion.h"
#include "Gui.h"

namespace he {
namespace gui {

/* CONSTRUCTOR - DESTRUCTOR */
TextBox::TextBox(RectF posSize,
                 const he::String& defaultString,
                 uint16 fontSize,
                 const he::String& customFont) :	m_Rect(posSize),
                                                    m_DefaultString(defaultString),
                                                    m_Active(true),
                                                    m_String(""),
                                                    m_HasFocus(false),
                                                    m_Hitrect(nullptr),
                                                    m_CursorPos(0),
                                                    m_Cursor("|"),
                                                    m_Entered(false),
                                                    m_KeyDown(false),
                                                    m_BackspaceDown(false),
                                                    m_BackSpaceTimer(0),
                                                    m_BackSpaceDelayTimer(0)
{
    m_Font = CONTENT->loadFont(customFont, fontSize, false);
    if (nullptr != m_Font)
    {
        m_Text.setFont(m_Font);
    }
    m_Text.setHorizontalAlignment(gui::Text::HAlignment_Left);
    m_Text.setVerticalAlignment(gui::Text::VAlignment_Center);
    m_Text.setBounds(vec2(m_Rect.width - 4, m_Rect.height - 4));

    m_Hitrect = NEW Hitregion(
        vec2(posSize.x + (posSize.width/2.0f), posSize.y + (posSize.height/2.0f)),
        vec2(posSize.width, posSize.height));

    m_Colors[TextBoxColor_Background] = Color(0.9f,0.9f,0.9f);
    m_Colors[TextBoxColor_Text] = Color(0.2f,0.2f,0.2f);
    m_Colors[TextBoxColor_Focus] = Color(0.0f,0.75f,1.0f);
    m_Colors[TextBoxColor_Edge] = Color(0.1f,0.1f,0.1f);

    m_BlinkTimer.restart();

    gui::SpriteCreator* const cr(GUI->getSpriteCreator());

    m_NormalSprite = cr->createSprite(
        vec2(posSize.width, posSize.height), gui::Sprite::UNIFORM_SCALE);

    m_HoverSprite = cr->createSprite(
        vec2(posSize.width, posSize.height), gui::Sprite::UNIFORM_SCALE);

    renderSprites();
}

TextBox::~TextBox()
{
    m_Font->release();
    delete m_Hitrect;

    gui::SpriteCreator* const cr(GUI->getSpriteCreator());
    cr->removeSprite(m_NormalSprite);
    cr->removeSprite(m_HoverSprite);
}

/* GENERAL */
void TextBox::tick()
{
    if (CONTROLS->getMouse()->isButtonPressed(io::MouseButton_Left))
    {
        if (m_Hitrect->hitTest(CONTROLS->getMouse()->getPosition()))
        {
            m_HasFocus = true;
            CONTROLS->getFocus(this);
        }
        else
        {
            CONTROLS->returnFocus(this);
            m_HasFocus = false;
        }
    }

    if (m_HasFocus && m_Active)
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
        else if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Backspace))
        {
            if (m_BackspaceDown == false)
            {
                // remove char before cursor
                if (m_String.size() > 0 && m_CursorPos > 0)
                {
                    m_String.erase(m_CursorPos - 1, 1);

                    --m_CursorPos;
                }

                m_BackspaceDown = true;
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
            const he::String& textEntered(CONTROLS->getKeyboard()->getTextEntered());
            if (textEntered.empty() == false)
            {
                m_String.insert(m_CursorPos, textEntered);
                ++m_CursorPos;
            }
        }

        if (CONTROLS->getKeyboard()->isKeyUp(io::Key_Backspace))
        {
            m_BackspaceDown = false;
            m_BackSpaceTimer = (uint32)(m_BlinkTimer.elapsed() * 10);
            m_BackSpaceDelayTimer = 0;
        }
        else if ((uint32)(m_BlinkTimer.elapsed() * 10) - m_BackSpaceTimer > 0 && m_BackSpaceDelayTimer >= 5)
        {
            m_BackSpaceTimer = (uint32)(m_BlinkTimer.elapsed() * 10);
            m_BackspaceDown = false;
        }
        else if (m_BackSpaceDelayTimer < 5)
        {
            m_BackSpaceDelayTimer = (uint32)(m_BlinkTimer.elapsed() * 10) - m_BackSpaceTimer;
        }

        if (CONTROLS->getKeyboard()->isKeyPressed(io::Key_Return))
        {
            m_Entered = true;
        }
        else
        {
            m_Entered = false;
        }
    }
}

void TextBox::draw(gui::Canvas2D* canvas)
{
    RectF textRect(m_Rect.x + 4, m_Rect.y + 2, m_Rect.width - 4, m_Rect.height - 2);

    if (m_Active)
    {
        if (m_HasFocus)
        {
            canvas->drawSprite(m_HoverSprite, vec2(m_Rect.x, m_Rect.y));
        }
        else
        {
            canvas->drawSprite(m_NormalSprite, vec2(m_Rect.x, m_Rect.y));
        }

        canvas->setColor(m_Colors[TextBoxColor_Text]);

        m_Text.clear();

        if (m_String.empty())
        {
            if (!m_HasFocus)
            {
                m_Text.addText(m_DefaultString.c_str());
            }
        }
        else
        {
            m_Text.addText(m_String.c_str());
        }

        canvas->fillText(m_Text, vec2(m_Rect.x + 4, m_Rect.y + 2));

        if (m_HasFocus)
        {
            if (static_cast<int>(m_BlinkTimer.elapsed() * 100) % 100 < 50)
            {
                he::String cursorText(m_String.substr(0, m_CursorPos));

                RectF cursorRect(textRect);

                if (cursorText != "")
                {
                    uint32 cursorX((uint32)m_Font->getStringWidth(cursorText));
                    cursorX -= 1;

                    cursorRect.x += cursorX;
                }

                m_Text.clear();
                m_Text.addText(m_Cursor.c_str());
                canvas->fillText(m_Text, vec2(cursorRect.x, cursorRect.y - 1));
            }
        }
    }
    else
    {
        /*
        canvas->setFillColor(Color(0.3f,0.3f,0.3f));
        canvas->fillRect(vec2(m_Rect.x, m_Rect.y), vec2(m_Rect.width, m_Rect.height));

        canvas->setStrokeColor(Color(0.1f,0.1f,0.1f));
        canvas->strokeRect(vec2(m_Rect.x, m_Rect.y), vec2(m_Rect.width, m_Rect.height));

        canvas->setFillColor(Color(0.5f,0.5f,0.5f));
        
        if (m_String == "")
        {
            if (!m_HasFocus)
            {
                m_Text.setBounds(vec2(m_Rect.width - 8, m_Rect.height - 8));

                m_Text.addText(m_DefaultString.c_str());
                canvas->fillText(m_Text, vec2(m_Rect.x + 4, m_Rect.y + 4));
            }
        }
        else
        {
            m_Text.setBounds(vec2(m_Rect.width - 8, m_Rect.height - 8));

            m_Text.addLine(m_String.c_str());
            canvas->fillText(m_Text, vec2(m_Rect.x + 4, m_Rect.y + 4));
        }*/
    }
}

void TextBox::resetText()
{
    m_String.clear();
    m_CursorPos = 0;
}

/* GETTERS */
const he::String& TextBox::getString() const
{
    return m_String;
}

bool TextBox::hasFocus() const
{
    return m_HasFocus;
}

bool TextBox::entered() const
{
    return m_Entered;
}

/* SETTERS */
void TextBox::setActive(bool isActive)
{
    m_Active = isActive;
}

void TextBox::setFocus(bool focus)
{
    m_HasFocus = focus;
}

void TextBox::setString(const he::String& string)
{
    m_String = string;
    m_CursorPos = (uint32)string.size();
}

void TextBox::setColors(	const Color& backgroundColor,
                            const Color& textColor,
                            const Color& focusColor,
                            const Color& edgeColor)
{
    m_Colors[TextBoxColor_Background] = backgroundColor;
    m_Colors[TextBoxColor_Text] = textColor;
    m_Colors[TextBoxColor_Focus] = focusColor;
    m_Colors[TextBoxColor_Edge] = edgeColor;
}

void TextBox::setSize(const vec2& size)
{
    m_Rect.width = size.x;
    m_Rect.height = size.y;

    m_Text.setBounds(vec2(m_Rect.width - 8, m_Rect.height - 4));

    m_NormalSprite->invalidate(vec2(m_Rect.width, m_Rect.height));
    m_HoverSprite->invalidate(vec2(m_Rect.width, m_Rect.height));

    renderSprites();

    m_Hitrect->setSize(vec2(m_Rect.width, m_Rect.height));
}

void TextBox::setPosition(const vec2& pos)
{
    m_Rect.x = pos.x;
    m_Rect.y = pos.y;

    m_Hitrect->setPosition(pos + (vec2(m_Rect.width, m_Rect.height) / 2.0f));
}

/* INTERNAL */
void TextBox::renderSprites()
{
    gui::SpriteCreator* const cr(GUI->getSpriteCreator());

    cr->setActiveSprite(m_NormalSprite);
    cr->rectangle(vec2(0, 0), vec2(m_Rect.width, m_Rect.height));
    cr->setColor(m_Colors[TextBoxColor_Background]);
    cr->fill();

    cr->setColor(m_Colors[TextBoxColor_Edge]);
    cr->stroke();

    cr->setActiveSprite(m_HoverSprite);
    cr->rectangle(vec2(0, 0), vec2(m_Rect.width, m_Rect.height));
    cr->setColor(m_Colors[TextBoxColor_Background]);
    cr->fill();

    cr->setColor(m_Colors[TextBoxColor_Edge]);
    cr->stroke();

    cr->setColor(m_Colors[TextBoxColor_Focus]);
    cr->stroke();
}

} } //end namespace