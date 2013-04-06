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
//Created: 2013/04/01

#include "HappySandBoxPCH.h" 
#include "UIDebug.h"

#include "Sandbox.h"
#include "system/SandboxRenderPipeline.h"

#include <tools/FPSGraph.h>
#include <Renderer2D.h>

namespace hs {

UIDebug::UIDebug()
    : m_FPSGraph(nullptr)
{
}

UIDebug::~UIDebug()
{
}

void UIDebug::load()
{
    /* GUI */
    m_FPSGraph = NEW he::tools::FPSGraph();
    m_FPSGraph->setType(he::tools::FPSGraph::Type_TextOnly);
    m_FPSGraph->setPos(he::vec2(910,35));
}

void UIDebug::unload()
{
    if (isVisible()) 
    {
        hide();
    }
    delete m_FPSGraph;
    m_FPSGraph = nullptr;
}

void UIDebug::show()
{
    UI::show();
    Sandbox* const sandbox(Sandbox::getInstance());
    sandbox->getRenderPipeline()->getDebugRenderer()->attachToRender(m_FPSGraph);
    sandbox->addToTickList(m_FPSGraph);
}

void UIDebug::hide()
{
    UI::hide();
    Sandbox* const sandbox(Sandbox::getInstance());
    sandbox->getRenderPipeline()->getDebugRenderer()->detachFromRender(m_FPSGraph);
    sandbox->removeFromTickList(m_FPSGraph);
}

} //end namespace