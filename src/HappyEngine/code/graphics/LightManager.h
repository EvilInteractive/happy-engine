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

#ifndef _HE_LIGHT_MANAGER_H_
#define _HE_LIGHT_MANAGER_H_
#pragma once

namespace he {
namespace gfx {

class AmbientLight;
class DirectionalLight;
class VertexLayout;

class SpotLight;
class PointLight;

class Scene;

struct ToneMapData
{
    ToneMapData()
        : shoulderStrength(0.2f)
        , linearStrength(0.52f)
        , linearAngle(1.14f)
        , toeStrength(0.81f)
        , toeNumerator(0.04f)
        , toeDenominator(1.1f)
        , exposureBias(0.55f)
    {}

    float shoulderStrength;
    float linearStrength;
    float linearAngle;
    float toeStrength;
    float toeNumerator;
    float toeDenominator;
    float exposureBias;
};

class HAPPY_ENTRY LightManager
{
public:
    LightManager(Scene* scene);
    virtual ~LightManager();

    AmbientLight* setAmbientLight(const Color&    color, 
                                  float           multiplier);

    PointLight* addPointLight();
    SpotLight* addSpotLight();

    DirectionalLight* setDirectionalLight(const vec3&   direction, 
                                          const Color&  color, 
                                          float         multiplier);

    AmbientLight* getAmbientLight() const;
    DirectionalLight* getDirectionalLight() const;
    const he::ObjectList<ObjectHandle>& getPointLights() const;
    const he::ObjectList<ObjectHandle>& getSpotLights() const;

    void removeAllLights();
    void remove(const ObjectHandle& light);

    static const VertexLayout& getVertexLayoutLightVolume();

    const ToneMapData& getToneMapData() const { return m_ToneMapData; }
    ToneMapData& getToneMapDataForEdit() { return m_ToneMapData; }

private:
    Scene* m_Scene;

    AmbientLight* m_AmbientLight;
    DirectionalLight* m_DirectionalLight;

    he::ObjectList<ObjectHandle> m_PointLightList;
    he::ObjectList<ObjectHandle> m_SpotLightList;

    ToneMapData m_ToneMapData;

    //Disable default copy constructor and default assignment operator
    LightManager(const LightManager&);
    LightManager& operator=(const LightManager&);
};

} } //end namespace

#endif
