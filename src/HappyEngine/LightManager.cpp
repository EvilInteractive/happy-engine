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

#include "LightManager.h"
#include "HappyNew.h"

namespace happyengine {
namespace graphics {

LightManager::LightManager()
{
}


LightManager::~LightManager()
{
}

AmbientLight::pointer LightManager::addAmbientLight(const math::Vector3& pos, 
                                                    const Color&         color, 
                                                    float                multiplier, 
                                                    float                range) 
{
    AmbientLight::pointer pLight(NEW AmbientLight());
    m_AmbientLightVector.push_back(pLight);

    pLight->position = pos;
    pLight->color = color.rgb();
    pLight->multiplier = multiplier;
    pLight->range = range;

    return pLight;
}
PointLight::pointer LightManager::addPointLight(const math::Vector3&  pos, 
                                                const Color&          color, 
                                                float                 multiplier, 
                                                float                 beginAttenuation, 
                                                float                 endAttentuation)
{
    PointLight::pointer pLight(NEW PointLight());
    m_PointLightVector.push_back(pLight);

    pLight->position = pos;
    pLight->color = color.rgb();
    pLight->multiplier = multiplier;
    pLight->beginAttenuation = beginAttenuation;
    pLight->endAttenuation = endAttentuation;

    return pLight;
}
SpotLight::pointer LightManager::addSpotLight(const math::Vector3&    pos, 
                                              const math::Vector3&    direction, 
                                              const Color&            color, 
                                              float                   multiplier,
                                              float                   fov, 
                                              float                   beginAttenuation, 
                                              float                   endAttentuation)
{
    SpotLight::pointer pLight(NEW SpotLight());
    m_SpotLightVector.push_back(pLight);

    pLight->position = pos;
    pLight->color = color.rgb();
    pLight->multiplier = multiplier;
    pLight->direction = -direction;
    pLight->cosCutoff = cosf(fov);
    pLight->beginAttenuation = beginAttenuation;
    pLight->endAttenuation = endAttentuation;

    return pLight;
}
DirectionalLight::pointer LightManager::addDirectionalLight(const math::Vector3&  direction, 
                                                            const Color&          color, 
                                                            float                 multiplier)
{
    DirectionalLight::pointer pLight(NEW DirectionalLight());
    m_DirectionalLightVector.push_back(pLight);

    pLight->color = color.rgb();
    pLight->direction = -direction;
    pLight->multiplier = multiplier;

    return pLight;
}

const std::vector<AmbientLight::pointer>& LightManager::getAmbientLights() const
{
    return m_AmbientLightVector;
}
const std::vector<PointLight::pointer>& LightManager::getPointLights() const
{
    return m_PointLightVector;
}
const std::vector<SpotLight::pointer>& LightManager::getSpotLights() const
{
    return m_SpotLightVector;
}
const std::vector<DirectionalLight::pointer>& LightManager::getDirectionalLights() const
{
    return m_DirectionalLightVector;
}

void LightManager::removeAllLights()
{
    m_AmbientLightVector.clear();
    m_PointLightVector.clear();
    m_SpotLightVector.clear();
    m_DirectionalLightVector.clear();
}
void LightManager::remove(const AmbientLight::pointer& pLight)
{
    m_AmbientLightVector.erase(std::remove(m_AmbientLightVector.begin(), m_AmbientLightVector.end(), pLight), m_AmbientLightVector.cend());
}
void LightManager::remove(const PointLight::pointer& pLight)
{
    m_PointLightVector.erase(std::remove(m_PointLightVector.begin(), m_PointLightVector.end(), pLight), m_PointLightVector.cend());
}
void LightManager::remove(const SpotLight::pointer& pLight)
{
    m_SpotLightVector.erase(std::remove(m_SpotLightVector.begin(), m_SpotLightVector.end(), pLight), m_SpotLightVector.cend());
}
void LightManager::remove(const DirectionalLight::pointer& pLight)
{
    m_DirectionalLightVector.erase(std::remove(m_DirectionalLightVector.begin(), m_DirectionalLightVector.end(), pLight), m_DirectionalLightVector.cend());
}

} } //end namespace