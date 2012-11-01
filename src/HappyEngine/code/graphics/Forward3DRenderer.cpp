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
//Created: 06/11/2011

#include "HappyPCH.h" 

#include "Forward3DRenderer.h"
#include "Scene.h"
#include "RenderTarget.h"
#include "CameraPerspective.h"
#include "IDrawable.h"
#include "CameraManager.h"
#include "View.h"

namespace he {
namespace gfx {


/* CONSTRUCTOR - DESCTRUCTOR */
Forward3DRenderer::Forward3DRenderer(DrawListContainer::BlendFilter blend): 
            m_RenderTarget(nullptr),
            m_View(nullptr),
            m_BlendFilter(blend)
{
}

Forward3DRenderer::~Forward3DRenderer()
{
}

void Forward3DRenderer::init( View3D* view, const RenderTarget* target)
{
    m_RenderTarget = target;
    m_View = view;
}

void Forward3DRenderer::draw()
{
    const Scene* scene(m_View->getScene());
    m_RenderTarget->prepareForRendering();

    GL::heSetCullFace(false);
    GL::heSetDepthFunc(DepthFunc_LessOrEqual);
    GL::heSetDepthRead(true);
    GL::heSetDepthWrite(true);
    GL::heBlendEnabled(m_BlendFilter == DrawListContainer::BlendFilter_Blend);

    const DrawListContainer& drawList(scene->getDrawList());
    const CameraPerspective* camera(m_View->getCamera());
    drawList.draw(m_BlendFilter, camera, [&camera](IDrawable* drawable)
    {
        drawable->applyMaterial(camera);
        drawable->draw();
    }); 
}


} } //end namespace