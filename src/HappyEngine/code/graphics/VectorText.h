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
//Created: 2014/09/23

#ifndef _HE_VECTORTEXT_H_
#define _HE_VECTORTEXT_H_
#pragma once

#include "IDrawable2D.h"

namespace he {
namespace gfx {
    class Model;
    class ModelMesh;
    class Simple2DEffect;
}
namespace gui {
    class Font;

class HAPPY_ENTRY VectorText
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

    /* CONSTRUCTOR - DESTRUCTOR */
    VectorText();
    ~VectorText();

    /* GENERAL */
    void create(Font* font, HAlignment halign, VAlignment valign, const he::String& text);
    void setScale(const float scale) { m_Scale = scale; }
    void setPostion(const he::vec2& pos) { m_Position = pos; }

    he::RectF getBound() const;

    /* DRAW */
    void draw2D(he::gui::Canvas2D* canvas, const he::mat33& transform);

private:
    /* DATAMEMBERS */
    float m_Scale;
    he::vec2 m_Position;
    gfx::ModelMesh* m_Text;
    gfx::Simple2DEffect* m_Effect;

    /* HIDE DEFAULT COPY & ASSIGNMENT */
    VectorText(const VectorText& text);
    VectorText& operator=(const VectorText& text);
};

} } //end namespace

#endif
