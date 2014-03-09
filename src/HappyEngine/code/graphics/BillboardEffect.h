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
//Created: 26/08/2011

#ifndef _HE_BILLBOARD_EFFECT_H_
#define _HE_BILLBOARD_EFFECT_H_
#pragma once

namespace he {
namespace gfx {

class Texture2D;
struct DrawContext;
class VertexLayout;
class MaterialInstance;
class MaterialParameter;

class BillboardEffect
{
public:
    BillboardEffect();
    ~BillboardEffect();

    void init(const VertexLayout& layout);
    void apply(const he::gfx::DrawContext& context) const;

    void setWorldViewProjection(const he::mat44& mat) const;
    void setDiffuseMap(const he::gfx::Texture2D* diffuseMap) const;
    void setTCScale(const vec2& scale) const;

private:
    MaterialInstance* m_Material;

    int8 m_WVP;
    int8 m_DiffTex;
    int8 m_TcScale;

    //Disable default copy constructor and default assignment operator
    BillboardEffect(const BillboardEffect&);
    BillboardEffect& operator=(const BillboardEffect&);
};

} } //end namespace

#endif
