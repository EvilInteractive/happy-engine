//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#ifndef _SIMPLE_2D_EFFECT_H_
#define _SIMPLE_2D_EFFECT_H_
#pragma once

#include "HappyTypes.h"
#include "Shader.h"
#include "Matrix.h"
#include "Vector3.h"

namespace happyengine {
namespace graphics {

class Simple2DEffect
{
public:
	Simple2DEffect();
    virtual ~Simple2DEffect();

	void load();
	void begin() const;
	void end() const;

	void setWorldMatrix(const happyengine::math::Matrix& mat) const;

private:

	happyengine::graphics::Shader* m_pShader;

	happyengine::uint m_ShaderWVPPos;

    //Disable default copy constructor and default assignment operator
    Simple2DEffect(const Simple2DEffect&);
    Simple2DEffect& operator=(const Simple2DEffect&);
};

} } //end namespace

#endif