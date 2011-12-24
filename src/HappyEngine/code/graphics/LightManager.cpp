//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
#include "HappyNew.h"

namespace he {
namespace gfx {

LightManager::LightManager(): m_pAmbientLight(NEW AmbientLight()), m_pDirectionalLight(NEW DirectionalLight())
{
}


LightManager::~LightManager()
{
}

const AmbientLight::pointer& LightManager::setAmbientLight(const Color&         color,
                                                    float                multiplier)
{
    m_pAmbientLight->color = color.rgb();
    m_pAmbientLight->multiplier = multiplier;

    return m_pAmbientLight;
}
PointLight::pointer LightManager::addPointLight(const vec3&  pos,
                                                const Color&          color,
                                                float                 multiplier,
                                                float                 beginAttenuation,
                                                float                 endAttentuation)
{
    PointLight::pointer pLight(NEW PointLight());
    m_PointLightVector.push_back(pLight);

    pLight->setPosition(pos);
    pLight->setColor(color);
    pLight->setMultiplier(multiplier);
    pLight->setAttenuation(beginAttenuation, endAttentuation);

    return pLight;
}
SpotLight::pointer LightManager::addSpotLight(const vec3&    pos,
                                              const vec3&    direction,
                                              const Color&            color,
                                              float                   multiplier,
                                              float                   fov,
                                              float                   beginAttenuation,
                                              float                   endAttentuation)
{
    SpotLight::pointer pLight(NEW SpotLight());
    m_SpotLightVector.push_back(pLight);

    pLight->setPosition(pos);
    pLight->setColor(color);
    pLight->setMultiplier(multiplier);
    pLight->setDirection(-direction);
    pLight->setFov(fov);
    pLight->setAttenuation(beginAttenuation, endAttentuation);

    return pLight;
}
DirectionalLight::pointer LightManager::setDirectionalLight(const vec3&  direction,
                                                            const Color& color,
                                                            float multiplier)
{
    m_pDirectionalLight->setDirection(direction);
    m_pDirectionalLight->setColor(color);
    m_pDirectionalLight->setMultiplier(multiplier);

    return m_pDirectionalLight;
}

const AmbientLight::pointer& LightManager::getAmbientLight() const
{
    return m_pAmbientLight;
}
const std::vector<PointLight::pointer>& LightManager::getPointLights() const
{
    return m_PointLightVector;
}
const std::vector<SpotLight::pointer>& LightManager::getSpotLights() const
{
    return m_SpotLightVector;
}
const DirectionalLight::pointer& LightManager::getDirectionalLight() const
{
    return m_pDirectionalLight;
}

void LightManager::removeAllLights()
{
    m_PointLightVector.clear();
    m_SpotLightVector.clear();
    //m_DirectionalLightVector.clear();
}
void LightManager::remove(const PointLight::pointer& pLight)
{
    m_PointLightVector.erase(std::remove(m_PointLightVector.begin(), m_PointLightVector.end(), pLight), m_PointLightVector.end());
}
void LightManager::remove(const SpotLight::pointer& pLight)
{
    m_SpotLightVector.erase(std::remove(m_SpotLightVector.begin(), m_SpotLightVector.end(), pLight), m_SpotLightVector.end());
}

} } //end namespace
