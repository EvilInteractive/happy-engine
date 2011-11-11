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
//Created: 11/11/2011

#include "StdAfx.h" 

#include "Picker.h"
#include "HappyNew.h"

#include "GraphicsEngine.h"
#include "OpenGL.h"
#include "HappyEngine.h"
#include "DrawManager.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESTRUCTOR */
Picker::Picker() :	m_pPickEffect(NEW PickEffect()),
					m_RenderFboID(0),
					m_bInitialized(false),
					m_pIDTexture(NEW Texture2D())
{
}

Picker::~Picker()
{
	delete m_pPickEffect;
}

/* GENERAL */
void Picker::initialize()
{
	m_pPickEffect->load();

	int width = GRAPHICS->getViewport().width, 
		height = GRAPHICS->getViewport().height;

	uint renderTexture;
	glGenTextures(1, &renderTexture);

	GL::heBindTexture2D(0, renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, width, height, 0, GL_RED, GL_UNSIGNED_INT, 0);
	m_pIDTexture->init(renderTexture, width, height, GL_R32I);

	glGenFramebuffers(1, &m_RenderFboID);
	GL::heBindFbo(m_RenderFboID);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);

	m_bInitialized = true;
}
uint Picker::pick(const vec2& screenPoint, const Camera* pCamera)
{
	ASSERT(m_bInitialized, "Initialize picker before using!");

	GL::heBlendEnabled(false);
	GL::heSetDepthWrite(true);

	GL::heBindFbo(m_RenderFboID);
	GL::heClearColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
	glClear(GL_COLOR_BUFFER_BIT);

	m_pPickEffect->begin();
	m_pPickEffect->setViewProjection(pCamera->getViewProjection());

	const std::vector<const IDrawable*>& drawList = GRAPHICS->getDrawList();
	std::vector<uint> ID1;

	// cull drawlist
	std::vector<DrawManager::DrawElement> culledDrawList;
	culledDrawList.reserve(drawList.size());

	uint i(0);
	std::for_each(drawList.cbegin(), drawList.cend(), [&](const IDrawable* pDrawable)
	{
		if (pDrawable->isVisible() && pDrawable->getModel()->isLoaded())
		{
			shapes::Sphere bS(pDrawable->getModel()->getBoundingSphere().getPosition() + pDrawable->getWorldMatrix().getTranslation(), 
				pDrawable->getModel()->getBoundingSphere().getRadius() * pDrawable->getWorldMatrix()(0, 0)); // HACK: only uniform scales

			if (DrawManager::viewClip(pCamera, bS) == false)
			{
				DrawManager::DrawElement e;
				e.pDrawable = pDrawable;
				e.sorter = lengthSqr(pCamera->getPosition() - e.pDrawable->getWorldMatrix().getTranslation());
				culledDrawList.push_back(e);

				ID1.push_back(i);
			}
		}

		++i;
	});

	// create list with items to be picked
	std::vector<const IDrawable*> pickList;
	std::vector<uint> ID2;

	i = 0;
	std::sort(culledDrawList.begin(), culledDrawList.end());
	std::for_each(culledDrawList.cbegin(), culledDrawList.cend(), [&](const DrawManager::DrawElement& e)
	{
		if (e.pDrawable->isPickable())
		{
			pickList.push_back(e.pDrawable);
			ID2.push_back(ID1[i]);
		}

		++i;
	});

	i = 1;
	std::for_each(pickList.cbegin(), pickList.cend(), [&](const IDrawable* pDrawable)
	{
		m_pPickEffect->setWorld(pDrawable->getWorldMatrix());
		m_pPickEffect->setID(i);

		GL::heBindVao(pDrawable->getModel()->getVertexArraysID());
		glDrawElements(GL_TRIANGLES, pDrawable->getModel()->getNumIndices(), pDrawable->getModel()->getIndexType(), 0);
	});

	uint id(0);
	glReadPixels(	static_cast<int>(screenPoint.x),
					static_cast<int>(screenPoint.y),
					1,1,GL_R32I, GL_UNSIGNED_INT, &id);

	if (id == 0)
		return UINT_MAX;
	else
		return ID2[(id - 1)];
}

} } //end namespace