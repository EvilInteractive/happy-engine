//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
#include "ContentManager.h"
#include "Console.h"
#include "Renderer2D.h"
#include "GraphicsEngine.h"
#include "Renderer2D.h"
#include "Text.h"
#include "Canvas2D.h"
#include "View.h"
#include "Font.h"

namespace he {
namespace tools {

/* CONSTRUCTOR - DESTRUCTOR */
FPSGraph::FPSGraph() :	m_GameTime(0.0f),
                        m_TBase(0.0f),
                        m_CurrentDTime(0.0f),
                        m_CurrentFPS(0),
                        m_Interval(0.5f),
                        m_pFont(CONTENT->loadFont("Ubuntu-Medium.ttf", 10)),
                        m_FPSGraphState(Type_ToConsole),
                        m_Pos(5.0f, 5.0f),
                        m_View(nullptr)
{
    CONSOLE->registerVar<int>(&m_FPSGraphState, "s_fps_graph");

    m_FpsHistory.reserve(300);
}


FPSGraph::~FPSGraph()
{
    m_pFont->release();
}

/* GENERAL */
void FPSGraph::tick(float dTime, float interval)
{
    m_GameTime += dTime;
    m_Interval = interval;

    if ((m_GameTime - m_TBase) >= m_Interval)
    {
        m_TBase = m_GameTime;

        ushort fps(cap(1 / dTime));

        m_CurrentFPS = fps;
        m_CurrentDTime = dTime;

        m_FpsHistory.push_back(fps);
    }
}

void FPSGraph::draw2D(gfx::Canvas2D* canvas)
{
    HE_IF_ASSERT(m_View != nullptr, "Set view first with setView!")
    if (m_GameTime > m_Interval)
    {
        switch (m_FPSGraphState)
        {
            case Type_Hide:
            {
                break;
            }
            case Type_ToConsole:
                drawToConsole(canvas);
                break;
            case Type_TextOnly:
            {
                drawTextOnly(canvas);
                break;
            }
            case Type_Full:
            {
                drawFull(canvas);
                break;
            }
        }
    }
}

ushort FPSGraph::cap(float fps)
{
   /* if (fps > 250.0f)
        return 250;
    else*/
        return static_cast<ushort>(fps);
}

void FPSGraph::drawToConsole(gfx::Canvas2D* /*canvas*/)
{
    if ((m_GameTime - m_TBase) < FLT_EPSILON)
    {
        HE_INFO("Fps: %d", (int)m_CurrentFPS);
    }
}

void FPSGraph::drawTextOnly(gfx::Canvas2D* canvas)
{
    //GUI->setAntiAliasing(false);
    //GUI->setColor(1.0f,1.0f,1.0f);

    canvas->setDepth(-1900);

    canvas->setFillColor(Color(1.0f,1.0f,1.0f));

    gui::Text txt(m_pFont);

    char buff[64];
    sprintf(buff, "FPS: %u (%u)", m_CurrentFPS, getAverageFPS());
    txt.addLine(std::string(buff));

    sprintf(buff, "DTime: %.3f ms", m_CurrentDTime * 1000.0f);
    txt.addLine(std::string(buff));
    
    //canvas->fillRect(m_Pos, vec2(128, 16));
    canvas->fillText(txt, m_Pos);

    canvas->restoreDepth();

    //m_pCanvas2D->draw2D(renderer);
}

void FPSGraph::drawFull(gfx::Canvas2D* /*canvas*/)
{
    //if (m_FpsHistory.size() == 0)
    //    return;

    ////GUI->setAntiAliasing(false);

    ////GUI->setColor(0.8f,0.8f,0.8f);
    ////GUI->fillShape2D(gui::Rectangle2D(m_Pos, vec2(100, 40)), true);

    //gui::Polygon2D poly;
    //gui::Polygon2D poly2;

    //uint i(0);
    //    
    //if (m_FpsHistory.size() > 50)
    //{
    //    i = m_FpsHistory.size() - 51;
    //}

    //uint j(m_FpsHistory.size());

    //ushort k(0);
    //ushort currentFPS(0);
    ////ushort prevFPS(0);

    //for (; i < j ; ++i)
    //{
    //    currentFPS = m_FpsHistory[i];
    //    //prevFPS = m_FpsHistory[i - 1];

    //    if (currentFPS > 80)
    //        currentFPS = 80;

    //    if (currentFPS < 2)
    //        currentFPS = 2;

    //    /*if (prevFPS > 80)
    //    prevFPS = 80;*/

    //    poly.addPoint(vec2(m_Pos.x + 100.0f - (k * 2), m_Pos.y + 40.0f - (currentFPS / 2)));
    //    /*//GUI->drawShape2D(gui::Line2D(   vec2(m_Pos.x + 100.0f - ((k + 1) * 2), m_Pos.y + 40.0f - (currentFPS / 2)),
    //                                    vec2(m_Pos.x + 100.0f - (k * 2), m_Pos.y + 40.0f - (prevFPS / 2))));*/

    //    ++k;
    //}

    //if (poly.getPolygon().getVertexCount() < 50)
    //{
    //    poly.addPoint(vec2(m_Pos.x + 100.0f - ((poly.getPolygon().getVertexCount() - 1) * 2), m_Pos.y + 40.0f));
    //}
    //else
    //{
    //    poly.addPoint(vec2(m_Pos.x, m_Pos.y + 40.0f));
    //}

    //poly.addPoint(vec2(m_Pos.x + 100.0f, m_Pos.y + 40.0f));

    //if (m_FpsHistory.size() > 50)
    //{
    //    i = m_FpsHistory.size() - 51;
    //}
    //else
    //{
    //    i = 0;
    //}

    //k = 0;
    //ushort currentDTime(0);
    ////ushort prevDTime(0);

    //for (; i < j ; ++i)
    //{
    //    currentDTime = static_cast<ushort>((1.0f / m_FpsHistory[i]) * 1000.0f);
    //    //prevDTime = static_cast<ushort>((1.0f / m_FpsHistory[i - 1]) * 1000.0f);

    //    if (currentDTime > 80)
    //        currentDTime = 80;

    //    if (currentDTime < 2)
    //        currentDTime = 2;

    //    /*if (prevDTime > 80)
    //    prevDTime = 80;*/

    //    poly2.addPoint(vec2(m_Pos.x + 100.0f - (k * 2), m_Pos.y + 40.0f - (currentDTime / 2)));
    //    /*//GUI->drawShape2D(gui::Line2D(   vec2(m_Pos.x + 100.0f - ((k + 1) * 2), m_Pos.y + 40.0f - (currentDTime / 2)),
    //                                    vec2(m_Pos.x + 100.0f - (k * 2), m_Pos.y + 40.0f - (prevDTime / 2))));*/
    //    ++k;
    //}

    //if (poly2.getPolygon().getVertexCount() < 50)
    //{
    //    poly2.addPoint(vec2(m_Pos.x + 100.0f - ((poly2.getPolygon().getVertexCount() - 1) * 2), m_Pos.y + 40.0f));
    //}
    //else
    //{
    //    poly2.addPoint(vec2(m_Pos.x, m_Pos.y + 40.0f));
    //}

    //poly2.addPoint(vec2(m_Pos.x + 100.0f, m_Pos.y + 40.0f));

    //if (m_CurrentFPS >= static_cast<ushort>(m_CurrentDTime * 1000.0f))
    //{
    //    //GUI->setColor(1.0f,0.7f,0.7f);
    //    //GUI->fillShape2D(poly);
    //    //GUI->setColor(1.0f,0.0f,0.0f);
    //    //GUI->drawShape2D(poly);
    //    //GUI->setColor(1.0f,0.9f,0.6f);
    //    //GUI->fillShape2D(poly2);
    //    //GUI->setColor(1.0f,1.0f,0.0f);
    //    //GUI->drawShape2D(poly2);
    //}
    //else
    //{
    //    //GUI->setColor(1.0f,0.9f,0.6f);
    //    //GUI->fillShape2D(poly2);
    //    //GUI->setColor(1.0f,1.0f,0.0f);
    //    //GUI->drawShape2D(poly2);
    //    //GUI->setColor(1.0f,0.7f,0.7f);
    //    //GUI->fillShape2D(poly);
    //    //GUI->setColor(1.0f,0.0f,0.0f);
    //    //GUI->drawShape2D(poly);
    //}

    //ushort avFPS(getAverageFPS());

    //if (avFPS > 80)
    //    avFPS = 80;

    ////GUI->setColor(0.0f,0.47f,1.0f);
    ////GUI->fillShape2D(gui::Rectangle2D(  vec2(m_Pos.x, m_Pos.y + 39.0f - (avFPS / 2)),
    //                                    vec2(10.0f, 3.0f)));

    ////GUI->setColor(0.1f,0.1f,0.1f);
    ////GUI->drawShape2D(gui::Rectangle2D(m_Pos + vec2(0.0f, -1.0f), vec2(101, 41)), true);

    ////GUI->setColor(1.0f,1.0f,1.0f);

    //// replaced stringstream by sprintf -> stringstream is very slow
    //char buff[64];

    //sprintf(buff, "FPS: %u (%u)", m_CurrentFPS, getAverageFPS());
    ////GUI->drawText(gui::Text(std::string(buff), m_pFont), vec2(m_Pos.x, m_Pos.y + 43.0f));

    //sprintf(buff, "DTime: %.3f ms", m_CurrentDTime * 1000.0f);
    ////GUI->drawText(gui::Text(std::string(buff), m_pFont), vec2(m_Pos.x, m_Pos.y + 56.0f));
}

/* GETTERS */
ushort FPSGraph::getMaxFPS() const
{
    ushort maxFPS(0);
    std::for_each(m_FpsHistory.cbegin(), m_FpsHistory.cend(), [&](ushort FPS)
    {
        if (FPS > maxFPS)
            maxFPS = FPS;
    });

    return maxFPS;
}

ushort FPSGraph::getMinFPS() const
{
    ushort minFPS(0xffff);
    std::for_each(m_FpsHistory.cbegin(), m_FpsHistory.cend(), [&](ushort FPS)
    {
        if (FPS < minFPS)
            minFPS = FPS;
    });

    return minFPS;
}

ushort FPSGraph::getAverageFPS() const
{
    if (m_FpsHistory.size() == 0)
        return 0;

    uint i(0);
        
    if ((m_FpsHistory.size() * m_Interval) > 60)
    {
        i = static_cast<uint>(m_FpsHistory.size() - (60 / m_Interval));
    }

    uint j((uint)m_FpsHistory.size());

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
        avFPS /= (uint)m_FpsHistory.size();
    }

    return (ushort)avFPS;
}

/* SETTERS */
void FPSGraph::setType(Type type)
{
    m_FPSGraphState = type;
}

void FPSGraph::setPos(const vec2& pos)
{
    m_Pos = pos;
}

void FPSGraph::setView( gfx::View* view )
{
    /*
    if (m_View != nullptr)
    {
        m_View->get2DRenderer()->removeCanvas(m_pCanvas2D);
        m_View->get2DRenderer()->detachFromRender(this);
    }*/
    m_View = view;
    
    m_View->get2DRenderer()->attachToRender(this);
    /*m_pCanvas2D = m_View->get2DRenderer()->createCanvasRelative(RectF(0, 0, 1, 1)); // TODO: is inefficient to use a fullscreen canvas
    */

    m_Pos.x = view->getViewport().width - 105.0f;
}

} } //end namespace