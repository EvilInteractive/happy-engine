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

#ifndef _HE_LIGHT_MANAGER_H_
#define _HE_LIGHT_MANAGER_H_
#pragma once

namespace he {
namespace gfx {

class AmbientLight;
class DirectionalLight;
class BufferLayout;

class LightManager
{
public:
    LightManager();
    virtual ~LightManager();

    AmbientLight* setAmbientLight(const Color&    color, 
                                  float           multiplier);

    ObjectHandle addPointLight(const vec3&   pos, 
                              const Color&  color, 
                              float         multiplier, 
                              float         beginAttenuation, 
                              float         endAttentuation);

    ObjectHandle addSpotLight(const vec3&    pos, 
                            const vec3&    direction, 
                            const Color&   color, 
                            float          multiplier,
                            float          fov, //0 -> piOver2
                            float          beginAttenuation, 
                            float          endAttentuation);

    DirectionalLight* setDirectionalLight(const vec3&   direction, 
                                          const Color&  color, 
                                          float         multiplier);

    AmbientLight* getAmbientLight() const;
    DirectionalLight* getDirectionalLight() const;
    const std::vector<ObjectHandle>& getPointLights() const;
    const std::vector<ObjectHandle>& getSpotLights() const;

    void removeAllLights();
    void remove(const ObjectHandle& light);

    static const BufferLayout& getVertexLayoutLightVolume();

private:

    AmbientLight* m_AmbientLight;
    DirectionalLight* m_DirectionalLight;

    std::vector<ObjectHandle> m_PointLightVector;
    std::vector<ObjectHandle> m_SpotLightVector;

    //Disable default copy constructor and default assignment operator
    LightManager(const LightManager&);
    LightManager& operator=(const LightManager&);
};

} } //end namespace

#endif
