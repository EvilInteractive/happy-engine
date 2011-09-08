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
#include "Simple2DTextureEffect.h"
#include "Matrix.h"
#include "Font.h"
#include "Vector2.h"
#include "HappyTypes.h"
#include "AssetContainerP.h"
#include "Model.h"

#include <map>

#define HE2D (happyengine::graphics::Simple2DRenderer::getSingleton())

namespace happyengine {
namespace graphics {

class Simple2DRenderer
{
private:

	// SINGLETON
	Simple2DRenderer();
	static Simple2DRenderer* m_pSingleton;

public:

	// CONSTRUCTOR - DESTRUCTOR
    virtual ~Simple2DRenderer();
	static Simple2DRenderer* getSingleton();

	// GENERAL
	void begin();
	void end();
	void initialize(bool useDefaultShader = true);

    // SETTERS
	void setColor(float r, float g, float b, float a = 1.0f);
	void setTransformationMatrix(const happyengine::math::Matrix& mat);
	void setAntiAliasing(bool bAA);
	void setStrokeSize(float strokeSize = 1.0f);
	void setFontHorizontalAlignment(FontHAlignment horizontalAlignment);
	void setFontVerticalAlignment(FontVAlignment verticalAlignment);

    // DRAW METHODS
	void drawText(const math::Vector2& pos, const std::string& text, const happyengine::graphics::Font::pointer& font) const;
	void drawRectangle(const math::Vector2& pos, const math::Vector2& size);
	void fillRectangle(const math::Vector2& pos, const math::Vector2& size);
	void drawEllipse(const math::Vector2& pos, const math::Vector2& size, uint steps = 360);
	void fillEllipse(const math::Vector2& pos, const math::Vector2& size, uint steps = 360);
	void drawPolygon(const std::vector<happyengine::math::Vector2>& points, happyengine::uint nrPoints, bool close = false);
	void fillPolygon(const std::vector<happyengine::math::Vector2>& points, happyengine::uint nrPoints);
	void drawTexture2D(const math::Vector2& pos, const graphics::Texture2D::pointer& tex2D, const math::Vector2& newDimensions = math::Vector2(0.0f,0.0f)) const;

private:

	math::Vector2 getNDCPos(const math::Vector2& pos) const;
	math::Vector2 getNDCSize(const math::Vector2& size) const;

	void cleanUpModelBuffer();

	// DATAMEMBERS
	float m_StrokeSize;
	bool m_bAntiAliasing;

	Color m_CurrentColor;
	FontHAlignment m_FontHAlignment;
	FontVAlignment m_FontVAlignment;

	VertexLayout m_VertexLayoutColor;
	VertexLayout m_VertexLayoutTexture;

	Simple2DEffect* m_pColorEffect;
	Simple2DTextureEffect* m_pTextureEffect;

	math::Matrix m_matWorld;
	math::Vector2 m_ViewPortSize;

	content::AssetContainerP<graphics::Model>* m_pModelBuffer;
	std::map<std::string, uint> m_ModelBufferIndex;
	uint m_TickCounter;

    //Disable default copy constructor and default assignment operator
    Simple2DRenderer(const Simple2DRenderer&);
    Simple2DRenderer& operator=(const Simple2DRenderer&);
};

} } //end namespace

#endif
