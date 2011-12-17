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
#include "HappyPCH.h" 

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
                        m_pFont(CONTENT->loadFont("Ubuntu-Bold.ttf", 10)),
                        m_FPSGraphState(2)
{
    CONSOLE->registerVar<int>(&m_FPSGraphState, "s_fps_graph");

    setPos(vec2(GRAPHICS->getViewport().width - 105.0f, 5.0f));
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
        m_TBase = m_GameTime;

        uint fps((uint)(1 / dTime));

        m_CurrentFPS = fps;
        m_CurrentDTime = dTime;

        if (fps > 99)
            fps = 99;

        if (fps > 1)
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
    GUI->setAntiAliasing(false);
    GUI->setColor(1.0f,1.0f,1.0f);

    std::stringstream stream;
    stream << "FPS: " << m_CurrentFPS << " (" << getAverageFPS() << ")";
    GUI->drawText(gui::Text(stream.str(), m_pFont), m_Pos);

    stream.str("");
    stream << "DTime: " << (m_CurrentDTime * 1000.0f) << " ms";
    GUI->drawText(gui::Text(stream.str(), m_pFont), m_Pos + vec2(0.0f, 13.0f));
}

void FPSGraph::drawFull()
{
    if (m_FpsHistory.size() == 0)
        return;

    GUI->setAntiAliasing(false);

    GUI->setColor(0.8f,0.8f,0.8f);
    GUI->fillShape2D(gui::Rectangle2D(m_Pos, vec2(100, 41)), true);
    GUI->setColor(0.1f,0.1f,0.1f);
    GUI->drawShape2D(gui::Rectangle2D(m_Pos + vec2(1.0f, -1.0f), vec2(102, 42)), true);

    gui::Polygon2D poly;
    gui::Polygon2D poly2;

    uint i(0);
        
    if (m_FpsHistory.size() > 50)
    {
        i = m_FpsHistory.size() - 51;
    }

    uint j(m_FpsHistory.size());

    uint k(0);

    for (; i < j ; ++i)
    {
        uint currentFPS(m_FpsHistory[i]);

        if (currentFPS > 80)
            currentFPS = 80;

        poly.addPoint(vec2(m_Pos.x + 100.0f - (k * 2), m_Pos.y + 41.0f - (currentFPS / 2)));

        ++k;
    }

    if (poly.getPolygon().getVertexCount() < 50)
    {
        poly.addPoint(vec2(m_Pos.x + 100.0f - (poly.getPolygon().getVertexCount() * 2), m_Pos.y + 41.0f));
    }
    else
    {
        poly.addPoint(vec2(m_Pos.x, m_Pos.y + 41.0f));
    }

    poly.addPoint(vec2(m_Pos.x + 100.0f, m_Pos.y + 41.0f));

    if (m_FpsHistory.size() > 50)
    {
        i = m_FpsHistory.size() - 51;
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

        poly2.addPoint(vec2(m_Pos.x + 100.0f - (k * 2), m_Pos.y + 41.0f - (currentDTime / 2)));

        ++k;
    }

    if (poly2.getPolygon().getVertexCount() < 50)
    {
        poly2.addPoint(vec2(m_Pos.x + 100.0f - (poly2.getPolygon().getVertexCount() * 2), m_Pos.y + 41.0f));
    }
    else
    {
        poly2.addPoint(vec2(m_Pos.x, m_Pos.y + 41.0f));
    }

    poly2.addPoint(vec2(m_Pos.x + 100.0f, m_Pos.y + 41.0f));

    if (m_CurrentFPS >= static_cast<uint>(m_CurrentDTime * 1000.0f))
    {
        GUI->setColor(1.0f,0.0f,0.0f);
        GUI->fillShape2D(poly);
        GUI->setColor(1.0f,1.0f,0.0f);
        GUI->fillShape2D(poly2);
    }
    else
    {
        GUI->setColor(1.0f,1.0f,0.0f);
        GUI->fillShape2D(poly2);
        GUI->setColor(1.0f,0.0f,0.0f);
        GUI->fillShape2D(poly);
    }

    uint avFPS(getAverageFPS());

    if (avFPS > 80)
        avFPS = 80;

    GUI->setColor(0.0f,0.0f,1.0f);
    GUI->drawShape2D(gui::Line2D(vec2(m_Pos.x, m_Pos.y + 40.0f - (avFPS / 2)),
                    vec2(m_Pos.x + 100.0f, m_Pos.y + 40.0f - (avFPS / 2))));

    GUI->setColor(1.0f,1.0f,1.0f);
    std::stringstream stream;
    stream << "FPS: " << m_CurrentFPS << " (" << getAverageFPS() << ")";
    GUI->drawText(gui::Text(stream.str(), m_pFont), vec2(m_Pos.x, m_Pos.y + 43.0f));

    stream.str("");
    stream << "DTime: " << (m_CurrentDTime * 1000.0f) << " ms";
    GUI->drawText(gui::Text(stream.str(), m_pFont), vec2(m_Pos.x, m_Pos.y + 56.0f));
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
    if (m_FpsHistory.size() == 0)
        return 0;

    uint i(0);
        
    if ((m_FpsHistory.size() * m_Interval) > 60)
    {
        i = static_cast<uint>(m_FpsHistory.size() - (60 / m_Interval));
    }

    uint j(m_FpsHistory.size());

    uint avFPS(0);

    for (; i < j ; ++i)
    {
        avFPS += m_FpsHistory[i];
    }

    if ((m_FpsHistory.size() * m_Interval) > 60)
    {
        avFPS /= static_cast<uint>(60 / m_Interval);
    }
    else
    {
        avFPS /= m_FpsHistory.size();
    }

    return avFPS;
}

/* SETTERS */
void FPSGraph::setType(int type)
{
    m_FPSGraphState = type;
}

void FPSGraph::setPos(vec2 pos)
{
    m_Pos = pos;
}

} } //end namespace