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

#include "EntityComponent.h"
#include "ITickable.h"

namespace he {
namespace gfx {
    class PointLight;
    class SpotLight;
    enum ShadowResolution;
}
namespace ge {
    
class PointLightComponent : public EntityComponent, public Object3D
{
    IMPLEMENT_IOBJECT3D_FROM(Object3D)
public:
    PointLightComponent();
    virtual ~PointLightComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         EntityComponent                            ///
    //////////////////////////////////////////////////////////////////////////
    virtual void visit(he::io::BinaryVisitor* const /*visitor*/) {}

    virtual void activate();
    virtual void deactivate();
    //////////////////////////////////////////////////////////////////////////
        
    void setMultiplier(float multiplier);
    void setAttenuation(float begin, float end);
    void setColor(const vec3& color);
    void setColor(const Color& color);

    const vec3& getOffset() const;
    float getMultiplier() const;
    float getBeginAttenuation() const;
    float getEndAttenuation() const;
    const vec3& getColor() const;

protected:
    virtual void init(Entity* pParent);

private:    
    Entity* m_Parent;   

    gfx::PointLight* m_PointLight;

    float m_Multiplier;
    vec3 m_Color;
    vec2 m_Attenuation;

    //Disable default copy constructor and default assignment operator
    PointLightComponent(const PointLightComponent&);
    PointLightComponent& operator=(const PointLightComponent&);
};

class SpotLightComponent : public EntityComponent, public Object3D
{
IMPLEMENT_IOBJECT3D_FROM(Object3D)
public:
    SpotLightComponent();
    virtual ~SpotLightComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         EntityComponent                            ///
    //////////////////////////////////////////////////////////////////////////    
    virtual void visit(he::io::BinaryVisitor* const /*visitor*/) {}

    virtual void activate();
    virtual void deactivate();
    //////////////////////////////////////////////////////////////////////////
    
    void setMultiplier(float multiplier);
    void setDirection(const vec3& direction);
    void setAttenuation(float begin, float end);
    void setColor(const vec3& color);
    void setColor(const Color& color);
    void setFov(float angle);
    void setShadow(const gfx::ShadowResolution shadow);

    float getMultiplier() const;
    const vec3& getDirection() const;
    float getBeginAttenuation() const;
    float getEndAttenuation() const;
    const vec3& getColor() const;
    float getFov() const;
    gfx::ShadowResolution getShadow() const;

protected:
    virtual void init(Entity* parent);

private:
    Entity* m_Parent;

    gfx::SpotLight* m_SpotLight;

    float m_Multiplier;
    float m_Fov;
    vec2 m_Attenuation;
    vec3 m_Color;
    vec3 m_Direction;
    gfx::ShadowResolution m_ShadowResolution;

    //Disable default copy constructor and default assignment operator
    SpotLightComponent(const SpotLightComponent&);
    SpotLightComponent& operator=(const SpotLightComponent&);
};

} } //end namespace

#endif
