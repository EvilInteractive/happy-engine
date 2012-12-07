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
                        m_FontSmall(CONTENT->loadFont("Ubuntu-Medium.ttf", 5, false)),
                        m_FPSGraphState(Type_TextOnly),
                        m_Pos(5.0f, 5.0f),
                        m_FpsHistory(300),
                        m_ActiveSprite(0),
                        m_AcumulatedDTime(0.f),
                        m_Ticks(0),
                        m_RecordTime(recordTime),
                        m_CurrentScale(4.0f)
{
    CONSOLE->registerVar(&m_FPSGraphState, "s_fps_graph");

    m_Sprites[0] = GUI->Sprites->createSprite(vec2(110,60));
    m_Sprites[1] = GUI->Sprites->createSprite(vec2(110,60));
}

FPSGraph::~FPSGraph()
{
    m_Font->release();
    m_FontSmall->release();

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
    canvas->setDepth(-1900);

    canvas->setFillColor(Color(1.0f,1.0f,1.0f));

    gui::Text txt(m_Font);

    char buff[64];
    sprintf(buff, "FPS: %u (%u)", m_CurrentFPS, getAverageFPS());
    txt.addLine(std::string(buff));

    sprintf(buff, "DTime: %.3f ms", m_CurrentDTime * 1000.0f);
    txt.addLine(std::string(buff));

    canvas->fillText(txt, m_Pos);

    canvas->restoreDepth();
}

void FPSGraph::drawFull(gfx::Canvas2D* canvas)
{
    if (m_FpsHistory.size() == 0)
        return;

    gui::Canvas2Dnew* cvs(canvas->getRenderer2D()->getNewCanvas());

    cvs->drawSprite(m_Sprites[m_ActiveSprite], m_Pos);

    gui::Text txt(m_Font);

    char buff[32];
    sprintf(buff, "%u (%u)", m_CurrentFPS, getAverageFPS());
    std::string buffS(buff);

    txt.addLine(buffS);

    cvs->setColor(Color((uint8)228,(uint8)211,(uint8)93));
    cvs->fillText(txt, m_Pos + vec2(5,48));

    txt.clear();
    std::string mS(" - ");
    txt.addLine(mS);

    cvs->setColor(Color(1.0f,1.0f,1.0f));
    cvs->fillText(txt, m_Pos + vec2(5,48) + vec2(m_Font->getStringWidth(buffS), 0));

    txt.clear();

    char buff0[32];
    sprintf(buff0, "%.3f MS", m_CurrentDTime * 1000.0f);
    txt.addLine(std::string(buff0));
    
    cvs->setColor(Color((uint8)94,(uint8)195,(uint8)247));
    cvs->fillText(txt, m_Pos + vec2(5,48) + vec2(m_Font->getStringWidth(buffS) + m_Font->getStringWidth(mS), 0));

    gui::Text txt2(m_FontSmall);

    cvs->setColor(Color(1.0f,1.0f,1.0f));

    char buff2[8];
    sprintf(buff2, "%.0f",  m_CurrentScale * 20 * 0.75f);
    txt2.addLine(std::string(buff2));

    cvs->fillText(txt2, m_Pos + vec2(5,9.0f));

    txt2.clear();

    char buff3[8];
    sprintf(buff3, "%.0f",  m_CurrentScale * 20 * 0.25f);
    txt2.addLine(std::string(buff3));
    cvs->fillText(txt2, m_Pos + vec2(5,29.0f));
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
    cr->setColor(Color((uint8)50,(uint8)47,(uint8)54));
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

        cr->setColor(Color((uint8)228,(uint8)211,(uint8)93,(uint8)100));
        cr->fill();

        cr->setColor(Color((uint8)228,(uint8)211,(uint8)93));//,0.6f));
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

        cr->setColor(Color((uint8)94,(uint8)195,(uint8)247,(uint8)100));
        cr->fill();

        cr->setColor(Color((uint8)94,(uint8)195,(uint8)247));//,0.6f));
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

        cr->setColor(Color((uint8)94,(uint8)195,(uint8)247,(uint8)100));
        cr->fill();

        cr->setColor(Color((uint8)94,(uint8)195,(uint8)247));
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

        cr->setColor(Color((uint8)228,(uint8)211,(uint8)93,(uint8)100));
        cr->fill();

        cr->setColor(Color((uint8)228,(uint8)211,(uint8)93));
        cr->stroke(); 
    }

    cr->setLineWidth(1.0f);

    cr->newPath();

    //cr->rectangle(vec2(5,14),vec2(5,1));
    cr->rectangle(vec2(5,24),vec2(5,1));
    //cr->rectangle(vec2(5,34),vec2(5,1));
    cr->rectangle(vec2(5,44),vec2(5,1));

    cr->setColor(Color((uint8)231,(uint8)227,(uint8)242));

    cr->fill();

    cr->newPath();

    cr->rectangle(vec2(0,0),vec2(110,5));
    cr->rectangle(vec2(0,0),vec2(5,65));
    cr->rectangle(vec2(105,0),vec2(5,65));
    cr->rectangle(vec2(0,45),vec2(110,15));
    cr->setColor(Color((uint8)30,(uint8)27,(uint8)34));
    cr->fill();

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