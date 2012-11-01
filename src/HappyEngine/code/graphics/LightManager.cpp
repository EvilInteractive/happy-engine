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
    : m_AmbientLight(NEW AmbientLight())
    , m_DirectionalLight(NEW DirectionalLight())
    , m_Scene(scene)
{
}


LightManager::~LightManager()
{
    delete m_AmbientLight;
    delete m_DirectionalLight;
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
    m_PointLightVector.push_back(lightHandle);

    return light;
}
SpotLight* LightManager::addSpotLight()
{
    ObjectHandle lightHandle(LightFactory::getInstance()->createSpotLight());
    SpotLight* light(LightFactory::getInstance()->getSpotLight(lightHandle));
    m_SpotLightVector.push_back(lightHandle);

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
const std::vector<ObjectHandle>& LightManager::getPointLights() const
{
    return m_PointLightVector;
}
const std::vector<ObjectHandle>& LightManager::getSpotLights() const
{
    return m_SpotLightVector;
}
DirectionalLight* LightManager::getDirectionalLight() const
{
    return m_DirectionalLight;
}

void LightManager::removeAllLights()
{
    LightFactory* lightFactory(LightFactory::getInstance());
    std::for_each(m_PointLightVector.cbegin(), m_PointLightVector.cend(), [&lightFactory](const ObjectHandle& handle)
    {
        lightFactory->destroyLight(handle);
    });
    std::for_each(m_SpotLightVector.cbegin(), m_SpotLightVector.cend(), [&lightFactory](const ObjectHandle& handle)
    {
        lightFactory->destroyLight(handle);
    });
    m_PointLightVector.clear();
    m_SpotLightVector.clear();
}
void LightManager::remove(const ObjectHandle& lightHandle)
{
    Light* light(LightFactory::getInstance()->get(lightHandle));
    if (light->getType() == LightType_Point)
    {
        m_PointLightVector.erase(std::remove(m_PointLightVector.begin(), m_PointLightVector.end(), lightHandle), m_PointLightVector.end());
    }
    else
    {
        m_SpotLightVector.erase(std::remove(m_SpotLightVector.begin(), m_SpotLightVector.end(), lightHandle), m_SpotLightVector.end());
    }
    LightFactory::getInstance()->destroyLight(lightHandle);
}

} } //end namespace
