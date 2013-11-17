//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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

#ifndef _HE_TEXTBOX_H_
#define _HE_TEXTBOX_H_
#pragma once

#include "Keyboard.h"

namespace he {
namespace gui {
    class Canvas2D;
    class Hitregion;
    class Font;
    class Sprite;
    class Text;

class TextBox
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    TextBox(	RectF posSize,
                const he::String& defaultString = he::String(""),
                uint16 fontSize = 12,
                const he::String& customFont = he::String("Ubuntu-Regular.ttf"));
    virtual ~TextBox();

    /* GENERAL */
    void tick();
    void draw(Canvas2D* canvas);

    void resetText();

    /* GETTERS */
    const he::String& getString() const;
    bool hasFocus() const;
    bool entered() const;

    /* SETTERS */
    void setActive(bool isActive);
    void setFocus(bool focus);
    void setString(const he::String& string);
    void setColors(	const Color& backgroundColor = Color(0.9f,0.9f,0.9f),
                    const Color& textColor = Color(0.2f,0.2f,0.2f),
                    const Color& focusColor = Color(0.0f,0.75f,1.0f),
                    const Color& edgeColor = Color(0.1f,0.1f,0.1f));
    void setPosition(const vec2& pos);
    void setSize(const vec2& size);

private:

    /* INTERNAL */
    void renderSprites();

    /* DATAMEMBERS */
    RectF m_Rect;

    Font* m_Font;
    Text m_Text;

    he::String m_DefaultString;
    he::String m_String;
    he::String m_Cursor;

    bool m_Active;
    bool m_HasFocus;
    bool m_Entered;
    bool m_KeyDown;

    bool m_BackspaceDown;

    Hitregion* m_Hitrect;

    enum TextBoxColor
    {
        TextBoxColor_Background,
        TextBoxColor_Text,
        TextBoxColor_Focus,
        TextBoxColor_Edge,

        TextBoxColor_MAX
    };

    Color m_Colors[TextBoxColor_MAX];

    uint32 m_CursorPos;

    boost::timer m_BlinkTimer;
    uint32 m_BackSpaceTimer;
    uint32 m_BackSpaceDelayTimer;

    he::PrimitiveList<io::Key> m_CharacterKeyCodes;

    Sprite* m_NormalSprite;
    Sprite* m_HoverSprite;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    TextBox(const TextBox&);
    TextBox& operator=(const TextBox&);
};

} } //end namespace

#endif
