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

#include "FPSGraph.h"
#include "HappyEngine.h"
#include "Vector2.h"
#include "FontLoader.h"

#include <algorithm>
#include <string>

namespace happyengine {
namespace tools {

/* CONSTRUCTOR - DESTRUCTOR */
FPSGraph::FPSGraph() :	m_GameTime(0.0f),
						m_TBase(0.0f),
						m_CurrentDTime(0.0f),
						m_CurrentFPS(0),
						m_Interval(0.5f),
                        m_pFont(CONTENT->loadFont("../data/fonts/Ubuntu-Regular.ttf", 10))
{
}


FPSGraph::~FPSGraph()
{
}

/* GENERAL */
void FPSGraph::tick(float dTime, float interval)
{
	using namespace happyengine;
	using namespace graphics;
	using namespace math;
	
	m_GameTime += dTime;
	m_Interval = interval;

	if( (m_GameTime - m_TBase) >= m_Interval)
	{
		m_TBase  += m_Interval;

		uint fps((uint)(1 / dTime));

		m_CurrentFPS = fps;
		m_CurrentDTime = dTime;

		if (fps < 0)
			fps = 0;
		else if (fps > 80)
			fps = 80;


		m_FpsHistory.push_back(fps);
	}

	if (m_GameTime > m_Interval)
	{
		if (m_FpsHistory.size() > 51)
			m_FpsHistory.erase(m_FpsHistory.begin());
	}
}

void FPSGraph::draw()
{
	using namespace happyengine;
	using namespace graphics;
	using namespace math;

	if (m_GameTime > m_Interval)
	{
		HE2D->setAntiAliasing(false);

		HE2D->setColor(1.0f,1.0f,1.0f,0.5f);
		HE2D->fillRectangle(Vector2(GRAPHICS->getViewport().width - 105.0f, 5.0f), Vector2(100, 40));
		HE2D->setColor(0.0f,0.0f,0.0f,0.5f);
		HE2D->setStrokeSize();
		HE2D->drawRectangle(Vector2(GRAPHICS->getViewport().width - 106.0f, 4.0f), Vector2(102, 42));

		std::vector<math::Vector2> points;
		uint i(0);

		std::for_each(m_FpsHistory.cbegin(), m_FpsHistory.cend(), [&](uint currentFps)
		{
			points.push_back(happyengine::math::Vector2(static_cast<float>(GRAPHICS->getViewport().width - 5 - (i * 2)), static_cast<float>(45 - (currentFps / 2))));
			++i;
		});

		HE2D->setColor(1.0f,0.0f,0.0f,0.8f);
		HE2D->setStrokeSize();
		HE2D->drawPolygon(points, points.size());

		points.clear();
		i = 0;

		std::for_each(m_FpsHistory.cbegin(), m_FpsHistory.cend(), [&](uint currentFps)
		{
			uint currentDTime = static_cast<uint>((1.0f / currentFps) * 1000.0f);

			if (currentDTime < 0)
				currentDTime = 0;
			else if (currentDTime > 80)
				currentDTime = 80;

			points.push_back(happyengine::math::Vector2(static_cast<float>(GRAPHICS->getViewport().width - 5 - (i * 2)), static_cast<float>(45 - (currentDTime / 2))));
			++i;
		});

		HE2D->setColor(1.0f,1.0f,0.0f,0.8f);
		HE2D->drawPolygon(points, points.size());

		HE2D->setColor(1.0f,1.0f,1.0f);
		HE2D->setFontVerticalAlignment(FontVAlignment_Top);

		std::stringstream stream;
		stream << "FPS: " << m_CurrentFPS;
		HE2D->drawText(Vector2(GRAPHICS->getViewport().width - 105.0f, 45), stream.str(), m_pFont);

		stream.str("");
		stream << "DTime: " << (m_CurrentDTime * 1000.0f) << " ms";
		HE2D->drawText(Vector2(GRAPHICS->getViewport().width - 105.0f, 58), stream.str(), m_pFont);
	}
}

} } //end namespace