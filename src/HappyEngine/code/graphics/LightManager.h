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

#ifndef _HE_LIGHT_MANAGER_H_
#define _HE_LIGHT_MANAGER_H_
#pragma once

#include <vector>

#include "Light.h"
#include "vec3.h"
#include "Color.h"

namespace he {
namespace gfx {

class LightManager
{
public:
    LightManager();
    virtual ~LightManager();

    const AmbientLight::pointer& setAmbientLight(const Color&         color, 
                                                 float                multiplier);

    PointLight::pointer addPointLight(const vec3&  pos, 
                                      const Color&          color, 
                                      float                 multiplier, 
                                      float                 beginAttenuation, 
                                      float                 endAttentuation);

    SpotLight::pointer addSpotLight(const vec3&    pos, 
                                    const vec3&    direction, 
                                    const Color&            color, 
                                    float                   multiplier,
                                    float                   fov, //0 -> piOver2
                                    float                   beginAttenuation, 
                                    float                   endAttentuation);

    DirectionalLight::pointer setDirectionalLight(const vec3&  direction, 
                                                  const Color&          color, 
                                                  float                 multiplier);

    const AmbientLight::pointer& getAmbientLight() const;
    const DirectionalLight::pointer& getDirectionalLight() const;
    const std::vector<PointLight::pointer>& getPointLights() const;
    const std::vector<SpotLight::pointer>& getSpotLights() const;

    void removeAllLights();
    void remove(const PointLight::pointer& pLight);
    void remove(const SpotLight::pointer& pLight);

    static const BufferLayout& getVertexLayoutLightVolume();

private:

    AmbientLight::pointer m_pAmbientLight;
    DirectionalLight::pointer m_pDirectionalLight;

    std::vector<PointLight::pointer> m_PointLightVector;
    std::vector<SpotLight::pointer> m_SpotLightVector;

    //Disable default copy constructor and default assignment operator
    LightManager(const LightManager&);
    LightManager& operator=(const LightManager&);
};

} } //end namespace

#endif
