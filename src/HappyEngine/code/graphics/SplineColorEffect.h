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
//Created: 06/11/2011

#ifndef _HE_SPLINE_COLOR_EFFECT_H_
#define _HE_SPLINE_COLOR_EFFECT_H_
#pragma once

#include "mat44.h"
#include "Shader.h"
#include "HappyTypes.h"
#include "Color.h"

namespace he {
namespace gfx {

class SplineColorEffect
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */
	SplineColorEffect();
    virtual ~SplineColorEffect();

	/* GENERAL */
	void load();
	void begin() const;
	void end() const;

	/* SETTERS */
	void setViewProjection(const mat44& mat);
	void setWorld(const mat44& mat);
	void setColor(const Color& color);

private:

	/* DATAMEMBERS */
	Shader* m_pShader;

	uint m_ShaderVPPos;
	uint m_ShaderWPos;
	uint m_ShaderColorPos;

    /* DEFAULT COPY & ASSIGNMENT */
    SplineColorEffect(const SplineColorEffect&);
    SplineColorEffect& operator=(const SplineColorEffect&);
};

} } //end namespace

#endif
