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
//Created: 17/08/2011
#include "HappyPCH.h"

#include "LightManager.h"
#include "LightFactory.h"

#include "PointLight.h"
#include "SpotLight.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "Light.h"

namespace he {
namespace gfx {

LightManager::LightManager(Scene* scene)
    : m_AmbientLight(HENew(AmbientLight)())
    , m_DirectionalLight(HENew(DirectionalLight)())
    , m_Scene(scene)
{
}


LightManager::~LightManager()
{
    HEDelete(m_AmbientLight);
    HEDelete(m_DirectionalLight);
    removeAllLights();
}

AmbientLight* LightManager::setAmbientLight(const Color& color,
                                            float        multiplier)
{
    m_AmbientLight->color = color.rgb();
    m_AmbientLight->multiplier = multiplier;

    return m_AmbientLight;
}
PointLight* LightManager::addPointLight()
{
    ObjectHandle lightHandle(LightFactory::getInstance()->createPointLight());
    PointLight* light(LightFactory::getInstance()->getPointLight(lightHandle));
    m_PointLightList.add(lightHandle);

    return light;
}
SpotLight* LightManager::addSpotLight()
{
    ObjectHandle lightHandle(LightFactory::getInstance()->createSpotLight());
    SpotLight* light(LightFactory::getInstance()->getSpotLight(lightHandle));
    m_SpotLightList.add(lightHandle);

    return light;
}
DirectionalLight* LightManager::setDirectionalLight(const vec3&  direction,
                                               const Color& color,
                                               float multiplier)
{
    m_DirectionalLight->setDirection(direction);
    m_DirectionalLight->setColor(color);
    m_DirectionalLight->setMultiplier(multiplier);

    return m_DirectionalLight;
}

AmbientLight* LightManager::getAmbientLight() const
{
    return m_AmbientLight;
}
const he::ObjectList<ObjectHandle>& LightManager::getPointLights() const
{
    return m_PointLightList;
}
const he::ObjectList<ObjectHandle>& LightManager::getSpotLights() const
{
    return m_SpotLightList;
}
DirectionalLight* LightManager::getDirectionalLight() const
{
    return m_DirectionalLight;
}

void LightManager::removeAllLights()
{
    LightFactory* lightFactory(LightFactory::getInstance());
    m_PointLightList.forEach([&lightFactory](const ObjectHandle& handle)
    {
        lightFactory->destroyLight(handle);
    });
    m_SpotLightList.forEach([&lightFactory](const ObjectHandle& handle)
    {
        lightFactory->destroyLight(handle);
    });
    m_PointLightList.clear();
    m_SpotLightList.clear();
}
void LightManager::remove(const ObjectHandle& lightHandle)
{
    Light* light(LightFactory::getInstance()->get(lightHandle));
    if (light->getType() == LightType_Point)
    {
        m_PointLightList.remove(lightHandle);
    }
    else
    {
        m_SpotLightList.remove(lightHandle);
    }
    LightFactory::getInstance()->destroyLight(lightHandle);
}

} } //end namespace
