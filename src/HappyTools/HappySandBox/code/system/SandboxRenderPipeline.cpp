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
//Created: 2013/04/01
#include "HappySandBoxPCH.h" 

#include "SandboxRenderPipeline.h"

#include "View.h"

#include "ShapeRenderer.h"
#include "Renderer2D.h"

namespace hs {

SandboxRenderPipeline::SandboxRenderPipeline()
    : m_UIRenderer(nullptr)
    , m_DebugRenderer(nullptr)
    , m_ShapeRenderer(nullptr)
{
}


SandboxRenderPipeline::~SandboxRenderPipeline()
{
    HEDelete(m_ShapeRenderer);
    HEDelete(m_UIRenderer);
    HEDelete(m_DebugRenderer);
}

void SandboxRenderPipeline::init( he::gfx::View* const view )
{
    m_ShapeRenderer = HENew(he::gfx::ShapeRenderer)();
    m_UIRenderer = HENew(UIRenderer)();
    m_DebugRenderer = HENew(DebugRenderer)();
    view->addRenderPlugin(m_ShapeRenderer);
    view->addRenderPlugin(m_UIRenderer);
    view->addRenderPlugin(m_DebugRenderer);
}

} //end namespace
