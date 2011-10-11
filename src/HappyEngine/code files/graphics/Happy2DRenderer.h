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
#include "mat44.h"
#include "Font.h"
#include "vec2.h"
#include "HappyTypes.h"
#include "AssetContainer.h"
#include "AssetContainerP.h"
#include "ModelMesh.h"
#include "Rect.h"
#include "Text.h"

#include <map>

namespace he {
namespace gfx {

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
	void setTransformation(const mat44& mat);
	// * Set the world translation. *
	void setTranslation(const vec2& translation);
	// * Set the world rotation. *
	void setRotation(const float degrees);
	// * Set the world scale. *
	void setScale(const vec2& scale);
	// * Reset world transformation. *
	void resetTransformation();

    /* DRAW METHODS */

	// * Draws the text as a 2D texture on the screen. *
	void drawString(const std::string& str, const Font::pointer& font, const vec2& pos);
	void drawStringInstanced(const std::string& str, const Font::pointer& font, const vec2& pos);
	// * Draws the text as a 2D texture in a rectangle the screen. *
	void drawString(const std::string& str, const Font::pointer& font, const RectF& rect = RectF(0.0f,0.0f,0.0f,0.0f));
	void drawStringInstanced(const std::string& str, const Font::pointer& font, const RectF& rect = RectF(0.0f,0.0f,0.0f,0.0f));
	// * Draws a line between 2 points with the current strokesize. *
	void drawLine(const vec2& point1, const vec2& point2) const;
	void drawLineInstanced(const vec2& point1, const vec2& point2) const;
	// * Draws the outline of a rectangle with the current strokesize. *
	void drawRectangle(const vec2& pos, const vec2& size);
	void drawRectangleInstanced(const vec2& pos, const vec2& size);
	// * Draws a filled rectangle. *
	void fillRectangle(const vec2& pos, const vec2& size);
	void fillRectangleInstanced(const vec2& pos, const vec2& size);
	// * Draws the outline of an ellipse. *
	void drawEllipse(const vec2& pos, const vec2& size, uint steps = 120);
	void drawEllipseInstanced(const vec2& pos, const vec2& size, uint steps = 120);
	// * Draws a filled ellipse. *
	void fillEllipse(const vec2& pos, const vec2& size, uint steps = 120);
	void fillEllipseInstanced(const vec2& pos, const vec2& size, uint steps = 120);
	// * Draws a polygon - open or closed. *
	void drawPolygon(const std::vector<he::vec2>& points, uint nrPoints, bool close = false) const;
	void drawPolygonInstanced(const std::vector<he::vec2>& points, uint nrPoints, bool close = false) const;
	// * Draws a filled polygon. *
	void fillPolygon(const std::vector<he::vec2>& points, uint nrPoints) const;
	void fillPolygonInstanced(const std::vector<he::vec2>& points, uint nrPoints) const;
	// * Draws a 2D texture with options for resizing, alpha, cliprect. *
	void drawTexture2D(	const Texture2D::pointer& tex2D, const vec2& pos,
						const vec2& newDimensions = vec2(0.0f,0.0f),
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

	mat44 m_matWorld;
	mat44 m_matOrthoGraphic;
	vec2 m_Translation;
	vec2 m_Scale;
	float m_Rotation;

	ModelMesh* m_pTextureQuad;

	vec2 m_ViewPortSize;

	ct::AssetContainerP<gfx::ModelMesh>* m_pModelBuffer;
	ct::AssetContainer<gfx::Texture2D::pointer>* m_pTextureBuffer;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    Happy2DRenderer(const Happy2DRenderer&);
    Happy2DRenderer& operator=(const Happy2DRenderer&);
};

} } //end namespace

#endif
