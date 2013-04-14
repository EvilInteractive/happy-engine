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

#ifndef _HE_TEXT_H_
#define _HE_TEXT_H_
#pragma once

namespace he {
namespace gui {
    class Font;

class HAPPY_ENTRY Text
{
public:

    enum HAlignment
    {
        HAlignment_Left, 
        HAlignment_Center, 
        HAlignment_Right
    };
    enum VAlignment
    {
        VAlignment_Top, 
        VAlignment_Center, 
        VAlignment_Bottom
    };

    enum OverFlowType
    {
        OverFlowType_Clip,
        OverFlowType_Wrap
    };

    /* CONSTRUCTOR - DESTRUCTOR */
    Text();
    virtual ~Text();

    /* GENERAL */
    void addTextExt(const char* text, ...);
    void addTextExt(const char* text, va_list& argList);
    void addText(const char* text, int len = -1);
    void addLine(const char* text, int len = -1);
    void clear();

    /* SETTERS */
    void setHorizontalAlignment(HAlignment alignment) { m_HAlignment = alignment; }
    void setVerticalAlignment(VAlignment alignment) { m_VAlignment = alignment; }
    void setBounds(const vec2& bounds = vec2());
    void setFont(Font* font);
    void setOverFlowType(OverFlowType overFlowType) { m_OverFlowType = overFlowType; }

    /* GETTERS */
    const char* getText() const { return m_Text; }
    const size_t getTextSize() const { return m_Size; }
    vec2 measureText();

    bool isEmpty() const { return m_Size == 0; }
    bool hasBounds() const { return m_HasBounds; }

    OverFlowType getOverFlowType() const { return m_OverFlowType; }

    HAlignment getHorizontalAlignment() const { return m_HAlignment; }
    VAlignment getVerticalAlignment() const { return m_VAlignment; }

    Font* getFont() const { return m_Font; }

    const vec2& getBounds() const { return m_Bounds; }

    /* DEFAULT COPY & ASSIGNMENT */
    Text(const Text& text);
    Text& operator=(const Text& text);

private:

    /* DATAMEMBERS */
    char* m_Text;
    size_t m_Size;
    size_t m_Capacity;
    void resize(const size_t newSize);

    static const int MAX_SCRATCH = 4096;
    char m_ScratchBuffer[MAX_SCRATCH];

    OverFlowType m_OverFlowType;

    HAlignment m_HAlignment;
    VAlignment m_VAlignment;

    Font* m_Font;

    vec2 m_Bounds;
    bool m_HasBounds;
};

} } //end namespace

#endif
