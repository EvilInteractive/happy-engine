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

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESCTRUCTOR */
SimpleForward3DRenderer::SimpleForward3DRenderer() :	m_pColorEffect(NEW SplineColorEffect())
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
}

void SimpleForward3DRenderer::begin(const Camera* pCamera)
{
	GL::heBlendEnabled(false);
	GL::heSetDepthWrite(true);

	m_pColorEffect->begin();

	m_ViewProjection = pCamera->getViewProjection();

	m_pColorEffect->setViewProjection(m_ViewProjection);
}

void SimpleForward3DRenderer::end()
{
	GL::heBlendEnabled(false);
	GL::heBindVao(0);
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