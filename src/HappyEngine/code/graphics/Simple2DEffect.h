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

#ifndef _HE_SIMPLE_2D_EFFECT_H_
#define _HE_SIMPLE_2D_EFFECT_H_
#pragma once

namespace he {
namespace gfx {

class MaterialInstance;
class MaterialParameter;

class Simple2DEffect
{
public:
    Simple2DEffect();
    ~Simple2DEffect();

    void init(const BufferLayout& layout);
    void begin(const he::gfx::DrawContext& context) const;
    void end() const;

    void setColor(const Color& color) const;
    void setWorldMatrix(const he::mat44& mat) const;
    void setDepth(float depth) const;

private:

    MaterialInstance* m_Material;

    MaterialParameter* m_WVP;
    MaterialParameter* m_Color;
    MaterialParameter* m_Depth;

    //Disable default copy constructor and default assignment operator
    Simple2DEffect(const Simple2DEffect&);
    Simple2DEffect& operator=(const Simple2DEffect&);
};

} } //end namespace

#endif
