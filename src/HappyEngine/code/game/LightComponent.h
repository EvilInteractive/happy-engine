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
//Created: 15/01/2012

#ifndef _HE_LIGHT_COMPONENT_H_
#define _HE_LIGHT_COMPONENT_H_
#pragma once

#include "IComponent.h"
#include "ITickable.h"

#include "PointLight.h"
#include "SpotLight.h"

#include "Random.h"

namespace he {
namespace ge {
    
class PointLightComponent : public IComponent, public ITickable
{
public:
    PointLightComponent();
    virtual ~PointLightComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         IComponent                                 ///
    //////////////////////////////////////////////////////////////////////////
    virtual void init(Entity* pParent);

    virtual void serialize(SerializerStream& stream);
    virtual void deserialize(const SerializerStream& stream);
    //////////////////////////////////////////////////////////////////////////

    virtual void tick(float dTime);

    void setLocalTransform(const mat44& mtxWorld);
    const mat44& getLocalTransform() const;
    
    void setOffset(const vec3& position);
    void setMultiplier(float multiplier);
    void setAttenuation(float begin, float end);
    void setColor(const vec3& color);
    void setColor(const Color& color);

    void setBroken(bool broken);

    const vec3& getOffset() const;
    float getMultiplier() const;
    float getBeginAttenuation() const;
    float getEndAttenuation() const;
    const vec3& getColor() const;


private:
    mat44 m_mtxLocalTransform;

    bool m_Broken;
    bool m_IsOn;
    float m_BrokenCounter;
    
    Entity* m_pParent;
    
    gfx::PointLight m_OriginalPointLight;
    gfx::PointLight::pointer m_pPointLight;

    static Random s_Random;

    //Disable default copy constructor and default assignment operator
    PointLightComponent(const PointLightComponent&);
    PointLightComponent& operator=(const PointLightComponent&);
};

class SpotLightComponent : public IComponent, public ITickable
{
public:
    SpotLightComponent();
    virtual ~SpotLightComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         IComponent                                 ///
    //////////////////////////////////////////////////////////////////////////
    virtual void init(Entity* pParent);

    virtual void serialize(SerializerStream& stream);
    virtual void deserialize(const SerializerStream& stream);
    //////////////////////////////////////////////////////////////////////////

    virtual void tick(float dTime);

    void setLocalTransform(const mat44& mtxWorld);
    const mat44& getLocalTransform() const;

    void setOffset(const vec3& position);
    void setMultiplier(float multiplier);
    void setDirection(const vec3& direction);
    void setAttenuation(float begin, float end);
    void setColor(const vec3& color);
    void setColor(const Color& color);
    void setFov(float angle);

    const vec3& getOffset() const;
    float getMultiplier() const;
    const vec3& getDirection() const;
    float getBeginAttenuation() const;
    float getEndAttenuation() const;
    const vec3& getColor() const;
    float getCosCutoff() const;
    float getFov() const;


private:
    mat44 m_mtxLocalTransform;

    Entity* m_pParent;

    gfx::SpotLight m_OriginalSpotLight;
    gfx::SpotLight::pointer m_pSpotLight;

    //Disable default copy constructor and default assignment operator
    SpotLightComponent(const SpotLightComponent&);
    SpotLightComponent& operator=(const SpotLightComponent&);
};

} } //end namespace

#endif
