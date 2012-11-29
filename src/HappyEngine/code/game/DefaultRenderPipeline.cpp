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
//Created: 10/11/2012
#include "HappyPCH.h" 

#include "DefaultRenderPipeline.h"

#include "View.h"

#include "Deferred3DRenderer.h"
#include "Forward3DRenderer.h"
#include "ShapeRenderer.h"
#include "Renderer2D.h"
#include "Picker.h"

namespace he {
namespace ge {

DefaultRenderPipeline::DefaultRenderPipeline()
    : m_OpacRenderer(nullptr)
    , m_TransparentRenderer(nullptr)
    , m_ShapeRenderer(nullptr)
    , m_2DRenderer(nullptr)
    , m_Picker(nullptr)
{
}


DefaultRenderPipeline::~DefaultRenderPipeline()
{
    delete m_OpacRenderer;
    delete m_TransparentRenderer;
    delete m_ShapeRenderer;
    delete m_2DRenderer;
    delete m_Picker;
}

void DefaultRenderPipeline::init( gfx::View* view, gfx::Scene* scene, const gfx::RenderSettings& settings )
{
    if (settings.enableDeferred)
    {
        gfx::Deferred3DRenderer* temp(NEW gfx::Deferred3DRenderer());
        temp->setScene(scene);
        m_OpacRenderer = temp;
    }
    else
    {
        gfx::Forward3DRenderer* temp(NEW gfx::Forward3DRenderer(gfx::RenderPass_Opac, true));
        temp->setScene(scene);
        m_OpacRenderer = temp;
    }
    m_TransparentRenderer = NEW gfx::Forward3DRenderer(gfx::RenderPass_Translucent, true);
    m_TransparentRenderer->setScene(scene);
    m_ShapeRenderer = NEW gfx::ShapeRenderer();
    m_2DRenderer = NEW gfx::Renderer2D();
    m_Picker = NEW gfx::Picker();
    m_Picker->init(view, scene);

    view->addRenderPlugin(m_OpacRenderer);
    view->addRenderPlugin(m_TransparentRenderer);
    view->addRenderPlugin(m_ShapeRenderer);
    view->addRenderPlugin(m_2DRenderer);
}

} } //end namespace
