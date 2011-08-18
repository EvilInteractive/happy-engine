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

#ifndef _LIGHT_MANAGER_H_
#define _LIGHT_MANAGER_H_
#pragma once

#include <vector>

#include "Light.h"
#include "Vector3.h"
#include "Color.h"

namespace happyengine {
namespace graphics {

class LightManager
{
public:
	LightManager();
    virtual ~LightManager();

    AmbientLight::pointer addAmbientLight(const math::Vector3& pos, 
                                          const Color&         color, 
                                          float                multiplier, 
                                          float                range);

    PointLight::pointer addPointLight(const math::Vector3&  pos, 
                                      const Color&          color, 
                                      float                 multiplier, 
                                      float                 beginAttenuation, 
                                      float                 endAttentuation);

    SpotLight::pointer addSpotLight(const math::Vector3&    pos, 
                                    const math::Vector3&    direction, 
                                    const Color&            color, 
                                    float                   multiplier,
                                    float                   fov, //0 -> piOver2
                                    float                   beginAttenuation, 
                                    float                   endAttentuation);

    DirectionalLight::pointer addDirectionalLight(const math::Vector3&  direction, 
                                                  const Color&          color, 
                                                  float                 multiplier);

    const std::vector<AmbientLight::pointer>& getAmbientLights() const;
    const std::vector<PointLight::pointer>& getPointLights() const;
    const std::vector<SpotLight::pointer>& getSpotLights() const;
    const std::vector<DirectionalLight::pointer>& getDirectionalLights() const;

    void removeAllLights();
    void remove(const AmbientLight::pointer& pLight);
    void remove(const PointLight::pointer& pLight);
    void remove(const SpotLight::pointer& pLight);
    void remove(const DirectionalLight::pointer& pLight);

private:

    std::vector<AmbientLight::pointer> m_AmbientLightVector;
    std::vector<PointLight::pointer> m_PointLightVector;
    std::vector<SpotLight::pointer> m_SpotLightVector;
    std::vector<DirectionalLight::pointer> m_DirectionalLightVector;

    //Disable default copy constructor and default assignment operator
    LightManager(const LightManager&);
    LightManager& operator=(const LightManager&);
};

} } //end namespace

#endif
