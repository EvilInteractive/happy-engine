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

#include "Font.h"
#include <string>
#include <vector>
#include "boost/shared_ptr.hpp"

namespace he {
namespace gui {

class Text
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
    Text(	gfx::Font* font,
            OverFlowType overflow = OverFlowType_Clip);
    Text(	const std::string& text,
            gfx::Font* font,
            OverFlowType overflow = OverFlowType_Clip);
    Text(	OverFlowType overflow = OverFlowType_Clip);
    Text(	const std::string& text,
            OverFlowType overflow = OverFlowType_Clip);
    virtual ~Text();

    /* GENERAL */
    void addLine(const std::string& string);
    void clear();

    /* SETTERS */
    void setLine(const std::string& string, uint lineNumber);
    void setHorizontalAlignment(HAlignment alignment);
    void setVerticalAlignment(VAlignment alignment);

    /* GETTERS */
    const std::string& getLine(uint lineNumber) const;
    const std::vector<std::string>& getText() const;

    bool isEmpty() const;

    OverFlowType getOverFlowType() const;

    HAlignment getHorizontalAlignment() const;
    VAlignment getVerticalAlignment() const;

    gfx::Font* getFont() const;

private:

    /* DATAMEMBERS */
    std::vector<std::string> m_Text;

    OverFlowType m_OverFlowType;

    HAlignment m_HAlignment;
    VAlignment m_VAlignment;

    gfx::Font* m_pFont;

    /* DEFAULT COPY & ASSIGNMENT */
    Text(const Text&);
    Text& operator=(const Text&);
};

} } //end namespace

#endif
