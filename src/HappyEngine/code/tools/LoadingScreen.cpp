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
//Created: 24/12/2011

#include "HappyPCH.h" 

#include "LoadingScreen.h"
#include "HappyEngine.h"
#include "ContentManager.h"
#include "Happy2DRenderer.h"
#include "GraphicsEngine.h"
#include <algorithm>

namespace he {
namespace tools {

LoadingScreen::LoadingScreen() :    m_pFontLoading(CONTENT->getDefaultFont(14)),
                                    m_pFontMessage(CONTENT->getDefaultFont(32)),
                                    m_pLogo(CONTENT->loadTexture("engine/he_logo_christmas.png"))
{
    /*SnowFlake temp;
    for (uint i(0); i < 250; ++i)
    {
        temp.pos.x = rand() % GRAPHICS->getScreenWidth();
        temp.pos.y = rand() % GRAPHICS->getScreenHeight();

        temp.size = (rand() % 10) + 5;

        temp.speed.x = -(((rand() % 10) + 5) / 100.0f);
        temp.speed.y = (rand() % 10 + 5) / 100.0f;

        m_SnowFlakes.push_back(temp);
    }*/
}

LoadingScreen::~LoadingScreen()
{
}

/* GENERAL */
void LoadingScreen::tick()
{
    /*std::for_each(m_SnowFlakes.begin(), m_SnowFlakes.end(), [&](SnowFlake s)
    {
        s.pos += s.speed;

        if (s.pos.x < (-s.size/2.0f))
        {
            s.pos.x = (float)GRAPHICS->getScreenWidth();
        }
        else if (s.pos.y > ((float)GRAPHICS->getScreenWidth() + s.size / 2.0f))
        {
            s.pos.x = 0.0f;
        }
    });*/
}

void LoadingScreen::draw()
{
    GUI->setColor(0.9f,0.9f,0.9f);

    GUI->fillShape2D(gui::Rectangle2D(vec2(0,0), vec2((float)GRAPHICS->getScreenWidth(),(float)GRAPHICS->getScreenHeight())), true);

    //GUI->setColor(1.0f,1.0f,1.0f);

    /*std::for_each(m_SnowFlakes.cbegin(), m_SnowFlakes.cend(), [](SnowFlake s)
    {
        GUI->fillShape2D(gui::Ellipse2D(s.pos,vec2(s.size,s.size),8), true);
    });*/

    /*gui::Text message("Happy Hollidays!", m_pFontMessage);
    message.setHorizontalAlignment(gui::Text::HAlignment_Center);
    message.setVerticalAlignment(gui::Text::VAlignment_Center);*/

    //GUI->setColor(1.0f,0.1f,0.1f);
    //GUI->drawText(message, RectF(0,0,(float)GRAPHICS->getScreenWidth(),(float)GRAPHICS->getScreenHeight()),true);

    GUI->drawTexture2D(m_pLogo, vec2(	GRAPHICS->getScreenWidth()/2.0f - m_pLogo->getWidth()/2.0f,
										GRAPHICS->getScreenHeight()/2.0f - m_pLogo->getHeight()/2.0f));

    gui::Text loading("HAPPY ENGINE // LOADING", m_pFontLoading);
    loading.setHorizontalAlignment(gui::Text::HAlignment_Left);
    loading.setVerticalAlignment(gui::Text::VAlignment_Bottom);

    GUI->setColor(0.3f,0.3f,0.3f);
    GUI->drawText(loading, RectF(   10.0f, 0.0f,
                                    GRAPHICS->getScreenWidth()*1.0f,
                                    GRAPHICS->getScreenHeight() - 10.0f), true);
}

}} //end namespace