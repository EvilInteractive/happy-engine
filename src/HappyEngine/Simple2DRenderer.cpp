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

#include "Simple2DRenderer.h"
#include "HappyNew.h"
#include "GL/glew.h"
#include "Model.h"
#include "HappyEngine.h"
#include <algorithm>

#include <vector>

namespace happyengine {
namespace graphics {

Simple2DRenderer* Simple2DRenderer::m_pSingleton = nullptr;

Simple2DRenderer::Simple2DRenderer() :	m_pColorEffect(NEW happyengine::graphics::Simple2DEffect()),
										m_pTextureEffect(NEW happyengine::graphics::Simple2DTextureEffect()),
										m_bAntiAliasing(false),
										m_StrokeSize(0.0f),
										m_CurrentColor(1.0f,1.0f,1.0f,1.0f),
										m_ViewPortSize(0.0f,0.0f)
{
    
}

Simple2DRenderer::~Simple2DRenderer()
{
	delete m_pColorEffect;
	delete m_pTextureEffect;
}

Simple2DRenderer* Simple2DRenderer::GetSingleton()
{
	if (m_pSingleton == nullptr) m_pSingleton = NEW happyengine::graphics::Simple2DRenderer();
	return m_pSingleton;
}

math::Vector2 Simple2DRenderer::getNDCPos(const math::Vector2& pos) const
{
	math::Vector2 ndcPos;
	ndcPos.x = (pos.x / m_ViewPortSize.x) * 2.0f - 1.0f;
	ndcPos.y = -((pos.y / m_ViewPortSize.y) * 2.0f - 1.0f);

	return ndcPos;
}
math::Vector2 Simple2DRenderer::getNDCSize(const math::Vector2& size) const
{
	math::Vector2 ndcSize;
	ndcSize.x = (size.x / m_ViewPortSize.x) * 2.0f;
	ndcSize.y = -(size.y / m_ViewPortSize.y) * 2.0f;

	return ndcSize;
}

void Simple2DRenderer::begin()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_ViewPortSize.x = static_cast<float>(GRAPHICS->getScreenWidth());
	m_ViewPortSize.y = static_cast<float>(GRAPHICS->getScreenHeight());

	m_pColorEffect->begin();
}

void Simple2DRenderer::end()
{
	glDisable(GL_BLEND);

	m_pColorEffect->end();

	setStrokeSize(1.0f);
}

void Simple2DRenderer::initialize(bool)
{
	m_VertexLayoutColor.addElement(VertexElement(0, VertexElement::Type_Vector2, VertexElement::Usage_Position, 8, 0, "inPosition"));
	m_VertexLayoutColor.addElement(VertexElement(1, VertexElement::Type_Vector4, VertexElement::Usage_Other, 16, 8, "inColor"));

	m_VertexLayoutTexture.addElement(VertexElement(0, VertexElement::Type_Vector2, VertexElement::Usage_Position, 8, 0, "inPosition"));
	m_VertexLayoutTexture.addElement(VertexElement(1, VertexElement::Type_Vector2, VertexElement::Usage_TextureCoordinate, 8, 8, "inTexCoord"));

	//glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

	m_pColorEffect->load();
	m_pTextureEffect->load();
}

void Simple2DRenderer::setColor(float r, float g, float b, float a)
{
	m_CurrentColor.r(r);
	m_CurrentColor.g(g);
	m_CurrentColor.b(b);
	m_CurrentColor.a(a);
}

void Simple2DRenderer::setTransformationMatrix(const happyengine::math::Matrix& mat)
{
	m_matWorld = mat;
}

void Simple2DRenderer::setAntiAliasing(bool bAA)
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

void Simple2DRenderer::setStrokeSize(float strokeSize)
{
	glLineWidth(strokeSize);

	m_StrokeSize = strokeSize;
}

void Simple2DRenderer::drawText(const math::Vector2&, const std::string&) const
{

}

void Simple2DRenderer::drawRectangle(const math::Vector2& pos, const math::Vector2& size) const
{
	math::Vector2 ndcPos(getNDCPos(pos));
	math::Vector2 ndcSize(getNDCSize(size));
	math::Vector2 ndcStrokeSize(getNDCSize(math::Vector2(m_StrokeSize, m_StrokeSize)));

	std::vector<VertexPosCol2D> vertices;
	vertices.push_back(VertexPosCol2D(math::Vector2(ndcPos.x - ndcStrokeSize.x/2.0f, ndcPos.y), m_CurrentColor.rgba()));
	vertices.push_back(VertexPosCol2D(math::Vector2(ndcPos.x + ndcSize.x + ndcStrokeSize.x/2.0f, ndcPos.y), m_CurrentColor.rgba()));
	vertices.push_back(VertexPosCol2D(math::Vector2(ndcPos.x + ndcSize.x, ndcPos.y - ndcStrokeSize.y/2.0f), m_CurrentColor.rgba()));
	vertices.push_back(VertexPosCol2D(math::Vector2(ndcPos.x + ndcSize.x, ndcPos.y + ndcSize.y + ndcStrokeSize.y/2.0f), m_CurrentColor.rgba()));
	vertices.push_back(VertexPosCol2D(math::Vector2(ndcPos.x + ndcSize.x + ndcStrokeSize.x/2.0f , ndcPos.y - ndcSize.y), m_CurrentColor.rgba()));
	vertices.push_back(VertexPosCol2D(math::Vector2(ndcPos.x - ndcStrokeSize.x/2.0f, ndcPos.y + ndcSize.y), m_CurrentColor.rgba()));
	vertices.push_back(VertexPosCol2D(math::Vector2(ndcPos.x, ndcPos.y + ndcSize.y + ndcStrokeSize.y/2.0f), m_CurrentColor.rgba()));
	vertices.push_back(VertexPosCol2D(math::Vector2(ndcPos.x, ndcPos.y - ndcStrokeSize.y/2.0f), m_CurrentColor.rgba()));
	
    std::vector<byte> indices;
    indices.push_back(0); indices.push_back(1);
	indices.push_back(2); indices.push_back(3);
	indices.push_back(4); indices.push_back(5);
	indices.push_back(6); indices.push_back(7);

	Model model;
    model.init();
    model.setVertices(&vertices[0], 8, m_VertexLayoutColor);
    model.setIndices(&indices[0], 8, IndexStride_Byte);

	m_pColorEffect->setWorldMatrix(m_matWorld);

	glBindVertexArray(model.getVertexArraysID());

	glDrawElements(GL_LINES, model.getNumIndices(), model.getIndexType(), 0);

    glBindVertexArray(0);
}

void Simple2DRenderer::fillRectangle(const math::Vector2& pos, const math::Vector2& size) const
{
	math::Vector2 ndcPos(getNDCPos(pos));
	math::Vector2 ndcSize(getNDCSize(size));

	std::vector<VertexPosCol2D> vertices;
    vertices.push_back(VertexPosCol2D(math::Vector2(ndcPos.x, ndcPos.y), m_CurrentColor.rgba()));
	vertices.push_back(VertexPosCol2D(math::Vector2(ndcPos.x + ndcSize.x, ndcPos.y), m_CurrentColor.rgba()));
	vertices.push_back(VertexPosCol2D(math::Vector2(ndcPos.x, ndcPos.y + ndcSize.y), m_CurrentColor.rgba()));
	vertices.push_back(VertexPosCol2D(math::Vector2(ndcPos.x + ndcSize.x, ndcPos.y + ndcSize.y), m_CurrentColor.rgba()));

    std::vector<byte> indices;
    indices.push_back(0); indices.push_back(3); indices.push_back(1);
    indices.push_back(2); indices.push_back(3); indices.push_back(0);

	Model model;
    model.init();
    model.setVertices(&vertices[0], 4, m_VertexLayoutColor);
    model.setIndices(&indices[0], 6, IndexStride_Byte);

	m_pColorEffect->setWorldMatrix(m_matWorld);

	glBindVertexArray(model.getVertexArraysID());

	glDrawElements(GL_TRIANGLES, model.getNumIndices(), model.getIndexType(), 0);

    glBindVertexArray(0);
}

void Simple2DRenderer::fillEllipse(const math::Vector2& pos, const math::Vector2& size) const
{
	math::Vector2 ndcPos(getNDCPos(pos));
	math::Vector2 ndcSize(getNDCSize(size));

	const float DEG2RAD = 3.14159f/180;

	std::vector<VertexPosCol2D> vertices;
	std::vector<byte> indices;

	for (int i = 0; i < 360; i++)
	{
		float degInRad = i * DEG2RAD;
		vertices.push_back(VertexPosCol2D(math::Vector2(ndcPos.x + cos(degInRad) * ndcSize.x, ndcPos.y + sin(degInRad) * ndcSize.y), m_CurrentColor.rgba()));
		indices.push_back(static_cast<byte>(i));
	}

	Model model;
    model.init();
    model.setVertices(&vertices[0], 360, m_VertexLayoutColor);
    model.setIndices(&indices[0], 360, IndexStride_Byte);

	m_pColorEffect->setWorldMatrix(m_matWorld);

	glBindVertexArray(model.getVertexArraysID());

	glDrawElements(GL_LINE_LOOP, model.getNumIndices(), model.getIndexType(), 0);

    glBindVertexArray(0);
}

void Simple2DRenderer::drawPolygon(const std::vector<happyengine::math::Vector2> &points, happyengine::uint nrPoints, bool close) const
{
	std::vector<VertexPosCol2D> vertices;
	std::vector<byte> indices;

	byte i(0);

	std::for_each(points.cbegin(), points.cend(), [&](happyengine::math::Vector2 point)
	{
		math::Vector2 ndcPoint(getNDCPos(point));
		vertices.push_back(VertexPosCol2D(math::Vector2(ndcPoint.x, ndcPoint.y), m_CurrentColor.rgba()));
		indices.push_back(i++);
	});

	Model model;
    model.init();
    model.setVertices(&vertices[0], nrPoints, m_VertexLayoutColor);
    model.setIndices(&indices[0], nrPoints, IndexStride_Byte);

	m_pColorEffect->setWorldMatrix(m_matWorld);

	glBindVertexArray(model.getVertexArraysID());

	if (close == false)
		glDrawElements(GL_LINE_STRIP, model.getNumIndices(), model.getIndexType(), 0);
	else
		glDrawElements(GL_LINE_LOOP, model.getNumIndices(), model.getIndexType(), 0);

    glBindVertexArray(0);
}

void Simple2DRenderer::drawTexture2D(const math::Vector2& pos, const graphics::Texture2D::pointer& tex2D, const math::Vector2& newDimensions) const
{
	math::Vector2 ndcPos(getNDCPos(pos));
	math::Vector2 ndcSize;
	
	if (newDimensions.x != 0.0f || newDimensions.y != 0.0f)
	{
		ndcSize = getNDCSize(math::Vector2(newDimensions));
	}
	else
	{
		ndcSize = getNDCSize(math::Vector2(
			static_cast<float>(tex2D->getWidth()),
			static_cast<float>(tex2D->getHeight())));
	}

	std::vector<VertexPosTex2D> vertices;
	vertices.push_back(VertexPosTex2D(math::Vector2(ndcPos.x, ndcPos.y), math::Vector2(0,1)));
	vertices.push_back(VertexPosTex2D(math::Vector2(ndcPos.x + ndcSize.x, ndcPos.y), math::Vector2(1,1)));
	vertices.push_back(VertexPosTex2D(math::Vector2(ndcPos.x, ndcPos.y + ndcSize.y), math::Vector2(0,0)));
	vertices.push_back(VertexPosTex2D(math::Vector2(ndcPos.x + ndcSize.x, ndcPos.y + ndcSize.y), math::Vector2(1,0)));

    std::vector<byte> indices;
    indices.push_back(0); indices.push_back(2); indices.push_back(1);
    indices.push_back(1); indices.push_back(2); indices.push_back(3);

	Model model;
    model.init();
    model.setVertices(&vertices[0], 4, m_VertexLayoutTexture);
    model.setIndices(&indices[0], 6, IndexStride_Byte);

	m_pColorEffect->end();
	m_pTextureEffect->begin();
	m_pTextureEffect->setWorldMatrix(m_matWorld);
	m_pTextureEffect->setDiffuseMap(tex2D);

	glBindVertexArray(model.getVertexArraysID());

	glDrawElements(GL_TRIANGLES, model.getNumIndices(), model.getIndexType(), 0);

    glBindVertexArray(0);

	m_pTextureEffect->end();
	m_pColorEffect->begin();
}

} } //end namespace