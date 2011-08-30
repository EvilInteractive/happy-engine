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
//Created: 25/08/2011

#ifndef _2D_RENDERER_H_
#define _2D_RENDERER_H_
#pragma once

#include "I2DRenderer.h"
#include "Vertex.h"
#include "Color.h"
#include "Texture2D.h"
#include "Simple2DEffect.h"
#include "Matrix.h"
#include "SDL.h"
#include "Vector2.h"
#include "HappyTypes.h"

#define HE2D (happyengine::graphics::Simple2DRenderer::GetSingleton())

namespace happyengine {
namespace graphics {

class Simple2DRenderer
{
private:
	Simple2DRenderer();
	static Simple2DRenderer* m_pSingleton;

public:
    virtual ~Simple2DRenderer();
	static Simple2DRenderer* GetSingleton();

	void begin();
	void end();

	void initialize(bool useDefaultShader = true);

	/*----------------*/
    /*	  SETTERS     */
    /*----------------*/
	void setColor(float r, float g, float b, float a = 1.0f);
	void setTransformationMatrix(const happyengine::math::Matrix &mat) const;
	void setAntiAliasing(bool bAA);
	void setStrokeSize(float strokeSize = 1.0f);

	/*----------------*/
    /*  DRAW METHODS  */
    /*----------------*/
	void drawText(const std::string &text, float x, float y) const;
	void drawRectangle(float x, float y, float width, float height) const;
	void fillRectangle(float x, float y, float width, float height) const;
	void fillEllipse(float x, float y, float width, float height) const;
	void drawPolygon(const std::vector<happyengine::math::Vector2> &points, happyengine::uint nrPoints, bool close = false) const;

private:
	bool m_bAntiAliasing;

	Color m_CurrentColor;
	VertexLayout m_VertexLayout;

	happyengine::graphics::Simple2DEffect* m_pEffect;

    //Disable default copy constructor and default assignment operator
    Simple2DRenderer(const Simple2DRenderer&);
    Simple2DRenderer& operator=(const Simple2DRenderer&);
};

} } //end namespace

#endif
