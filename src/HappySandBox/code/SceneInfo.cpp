//HappySandBox Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappySandBox.
//
//    HappySandBox is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappySandBox is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappySandBox.  If not, see <http://www.gnu.org/licenses/>.
//
//Author:  Sebastiaan Sprengers
//Created: 11/12/2011

#include "SceneInfo.h"

#include "HappyPCH.h"
#include "Game.h"
#include "ContentManager.h"
#include "CameraManager.h"
#include "Happy2DRenderer.h"
#include <string>
#include "Rect.h"
#include "GraphicsEngine.h"
#include "HappyTypes.h"
#include "Camera.h"

using namespace he;

namespace happysandbox {

/* CONSTRUCTOR & DESTRUCTOR */
SceneInfo::SceneInfo() :    m_pFPSGraph(NEW he::tools::FPSGraph()),
                            m_pFont(CONTENT->loadFont("Ubuntu-Bold.ttf", 10)),
                            m_CamPos(0,0,0), m_ScreenDimensions(0,0)
{
    m_pFPSGraph->setType(1);

    m_pFPSGraph->setPos(he::vec2(GRAPHICS->getViewport().width - 105.0f, 5.0f));
}

SceneInfo::~SceneInfo()
{
    delete m_pFPSGraph;
}

/* GENERAL */
void SceneInfo::tick(float dTime)
{
    vec2 screenDimensions(static_cast<float>(GRAPHICS->getScreenWidth()), static_cast<float>(GRAPHICS->getScreenHeight()));

    if (m_ScreenDimensions != screenDimensions)
    {
        m_ScreenDimensions = screenDimensions;

        m_pFPSGraph->setPos(vec2(m_ScreenDimensions.x - 105.0f, 5.0f));
    }

    m_pFPSGraph->tick(dTime, 0.5f);

	m_CamPos = CAMERAMANAGER->getActiveCamera()->getPosition();
}

void SceneInfo::draw()
{
    m_pFPSGraph->draw();

    // current camera position
    std::stringstream stream;
    std::string camPos = "CamPos:";

    for (he::uint i(0); i < 3; ++i)
    {
        std::string temp;

        if (i < 1)
            stream << m_CamPos.x;
        else if (i < 2)
            stream << m_CamPos.y;
        else
            stream << m_CamPos.z;

        temp = stream.str();

        if (temp.find('.') != temp.npos)
            temp = temp.substr(0, temp.find('.') + 4);

        camPos += " ";
        camPos += temp;

        if (i < 2)
            camPos += " ;";

        stream.str("");
    }

    gui::Text txt(camPos, m_pFont);
    txt.setHorizontalAlignment(gui::Text::HAlignment_Right);
    txt.setVerticalAlignment(gui::Text::VAlignment_Top);

    GUI->drawText(txt, RectF(m_ScreenDimensions.x - 205.0f, 35.0f, 200.0f, 20.0f));
}

} //end namespace