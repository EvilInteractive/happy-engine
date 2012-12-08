//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 23/11/2012
#include "HappyPCH.h" 

#include "MaterialGeneratorGraph.h"
#include "ShaderGenerator.h"

#include "GraphicsEngine.h"
#include "ContentManager.h"
#include "Game.h"
#include "View.h"
#include "Window.h"

#include "Renderer2D.h"
#include "Canvas2D.h"

#include "NodeGraph.h"
#include "MaterialGeneratorNode.h"

#include "ControlsManager.h"
#include "IKeyboard.h"
#include "IMouse.h"

#include "MaterialGeneratorMathNodes.h"
#include "Canvas2Dnew.h"
#include "Font.h"

#define ZOOM_STEP 0.1f
#define ZOOM_MIN 0.1f
#define ZOOM_MAX 10.0f

namespace he {
namespace tools {

MaterialGeneratorGraph::MaterialGeneratorGraph()
    : m_Generator(NEW ct::ShaderGenerator)
    , m_Renderer(nullptr)
    , m_Window(nullptr)
    , m_View(nullptr)
    , m_IsActive(false)
    , m_Offset(0, 0)
    , m_Scale(1.0f)
    , m_GrabWorldPos(0.0f, 0.0f)
    , m_State(State_Idle)
{
    MaterialGeneratorNodeAdd* add(NEW MaterialGeneratorNodeAdd(this, vec2(12, 12)));
    m_NodeList.add(add);
    MaterialGeneratorNodeAdd* add2(NEW MaterialGeneratorNodeAdd(this, vec2(200, 300)));
    m_NodeList.add(add2);

    he::gfx::Font* font(CONTENT->loadFont("DejaVuSansMono.ttf", 12));
    m_DebugText.setFont(font);
    font->release();
}


MaterialGeneratorGraph::~MaterialGeneratorGraph()
{
    m_NodeList.forEach([](MaterialGeneratorNode* const node)
    {
        delete node;
    });
    m_Renderer->detachFromRender(this);
    delete m_Renderer;
    delete m_Generator;
    if (m_View != nullptr)
    {
        GRAPHICS->removeView(m_View);
    }
    if (m_Window != nullptr)
    {
        GRAPHICS->removeWindow(m_Window);
    }
}

void MaterialGeneratorGraph::init()
{
    m_Window = GRAPHICS->createWindow();
    m_View = GRAPHICS->createView();

    m_Window->setResizable(true);
    m_Window->create();
    m_Window->setCursorVisible(true);
    m_Window->setFullscreen(false);
    m_Window->setVSync(false);
    m_Window->setWindowDimension(1280, 720);
    m_Window->setWindowTitle("Happy Material Editor");
    m_Window->close();

    eventCallback0<void> lostfocusCallback([this]()
    {
        if (m_IsActive == true)
        {
            m_IsActive = false;
            GAME->removeFromTickList(this);
        }
    });
    eventCallback0<void> closeCallback([this]()
    {
        if (m_IsActive == true)
        {
            m_IsActive = false;
            GAME->removeFromTickList(this);
        }
    });
    eventCallback0<void> gainfocusCallback([this]()
    {
        if (m_IsActive == false)
        {
            m_IsActive = true;
            GAME->addToTickList(this);
        }
    });
    m_Window->GainedFocus += gainfocusCallback;
    m_Window->LostFocus += lostfocusCallback;
    m_Window->Closed += closeCallback;
    
    m_View->setWindow(m_Window);
    m_View->setRelativeViewport(he::RectF(0, 0, 1.0f, 1.0f));
    m_Renderer = NEW gfx::Renderer2D();
    m_View->addRenderPlugin(m_Renderer);
    gfx::RenderSettings settings;
    settings.enablePost = false;
    m_View->init(settings);

    m_Renderer->attachToRender(this);
}

void MaterialGeneratorGraph::open()
{
    m_Window->open();
}

void MaterialGeneratorGraph::close()
{
    m_Window->close();
}

void MaterialGeneratorGraph::tick( float /*dTime*/ )
{
    const io::ControlsManager* const controls(CONTROLS);
    //const io::IKeyboard* const keyboard(controls->getKeyboard());
    const io::IMouse* const mouse(controls->getMouse());
    switch (m_State)
    {
        case State_Idle:
        {
            if (mouse->isButtonPressed(io::MouseButton_Left) || mouse->isButtonPressed(io::MouseButton_Middle))
            {
                m_State = State_Pan;
                m_GrabWorldPos = screenToWorldPos(mouse->getPosition());
            }
        } break;
        case State_Pan:
        {
            const vec2 worldMouse(screenToWorldPos(mouse->getPosition()));
            const vec2 diff(worldMouse - m_GrabWorldPos);
            m_Offset -= diff; 

            if (mouse->isButtonReleased(io::MouseButton_Left) || mouse->isButtonReleased(io::MouseButton_Middle))
            {
                m_State = State_Idle;
            }
        } break;
        case State_MoveNode:
        {

        } break;
    }
    const int scroll(mouse->getScroll());
    if (scroll != 0)
    {
        const vec2 mousePos(mouse->getPosition());
        const vec2 mouseWorldPos(screenToWorldPos(mouse->getPosition()));
        m_Scale += mouse->getScroll() * ZOOM_STEP;
        m_Scale = he::clamp(m_Scale, ZOOM_MIN, ZOOM_MAX);
        const vec2 wrongMouseWorldPos(screenToWorldPos(mousePos));
        const vec2 offset(wrongMouseWorldPos - mouseWorldPos);
        m_Offset -= offset;
    }
}

bool MaterialGeneratorGraph::isOpen() const
{
    return m_Window->isOpen();
}

void MaterialGeneratorGraph::draw2D( gfx::Canvas2D* canvas )
{
    const mat33 transform(mat33::createScale2D(vec2(m_Scale, m_Scale)) * mat33::createTranslation2D(-m_Offset));
    const vec2 transformedSize(static_cast<float>(m_View->getViewport().width) / m_Scale, static_cast<float>(m_View->getViewport().height) / m_Scale);
    const RectF clipRect(0.0f, 0.0f, static_cast<float>(m_View->getViewport().width), static_cast<float>(m_View->getViewport().height));
    m_NodeList.forEach([canvas, &transform, &clipRect](MaterialGeneratorNode* const node)
    {
        node->draw2D(canvas, transform, clipRect);
    });

    // DEBUG
    m_DebugText.clear();
    const vec2 mouseWorld(screenToWorldPos(CONTROLS->getMouse()->getPosition()));
    m_DebugText.addTextExt("&5F5Zoom: &AFA%.2f\n&5F5Region: &AFA%.2f, %.2f, %.2f, %.2f\n&5F5Mouse: &AFA%.2f, %.2f", 
        m_Scale, m_Offset.x, m_Offset.y, transformedSize.x, transformedSize.y, mouseWorld.x, mouseWorld.y);
    gui::Canvas2Dnew* const cvs(canvas->getRenderer2D()->getNewCanvas());
    cvs->setColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
    cvs->fillText(m_DebugText, vec2(12, 12));
}

he::vec2 MaterialGeneratorGraph::screenToWorldPos( const vec2& mousePos ) const
{
    return m_Offset + mousePos / m_Scale;
}

he::vec2 MaterialGeneratorGraph::worldToScreenPos( const vec2& worldPos ) const
{
    return (worldPos - m_Offset) * m_Scale;
}

} } //end namespace
