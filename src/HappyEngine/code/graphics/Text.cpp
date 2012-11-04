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
//Created: 09/10/2011
#include "HappyPCH.h" 

#include "Text.h"
#include "Font.h"
#include "ContentManager.h"

namespace he {
namespace gui {

/* CONSTRUCTOR - DESTRUCTOR */
Text::Text(	gfx::Font* font,
            OverFlowType overflow) :	m_Font(font),
                                        m_OverFlowType(overflow),
                                        m_HAlignment(HAlignment_Left),
                                        m_VAlignment(VAlignment_Top),
                                        m_Bounds(),
                                        m_HasBounds(false)
{
    m_Font->instantiate();
}
Text::Text(	const std::string& text,
            gfx::Font* font,
            OverFlowType overflow) :	m_Font(font),
                                        m_OverFlowType(overflow),
                                        m_HAlignment(HAlignment_Left),
                                        m_VAlignment(VAlignment_Top),
                                        m_Bounds(),
                                        m_HasBounds(false)
{
    m_Font->instantiate();
    addText(text);
}

Text::Text() :	m_OverFlowType(OverFlowType_Clip),
                m_HAlignment(HAlignment_Left),
                m_VAlignment(VAlignment_Top),
                m_Bounds(),
                m_HasBounds(false),
                m_Font(nullptr)
{

}

Text::~Text()
{
    if (m_Font != nullptr)
        m_Font->release();
}

/* GENERAL */
void Text::addText( const std::string& text )
{
    size_t start(0);
    for (size_t end(0); end < text.size(); ++end)
    {
        if (text[end] == '\n')
        {
            addLine(text.substr(start, end - start));
            start = end + 1;
        }
    }
    addLine(text.substr(start, text.size() - start));
}

void Text::addLine(const std::string& string)
{
    m_Text.add(string);
}

void Text::clear()
{
    m_Text.clear();
}

/* SETTERS */
void Text::setLine(const std::string& string, uint32 lineNumber)
{
    if (lineNumber < m_Text.size())
    {
        m_Text[lineNumber] = string;
    }
}

void Text::setHorizontalAlignment(HAlignment alignment)
{
    m_HAlignment = alignment;
}

void Text::setVerticalAlignment(VAlignment alignment)
{
    m_VAlignment = alignment;
}

void Text::setOverFlowType( OverFlowType overFlowType )
{
    m_OverFlowType = overFlowType;
}

void Text::setBounds(const vec2& bounds)
{
    if (bounds != vec2())
    {
        m_Bounds = bounds;
        m_HasBounds = true;
    }
    else
    {
        m_HasBounds = false;
    }
}

/* GETTERS */
const std::string& Text::getLine(uint32 lineNumber) const
{
    return m_Text[lineNumber];
}

const he::ObjectList<std::string>& Text::getText() const
{
    return m_Text;
}

bool Text::isEmpty() const
{
    return m_Text.empty();
}

bool Text::hasBounds() const
{
    return m_HasBounds;
}

Text::OverFlowType Text::getOverFlowType() const
{
    return m_OverFlowType;
}

Text::HAlignment Text::getHorizontalAlignment() const
{
    return m_HAlignment;
}

Text::VAlignment Text::getVerticalAlignment() const
{
    return m_VAlignment;
}

gfx::Font* Text::getFont() const
{
    return m_Font;
}

const vec2& Text::getBounds() const
{
    return m_Bounds;
}

void Text::setFont( gfx::Font* font )
{
    if (m_Font != nullptr)
        m_Font->release();
    m_Font = font;
    m_Font->instantiate();
}

} } //end namespace