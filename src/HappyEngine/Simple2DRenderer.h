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

namespace happyengine {
namespace graphics {

class Simple2DRenderer
{
public:
	Simple2DRenderer();
    virtual ~Simple2DRenderer();

	void begin();
	void end();

	void initialize(bool useDefaultShader = true);

	void setColor(float r, float g, float b, float a = 1.0f);

	/*----------------*/
    /*  DRAW METHODS  */
    /*----------------*/
	void drawRectangle(float x, float y, float width, float height, float strokeSize = 1.0f) const;

private:

	uint m_FboId;

    // Textures:
    //      - Color.rgba      GL_RGBA8
    static const int TEXTURES = 1;
    static const int TEXTURE_FORMAT;
    static const int TEXTURE_INTERNALFORMAT;
    static const int TEXTURE_ATTACHMENT;
    uint m_TextureId[TEXTURES];
    uint m_DepthBufferId;

	Texture2D::pointer m_pTexture;

	Color m_CurrentColor;
	VertexLayout m_VertexLayout;

	happyengine::graphics::Simple2DEffect* m_pEffect;

    //Disable default copy constructor and default assignment operator
    Simple2DRenderer(const Simple2DRenderer&);
    Simple2DRenderer& operator=(const Simple2DRenderer&);
};

} } //end namespace

#endif
