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

#ifndef _HE_HAPPY_2D_RENDERER_H_
#define _HE_HAPPY_2D_RENDERER_H_
#pragma once

#include "Vertex.h"
#include "Color.h"
#include "Texture2D.h"
#include "Simple2DEffect.h"
#include "Simple2DTextureEffect.h"
#include "Matrix.h"
#include "Font.h"
#include "Vector2.h"
#include "HappyTypes.h"
#include "AssetContainer.h"
#include "AssetContainerP.h"
#include "ModelMesh.h"
#include "Rect.h"
#include "Text.h"

#include <map>

namespace happyengine {
namespace graphics {

class Happy2DRenderer
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */
	Happy2DRenderer();
    virtual ~Happy2DRenderer();

	/* GENERAL */
	void initialize();
	void begin();
	void end();

	void clearInstancingBuffer();

    /* SETTERS */

	// * Set the color of the 2D brush. *
	void setColor(float r, float g, float b, float a = 1.0f);
	void setColor(const Color& color);
	// * Turn on/off anti-aliasing. *
	void setAntiAliasing(bool bAA);
	// * Set the strokesize. *
	void setStrokeSize(const float strokeSize = 1.0f);
	// * Alignment options for text. *
	void setFontHorizontalAlignment(Font::HAlignment horizontalAlignment);
	void setFontVerticalAlignment(Font::VAlignment verticalAlignment);
	// * Set the transformation matrix. *
	void setTransformation(const math::Matrix& mat);
	// * Set the world translation. *
	void setTranslation(const math::Vector2& translation);
	// * Set the world rotation. *
	void setRotation(const float degrees);
	// * Set the world scale. *
	void setScale(const math::Vector2& scale);
	// * Reset world transformation. *
	void resetTransformation();

    /* DRAW METHODS */

	// * Draws the text as a 2D texture on the screen. *
	void drawString(const std::string& str, const Font::pointer& font, const math::Vector2& pos);
	void drawStringInstanced(const std::string& str, const Font::pointer& font, const math::Vector2& pos);
	// * Draws the text as a 2D texture in a rectangle the screen. *
	void drawString(const std::string& str, const Font::pointer& font, const RectF& rect = RectF(0.0f,0.0f,0.0f,0.0f));
	void drawStringInstanced(const std::string& str, const Font::pointer& font, const RectF& rect = RectF(0.0f,0.0f,0.0f,0.0f));
	// * Draws a line between 2 points with the current strokesize. *
	void drawLine(const math::Vector2& point1, const math::Vector2& point2) const;
	void drawLineInstanced(const math::Vector2& point1, const math::Vector2& point2) const;
	// * Draws the outline of a rectangle with the current strokesize. *
	void drawRectangle(const math::Vector2& pos, const math::Vector2& size);
	void drawRectangleInstanced(const math::Vector2& pos, const math::Vector2& size);
	// * Draws a filled rectangle. *
	void fillRectangle(const math::Vector2& pos, const math::Vector2& size);
	void fillRectangleInstanced(const math::Vector2& pos, const math::Vector2& size);
	// * Draws the outline of an ellipse. *
	void drawEllipse(const math::Vector2& pos, const math::Vector2& size, uint steps = 120);
	void drawEllipseInstanced(const math::Vector2& pos, const math::Vector2& size, uint steps = 120);
	// * Draws a filled ellipse. *
	void fillEllipse(const math::Vector2& pos, const math::Vector2& size, uint steps = 120);
	void fillEllipseInstanced(const math::Vector2& pos, const math::Vector2& size, uint steps = 120);
	// * Draws a polygon - open or closed. *
	void drawPolygon(const std::vector<happyengine::math::Vector2>& points, uint nrPoints, bool close = false) const;
	void drawPolygonInstanced(const std::vector<happyengine::math::Vector2>& points, uint nrPoints, bool close = false) const;
	// * Draws a filled polygon. *
	void fillPolygon(const std::vector<happyengine::math::Vector2>& points, uint nrPoints) const;
	void fillPolygonInstanced(const std::vector<happyengine::math::Vector2>& points, uint nrPoints) const;
	// * Draws a 2D texture with options for resizing, alpha, cliprect. *
	void drawTexture2D(	const Texture2D::pointer& tex2D, const math::Vector2& pos,
						const math::Vector2& newDimensions = math::Vector2(0.0f,0.0f),
						const float alpha = 1.0f, const RectF& regionToDraw = RectF(0.0f,0.0f,0.0f,0.0f));

private:

	void updateTransformationMatrix();

	/* DATAMEMBERS */
	float m_StrokeSize;
	bool m_bAntiAliasing;

	Color m_CurrentColor;

	Font::HAlignment m_FontHAlignment;
	Font::VAlignment m_FontVAlignment;

	VertexLayout m_VertexLayoutColor;
	VertexLayout m_VertexLayoutTexture;

	Simple2DEffect* m_pColorEffect;
	Simple2DTextureEffect* m_pTextureEffect;

	math::Matrix m_matWorld;
	math::Matrix m_matOrthoGraphic;
	math::Vector2 m_Translation;
	math::Vector2 m_Scale;
	float m_Rotation;

	ModelMesh* m_pTextureQuad;

	math::Vector2 m_ViewPortSize;

	content::AssetContainerP<graphics::ModelMesh>* m_pModelBuffer;
	content::AssetContainer<graphics::Texture2D::pointer>* m_pTextureBuffer;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    Happy2DRenderer(const Happy2DRenderer&);
    Happy2DRenderer& operator=(const Happy2DRenderer&);
};

} } //end namespace

#endif
