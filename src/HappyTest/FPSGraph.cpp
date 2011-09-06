//HappyTest Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyTest.
//
//    HappyTest is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyTest is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyTest.  If not, see <http://www.gnu.org/licenses/>.
//
//Author:  Sebastiaan Sprengers
//Created: 30/08/2011

#include "FPSGraph.h"
#include "Simple2DRenderer.h"
#include "HappyEngine.h"
#include <algorithm>
#include "Vector2.h"

namespace happytest {

FPSGraph::FPSGraph() :	m_GameTime(0.0f),
						m_TBase(0.0f)
{
}


FPSGraph::~FPSGraph()
{
}

void FPSGraph::show(float dTime, float interval)
{
	using namespace happyengine;
	using namespace graphics;
	using namespace math;
	
	m_GameTime += dTime;

	if( (m_GameTime - m_TBase) >= interval )
	{
		m_TBase  += interval;

		uint fps((uint)(1 / dTime));

		m_FpsHistory.push_back(fps);
	}

	if (m_GameTime > interval)
	{
		if (m_FpsHistory.size() > 51)
			m_FpsHistory.erase(m_FpsHistory.begin());

		HE2D->setColor(1.0f,1.0f,1.0f,0.5f);
		HE2D->fillRectangle(Vector2(GRAPHICS->getViewport().width - 105.0f, 5.0f), Vector2(100, 40));
		HE2D->setColor(0.0f,0.0f,0.0f,0.5f);
		HE2D->setStrokeSize();
		HE2D->drawRectangle(Vector2(GRAPHICS->getViewport().width - 105.0f, 5.0f), Vector2(100, 40));

		std::vector<math::Vector2> points;
		uint i(0);

		std::for_each(m_FpsHistory.cbegin(), m_FpsHistory.cend(), [&](uint currentFps)
		{
			points.push_back(happyengine::math::Vector2((float)(GRAPHICS->getViewport().width - 5 - (i * 2)), (float)(45 - (currentFps / 2))));
			++i;
		});

		HE2D->setColor(1.0f,0.0f,0.0f,0.8f);
		HE2D->setStrokeSize();
		HE2D->drawPolygon(points, points.size());
	}
}

} //end namespace