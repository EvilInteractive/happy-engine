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

#ifndef _HE_SIMPLE_FORWARD_3D_RENDERER_H_
#define _HE_SIMPLE_FORWARD_3D_RENDERER_H_
#pragma once

#include "ModelMesh.h"
#include "mat44.h"
#include "Color.h"
#include "SplineColorEffect.h"
#include "Camera.h"

namespace he {
namespace gfx {

class SimpleForward3DRenderer
{
public:

	/* CONSTRUCTOR - DESCTRUCTOR */
	SimpleForward3DRenderer();
	virtual ~SimpleForward3DRenderer();

	/* GENERAL */
	void init();
	void begin(const Camera* pCamera);
	void end();

	/* DRAW METHODS */
	void drawSpline(const ModelMesh::pointer& spline, const mat44& world, const Color& color) const;

private:

	/* DATAMEMBERS */
	SplineColorEffect* m_pColorEffect;

	mat44 m_ViewProjection;

	uint m_RenderFboID;
	Texture2D::pointer m_pRenderTexture;

	/* DEFAULT COPY & ASSIGNMENT OPERATOR */
	SimpleForward3DRenderer(const SimpleForward3DRenderer&);
	SimpleForward3DRenderer& operator=(const SimpleForward3DRenderer&);
};

} } //end namespace

#endif
