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

#include "StdAfx.h" 

#include "SimpleForward3DRenderer.h"
#include "OpenGL.h"
#include "HeAssert.h"
#include "HappyNew.h"
#include "HappyEngine.h"
#include "GraphicsEngine.h"
#include "Happy2DRenderer.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESCTRUCTOR */
SimpleForward3DRenderer::SimpleForward3DRenderer() :	m_pColorEffect(NEW SplineColorEffect()),
														m_RenderFboID(0),
														m_pRenderTexture(NEW Texture2D())
{
}

SimpleForward3DRenderer::~SimpleForward3DRenderer()
{
	delete m_pColorEffect;
}

/* GENERAL */
void SimpleForward3DRenderer::init()
{
	m_pColorEffect->load();

	int width = GRAPHICS->getViewport().width, 
		height = GRAPHICS->getViewport().height;

	uint renderTexture;
	glGenTextures(1, &renderTexture);

	GL::heBindTexture2D(0, renderTexture);
	/*glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	m_pRenderTexture->init(renderTexture, width, height, GL_RGBA8);

	glGenFramebuffers(1, &m_RenderFboID);
	GL::heBindFbo(m_RenderFboID);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GRAPHICS->getDeferredRenderer()->getDepthTexture()->getID(), 0);
}

void SimpleForward3DRenderer::begin(const Camera* pCamera)
{
	GL::heBlendEnabled(true);
	GL::heBlendFunc(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);
	GL::heSetDepthWrite(true);	

	GL::heBindFbo(m_RenderFboID);
	GL::heClearColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
	glClear(GL_COLOR_BUFFER_BIT);

	m_pColorEffect->begin();

	m_ViewProjection = pCamera->getViewProjection();

	m_pColorEffect->setViewProjection(m_ViewProjection);
}

void SimpleForward3DRenderer::end()
{
	GL::heBindFbo(0);

	HE2D->begin();
	HE2D->drawTexture2D(m_pRenderTexture, vec2(0.0f,0.0f), vec2(-static_cast<float>(m_pRenderTexture->getWidth()), static_cast<float>(m_pRenderTexture->getHeight())));
	HE2D->end();
}

/* DRAW METHODS */
void SimpleForward3DRenderer::drawSpline(const ModelMesh::pointer& spline, const mat44& world, const Color& color) const
{
	m_pColorEffect->setWorld(world);
	m_pColorEffect->setColor(color);

	GL::heBindVao(spline->getVertexArraysID());
	glDrawElements(GL_LINES, spline->getNumIndices(), spline->getIndexType(), 0);
}

} } //end namespace