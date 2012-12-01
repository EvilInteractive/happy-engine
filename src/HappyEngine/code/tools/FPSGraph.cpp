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
FPSGraph::FPSGraph(uint16 recordTime) :
                        m_GameTime(0.0f),
                        m_TBase(0.0f),
                        m_CurrentDTime(0.0f),
                        m_CurrentFPS(0),
                        m_Interval(0.5f),
                        m_Font(CONTENT->loadFont("Ubuntu-Medium.ttf", 10, false)),
                        m_FPSGraphState(Type_TextOnly),
                        m_Pos(5.0f, 5.0f),
                        m_FpsHistory(300),
                        m_ActiveSprite(0),
                        m_AcumulatedDTime(0.f),
                        m_Ticks(0),
                        m_RecordTime(recordTime)
{
    CONSOLE->registerVar(&m_FPSGraphState, "s_fps_graph");

    m_Sprites[0] = GUI->Sprites->createSprite(vec2(100,40));
    m_Sprites[1] = GUI->Sprites->createSprite(vec2(100,40));
}

FPSGraph::~FPSGraph()
{
    m_Font->release();

    m_Sprites[0]->release();
    m_Sprites[1]->release();
}

/* GENERAL */
void FPSGraph::tick(float dTime, float interval)
{
    m_GameTime += dTime;
    m_Interval = interval;

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

    cvs->drawSprite(m_Sprites[m_ActiveSprite],m_Pos);

    cvs->setColor(Color(1.0f,1.0f,1.0f));

    gui::Text txt(m_Font);

    char buff[64];
    sprintf(buff, "FPS: %u (%u)", m_CurrentFPS, getAverageFPS());
    txt.addLine(std::string(buff));

    sprintf(buff, "DTime: %.3f ms", m_CurrentDTime * 1000.0f);
    txt.addLine(std::string(buff));
    
    cvs->fillText(txt, m_Pos + vec2(0,43));
}

void FPSGraph::renderGraph()
{
    gui::SpriteCreator* cr(GUI->Sprites);

    cr->setActiveSprite(m_Sprites[m_ActiveSprite]);
    cr->rectangle(vec2(0,0),vec2(100,40));
    cr->setColor(Color(0.9f,0.9f,0.9f,0.8f));
    cr->fill();

    PrimitiveList<vec2> poly0;
    PrimitiveList<vec2> poly1;

    uint32 i(0);
                
    if (m_FpsHistory.size() > 50)
    {
        i = m_FpsHistory.size() - 51;
    }

    uint32 j(m_FpsHistory.size());

    uint8 k(0);
    uint16 currentFPS(0);

    cr->newPath();

    for (; i < j ; ++i)
    {
        currentFPS = m_FpsHistory[i];

        if (currentFPS > 80)
            currentFPS = 80;

        if (currentFPS < 2)
            currentFPS = 2;

        if (k == 0)
        {
            poly0.add(vec2(105.0f - (k * 2), 40.0f - (currentFPS / 2)));
        }

        poly0.add(vec2(100.0f - (k * 2), 40.0f - (currentFPS / 2)));

        ++k;
    }

    if (k < 50)
    {
        poly0.add(vec2(100.0f - ((k - 1) * 2), 45.0f));
    }
    else
    {
        poly0.add(vec2(-5.0f, 40.0f - (currentFPS / 2)));
        poly0.add(vec2(-5.0f, 45.0f));
    }

    poly0.add(vec2(105.0f, 45.0f));

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

        if (currentDTime > 80)
            currentDTime = 80;

        if (currentDTime < 2)
            currentDTime = 2;

        if (k == 0)
        {
            poly1.add(vec2(105.0f - (k * 2), 40.0f - (currentDTime / 2)));
        }

        poly1.add(vec2(100.0f - (k * 2), 40.0f - (currentDTime / 2)));

        ++k;
    }

    if (k < 50)
    {
        poly1.add(vec2(100.0f - ((k - 1) * 2), 45.0f));
    }
    else
    {
        poly1.add(vec2(-5.0f, 40.0f - (currentDTime / 2)));
        poly1.add(vec2(-5.0f, 45.0f));
    }

    poly1.add(vec2(105.0f, 45.0f));

    cr->setLineWidth(1.0f);
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

        cr->setColor(Color(1.0f,0.7f,0.7f,0.6f));
        cr->fill();

        cr->setColor(Color(1.0f,0.0f,0.0f,0.6f));
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

        cr->setColor(Color(1.0f,0.9f,0.6f,0.6f));
        cr->fill();

        cr->setColor(Color(1.0f,1.0f,0.0f,0.6f));
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

        cr->setColor(Color(1.0f,0.9f,0.6f,0.6f));
        cr->fill();

        cr->setColor(Color(1.0f,1.0f,0.0f,0.6f));
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

        cr->setColor(Color(1.0f,0.7f,0.7f,0.6f));
        cr->fill();

        cr->setColor(Color(1.0f,0.0f,0.0f,0.6f));
        cr->stroke(); 
    }

    cr->newPath();

    cr->rectangle(vec2(0,0),vec2(100,40));
    cr->setColor(Color(0.2f,0.2f,0.2f,0.6f));
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