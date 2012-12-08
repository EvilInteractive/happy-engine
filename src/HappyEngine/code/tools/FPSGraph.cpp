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
#include "Sprite.h"
#include "Gui.h"
#include "Canvas2Dnew.h"
#include "SystemStats.h"

namespace he {
namespace tools {

/* CONSTRUCTOR - DESTRUCTOR */
FPSGraph::FPSGraph(float interval, uint16 recordTime) :
                        m_GameTime(0.0f),
                        m_TBase(0.0f),
                        m_CurrentDTime(0.0f),
                        m_CurrentFPS(0),
                        m_Interval(interval),
                        m_Font(CONTENT->loadFont("Ubuntu-Medium.ttf", 6, false)),
                        m_FPSGraphState(Type_TextOnly),
                        m_Pos(5.0f, 5.0f),
                        m_FpsHistory(300),
                        m_ActiveSprite(0),
                        m_AcumulatedDTime(0.f),
                        m_Ticks(0),
                        m_RecordTime(recordTime),
                        m_CurrentScale(4.0f),
                        m_CurrentCPU(0.0f),
                        m_ColorWhite(Color(1.0f,1.0f,1.0f)),
                        m_ColorWhiteAlpha(Color(1.0f,1.0f,1.0f,0.6f)),
                        m_ColorYellow(Color((uint8)228,(uint8)211,(uint8)93)),
                        m_ColorYellowAlpha(Color((uint8)228,(uint8)211,(uint8)93,(uint8)100)),
                        m_ColorBlue(Color((uint8)94,(uint8)195,(uint8)247)),
                        m_ColorBlueAlpha(Color((uint8)94,(uint8)195,(uint8)247,(uint8)100)),
                        m_ColorGrey(Color((uint8)50,(uint8)47,(uint8)54)),
                        m_ColorDarkGrey(Color((uint8)30,(uint8)27,(uint8)34))
{
    m_Text.setFont(m_Font);
    m_Text.setBounds(vec2(100,20));

    CONSOLE->registerVar(&m_FPSGraphState, "s_fps_graph");

    m_Sprites[0] = GUI->Sprites->createSprite(vec2(110,82));
    m_Sprites[1] = GUI->Sprites->createSprite(vec2(110,82));

    SystemStats::init();
}

FPSGraph::~FPSGraph()
{
    m_Font->release();

    m_Sprites[0]->release();
    m_Sprites[1]->release();
}

/* GENERAL */
void FPSGraph::tick(float dTime)
{
    m_GameTime += dTime;

    m_AcumulatedDTime += dTime;
    ++m_Ticks;

    if ((m_GameTime - m_TBase) >= m_Interval)
    {
        m_TBase = m_GameTime;

        m_CurrentDTime = m_AcumulatedDTime / m_Ticks;

        uint16 fps(cap(1 / (m_CurrentDTime)));

        m_AcumulatedDTime = 0.f;
        m_Ticks = 0;

        m_CurrentFPS = fps;
        m_FpsHistory.add(fps);

        if (m_FPSGraphState == Type_Full)
        {
            renderGraph();
        }

        // limit recording time to save memory
        if (m_FpsHistory.size() * m_Interval > m_RecordTime)
        {
            m_FpsHistory.orderedRemoveAt(0);
        }

        m_CurrentCPU = HESTATS->getCpuUsage();
    }
}

void FPSGraph::draw2D(gfx::Canvas2D* canvas)
{
    if (m_GameTime <= m_Interval)
        return;

    switch (m_FPSGraphState)
    {
        case Type_Hide:
        {
            break;
        }
        case Type_ToConsole:
        {
            drawToConsole(canvas);
            break;
        }
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

inline uint16 FPSGraph::cap(const float& fps) const
{
    return static_cast<uint16>(fps);
}

inline uint16 FPSGraph::cap(const uint32& fps) const
{
    return static_cast<uint16>(fps);
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
    gui::Canvas2Dnew* cvs(canvas->getRenderer2D()->getNewCanvas());

	m_Text.clear();
	m_Text.addTextExt("%u (%u) FPS", m_CurrentFPS, getAverageFPS());
    m_Text.setHorizontalAlignment(gui::Text::HAlignment_Left);

    cvs->setColor(m_ColorYellow);
    cvs->fillText(m_Text, m_Pos);

    m_Text.clear();
    m_Text.addTextExt("%.3f MS", m_CurrentDTime * 1000.0f);
    m_Text.setHorizontalAlignment(gui::Text::HAlignment_Right);
    
    cvs->setColor(m_ColorBlue);
    cvs->fillText(m_Text, m_Pos);

    m_Text.clear();
	m_Text.addTextExt("MEM");
    m_Text.setHorizontalAlignment(gui::Text::HAlignment_Left);

    cvs->setColor(m_ColorWhite);
    cvs->fillText(m_Text, m_Pos + vec2(0,11));

    m_Text.clear();
	m_Text.addTextExt("%u - %u (%u)",
        (uint32)(HESTATS->getVirtualMemoryUsed() / (1024 * 1024)),
        (uint32)(HESTATS->getMemoryUsed() / (1024 * 1024)),
        (uint64)(HESTATS->getTotalMemory() / (1024 * 1024)));
    m_Text.setHorizontalAlignment(gui::Text::HAlignment_Right);

    cvs->fillText(m_Text, m_Pos + vec2(0,11));

    m_Text.clear();
	m_Text.addTextExt("CPU");
    m_Text.setHorizontalAlignment(gui::Text::HAlignment_Left);

    cvs->fillText(m_Text, m_Pos + vec2(0,22));

    m_Text.clear();
	m_Text.addTextExt("%.2f", m_CurrentCPU);
    m_Text.setHorizontalAlignment(gui::Text::HAlignment_Right);

    cvs->fillText(m_Text, m_Pos + vec2(0,22));
}

void FPSGraph::drawFull(gfx::Canvas2D* canvas)
{
    if (m_FpsHistory.size() == 0)
        return;

    gui::Canvas2Dnew* cvs(canvas->getRenderer2D()->getNewCanvas());

    cvs->drawSprite(m_Sprites[m_ActiveSprite], m_Pos);

	m_Text.clear();
	m_Text.addTextExt("%u (%u) FPS", m_CurrentFPS, getAverageFPS());
    m_Text.setHorizontalAlignment(gui::Text::HAlignment_Left);

    cvs->setColor(m_ColorYellow);
    cvs->fillText(m_Text, m_Pos + vec2(5,48));

    m_Text.clear();
    m_Text.addTextExt("%.3f MS", m_CurrentDTime * 1000.0f);
    m_Text.setHorizontalAlignment(gui::Text::HAlignment_Right);
    
    cvs->setColor(m_ColorBlue);
    cvs->fillText(m_Text, m_Pos + vec2(5,48));

    cvs->setColor(m_ColorWhiteAlpha);

    m_Text.clear();
    m_Text.addTextExt("%.0f",  m_CurrentScale * 20 * 0.75f);
    m_Text.setHorizontalAlignment(gui::Text::HAlignment_Left);

    cvs->fillText(m_Text, m_Pos + vec2(5,9.0f));

    m_Text.clear();
    m_Text.addTextExt("%.0f",  m_CurrentScale * 20 * 0.25f);

    cvs->fillText(m_Text, m_Pos + vec2(5,29.0f));

    m_Text.clear();
	m_Text.addTextExt("MEM");
    m_Text.setHorizontalAlignment(gui::Text::HAlignment_Left);

    cvs->setColor(m_ColorWhite);
    cvs->fillText(m_Text, m_Pos + vec2(5,59));

    m_Text.clear();
	m_Text.addTextExt("%u - %u (%u)",
        (uint32)(HESTATS->getVirtualMemoryUsed() / (1024 * 1024)),
        (uint32)(HESTATS->getMemoryUsed() / (1024 * 1024)),
        (uint64)(HESTATS->getTotalMemory() / (1024 * 1024)));
    m_Text.setHorizontalAlignment(gui::Text::HAlignment_Right);

    cvs->fillText(m_Text, m_Pos + vec2(5,59));

    m_Text.clear();
	m_Text.addTextExt("CPU");
    m_Text.setHorizontalAlignment(gui::Text::HAlignment_Left);

    cvs->fillText(m_Text, m_Pos + vec2(5,70));

    m_Text.clear();
	m_Text.addTextExt("%.2f", m_CurrentCPU);
    m_Text.setHorizontalAlignment(gui::Text::HAlignment_Right);

    cvs->fillText(m_Text, m_Pos + vec2(5,70));
}

void FPSGraph::updateScale(uint16 currentMaxFpsInFrame)
{
    // cap
    currentMaxFpsInFrame -= (currentMaxFpsInFrame % 20);
    currentMaxFpsInFrame += 20;

    m_CurrentScale = currentMaxFpsInFrame / 20.0f;
}

void FPSGraph::renderGraph()
{
    gui::SpriteCreator* cr(GUI->Sprites);

    cr->setActiveSprite(m_Sprites[m_ActiveSprite]);

    cr->setLineJoin(gui::LINE_JOIN_ROUND);

    cr->newPath();

    cr->rectangle(vec2(5,5),vec2(100,40));
    cr->setColor(m_ColorGrey);
    cr->fill();

    PrimitiveList<vec2> poly0(60);
    PrimitiveList<vec2> poly1(60);

    uint32 i(0);
                
    if (m_FpsHistory.size() > 50)
    {
        i = m_FpsHistory.size() - 51;
    }

    uint32 j(m_FpsHistory.size());

    uint8 k(0);
    uint16 currentFPS(0);

    cr->newPath();

    uint16 maxFpsInGraph(0);

    for (; i < j ; ++i)
    {
        currentFPS = m_FpsHistory[i];

        if (currentFPS > maxFpsInGraph)
            maxFpsInGraph = currentFPS;

        if (k == 0)
        {
            poly0.add(vec2(110.0f - (k * 2), 45.0f - (currentFPS / (m_CurrentScale / 2.0f))));
        }

        poly0.add(vec2(105.0f - (k * 2), 45.0f - (currentFPS / (m_CurrentScale / 2.0f))));

        ++k;
    }

    if (k < 50)
    {
        poly0.add(vec2(105.0f - ((k - 1) * 2), 50.0f));
    }
    else
    {
        poly0.add(vec2(0.0f, 45.0f - (currentFPS / (m_CurrentScale / 2.0f))));
        poly0.add(vec2(0.0f, 50.0f));
    }

    poly0.add(vec2(110.0f, 50.0f));

    if (m_FpsHistory.size() > 50)
    {
        i = m_FpsHistory.size() - 51;
    }
    else
    {
        i = 0;
    }

    k = 0;
    uint16 currentDTime(0);

    for (; i < j ; ++i)
    {
        currentDTime = static_cast<uint16>((1.0f / m_FpsHistory[i]) * 1000.0f);

        if (currentDTime > maxFpsInGraph)
            maxFpsInGraph = currentDTime;

        if (k == 0)
        {
            poly1.add(vec2(110.0f - (k * 2), 45.0f - (currentDTime / (m_CurrentScale / 2.0f))));
        }

        poly1.add(vec2(105.0f - (k * 2), 45.0f - (currentDTime / (m_CurrentScale / 2.0f))));

        ++k;
    }

    if (k < 50)
    {
        poly1.add(vec2(105.0f - ((k - 1) * 2), 50.0f));
    }
    else
    {
        poly1.add(vec2(0.0f, 45.0f - (currentDTime / (m_CurrentScale / 2.0f))));
        poly1.add(vec2(0.0f, 50.0f));
    }

    poly1.add(vec2(110.0f, 50.0f));

    updateScale(maxFpsInGraph);

    cr->setLineWidth(1.5f);
    cr->newPath();

    if (m_CurrentFPS >= static_cast<uint16>(m_CurrentDTime * 1000.0f))
    {
        i = 0;

        poly0.forEach([&](vec2 p)
        {
            if (i == 0)
            {
                cr->moveTo(p);

                ++i;
            }
            else
            {
                cr->lineTo(p);
            }
        });

        cr->setColor(m_ColorYellowAlpha);
        cr->fill();

        cr->setColor(m_ColorYellow);//,0.6f));
        cr->stroke();

        cr->newPath();
        i = 0;

        poly1.forEach([&](vec2 p)
        {
            if (i == 0)
            {
                cr->moveTo(p);

                ++i;
            }
            else
            {
                cr->lineTo(p);
            }
        });

        cr->setColor(m_ColorBlueAlpha);
        cr->fill();

        cr->setColor(m_ColorBlue);
        cr->stroke();
    }
    else
    {
        i = 0;

        poly1.forEach([&](vec2 p)
        {
            if (i == 0)
            {
                cr->moveTo(p);

                ++i;
            }
            else
            {
                cr->lineTo(p);
            }
        });

        cr->setColor(m_ColorBlueAlpha);
        cr->fill();

        cr->setColor(m_ColorBlue);
        cr->stroke();

        i = 0;
        cr->newPath();

        poly0.forEach([&](vec2 p)
        {
            if (i == 0)
            {
                cr->moveTo(p);

                ++i;
            }
            else
            {
                cr->lineTo(p);
            }
        });

        cr->setColor(m_ColorYellowAlpha);
        cr->fill();

        cr->setColor(m_ColorYellow);
        cr->stroke(); 
    }

    cr->setLineWidth(1.0f);

    cr->newPath();

    cr->rectangle(vec2(5,24),vec2(5,1));
    cr->rectangle(vec2(5,44),vec2(5,1));

    cr->setColor(m_ColorWhite);

    cr->fill();

    cr->newPath();

    cr->rectangle(vec2(0,0),vec2(110,5));
    cr->rectangle(vec2(0,0),vec2(5,65));
    cr->rectangle(vec2(105,0),vec2(5,65));
    cr->rectangle(vec2(0,45),vec2(110,50));

    cr->setColor(m_ColorDarkGrey);
    cr->fill();

    cr->newPath();

    cr->rectangle(vec2(0,0),vec2(110,82));

    cr->setColor(Color((uint8)20,(uint8)20,(uint8)20));

    cr->stroke();

    cr->renderSpriteAsync();

    ++m_ActiveSprite;

    if (m_ActiveSprite > 1)
        m_ActiveSprite = 0;
}

/* GETTERS */
uint16 FPSGraph::getMaxFPS() const
{
    uint16 maxFPS(0);
    m_FpsHistory.forEach([&](uint16 FPS)
    {
        if (FPS > maxFPS)
            maxFPS = FPS;
    });

    return maxFPS;
}

uint16 FPSGraph::getMinFPS() const
{
    uint16 minFPS(0xff);
    m_FpsHistory.forEach([&](uint16 FPS)
    {
        if (FPS < minFPS)
            minFPS = FPS;
    });

    return minFPS;
}

uint16 FPSGraph::getAverageFPS() const
{
    if (m_FpsHistory.size() == 0)
        return 0;

    uint32 i(0);
    uint32 j((uint32)m_FpsHistory.size());

    uint32 avFPS(0);

    for (; i < j ; ++i)
    {
        avFPS += m_FpsHistory[i];
    }

    avFPS /= (uint32)m_FpsHistory.size();

    return cap(avFPS);
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

} } //end namespace