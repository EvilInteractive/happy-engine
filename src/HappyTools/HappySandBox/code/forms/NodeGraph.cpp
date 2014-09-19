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
#include "HappySandBoxPCH.h"
#include "NodeGraph.h"

#include <ContentManager.h>
#include <Canvas2D.h>
#include <GraphicsEngine.h>
#include <Renderer2D.h>
#include <View.h>
#include <ControlsManager.h>
#include <Mouse.h>
#include <Font.h>

namespace hs {

NodeGraph::NodeGraph(QWidget *parent)
: RenderWidget(parent)
, m_View(GRAPHICS->createView())
, m_2DRenderer(NEW he::gfx::Renderer2D())
, m_Offset(0, 0)
, m_Scale(1.0f)
{
    QWidget::setMouseTracking(true);

    create(true);

    m_View->setWindow(this);
    m_View->addRenderPlugin(m_2DRenderer);
    he::gfx::CameraSettings settings;
    m_View->init(settings, true);

    m_2DRenderer->attachToRender(this);

    he::gui::Font* font(CONTENT->loadFont("Ubuntu-Medium.ttf", 14));
    m_DebugText.setFont(font);
    m_DebugText.setHorizontalAlignment(he::gui::Text::HAlignment_Left);
    font->release();
}

NodeGraph::~NodeGraph()
{
    m_2DRenderer->detachFromRender(this);

    GRAPHICS->removeView(m_View);
    delete m_2DRenderer;

    destroy();
}

he::vec2 NodeGraph::screenToWorldPos( const he::vec2& screenPos ) const
{
    return m_Offset + screenPos / m_Scale;
}

he::vec2 NodeGraph::worldToScreenPos( const he::vec2& worldPos ) const
{
    return (worldPos - m_Offset) * m_Scale;
}

void NodeGraph::draw2D( he::gui::Canvas2D* canvas )
{
    const he::vec2 canvasSize(canvas->getSize());
    const he::mat33 transform(he::mat33::createScale2D(he::vec2(m_Scale, m_Scale)) * he::mat33::createTranslation2D(-m_Offset));
    const he::vec2 transformedSize(canvasSize.x / m_Scale, canvasSize.y / m_Scale);
    const he::RectF clipRect(0.0f, 0.0f, canvasSize.x, canvasSize.y);

    // DEBUG
    m_DebugText.clear();
    const he::vec2 mouseWorld(screenToWorldPos(CONTROLS->getMouse(getHandle())->getPosition()));
    m_DebugText.addTextExt("&5F5Zoom: &AFA%.2f\n&5F5Region: &AFA%.2f, %.2f, %.2f, %.2f\n&5F5Mouse: &AFA%.2f, %.2f\n\n&5F5: Undo/Redo: &5F5\n", 
        m_Scale, m_Offset.x, m_Offset.y, transformedSize.x, transformedSize.y, mouseWorld.x, mouseWorld.y);

    canvas->setColor(he::Color(0.0f, 0.0f, 0.0f, 1.0f));
    canvas->fillText(m_DebugText, he::vec2(14.0f, 14.0f));
    canvas->setColor(he::Color(1.0f, 1.0f, 1.0f, 1.0f));
    canvas->fillText(m_DebugText, he::vec2(12.0f, 12.0f));
}

}
