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
#include "StdAfx.h" 

#include "Happy2DRenderer.h"
#include "HappyNew.h"
#include "OpenGL.h"
#include "HappyEngine.h"
#include "MathFunctions.h"
#include "GraphicsEngine.h"

#include <algorithm>
#include <vector>

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESTRUCTOR */
Happy2DRenderer::Happy2DRenderer() :	m_pColorEffect(NEW he::gfx::Simple2DEffect()),
										m_pTextureEffect(NEW he::gfx::Simple2DTextureEffect()),
										m_bAntiAliasing(false),
										m_StrokeSize(0.0f),
										m_CurrentColor(1.0f,1.0f,1.0f,1.0f),
										m_ViewPortSize(0.0f,0.0f),
										m_FontHAlignment(Font::HAlignment_Left),
										m_FontVAlignment(Font::VAlignment_Bottom),
                                        m_pModelBuffer(NEW ct::AssetContainer<gfx::ModelMesh*>( [](gfx::ModelMesh* pMesh) { delete pMesh; } )),
										m_pTextureBuffer(NEW ct::AssetContainer<gfx::Texture2D::pointer>()),
										m_pTextureQuad(NEW ModelMesh("")),
										m_Translation(vec2(0.0f,0.0f)),
										m_Scale(vec2(1.0f,1.0f)),
										m_Rotation(0.0f)
{
    
}

Happy2DRenderer::~Happy2DRenderer()
{
	delete m_pColorEffect;
	delete m_pTextureEffect;
	delete m_pModelBuffer;
	delete m_pTextureBuffer;
	delete m_pTextureQuad;
}

void Happy2DRenderer::clearInstancingBuffer()
{
	m_pModelBuffer->removeAllAssets();
	m_pTextureBuffer->removeAllAssets();
}

void Happy2DRenderer::updateTransformationMatrix()
{
	m_matWorld = mat44::createTranslation(vec3(m_Translation.x, m_Translation.y, 0.0f)) *
				 mat44::createRotation(vec3(0.0f,0.0f,1.0f), m_Rotation) *
				 mat44::createScale(m_Scale.x, m_Scale.y, 1.0f);
}

/* GENERAL */
void Happy2DRenderer::begin()
{
    GL::heBlendEnabled(true);
    GL::heBlendFunc(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);
    GL::heSetDepthWrite(false);

	m_ViewPortSize.x = static_cast<float>(GRAPHICS->getScreenWidth());
	m_ViewPortSize.y = static_cast<float>(GRAPHICS->getScreenHeight());

	m_pColorEffect->begin();
}

void Happy2DRenderer::end()
{
	m_pColorEffect->end();

    GL::heBlendEnabled(false);
    GL::heSetDepthWrite(true);

	setStrokeSize();

	resetTransformation();
	m_matOrthoGraphic = mat44::createOrthoLH(0.0f, m_ViewPortSize.x, 0.0f, m_ViewPortSize.y, 0.0f, 10.0f);
}

void Happy2DRenderer::initialize()
{
	m_VertexLayoutColor.addElement(VertexElement(0, VertexElement::Type_Vec2, VertexElement::Usage_Position, 8, 0));

	m_VertexLayoutTexture.addElement(VertexElement(0, VertexElement::Type_Vec2, VertexElement::Usage_Position, 8, 0));
	m_VertexLayoutTexture.addElement(VertexElement(1, VertexElement::Type_Vec2, VertexElement::Usage_TextureCoordinate, 8, 8));
    
	m_ViewPortSize.x = static_cast<float>(GRAPHICS->getScreenWidth());
	m_ViewPortSize.y = static_cast<float>(GRAPHICS->getScreenHeight());

	m_matOrthoGraphic = mat44::createOrthoLH(0.0f, m_ViewPortSize.x, 0.0f, m_ViewPortSize.y, 0.0f, 10.0f);

	// effects
	m_pColorEffect->load();
	m_pTextureEffect->load();

	// model texturequad
	m_pTextureQuad->init();

	std::vector<VertexPosTex2D> vertices;
	vertices.push_back(
		VertexPosTex2D(vec2(-0.5f, 0.5f),
		vec2(0, 0)));

	vertices.push_back(
		VertexPosTex2D(vec2(0.5f, 0.5f),
		vec2(1, 0)));

	vertices.push_back(
		VertexPosTex2D(vec2(-0.5f, -0.5f),
		vec2(0, 1)));

	vertices.push_back(
		VertexPosTex2D(vec2(0.5f, -0.5f),
		vec2(1, 1)));

    std::vector<byte> indices;
    indices.push_back(2); indices.push_back(1); indices.push_back(0);
	indices.push_back(1); indices.push_back(2); indices.push_back(3);

    m_pTextureQuad->setVertices(&vertices[0], 4, m_VertexLayoutTexture);
    m_pTextureQuad->setIndices(&indices[0], 6, IndexStride_Byte);
}

/* SETTERS */
void Happy2DRenderer::setColor(float r, float g, float b, float a)
{
	m_CurrentColor.r(r);
	m_CurrentColor.g(g);
	m_CurrentColor.b(b);
	m_CurrentColor.a(a);
}

void Happy2DRenderer::setColor(const Color& color)
{
	m_CurrentColor = color;
}

void Happy2DRenderer::setAntiAliasing(bool bAA)
{
	if (bAA)
	{
		glEnable(GL_LINE_SMOOTH);
	}
	else
	{
		glDisable(GL_LINE_SMOOTH);
	}
}

void Happy2DRenderer::setStrokeSize(const float strokeSize)
{
	glLineWidth(strokeSize);

	m_StrokeSize = strokeSize;
}

void Happy2DRenderer::setFontHorizontalAlignment(Font::HAlignment horizontalAlignment)
{
	m_FontHAlignment = horizontalAlignment;
}

void Happy2DRenderer::setFontVerticalAlignment(Font::VAlignment verticalAlignment)
{
	m_FontVAlignment = verticalAlignment;
}

void Happy2DRenderer::setTransformation(const mat44& mat)
{
	m_matWorld = m_matOrthoGraphic * mat;
}

void Happy2DRenderer::setTranslation(const vec2& translation)
{
	m_Translation = translation;
	updateTransformationMatrix();
}

void Happy2DRenderer::setRotation(const float degrees)
{
	m_Rotation = toRadians(degrees);
	updateTransformationMatrix();
}

void Happy2DRenderer::setScale(const vec2& scale)
{
	m_Scale = scale;
	updateTransformationMatrix();
}

void Happy2DRenderer::resetTransformation()
{
	m_Translation = vec2(0.0f,0.0f);
	m_Rotation = 0.0f;
	m_Scale = vec2(1.0f,1.0f);

	updateTransformationMatrix();
}

/* DRAW METHODS */
void Happy2DRenderer::drawString(const std::string& str, const Font::pointer& font, const vec2& pos)
{
	Texture2D::pointer texFont(font->createTextureText(str, m_CurrentColor, m_bAntiAliasing));

	drawTexture2D(texFont, pos, vec2(static_cast<float>(texFont->getWidth()), -static_cast<float>(texFont->getHeight())));
}
void Happy2DRenderer::drawStringInstanced(const std::string& str, const Font::pointer& font, const vec2& pos)
{
	std::stringstream stream;
	stream << "TEXT." << m_CurrentColor.r() << "." << m_CurrentColor.g()
		<< "." << m_CurrentColor.b() << "." << m_CurrentColor.a() << "."
		<< str << font->getPath();

	if (m_pTextureBuffer->isAssetPresent(stream.str()))
	{
		Texture2D::pointer texFont = m_pTextureBuffer->getAsset(stream.str());
		drawTexture2D(texFont, pos, vec2(static_cast<float>(texFont->getWidth()), -static_cast<float>(texFont->getHeight())));
	}
	else
	{
		Texture2D::pointer texFont(font->createTextureText(str, m_CurrentColor, m_bAntiAliasing));

		m_pTextureBuffer->addAsset(stream.str(), texFont);

		drawTexture2D(texFont, pos, vec2(static_cast<float>(texFont->getWidth()), -static_cast<float>(texFont->getHeight())));
	}
}

void Happy2DRenderer::drawString(const std::string& str, const Font::pointer& font, const RectF& rect)
{
	vec2 textSize;
	Texture2D::pointer texFont(font->createTextureText(str, m_CurrentColor, m_bAntiAliasing, &textSize));

	vec2 position;

	switch (m_FontHAlignment)
    {
		case Font::HAlignment_Left: position.x = rect.x; break;
		case Font::HAlignment_Center: position.x = rect.x + rect.width/2 - textSize.x/2; break;
        case Font::HAlignment_Right: position.x = rect.x + rect.width - textSize.x; break;
        default: ASSERT("unkown font alignment");
    }
	switch (m_FontVAlignment)
    {
        case Font::VAlignment_Top: position.y = rect.y; break;
        case Font::VAlignment_Center: position.y = rect.y + rect.height/2 - textSize.y/2; break;
        case Font::VAlignment_Bottom: position.y = rect.y + rect.height - textSize.y; break;
        default: ASSERT("unkown font alignment");
    }

	drawTexture2D(texFont, position, vec2(static_cast<float>(texFont->getWidth()), -static_cast<float>(texFont->getHeight())));
}
void Happy2DRenderer::drawStringInstanced(const std::string& str, const Font::pointer& font, const RectF& rect)
{
	std::stringstream stream;
	stream << "TEXTR." << m_CurrentColor.r() << "." << m_CurrentColor.g()
		<< "." << m_CurrentColor.b() << "." << m_CurrentColor.a() << "."
		<< rect.x << "." << rect.y << "." << rect.width << "." << rect.height
		<< "." << str << font->getPath();

	Texture2D::pointer texFont;

	//vec2 textSize;

	if (m_pTextureBuffer->isAssetPresent(stream.str()))
	{
		texFont = m_pTextureBuffer->getAsset(stream.str());
	}
	else
	{
		texFont = font->createTextureText(str, m_CurrentColor, m_bAntiAliasing);

		m_pTextureBuffer->addAsset(stream.str(), texFont);
	}

	vec2 position;

	switch (m_FontHAlignment)
    {
		case Font::HAlignment_Left: position.x = rect.x; break;
		case Font::HAlignment_Center: position.x = rect.x + rect.width/2 - texFont->getWidth()/2; break;
        case Font::HAlignment_Right: position.x = rect.x + rect.width - texFont->getWidth(); break;
        default: ASSERT("unkown font alignment");
    }
	switch (m_FontVAlignment)
    {
        case Font::VAlignment_Top: position.y = rect.y; break;
        case Font::VAlignment_Center: position.y = rect.y + rect.height/2 - texFont->getHeight()/2; break;
        case Font::VAlignment_Bottom: position.y = rect.y + rect.height - texFont->getHeight(); break;
        default: ASSERT("unkown font alignment");
    }

	drawTexture2D(texFont, position, vec2(static_cast<float>(texFont->getWidth()), -static_cast<float>(texFont->getHeight())));
}

void Happy2DRenderer::drawLine(const vec2& point1, const vec2& point2) const
{
	ModelMesh model("");

	std::vector<VertexPos2D> vertices;
	std::vector<byte> indices;

	byte i(0);

	vertices.push_back(VertexPos2D(vec2(point1.x, point1.y)));
	indices.push_back(i++);

	vertices.push_back(VertexPos2D(vec2(point2.x, point2.y)));
	indices.push_back(i++);

	model.init();
	model.setVertices(&vertices[0], 2, m_VertexLayoutColor);
	model.setIndices(&indices[0], 2, IndexStride_Byte);

	m_pColorEffect->setWorldMatrix(m_matOrthoGraphic * m_matWorld);
	m_pColorEffect->setColor(m_CurrentColor);

	GL::heBindVao(model.getVertexArraysID());
	glDrawElements(GL_LINE_STRIP, model.getNumIndices(), model.getIndexType(), 0);
}
void Happy2DRenderer::drawLineInstanced(const vec2& point1, const vec2& point2) const
{
	ModelMesh* pModel;

	std::stringstream stream;
	stream << "L.X1." << point1.x << ".Y1." << point1.y << ".X2." << point2.x << ".Y2." << point2.y;

	if (m_pModelBuffer->isAssetPresent(stream.str()))
	{
		pModel =  m_pModelBuffer->getAsset(stream.str());
	}
	else
	{
		std::vector<VertexPos2D> vertices;
		std::vector<byte> indices;

		byte i(0);

		vertices.push_back(VertexPos2D(vec2(point1.x, point1.y)));
		indices.push_back(i++);

		vertices.push_back(VertexPos2D(vec2(point2.x, point2.y)));
		indices.push_back(i++);

		pModel = NEW ModelMesh(stream.str());
		pModel->init();
		pModel->setVertices(&vertices[0], 2, m_VertexLayoutColor);
		pModel->setIndices(&indices[0], 2, IndexStride_Byte);

		m_pModelBuffer->addAsset(pModel->getName(), pModel);
	}

	m_pColorEffect->setWorldMatrix(m_matOrthoGraphic * m_matWorld);
	m_pColorEffect->setColor(m_CurrentColor);

    GL::heBindVao(pModel->getVertexArraysID());
	glDrawElements(GL_LINE_STRIP, pModel->getNumIndices(), pModel->getIndexType(), 0);
}

void Happy2DRenderer::drawRectangle(const vec2& pos, const vec2& size)
{
	ModelMesh model("");

	std::vector<VertexPos2D> vertices;
	vertices.push_back(VertexPos2D(vec2(size.x, 0)));
	vertices.push_back(VertexPos2D(vec2(0, 0)));
	vertices.push_back(VertexPos2D(vec2(size.x - m_StrokeSize/2.0f, m_StrokeSize/2.0f)));
	vertices.push_back(VertexPos2D(vec2(size.x - m_StrokeSize/2.0f, size.y - m_StrokeSize/2.0f)));
	vertices.push_back(VertexPos2D(vec2(size.x, size.y)));
	vertices.push_back(VertexPos2D(vec2(0, size.y)));
	vertices.push_back(VertexPos2D(vec2(m_StrokeSize/2.0f, m_StrokeSize/2.0f)));
	vertices.push_back(VertexPos2D(vec2(m_StrokeSize/2.0f, size.y - m_StrokeSize/2.0f)));
	
	std::vector<byte> indices;
	indices.push_back(0); indices.push_back(1);
	indices.push_back(2); indices.push_back(3);
	indices.push_back(4); indices.push_back(5);
	indices.push_back(6); indices.push_back(7);

	model.init();
	model.setVertices(&vertices[0], 8, m_VertexLayoutColor);
	model.setIndices(&indices[0], 8, IndexStride_Byte);

	vec2 translation(pos.x, pos.y);

	m_Translation += translation;
	updateTransformationMatrix();

	m_pColorEffect->setWorldMatrix(m_matOrthoGraphic * m_matWorld);
	m_pColorEffect->setColor(m_CurrentColor);

	m_Translation -= translation;
	updateTransformationMatrix();
    
    GL::heBindVao(model.getVertexArraysID());
	glDrawElements(GL_LINES, model.getNumIndices(), model.getIndexType(), 0);
}
void Happy2DRenderer::drawRectangleInstanced(const vec2& pos, const vec2& size)
{
	ModelMesh* pModel;

	std::stringstream stream;
	stream << "HR.W." << size.x << ".H." << size.y << ".S." << m_StrokeSize;

	if (m_pModelBuffer->isAssetPresent(stream.str()))
	{
		pModel =  m_pModelBuffer->getAsset(stream.str());
	}
	else
	{
		std::vector<VertexPos2D> vertices;
		vertices.push_back(VertexPos2D(vec2(size.x, 0)));
		vertices.push_back(VertexPos2D(vec2(0, 0)));
		vertices.push_back(VertexPos2D(vec2(size.x - m_StrokeSize/2.0f, m_StrokeSize/2.0f)));
		vertices.push_back(VertexPos2D(vec2(size.x - m_StrokeSize/2.0f, size.y - m_StrokeSize/2.0f)));
		vertices.push_back(VertexPos2D(vec2(size.x, size.y)));
		vertices.push_back(VertexPos2D(vec2(0, size.y)));
		vertices.push_back(VertexPos2D(vec2(m_StrokeSize/2.0f, m_StrokeSize/2.0f)));
		vertices.push_back(VertexPos2D(vec2(m_StrokeSize/2.0f, size.y - m_StrokeSize/2.0f)));
	
		std::vector<byte> indices;
		indices.push_back(0); indices.push_back(1);
		indices.push_back(2); indices.push_back(3);
		indices.push_back(4); indices.push_back(5);
		indices.push_back(6); indices.push_back(7);

		pModel = NEW ModelMesh(stream.str());
		pModel->init();
		pModel->setVertices(&vertices[0], 8, m_VertexLayoutColor);
		pModel->setIndices(&indices[0], 8, IndexStride_Byte);

		m_pModelBuffer->addAsset(pModel->getName(), pModel);
	}

	vec2 translation(pos.x, pos.y);

	m_Translation += translation;
	updateTransformationMatrix();

	m_pColorEffect->setWorldMatrix(m_matOrthoGraphic * m_matWorld);
	m_pColorEffect->setColor(m_CurrentColor);

	m_Translation -= translation;
	updateTransformationMatrix();

    GL::heBindVao(pModel->getVertexArraysID());
	glDrawElements(GL_LINES, pModel->getNumIndices(), pModel->getIndexType(), 0);
}

void Happy2DRenderer::fillRectangle(const vec2& pos, const vec2& size)
{
	ModelMesh model("");

	std::vector<VertexPos2D> vertices;
	vertices.push_back(VertexPos2D(vec2(-size.x/2, size.y/2)));
	vertices.push_back(VertexPos2D(vec2(size.x/2, size.y/2)));
	vertices.push_back(VertexPos2D(vec2(-size.x/2, -size.y/2)));
	vertices.push_back(VertexPos2D(vec2(size.x/2, -size.y/2)));

	std::vector<byte> indices;
	indices.push_back(2); indices.push_back(1); indices.push_back(0);
	indices.push_back(1); indices.push_back(2); indices.push_back(3);

	model.init();
	model.setVertices(&vertices[0], 4, m_VertexLayoutColor);
	model.setIndices(&indices[0], 6, IndexStride_Byte);

	vec2 translation(pos.x + size.x/2, pos.y + size.y/2);

	m_Translation += translation;
	updateTransformationMatrix();

	m_pColorEffect->setWorldMatrix(m_matOrthoGraphic * m_matWorld);
	m_pColorEffect->setColor(m_CurrentColor);

	m_Translation -= translation;
	updateTransformationMatrix();

    GL::heBindVao(model.getVertexArraysID());
	glDrawElements(GL_TRIANGLES, model.getNumIndices(), model.getIndexType(), 0);
}
void Happy2DRenderer::fillRectangleInstanced(const vec2& pos, const vec2& size)
{
	ModelMesh* pModel;

	std::stringstream stream;
	stream << "FR.W." << size.x << ".H." << size.y;

	if (m_pModelBuffer->isAssetPresent(stream.str()))
	{
		pModel =  m_pModelBuffer->getAsset(stream.str());
	}
	else
	{
		std::vector<VertexPos2D> vertices;
		vertices.push_back(VertexPos2D(vec2(-size.x/2, size.y/2)));
		vertices.push_back(VertexPos2D(vec2(size.x/2, size.y/2)));
		vertices.push_back(VertexPos2D(vec2(-size.x/2, -size.y/2)));
		vertices.push_back(VertexPos2D(vec2(size.x/2, -size.y/2)));

		std::vector<byte> indices;
		indices.push_back(2); indices.push_back(1); indices.push_back(0);
		indices.push_back(1); indices.push_back(2); indices.push_back(3);

		pModel = NEW ModelMesh(stream.str());
		pModel->init();
		pModel->setVertices(&vertices[0], 4, m_VertexLayoutColor);
		pModel->setIndices(&indices[0], 6, IndexStride_Byte);

		m_pModelBuffer->addAsset(pModel->getName(), pModel);
	}

	vec2 translation(pos.x + size.x/2, pos.y + size.y/2);

	m_Translation += translation;
	updateTransformationMatrix();

	m_pColorEffect->setWorldMatrix(m_matOrthoGraphic * m_matWorld);
	m_pColorEffect->setColor(m_CurrentColor);

	m_Translation -= translation;
	updateTransformationMatrix();
    
    GL::heBindVao(pModel->getVertexArraysID());
	glDrawElements(GL_TRIANGLES, pModel->getNumIndices(), pModel->getIndexType(), 0);
}

void Happy2DRenderer::drawEllipse(const vec2& pos, const vec2& size, uint steps)
{
	ModelMesh model("");

	const float DEG2RAD = 3.14159f/180.0f;

	std::vector<VertexPos2D> vertices;
	std::vector<uint> indices;

	uint stepSize(360 / steps);
	uint index(0);

	for (uint i = 0; i < 360; i += stepSize)
	{
		float degInRad = i * DEG2RAD;
		vertices.push_back(VertexPos2D(vec2(0 + cosf(degInRad) * size.x/2, 0 + sinf(degInRad) * size.y/2)));
		
		indices.push_back(index++);
	}

	model.init();
	model.setVertices(&vertices[0], steps, m_VertexLayoutColor);
	model.setIndices(&indices[0], steps, IndexStride_UInt);

	vec2 translation(pos.x, pos.y);

	m_Translation += translation;
	updateTransformationMatrix();

	m_pColorEffect->setWorldMatrix(m_matOrthoGraphic * m_matWorld);
	m_pColorEffect->setColor(m_CurrentColor);

	m_Translation -= translation;
	updateTransformationMatrix();

    GL::heBindVao(model.getVertexArraysID());
	glDrawElements(GL_LINE_LOOP, model.getNumIndices(), model.getIndexType(), 0);
}
void Happy2DRenderer::drawEllipseInstanced(const vec2& pos, const vec2& size, uint steps)
{
	ModelMesh* pModel;

	std::stringstream stream;
	stream << "HE.W." << size.x << ".H." << size.y;

	if (m_pModelBuffer->isAssetPresent(stream.str()))
	{
		pModel =  m_pModelBuffer->getAsset(stream.str());
	}
	else
	{
		const float DEG2RAD = 3.14159f/180.0f;

		std::vector<VertexPos2D> vertices;
		std::vector<uint> indices;

		uint stepSize(360 / steps);
		uint index(0);

		for (uint i = 0; i < 360; i += stepSize)
		{
			float degInRad = i * DEG2RAD;
			vertices.push_back(VertexPos2D(vec2(0 + cosf(degInRad) * size.x/2, 0 + sinf(degInRad) * size.y/2)));
		
			indices.push_back(index++);
		}

		pModel = NEW ModelMesh(stream.str());
		pModel->init();
		pModel->setVertices(&vertices[0], steps, m_VertexLayoutColor);
		pModel->setIndices(&indices[0], steps, IndexStride_UInt);

		m_pModelBuffer->addAsset(pModel->getName(), pModel);
	}

	vec2 translation(pos.x, pos.y);

	m_Translation += translation;
	updateTransformationMatrix();

	m_pColorEffect->setWorldMatrix(m_matOrthoGraphic * m_matWorld);
	m_pColorEffect->setColor(m_CurrentColor);

	m_Translation -= translation;
	updateTransformationMatrix();

    GL::heBindVao(pModel->getVertexArraysID());
	glDrawElements(GL_LINE_LOOP, pModel->getNumIndices(), pModel->getIndexType(), 0);
}

void Happy2DRenderer::fillEllipse(const vec2& pos, const vec2& size, uint steps)
{
	ModelMesh model("");

	const float DEG2RAD = 3.14159f/180.0f;

	std::vector<VertexPos2D> vertices;
	std::vector<uint> indices;

	uint stepSize(360 / steps);
	uint index(0);

	vertices.push_back(VertexPos2D(vec2(0, 0)));
	indices.push_back(1);

	for (int i = 0; i > -360; i -= stepSize)
	{
		float degInRad = i * DEG2RAD;
		vertices.push_back(VertexPos2D(vec2(0 + cosf(degInRad) * size.x/2, 0 + sinf(degInRad) * size.y/2)));
		
		indices.push_back(steps - index);
		++index;
	}

	model.init();
	model.setVertices(&vertices[0], steps + 1, m_VertexLayoutColor);
	model.setIndices(&indices[0], steps + 1, IndexStride_UInt);

	vec2 translation(pos.x, pos.y);

	m_Translation += translation;
	updateTransformationMatrix();

	m_pColorEffect->setWorldMatrix(m_matOrthoGraphic * m_matWorld);
	m_pColorEffect->setColor(m_CurrentColor);

	m_Translation -= translation;
	updateTransformationMatrix();

    GL::heBindVao(model.getVertexArraysID());
	glDrawElements(GL_TRIANGLE_FAN, model.getNumIndices(), model.getIndexType(), 0);
}
void Happy2DRenderer::fillEllipseInstanced(const vec2& pos, const vec2& size, uint steps)
{
	ModelMesh* pModel;

	std::stringstream stream;
	stream << "FE.Width" << size.x << "Height" << size.y;

	if (m_pModelBuffer->isAssetPresent(stream.str()))
	{
		pModel =  m_pModelBuffer->getAsset(stream.str());
	}
	else
	{
		const float DEG2RAD = 3.14159f/180.0f;

		std::vector<VertexPos2D> vertices;
		std::vector<uint> indices;

		uint stepSize(360 / steps);
		uint index(0);

		vertices.push_back(VertexPos2D(vec2(0, 0)));
		indices.push_back(1);

		for (int i = 0; i < 360; i -= stepSize)
		{
			float degInRad = i * DEG2RAD;
			vertices.push_back(VertexPos2D(vec2(0 + cosf(degInRad) * size.x/2, 0 + sinf(degInRad) * size.y/2)));
		
			indices.push_back(steps - index);
			++index;
		}

		pModel = NEW ModelMesh(stream.str());
		pModel->init();
		pModel->setVertices(&vertices[0], steps + 1, m_VertexLayoutColor);
		pModel->setIndices(&indices[0], steps + 1, IndexStride_UInt);

		m_pModelBuffer->addAsset(pModel->getName(), pModel);
	}

	vec2 translation(pos.x, pos.y);

	m_Translation += translation;
	updateTransformationMatrix();

	m_pColorEffect->setWorldMatrix(m_matOrthoGraphic * m_matWorld);
	m_pColorEffect->setColor(m_CurrentColor);

	m_Translation -= translation;
	updateTransformationMatrix();

    GL::heBindVao(pModel->getVertexArraysID());
	glDrawElements(GL_TRIANGLE_FAN, pModel->getNumIndices(), pModel->getIndexType(), 0);
}

void Happy2DRenderer::drawPolygon(const std::vector<he::vec2> &points, he::uint nrPoints, bool close) const
{
	std::vector<VertexPos2D> vertices;
	std::vector<byte> indices;

	byte i(0);

	std::for_each(points.cbegin(), points.cend(), [&](he::vec2 point)
	{
		vertices.push_back(VertexPos2D(vec2(point.x, point.y)));
		indices.push_back(i++);
	});

    ModelMesh model("");
    model.init();
    model.setVertices(&vertices[0], nrPoints, m_VertexLayoutColor);
    model.setIndices(&indices[0], nrPoints, IndexStride_Byte);
	
	m_pColorEffect->setWorldMatrix(m_matOrthoGraphic);
	m_pColorEffect->setColor(m_CurrentColor);

    GL::heBindVao(model.getVertexArraysID());

	if (close == false)
		glDrawElements(GL_LINE_STRIP, model.getNumIndices(), model.getIndexType(), 0);
	else
		glDrawElements(GL_LINE_LOOP, model.getNumIndices(), model.getIndexType(), 0);
}
void Happy2DRenderer::drawPolygonInstanced(const std::vector<he::vec2> &points, he::uint nrPoints, bool close) const
{
	ModelMesh* pModel;

	std::stringstream stream;
	stream << "P";

	std::for_each(points.cbegin(), points.cend(), [&](he::vec2 p)
	{
		stream << ".X." << p.x << ".Y." << p.y;
	});

	if (m_pModelBuffer->isAssetPresent(stream.str()))
	{
		pModel = m_pModelBuffer->getAsset(stream.str());
	}
	else
	{
		std::vector<VertexPos2D> vertices;
		std::vector<byte> indices;

		byte i(0);

		std::for_each(points.cbegin(), points.cend(), [&](he::vec2 point)
		{
			vertices.push_back(VertexPos2D(vec2(point.x, point.y)));
			indices.push_back(i++);
		});

		pModel = NEW ModelMesh(stream.str());
		pModel->init();
		pModel->setVertices(&vertices[0], nrPoints, m_VertexLayoutColor);
		pModel->setIndices(&indices[0], nrPoints, IndexStride_Byte);


		m_pModelBuffer->addAsset(pModel->getName(), pModel);
	}

	m_pColorEffect->setWorldMatrix(m_matOrthoGraphic * m_matWorld);
	m_pColorEffect->setColor(m_CurrentColor);

    GL::heBindVao(pModel->getVertexArraysID());
	if (close == false)
		glDrawElements(GL_LINE_STRIP, pModel->getNumIndices(), pModel->getIndexType(), 0);
	else
		glDrawElements(GL_LINE_LOOP, pModel->getNumIndices(), pModel->getIndexType(), 0);
}

void Happy2DRenderer::fillPolygon(const std::vector<he::vec2>& /*points*/, he::uint /*nrPoints*/) const
{
	/*
	std::vector<VertexPos2D> vertices;
	std::vector<byte> indices;

	byte i(0);

	std::for_each(points.cbegin(), points.cend(), [&](he::vec2 point)
	{
		vertices.push_back(VertexPos2D(vec2(point.x, point.y)));
		indices.push_back(i++);
	});

	ModelMesh model("");
    model.init();
    model.setVertices(&vertices[0], nrPoints, m_VertexLayoutColor);
    model.setIndices(&indices[0], nrPoints, IndexStride_Byte);

	m_pColorEffect->setWorldMatrix(m_matWorld);
	m_pColorEffect->setColor(m_CurrentColor);

    GL::heBindVao(model.getVertexArraysID());
	glDrawElements(GL_TRIANGLE_STRIP, model.getNumIndices(), model.getIndexType(), 0);
	*/
}
void Happy2DRenderer::fillPolygonInstanced(const std::vector<he::vec2>& /*points*/, he::uint /*nrPoints*/) const
{
	/*
	std::vector<VertexPos2D> vertices;
	std::vector<byte> indices;

	byte i(0);

	std::for_each(points.cbegin(), points.cend(), [&](he::vec2 point)
	{
		vertices.push_back(VertexPos2D(vec2(point.x, point.y)));
		indices.push_back(i++);
	});

	ModelMesh model("");
    model.init();
    model.setVertices(&vertices[0], nrPoints, m_VertexLayoutColor);
    model.setIndices(&indices[0], nrPoints, IndexStride_Byte);

	m_pColorEffect->setWorldMatrix(m_matWorld);
	m_pColorEffect->setColor(m_CurrentColor);

    GL::heBindVao(model.getVertexArraysID());
	glDrawElements(GL_TRIANGLE_STRIP, model.getNumIndices(), model.getIndexType(), 0);
	*/
}

void Happy2DRenderer::drawTexture2D(const Texture2D::pointer& tex2D, const vec2& pos, const vec2& newDimensions, const float alpha, const RectF& regionToDraw)
{
	vec2 tcOffset(0.0f,0.0f);
	vec2 tcScale(1.0f,1.0f);
	vec2 size;

	if (regionToDraw != RectF(vec2(0.0f,0.0f), vec2(0.0f,0.0f)))
	{
		tcScale.x = regionToDraw.width / tex2D->getWidth();
		tcScale.y = regionToDraw.height / tex2D->getHeight();

		tcOffset.x = regionToDraw.x / tex2D->getWidth();
		tcOffset.y = 1 - (regionToDraw.y / tex2D->getHeight()) - tcScale.y;
	}

	if (newDimensions != vec2(0.0f,0.0f))
	{
		size = vec2(abs(newDimensions.x), abs(newDimensions.y));

		if (newDimensions.x < 0) tcScale.x *= -1.0f;
		if (newDimensions.y < 0) tcScale.y *= -1.0f;
	}
	else
	{
		size = vec2(static_cast<float>(tex2D->getWidth()), static_cast<float>(tex2D->getHeight()));
	}

	mat44 world(	mat44::createScale(size.x, size.y, 1.0f) *
						mat44::createTranslation(vec3(pos.x / size.x, pos.y / size.y, 0.0f)));

	vec2 translation(pos.x + size.x/2, pos.y + size.y/2);

	m_Translation += translation;
	m_Scale *= size;
	updateTransformationMatrix();

	m_pColorEffect->end();
	m_pTextureEffect->begin();

	m_pTextureEffect->setWorldMatrix(m_matOrthoGraphic * m_matWorld);

	m_Translation -= translation;
	m_Scale /= size;
	updateTransformationMatrix();

	m_pTextureEffect->setDiffuseMap(tex2D);
	m_pTextureEffect->setAlpha(alpha);
	m_pTextureEffect->setTCOffset(tcOffset);
	m_pTextureEffect->setTCScale(tcScale);

    GL::heBindVao(m_pTextureQuad->getVertexArraysID());
	glDrawElements(GL_TRIANGLES, m_pTextureQuad->getNumIndices(), m_pTextureQuad->getIndexType(), 0);

	m_pTextureEffect->end();
	m_pColorEffect->begin();
}

} } //end namespace