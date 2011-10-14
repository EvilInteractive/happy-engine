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
//Created: 30/08/2011
#include "StdAfx.h" 

#include "FPSGraph.h"
#include "HappyEngine.h"
#include "vec2.h"
#include "ContentManager.h"
#include "Console.h"
#include "Happy2DRenderer.h"
#include "GraphicsEngine.h"

#include <algorithm>
#include <string>

namespace he {
namespace tools {

/* CONSTRUCTOR - DESTRUCTOR */
FPSGraph::FPSGraph() :	m_GameTime(0.0f),
						m_TBase(0.0f),
						m_CurrentDTime(0.0f),
						m_CurrentFPS(0),
						m_Interval(0.5f),
                        m_pFont(CONTENT->loadFont("Ubuntu-Regular.ttf", 10)),
						m_FPSGraphState(2)
{
	CONSOLE->registerValue<int>(&m_FPSGraphState, "fps_graph");
}


FPSGraph::~FPSGraph()
{
}

/* GENERAL */
void FPSGraph::tick(float dTime, float interval)
{
	m_GameTime += dTime;
	m_Interval = interval;

	if( (m_GameTime - m_TBase) >= m_Interval)
	{
		m_TBase  += m_Interval;

		uint fps((uint)(1 / dTime));

		m_CurrentFPS = fps;
		m_CurrentDTime = dTime;

		if (fps > 99)
			fps = 99;

		m_FpsHistory.push_back(fps);
	}
}

void FPSGraph::draw()
{
	if (m_GameTime > m_Interval)
	{
		switch (m_FPSGraphState)
		{
			case 0:
			{
				break;
			}
			case 1:
			{
				drawTextOnly();
				break;
			}
			case 2:
			{
				drawFull();
				break;
			}
		}
	}
}

void FPSGraph::drawTextOnly()
{
	HE2D->setAntiAliasing(false);
	HE2D->resetTransformation();

	HE2D->setColor(1.0f,1.0f,1.0f);

	std::stringstream stream;
	stream << "FPS: " << m_CurrentFPS << " (" << getAverageFPS() << ")";
	HE2D->drawString(stream.str(), m_pFont, vec2(GRAPHICS->getViewport().width - 105.0f, 5));

	stream.str("");
	stream << "DTime: " << (m_CurrentDTime * 1000.0f) << " ms";
	HE2D->drawString(stream.str(), m_pFont, vec2(GRAPHICS->getViewport().width - 105.0f, 18));
}

void FPSGraph::drawFull()
{
	//using namespace gfx;

	HE2D->setAntiAliasing(false);
	HE2D->resetTransformation();

	HE2D->setColor(1.0f,1.0f,1.0f,0.5f);
	HE2D->fillRectangleInstanced(vec2(GRAPHICS->getViewport().width - 105.0f, 5.0f), vec2(100, 40));
	HE2D->setColor(0.0f,0.0f,0.0f,0.5f);
	HE2D->setStrokeSize();
	HE2D->drawRectangleInstanced(vec2(GRAPHICS->getViewport().width - 106.0f, 4.0f), vec2(102, 42));

	uint avFPS(getAverageFPS());

	if (avFPS > 80)
		avFPS = 80;

	HE2D->setColor(0.0f,0.0f,1.0f,0.4f);
	HE2D->drawLine(vec2(static_cast<float>(GRAPHICS->getViewport().width - 105.0f), static_cast<float>(45 - (avFPS / 2))),
					vec2(static_cast<float>(GRAPHICS->getViewport().width - 5.0f), static_cast<float>(45 - (avFPS / 2))));

	std::vector<vec2> points;

	uint i(0);
		
	if (static_cast<uint>(m_GameTime) / m_Interval > 50)
	{
		i = static_cast<uint>((m_GameTime / m_Interval) - 51);
	}

	uint j(0);
		
	if (static_cast<uint>(m_GameTime) / m_Interval > 50)
	{
		j = static_cast<uint>(m_GameTime / m_Interval);
	}
	else
	{
		j = m_FpsHistory.size();
	}

	uint k(0);

	for (; i < j ; ++i)
	{
		uint currentFPS(m_FpsHistory[i]);

		if (currentFPS > 80)
			currentFPS = 80;

		points.push_back(vec2(static_cast<float>(GRAPHICS->getViewport().width - 5 - (k * 2)), static_cast<float>(46 - (currentFPS / 2))));

		++k;
	}

	HE2D->setColor(1.0f,0.0f,0.0f,0.8f);
		
	HE2D->resetTransformation();
	HE2D->setStrokeSize();

	if (points.size() > 0) 
		HE2D->drawPolygon(points, points.size());

	points.clear();

	if (static_cast<uint>(m_GameTime) / m_Interval > 50)
	{
		i = static_cast<uint>((m_GameTime / m_Interval) - 51);
	}
	else
	{
		i = 0;
	}

	k = 0;

	for (; i < j ; ++i)
	{
		uint currentDTime(static_cast<uint>((1.0f / m_FpsHistory[i]) * 1000.0f));

		if (currentDTime > 80)
			currentDTime = 80;

		points.push_back(vec2(static_cast<float>(GRAPHICS->getViewport().width - 5 - (k * 2)), static_cast<float>(46 - (currentDTime / 2))));

		++k;
	}

	HE2D->setColor(1.0f,1.0f,0.0f,0.8f);

	if (points.size() > 0) 
		HE2D->drawPolygon(points, points.size());

	HE2D->setColor(1.0f,1.0f,1.0f);

	std::stringstream stream;
	stream << "FPS: " << m_CurrentFPS << " (" << getAverageFPS() << ")";
	HE2D->drawString(stream.str(), m_pFont, vec2(GRAPHICS->getViewport().width - 105.0f, 45));

	stream.str("");
	stream << "DTime: " << (m_CurrentDTime * 1000.0f) << " ms";
	HE2D->drawString(stream.str(), m_pFont, vec2(GRAPHICS->getViewport().width - 105.0f, 58));
}

/* GETTERS */
uint FPSGraph::getMaxFPS() const
{
	uint maxFPS(0);
	std::for_each(m_FpsHistory.cbegin(), m_FpsHistory.cend(), [&](uint FPS)
	{
		if (FPS > maxFPS)
			maxFPS = FPS;
	});

	return maxFPS;
}

uint FPSGraph::getMinFPS() const
{
	uint minFPS(9999);
	std::for_each(m_FpsHistory.cbegin(), m_FpsHistory.cend(), [&](uint FPS)
	{
		if (FPS < minFPS)
			minFPS = FPS;
	});

	return minFPS;
}

uint FPSGraph::getAverageFPS() const
{
	uint i(0);
		
	if (static_cast<uint>(m_GameTime) > 60)
	{
		i = static_cast<uint>((m_GameTime / m_Interval) - (60 / m_Interval));
	}

	uint j(0);
		
	if (static_cast<uint>(m_GameTime) > 60)
	{
		j = static_cast<uint>(m_GameTime / m_Interval);
	}
	else
	{
		j = m_FpsHistory.size();
	}

	uint avFPS(0);

	for (; i < j ; ++i)
	{
		avFPS += m_FpsHistory[i];
	}

	if (static_cast<uint>(m_GameTime) > 60)
	{
		avFPS /= static_cast<uint>(60 / m_Interval);
	}
	else
	{
		avFPS /= m_FpsHistory.size();
	}

	return avFPS;
}

} } //end namespace