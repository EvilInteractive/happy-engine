//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
//Author:  Bastian Damman
//Created: 2014/10/19

#include "HappyPlugin2DTestPCH.h" 
#include "HappyPlugin2DTestMain.h"

#include <GraphicsEngine.h>
#include <ContentManager.h>
#include <View.h>
#include <Renderer2D.h>
#include <RenderTarget.h>
#include <Font.h>
#include <Canvas2D.h>

ht::HappyPlugin2DTestMain::HappyPlugin2DTestMain()
    : m_View(nullptr)
    , m_Renderer(nullptr)
    , m_DebugText(nullptr)
{

}

ht::HappyPlugin2DTestMain::~HappyPlugin2DTestMain()
{

}

void ht::HappyPlugin2DTestMain::init(he::gfx::Window* const window, const he::RectF& relViewport)
{
    HE_INFO("HappyPluginTestMain init");
    he::gfx::GraphicsEngine* const graphicsEngine(GRAPHICS);
    m_View = graphicsEngine->createView();

    he::ct::ContentManager* contentMan(CONTENT);
    he::gfx::CameraSettings cameraSettings;
    cameraSettings.setRelativeViewport(relViewport);

    m_Renderer = NEW he::gfx::Renderer2D();
    m_View->addRenderPlugin(m_Renderer);
    m_View->setWindow(window, he::gfx::View::eViewInsertMode_First);
    m_View->init(cameraSettings);

    m_Renderer->attachToRender(this);

    he::gui::Font* const debugFont(contentMan->getDefaultFont(16));
    m_DebugText = NEW he::gui::Text();
    m_DebugText->setFont(debugFont);
    debugFont->release();
}

void ht::HappyPlugin2DTestMain::terminate()
{
    m_Renderer->detachFromRender(this);

    he::gfx::GraphicsEngine* const graphicsEngine(GRAPHICS);
    graphicsEngine->removeView(m_View);
    m_View = nullptr;

    delete m_Renderer;
    m_Renderer = nullptr;

    delete m_DebugText;
    m_DebugText = nullptr;

    HE_INFO("HappyPluginTestMain terminated");
}

void ht::HappyPlugin2DTestMain::onLoadLevel( const he::Path& /*path*/ )
{
    srand(static_cast<unsigned int>(time(0)));
    m_Grid.suspendLayout();
    m_GridInGrid.suspendLayout();

    m_Grid.initGrid(4, 5);
    m_Grid.setColumnWidth(2, 128.0f);
    m_Grid.setRowHeight(0, 128.0f);
    m_Grid.setLayoutBound(he::RectF(m_View->getViewport()));

    m_GridInGrid.initGrid(3, 3);
    m_GridInGrid.setRowHeight(0, 16.0f);
    m_GridInGrid.setColumnWidth(0, 16.0f);
    m_Grid.setAt(0, 0, 0, &m_GridInGrid);

    const static int rectCount(3*3 + 4*5 - 1);
    int hueList[rectCount];
    for (int i(0); i < rectCount; ++i)
    {
        hueList[i] = 360 / rectCount * i;
    }
    for (int i(rectCount - 1); i > 0; --i)
    {
        std::swap(hueList[i], hueList[rand()%(i+1)]);
    }

    m_RectList.resize(rectCount);
    size_t rectCounter(0);
    for (he::uint8 x(0); x < 3; ++x)
    {
        for (he::uint8 y(0); y < 3; ++y)
        {
            ColouredRect& rect(m_RectList[rectCounter++]);
            rect.setColor(he::Color::fromHSB(hueList[rectCounter-1] / 360.0f, 0.5f, 0.5f));
            rect.setLayoutMinSize(he::vec2(8.0f + rand() % 64, 8.0f + rand() % 64));
            rect.setLayoutMaxSize(rect.getLayoutMinSize() + he::vec2(1.0f + rand() % 64, 1.0f + rand() % 64));
            rect.setLayoutHAlignment(he::checked_numcast<he::gui::ELayoutHAlignment>(rand()%3));
            rect.setLayoutVAlignment(he::checked_numcast<he::gui::ELayoutVAlignment>(rand()%3));
            rect.setLayoutPadding(he::vec4(1.0f + rand() % 8, 1.0f + rand() % 8, 1.0f + rand() % 8, 1.0f + rand() % 8));
            m_GridInGrid.setAt(x, y, 0, &rect);
        }
    }
    for (he::uint8 x(0); x < 4; ++x)
    {
        for (he::uint8 y(0); y < 5; ++y)
        {
            if (x != 0 || y != 0)
            {
                ColouredRect& rect(m_RectList[rectCounter++]);
                rect.setColor(he::Color::fromHSB(hueList[rectCounter-1] / 360.0f, 1.0f, 1.0f));
                rect.setLayoutMinSize(he::vec2(16.0f + rand() % 128, 16.0f + rand() % 128));
                rect.setLayoutMaxSize(rect.getLayoutMinSize() + he::vec2(1.0f + rand() % 128, 1.0f + rand() % 128));
                rect.setLayoutHAlignment(he::checked_numcast<he::gui::ELayoutHAlignment>(rand()%3));
                rect.setLayoutVAlignment(he::checked_numcast<he::gui::ELayoutVAlignment>(rand()%3));
                rect.setLayoutPadding(he::vec4(1.0f + rand() % 8, 1.0f + rand() % 8, 1.0f + rand() % 8, 1.0f + rand() % 8));
                m_Grid.setAt(x, y, 0, &rect);
            }
        }
    }

    m_Grid.resumeLayout();
    m_GridInGrid.resumeLayout();
    for (int i(0); i < rectCount; ++i)
    {
        ColouredRect& rect(m_RectList[i]);
        rect.resumeLayout();
        rect.createDebugText();
    }
}

void ht::HappyPlugin2DTestMain::onUnloadLevel()
{
}

void ht::HappyPlugin2DTestMain::onLevelLoaded()
{

}

void ht::HappyPlugin2DTestMain::onStartGame()
{

}

void ht::HappyPlugin2DTestMain::onStopGame()
{

}

void ht::HappyPlugin2DTestMain::onPauseGame()
{

}

void ht::HappyPlugin2DTestMain::onResumeGame()
{
}

void ht::HappyPlugin2DTestMain::draw2D( he::gui::Canvas2D* canvas )
{
    m_RectList.forEach([canvas](ColouredRect& rect)
    {
        rect.draw2D(canvas);
    });
}
