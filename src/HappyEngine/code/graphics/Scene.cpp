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
//Created: 14/07/2012
#include "HappyPCH.h" 

#include "Scene.h"

#include "DrawListContainer.h"
#include "IDrawable.h"

#include "CameraManager.h"
#include "InstancingManager.h"
#include "LightManager.h"

namespace he {
namespace gfx {
IMPLEMENT_OBJECT(Scene)

#pragma warning(disable:4355) // use of this in initializerlist
Scene::Scene():
          m_CameraManager(NEW ge::CameraManager)
        , m_LightManager(NEW LightManager(this))
        , m_InstancingManager(NEW InstancingManager)
{
}
#pragma warning(default:4355)


Scene::~Scene()
{
    delete m_LightManager;
    delete m_InstancingManager;
    delete m_CameraManager;
}

void Scene::forceReevalute( IDrawable* drawable )
{
    m_DrawList.forceReevalute(drawable);
}

void Scene::doReevalute( IDrawable* drawable )
{
    m_DrawList.doReevalute(drawable);
}

void Scene::attachToScene( IDrawable* drawable )
{
    drawable->calculateBound();
    m_DrawList.insert(drawable);
    drawable->setScene(this);
}

void Scene::detachFromScene( IDrawable* drawable )
{
    m_DrawList.remove(drawable);
    drawable->setScene(nullptr);
}

void Scene::prepareForRendering()
{
    m_DrawList.prepareForRendering();
}

} } //end namespace