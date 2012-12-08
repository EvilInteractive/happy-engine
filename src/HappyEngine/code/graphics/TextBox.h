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

#include <string>
#include "Rect.h"
#include "Font.h"
#include "Hitregion.h"
#include "Color.h"
#include <map>
#include "Font.h"
#include "boost/timer.hpp"
#include <vector>
#include "Keyboard.h"
#include "Canvas2D.h"

namespace he {
namespace gui {

class TextBox
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    TextBox(	RectF posSize,
                const std::string& defaultString = std::string(""),
                uint16 fontSize = 12,
                const std::string& customFont = std::string("Ubuntu-Regular.ttf"));
    virtual ~TextBox();

    /* GENERAL */
    void tick();
    void draw(gfx::Canvas2D* canvas);

    void resetText();

    /* GETTERS */
    const std::string& getString() const;
    bool hasFocus() const;
    bool entered() const;

    /* SETTERS */
    void setActive(bool isActive);
    void setFocus(bool focus);
    void setString(const std::string& string);
    void setColors(	const Color& backgroundColor = Color(0.9f,0.9f,0.9f),
                    const Color& textColor = Color(0.2f,0.2f,0.2f),
                    const Color& focusColor = Color(0.0f,0.75f,1.0f),
                    const Color& edgeColor = Color(0.1f,0.1f,0.1f));
    void setPosition(const vec2& pos);
    void setSize(const vec2& size);

private:

    /* DATAMEMBERS */
    RectF m_Rect;

    gfx::Font* m_Font;
    gui::Text m_Text;

    std::string m_DefaultString;
    std::string m_String;
    std::string m_Cursor;

    bool m_bActive;
    bool m_bHasFocus;
    bool m_bEntered;
    bool m_bKeyDown;

    bool m_BackspaceDown;

    gui::Hitregion* m_Hitrect;

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

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    TextBox(const TextBox&);
    TextBox& operator=(const TextBox&);
};

} } //end namespace

#endif
