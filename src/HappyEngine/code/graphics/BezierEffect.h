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
//Created: 2014/09/03

#ifndef _HE_BEZIER_EFFECT_H_
#define _HE_BEZIER_EFFECT_H_
#pragma once

namespace he {
namespace gfx {

struct DrawContext;
class VertexLayout;
class MaterialInstance;
class MaterialParameter;

class BezierEffect
{
public:
    BezierEffect();
    ~BezierEffect();

    void init(const VertexLayout& layout);
    void apply(const he::gfx::DrawContext& context) const;

    void setColor(const Color& color) const;
    void setWVPMatrix(const he::mat44& mat) const;
    void setRadius(float depth) const;
    void setCurve(const he::vec2& begin, const he::vec2& tangent1, const he::vec2& tangent2, const he::vec2& endPos) const;

private:

    MaterialInstance* m_Material;

    int8 m_WVP;
    int8 m_Color;
    int8 m_Radius;
    int8 m_BeginEnd;
    int8 m_Tangents;

    //Disable default copy constructor and default assignment operator
    BezierEffect(const BezierEffect&);
    BezierEffect& operator=(const BezierEffect&);
};

} } //end namespace

#endif
