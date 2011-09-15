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

#include "Happy2DRenderer.h"
#include "HappyNew.h"
#include "GL/glew.h"
#include "HappyEngine.h"
#include "MathFunctions.h"

#include <algorithm>
#include <vector>

namespace happyengine {
namespace graphics {

/* CONSTRUCTOR - DESTRUCTOR */
Happy2DRenderer::Happy2DRenderer() :	m_pColorEffect(NEW happyengine::graphics::Simple2DEffect()),
										m_pTextureEffect(NEW happyengine::graphics::Simple2DTextureEffect()),
										m_bAntiAliasing(false),
										m_StrokeSize(0.0f),
										m_CurrentColor(1.0f,1.0f,1.0f,1.0f),
										m_ViewPortSize(0.0f,0.0f),
										m_FontHAlignment(FontHAlignment_Left),
										m_FontVAlignment(FontVAlignment_Bottom),
										m_pModelBuffer(NEW content::AssetContainerP<graphics::ModelMesh>()),
										m_TickCounter(0)
{
    
}

Happy2DRenderer::~Happy2DRenderer()
{
	delete m_pColorEffect;
	delete m_pTextureEffect;
	delete m_pModelBuffer;
}

math::Vector2 Happy2DRenderer::getNDCPos(const math::Vector2& pos) const
{
	math::Vector2 ndcPos;
	ndcPos.x = (pos.x / m_ViewPortSize.x) * 2.0f - 1.0f;
	ndcPos.y = -((pos.y / m_ViewPortSize.y) * 2.0f - 1.0f);

	return ndcPos;
}
math::Vector2 Happy2DRenderer::getNDCSize(const math::Vector2& size) const
{
	math::Vector2 ndcSize;
	ndcSize.x = (size.x / m_ViewPortSize.x) * 2.0f;
	ndcSize.y = -(size.y / m_ViewPortSize.y) * 2.0f;

	return ndcSize;
}
void Happy2DRenderer::cleanUpModelBuffer()
{
	using namespace std;

	vector<string> keysDeleted; 

	for_each(m_ModelBufferIndex.begin(), m_ModelBufferIndex.end(), [&](pair<string, uint> bufferIndex)
	{
		if (bufferIndex.second > 10)
		{
			m_pModelBuffer->RemoveAsset(bufferIndex.first);
			keysDeleted.push_back(bufferIndex.first);
		}
		else
		{
			++bufferIndex.second;
		}
	});

	for_each(keysDeleted.begin(), keysDeleted.end(), [&](string key)
	{
		m_ModelBufferIndex.erase(key);
	});
}

void Happy2DRenderer::updateTransformationMatrix()
{
	m_matWorld = math::Matrix::createScale(m_Scale.x, m_Scale.y, 1.0f) *
				 math::Matrix::createRotation(math::Vector3(0.0f,0.0f,-1.0f), m_Rotation) *
				 math::Matrix::createTranslation(math::Vector3(m_Translation.x, m_Translation.y, 0.0f));
}

/* GENERAL */
void Happy2DRenderer::begin()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE); //disable enable writing to depth buffer

	m_ViewPortSize.x = static_cast<float>(GRAPHICS->getScreenWidth());
	m_ViewPortSize.y = static_cast<float>(GRAPHICS->getScreenHeight());

	m_pColorEffect->begin();
}

void Happy2DRenderer::end()
{
	m_pColorEffect->end();

	glDisable(GL_BLEND);
    glDepthMask(GL_TRUE); //disable enable writing to depth buffer

	setStrokeSize();

	//cleanUpModelBuffer();
}

void Happy2DRenderer::initialize()
{
	m_VertexLayoutColor.addElement(VertexElement(0, VertexElement::Type_Vector2, VertexElement::Usage_Position, 8, 0, "inPosition"));
	m_VertexLayoutColor.addElement(VertexElement(1, VertexElement::Type_Vector4, VertexElement::Usage_Other, 16, 8, "inColor"));

	m_VertexLayoutTexture.addElement(VertexElement(0, VertexElement::Type_Vector2, VertexElement::Usage_Position, 8, 0, "inPosition"));
	m_VertexLayoutTexture.addElement(VertexElement(1, VertexElement::Type_Vector2, VertexElement::Usage_TextureCoordinate, 8, 8, "inTexCoord"));

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

	m_pColorEffect->load();
	m_pTextureEffect->load();
}

/* SETTERS */
void Happy2DRenderer::setColor(float r, float g, float b, float a)
{
	m_CurrentColor.r(r);
	m_CurrentColor.g(g);
	m_CurrentColor.b(b);
	m_CurrentColor.a(a);
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

void Happy2DRenderer::setStrokeSize(float strokeSize)
{
	glLineWidth(strokeSize);

	m_StrokeSize = strokeSize;
}

void Happy2DRenderer::setFontHorizontalAlignment(FontHAlignment horizontalAlignment)
{
	m_FontHAlignment = horizontalAlignment;
}

void Happy2DRenderer::setFontVerticalAlignment(FontVAlignment verticalAlignment)
{
	m_FontVAlignment = verticalAlignment;
}

void Happy2DRenderer::setTranslation(const math::Vector2& translation)
{
	m_Translation = translation;
	updateTransformationMatrix();
}

void Happy2DRenderer::setRotation(const float radians)
{
	m_Rotation = radians;
	updateTransformationMatrix();
}

void Happy2DRenderer::setScale(const math::Vector2& scale)
{
	m_Scale = scale;
	updateTransformationMatrix();
}

void Happy2DRenderer::resetTransformation()
{
	m_matWorld = math::Matrix::Identity;

	m_Translation = math::Vector2(0.0f,0.0f);
	m_Rotation = 0.0f;
	m_Scale = math::Vector2(0.0f,0.0f);
}

/* DRAW METHODS */
void Happy2DRenderer::drawText(const math::Vector2& pos, const std::string& text, const happyengine::graphics::Font::pointer& font) const
{
	Texture2D::pointer texFont(font->createTextureText(text, m_CurrentColor, m_FontHAlignment, m_FontVAlignment, m_bAntiAliasing));

	drawTexture2D(pos, texFont, math::Vector2(static_cast<float>(texFont->getWidth()), -static_cast<float>(texFont->getHeight())));
}

void Happy2DRenderer::drawRectangle(const math::Vector2& pos, const math::Vector2& size)
{
	ModelMesh* pModel;

	std::stringstream stream;
	stream << "HR.Width" << size.x << "Height" << size.y;

	if (m_pModelBuffer->IsAssetPresent(stream.str()))
	{
		pModel =  m_pModelBuffer->GetAsset(stream.str());
		//--m_ModelBufferIndex[stream.str()];
	}
	else
	{
		math::Vector2 ndcSize(getNDCSize(size));
		math::Vector2 ndcStrokeSize(getNDCSize(math::Vector2(m_StrokeSize, m_StrokeSize)));

		std::vector<VertexPosCol2D> vertices;
		vertices.push_back(VertexPosCol2D(math::Vector2(0 + ndcSize.x, 0), m_CurrentColor.rgba()));
		vertices.push_back(VertexPosCol2D(math::Vector2(0, 0), m_CurrentColor.rgba()));
		vertices.push_back(VertexPosCol2D(math::Vector2(0 + ndcSize.x - ndcStrokeSize.x/2.0f, 0 + ndcStrokeSize.y/2.0f), m_CurrentColor.rgba()));
		vertices.push_back(VertexPosCol2D(math::Vector2(0 + ndcSize.x - ndcStrokeSize.x/2.0f, 0+ ndcSize.y - ndcStrokeSize.y/2.0f), m_CurrentColor.rgba()));
		vertices.push_back(VertexPosCol2D(math::Vector2(0 + ndcSize.x, 0 + ndcSize.y), m_CurrentColor.rgba()));
		vertices.push_back(VertexPosCol2D(math::Vector2(0, 0 + ndcSize.y), m_CurrentColor.rgba()));
		vertices.push_back(VertexPosCol2D(math::Vector2(0 + ndcStrokeSize.x/2.0f, 0 + ndcStrokeSize.y/2.0f), m_CurrentColor.rgba()));
		vertices.push_back(VertexPosCol2D(math::Vector2(0 + ndcStrokeSize.x/2.0f, 0 + ndcSize.y - ndcStrokeSize.y/2.0f), m_CurrentColor.rgba()));
	
		std::vector<byte> indices;
		indices.push_back(0); indices.push_back(1);
		indices.push_back(2); indices.push_back(3);
		indices.push_back(4); indices.push_back(5);
		indices.push_back(6); indices.push_back(7);

		pModel = NEW ModelMesh(stream.str());
		pModel->init();
		pModel->setVertices(&vertices[0], 8, m_VertexLayoutColor);
		pModel->setIndices(&indices[0], 8, IndexStride_Byte);

		m_pModelBuffer->AddAsset(stream.str(), pModel);
		//m_ModelBufferIndex[stream.str()] = 0;
	}

	math::Vector2 ndcPos(getNDCPos(pos));
	math::Matrix mat = m_matWorld * math::Matrix::createTranslation(math::Vector3(ndcPos.x, ndcPos.y, 0.0f));

	m_pColorEffect->setWorldMatrix(mat);

	glBindVertexArray(pModel->getVertexArraysID());

	glDrawElements(GL_LINES, pModel->getNumIndices(), pModel->getIndexType(), 0);

    glBindVertexArray(0);
}

void Happy2DRenderer::fillRectangle(const math::Vector2& pos, const math::Vector2& size)
{
	ModelMesh* pModel;

	std::stringstream stream;
	stream << "FR.Width" << size.x << "Height" << size.y;

	if (m_pModelBuffer->IsAssetPresent(stream.str()))
	{
		pModel =  m_pModelBuffer->GetAsset(stream.str());
		//--m_ModelBufferIndex[stream.str()];
	}
	else
	{
		math::Vector2 ndcSize(getNDCSize(size));

		std::vector<VertexPosCol2D> vertices;
		vertices.push_back(VertexPosCol2D(math::Vector2(0, 0), m_CurrentColor.rgba()));
		vertices.push_back(VertexPosCol2D(math::Vector2(0 + ndcSize.x, 0), m_CurrentColor.rgba()));
		vertices.push_back(VertexPosCol2D(math::Vector2(0, 0 + ndcSize.y), m_CurrentColor.rgba()));
		vertices.push_back(VertexPosCol2D(math::Vector2(0 + ndcSize.x, 0 + ndcSize.y), m_CurrentColor.rgba()));

		std::vector<byte> indices;
		indices.push_back(0); indices.push_back(3); indices.push_back(1);
		indices.push_back(2); indices.push_back(3); indices.push_back(0);

		pModel = NEW ModelMesh(stream.str());
		pModel->init();
		pModel->setVertices(&vertices[0], 4, m_VertexLayoutColor);
		pModel->setIndices(&indices[0], 6, IndexStride_Byte);

		m_pModelBuffer->AddAsset(stream.str(), pModel);
		//m_ModelBufferIndex[stream.str()] = 0;
	}

	math::Vector2 ndcPos(getNDCPos(pos));
	math::Matrix mat = m_matWorld * math::Matrix::createTranslation(math::Vector3(ndcPos.x, ndcPos.y, 0.0f));

	m_pColorEffect->setWorldMatrix(mat);

	glBindVertexArray(pModel->getVertexArraysID());

	glDrawElements(GL_TRIANGLES, pModel->getNumIndices(), pModel->getIndexType(), 0);

    glBindVertexArray(0);
}

void Happy2DRenderer::drawEllipse(const math::Vector2& pos, const math::Vector2& size, uint steps)
{
	ModelMesh* pModel;

	std::stringstream stream;
	stream << "HE.Width" << size.x << "Height" << size.y;

	if (m_pModelBuffer->IsAssetPresent(stream.str()))
	{
		pModel =  m_pModelBuffer->GetAsset(stream.str());
		//--m_ModelBufferIndex[stream.str()];
	}
	else
	{
		math::Vector2 ndcSize(getNDCSize(size/2));

		const float DEG2RAD = 3.14159f/180.0f;

		std::vector<VertexPosCol2D> vertices;
		std::vector<uint> indices;

		uint stepSize(360 / steps);
		uint index(0);

		for (uint i = 0; i < 360; i += stepSize)
		{
			float degInRad = i * DEG2RAD;
			vertices.push_back(VertexPosCol2D(math::Vector2(0 + cosf(degInRad) * ndcSize.x, 0 + sinf(degInRad) * ndcSize.y), m_CurrentColor.rgba()));
		
			indices.push_back(index++);
		}

		pModel = NEW ModelMesh(stream.str());
		pModel->init();
		pModel->setVertices(&vertices[0], steps, m_VertexLayoutColor);
		pModel->setIndices(&indices[0], steps, IndexStride_UInt);

		m_pModelBuffer->AddAsset(stream.str(), pModel);
		//m_ModelBufferIndex[stream.str()] = 0;
	}

	math::Vector2 ndcPos(getNDCPos(pos));
	math::Matrix mat = m_matWorld * math::Matrix::createTranslation(math::Vector3(ndcPos.x, ndcPos.y, 0.0f));

	m_pColorEffect->setWorldMatrix(mat);

	glBindVertexArray(pModel->getVertexArraysID());

	glDrawElements(GL_LINE_LOOP, pModel->getNumIndices(), pModel->getIndexType(), 0);

    glBindVertexArray(0);
}

void Happy2DRenderer::fillEllipse(const math::Vector2& pos, const math::Vector2& size, uint steps)
{
	ModelMesh* pModel;

	std::stringstream stream;
	stream << "FE.Width" << size.x << "Height" << size.y;

	if (m_pModelBuffer->IsAssetPresent(stream.str()))
	{
		pModel =  m_pModelBuffer->GetAsset(stream.str());
		//--m_ModelBufferIndex[stream.str()];
	}
	else
	{
		math::Vector2 ndcSize(getNDCSize(size/2));

		const float DEG2RAD = 3.14159f/180.0f;

		std::vector<VertexPosCol2D> vertices;
		std::vector<uint> indices;

		uint stepSize(360 / steps);
		uint index(0);

		vertices.push_back(VertexPosCol2D(math::Vector2(0, 0), m_CurrentColor.rgba()));
		indices.push_back(1);

		for (uint i = 0; i < 360; i += stepSize)
		{
			float degInRad = i * DEG2RAD;
			vertices.push_back(VertexPosCol2D(math::Vector2(0 + cosf(degInRad) * ndcSize.x, 0 + sinf(degInRad) * ndcSize.y), m_CurrentColor.rgba()));
		
			indices.push_back(steps - index);
			++index;
		}

		pModel = NEW ModelMesh(stream.str());
		pModel->init();
		pModel->setVertices(&vertices[0], steps + 1, m_VertexLayoutColor);
		pModel->setIndices(&indices[0], steps + 1, IndexStride_UInt);

		m_pModelBuffer->AddAsset(stream.str(), pModel);
		//m_ModelBufferIndex[stream.str()] = 0;
	}

	math::Vector2 ndcPos(getNDCPos(pos));
	math::Matrix mat = m_matWorld * math::Matrix::createTranslation(math::Vector3(ndcPos.x, ndcPos.y, 0.0f));

	m_pColorEffect->setWorldMatrix(mat);

	glBindVertexArray(pModel->getVertexArraysID());

	glDrawElements(GL_TRIANGLE_FAN, pModel->getNumIndices(), pModel->getIndexType(), 0);

    glBindVertexArray(0);
}

void Happy2DRenderer::drawPolygon(const std::vector<happyengine::math::Vector2> &points, happyengine::uint nrPoints, bool close)
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

    ModelMesh model("");
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

void Happy2DRenderer::fillPolygon(const std::vector<happyengine::math::Vector2>& points, happyengine::uint nrPoints)
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

	ModelMesh model("");
    model.init();
    model.setVertices(&vertices[0], nrPoints, m_VertexLayoutColor);
    model.setIndices(&indices[0], nrPoints, IndexStride_Byte);

	m_pColorEffect->setWorldMatrix(m_matWorld);

	glBindVertexArray(model.getVertexArraysID());
	
	glDrawElements(GL_TRIANGLE_STRIP, model.getNumIndices(), model.getIndexType(), 0);

    glBindVertexArray(0);
}

void Happy2DRenderer::drawTexture2D(const math::Vector2& pos, const Texture2D::pointer& tex2D, const math::Vector2& newDimensions, const float alpha) const
{
	math::Vector2 ndcPos(getNDCPos(pos));
	math::Vector2 ndcSize;
	
	if (newDimensions.x != 0.0f || newDimensions.y != 0.0f)
	{
		ndcSize = getNDCSize(math::abs(newDimensions));
	}
	else
	{
		ndcSize = getNDCSize(math::Vector2(
			static_cast<float>(tex2D->getWidth()),
			static_cast<float>(tex2D->getHeight())));
	}

	std::vector<VertexPosTex2D> vertices;

	vertices.push_back(
		VertexPosTex2D(math::Vector2(ndcPos.x, ndcPos.y),
		math::Vector2(newDimensions.x < 0 ? 1.0f : 0.0f, newDimensions.y < 0 ? 0.0f : 1.0f)));

	vertices.push_back(
		VertexPosTex2D(math::Vector2(ndcPos.x + ndcSize.x, ndcPos.y),
		math::Vector2(newDimensions.x < 0 ? 0.0f : 1.0f, newDimensions.y < 0 ? 0.0f : 1.0f)));

	vertices.push_back(
		VertexPosTex2D(math::Vector2(ndcPos.x, ndcPos.y + ndcSize.y),
		math::Vector2(newDimensions.x < 0 ? 1.0f : 0.0f, newDimensions.y < 0 ? 1.0f : 0.0f)));

	vertices.push_back(
		VertexPosTex2D(math::Vector2(ndcPos.x + ndcSize.x, ndcPos.y + ndcSize.y),
		math::Vector2(newDimensions.x < 0 ? 0.0f : 1.0f, newDimensions.y < 0 ? 1.0f : 0.0f)));

    std::vector<byte> indices;
    indices.push_back(0); indices.push_back(2); indices.push_back(1);
    indices.push_back(1); indices.push_back(2); indices.push_back(3);

	ModelMesh model("");
    model.init();
    model.setVertices(&vertices[0], 4, m_VertexLayoutTexture);
    model.setIndices(&indices[0], 6, IndexStride_Byte);

	m_pColorEffect->end();
	m_pTextureEffect->begin();
	m_pTextureEffect->setWorldMatrix(m_matWorld);
	m_pTextureEffect->setDiffuseMap(tex2D);
	m_pTextureEffect->setAlpha(alpha);

	glBindVertexArray(model.getVertexArraysID());

	glDrawElements(GL_TRIANGLES, model.getNumIndices(), model.getIndexType(), 0);

    glBindVertexArray(0);

	m_pTextureEffect->end();
	m_pColorEffect->begin();
}

} } //end namespace