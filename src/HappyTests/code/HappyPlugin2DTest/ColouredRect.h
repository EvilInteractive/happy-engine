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
//Author:  Bastian Damman
//Created: 2014/10/19

#ifndef _HT_ColouredRect_H_
#define _HT_ColouredRect_H_
#pragma once

#include <DefaultLayoutable.h>
#include <IDrawable2D.h>

namespace ht {

class ColouredRect : public he::gui::DefaultLayoutable, public he::gfx::IDrawable2D
{
public:
    ColouredRect();
    ~ColouredRect();

    void createDebugText();
    
    void setColor(const he::Color& color) { m_Color = color; }
    void setBackgroundColor(const he::Color& color) { m_BackgroundColor = color; }

    void draw2D(he::gui::Canvas2D* canvas) override;

private:
    he::Color m_Color;
    he::Color m_BackgroundColor;
    he::gui::Text m_DebugText;
};

} //end namespace

#endif
