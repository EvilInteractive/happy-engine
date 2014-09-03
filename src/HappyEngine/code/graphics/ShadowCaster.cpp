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
//Created: 13/10/2012
#include "HappyPCH.h" 

#include "ShadowCaster.h"

#include "Scene.h"
#include "LightManager.h"
#include "LightFactory.h"
#include "SpotLight.h"
#include "Texture2D.h"
#include "Canvas2D.h"

namespace he {
namespace gfx {

ShadowCaster::ShadowCaster()
{
}

ShadowCaster::~ShadowCaster()
{
}

void ShadowCaster::init()
{
    for (uint32 i(1); i < ShadowResolution_MAX; ++i)
    {
        m_SpotLightShadowRenderers[i - 1].init((ShadowResolution)i);
    }
}

void ShadowCaster::render(Scene* scene)
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    LightManager* lightMan(scene->getLightManager());
    LightFactory* lightFactory(LightFactory::getInstance());
    
    const he::ObjectList<ObjectHandle>& spotLights(lightMan->getSpotLights());
    spotLights.forEach([&](const ObjectHandle& handle)
    {
        SpotLight* light(lightFactory->getSpotLight(handle));
        if (light->getVisibleLastFrame() && light->getShadowResolution() != ShadowResolution_None)
        {
            m_SpotLightShadowRenderers[light->getShadowResolution() - 1].render(scene, light);
            m_LastTexture = light->getShadowMap()->getHandle();
        }
    });
}

void ShadowCaster::draw2D( he::gui::Canvas2D* canvas )
{
    if (m_LastTexture != ObjectHandle::unassigned)
    {
        TextureFactory* texFac(TextureFactory::getInstance());
        if (texFac->isAlive(m_LastTexture))
        {
            Texture2D* tex(texFac->get(m_LastTexture));
            canvas->drawImage(tex, vec2(12, 368), vec2(128, 128));
        }
        else
        {
            m_LastTexture = ObjectHandle::unassigned;
        }
    }
}


} } //end namespace
