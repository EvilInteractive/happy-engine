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
    ENUM(ShadowResolution, uint8);
}
namespace ge {
struct EntityComponentDesc;
class HAPPY_ENTRY PointLightComponent : public EntityComponent
{
public:
    PointLightComponent();
    ~PointLightComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         EntityComponent                            ///
    //////////////////////////////////////////////////////////////////////////
    void visit(he::io::BinaryVisitor* const /*visitor*/) {} // override, final

    void activate(); // override, final
    void deactivate(); // override, final

    const he::FixedString& getComponentID() const { return HEFS::strPointLightComponent; } // override, final

    //// Editor //////////////////////////////////////////////////////////////
    static void fillEntityComponentDesc(EntityComponentDesc& desc);
    bool setProperty(const Property* const inProperty); // override, final
    bool getProperty(Property* const inOutProperty); // override, final
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
        
    void setMultiplier(float multiplier);
    void setAttenuation(float begin, float end);
    void setAttenuation(const vec2& att);
    void setColor(const vec3& color);
    void setColor(const Color& color);

    const vec3& getOffset() const;
    float getMultiplier() const;
    float getBeginAttenuation() const;
    float getEndAttenuation() const;
    const vec3& getColor() const;

private:  
    void init(Entity* parent); // override, final
  
    Entity* m_Parent;   

    gfx::PointLight* m_PointLight;

    float m_Multiplier;
    vec3 m_Color;
    vec2 m_Attenuation;

    //Disable default copy constructor and default assignment operator
    PointLightComponent(const PointLightComponent&);
    PointLightComponent& operator=(const PointLightComponent&);
};

class HAPPY_ENTRY SpotLightComponent : public EntityComponent
{
public:
    SpotLightComponent();
    ~SpotLightComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         EntityComponent                            ///
    //////////////////////////////////////////////////////////////////////////    
    void visit(he::io::BinaryVisitor* const /*visitor*/) {} // override, final

    void activate(); // override, final
    void deactivate(); // override, final

    const he::FixedString& getComponentID() const { return HEFS::strSpotLightComponent; } // override, final

    //// Editor //////////////////////////////////////////////////////////////
    static void fillEntityComponentDesc(EntityComponentDesc& desc);
    bool setProperty(const Property* const inProperty); // override, final
    bool getProperty(Property* const inOutProperty); // override, final
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    
    void setMultiplier(float multiplier);
    void setDirection(const vec3& direction);
    void setAttenuation(const he::vec2& att);
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
    void init(Entity* parent); // override, final

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
