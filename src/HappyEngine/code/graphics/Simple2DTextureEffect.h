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

#ifndef _HE_SIMPLE_2D_TEXTURE_EFFECT_H_
#define _HE_SIMPLE_2D_TEXTURE_EFFECT_H_
#pragma once

#include "HappyTypes.h"
#include "Shader.h"
#include "mat44.h"
#include "vec2.h"
#include "vec3.h"
#include "Texture2D.h"

namespace he {
namespace gfx {

class Simple2DTextureEffect
{
public:
    Simple2DTextureEffect();
    virtual ~Simple2DTextureEffect();

    void load();
    void begin() const;
    void end() const;

    void setWorldMatrix(const he::mat44& mat) const;
    void setDiffuseMap(const he::gfx::Texture2D* diffuseMap) const;
    void setAlpha(const float alpha) const;
    void setTCOffset(const vec2& offset) const;
    void setTCScale(const vec2& scale) const;
    void setDepth(float depth) const;

private:
    gfx::Shader* m_pShader;

    uint m_ShaderWVPPos;
    uint m_ShaderDiffTexPos;
    uint m_ShaderAlphaPos;
    uint m_ShaderTCOffsetPos;
    uint m_ShaderTCScalePos;
    uint m_ShaderDepthPos;

    //Disable default copy constructor and default assignment operator
    Simple2DTextureEffect(const Simple2DTextureEffect&);
    Simple2DTextureEffect& operator=(const Simple2DTextureEffect&);
};

} } //end namespace

#endif
