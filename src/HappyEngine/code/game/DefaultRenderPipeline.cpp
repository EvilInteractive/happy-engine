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
//Created: 10/11/2012
#include "HappyPCH.h" 

#include "DefaultRenderPipeline.h"

#include "View.h"

#include "Deferred3DRenderer.h"
#include "Forward3DRenderer.h"
#include "ShapeRenderer.h"
#include "Renderer2D.h"
#include "GlobalSettings.h"

namespace he {
namespace ge {

DefaultRenderPipeline::DefaultRenderPipeline()
    : m_OpacRenderer(nullptr)
    , m_TransparentRenderer(nullptr)
    , m_ShapeRenderer(nullptr)
    , m_2DRenderer(nullptr)
    , m_IsDeferred(false)
{
}


DefaultRenderPipeline::~DefaultRenderPipeline()
{
    if (m_IsDeferred == true)
        m_2DRenderer->detachFromRender(static_cast<gfx::Deferred3DRenderer*>(m_OpacRenderer));
    HEDelete(m_OpacRenderer);
    HEDelete(m_TransparentRenderer);
    HEDelete(m_ShapeRenderer);
    HEDelete(m_2DRenderer);
}

void DefaultRenderPipeline::init( gfx::View* const view, gfx::Scene* const scene )
{
    const gfx::RenderSettings settings(GlobalSettings::getInstance()->getRenderSettings());
    m_2DRenderer = HENew(gfx::Renderer2D)();
    if (settings.enableDeferred)
    {
        gfx::Deferred3DRenderer* const temp(HENew(gfx::Deferred3DRenderer)());
        temp->setScene(scene);
        m_OpacRenderer = temp;
        m_2DRenderer->attachToRender(temp);
        m_IsDeferred = true;
    }
    else
    {
        gfx::Forward3DRenderer* const temp(HENew(gfx::Forward3DRenderer)(gfx::RenderPass_Opac, true));
        temp->setScene(scene);
        m_OpacRenderer = temp;
        m_IsDeferred = false;
    }
    m_TransparentRenderer = HENew(gfx::Forward3DRenderer)(gfx::RenderPass_Translucent, true);
    m_TransparentRenderer->setScene(scene);
    m_ShapeRenderer = HENew(gfx::ShapeRenderer)();

    view->addRenderPlugin(m_OpacRenderer);
    view->addRenderPlugin(m_TransparentRenderer);
    view->addRenderPlugin(m_ShapeRenderer);
    view->addRenderPlugin(m_2DRenderer);
}

} } //end namespace
