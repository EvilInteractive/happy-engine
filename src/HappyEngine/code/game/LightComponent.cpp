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
#include "HappyPCH.h" 

#include "LightComponent.h"
#include "Entity.h"
#include "GraphicsEngine.h"
#include "Game.h"

#include "LightFactory.h"
#include "LightManager.h"
#include "SpotLight.h"
#include "PointLight.h"

#include "Scene.h"
#include "Light.h"

#include "EntityComponentDesc.h"
#include "EntityProperty.h"

namespace he {
namespace ge {
    
#pragma region Pointlight
PointLightComponent::PointLightComponent()
    : m_PointLight(nullptr)
    , m_Parent(nullptr)
    , m_Multiplier(1.0f)
    , m_Color(1.0f, 1.0f, 1.0f)
    , m_Attenuation(1.0f, 10.0f)
{
}

PointLightComponent::~PointLightComponent()
{
    HE_ASSERT(m_PointLight == nullptr, "PointLightComponent is still active when being removed!");
}

void PointLightComponent::init(Entity* parent)
{
    HE_ASSERT(parent != nullptr, "Parent can not be nullptr! - fatal crash");
    m_Parent = parent;
}

void PointLightComponent::activate()
{
    HE_IF_ASSERT(m_PointLight == nullptr, "PointLightComponent is already active!")
    HE_IF_ASSERT(m_Parent != nullptr, "PointLightComponent is not initialized!")
    {
        m_PointLight = m_Parent->getScene()->getLightManager()->addPointLight();
        m_PointLight->setMultiplier(m_Multiplier);
        m_PointLight->setColor(m_Color);
        m_PointLight->setAttenuation(m_Attenuation.x, m_Attenuation.y);
        attach(m_PointLight);
    }
}

void PointLightComponent::deactivate()
{
    HE_IF_ASSERT(m_PointLight != nullptr, "PointLightComponent is not active!")
    {
        const ObjectHandle pointlightHandle(m_PointLight->getHandle());
        detach(m_PointLight);
        m_PointLight = nullptr;
        m_Parent->getScene()->getLightManager()->remove(pointlightHandle);
    }
}

void PointLightComponent::setMultiplier( float multiplier )
{
    m_Multiplier = multiplier;
    if (m_PointLight != nullptr)
        m_PointLight->setMultiplier(multiplier);
}

void PointLightComponent::setAttenuation( float begin, float end )
{
    m_Attenuation.x = begin;
    m_Attenuation.y = end;
    if (m_PointLight != nullptr)
        m_PointLight->setAttenuation(begin, end);
}

void PointLightComponent::setAttenuation( const vec2& att )
{
    setAttenuation(att.x, att.y);
}

void PointLightComponent::setColor( const vec3& color )
{
    m_Color = color;
    if (m_PointLight != nullptr)
        m_PointLight->setColor(color);
}

void PointLightComponent::setColor( const Color& color )
{
    m_Color = color.rgb();
    if (m_PointLight != nullptr) 
        m_PointLight->setColor(color);
}

float PointLightComponent::getMultiplier() const
{
    return m_Multiplier;
}

float PointLightComponent::getBeginAttenuation() const
{
    return m_Attenuation.x;
}

float PointLightComponent::getEndAttenuation() const
{
    return m_Attenuation.y;
}

const vec3& PointLightComponent::getColor() const
{ 
    return m_Color;
}

void PointLightComponent::fillEntityComponentDesc( EntityComponentDesc& desc )
{
    desc.m_ID = HEFS::strPointLightComponent;
    desc.m_DisplayName = "Point Light";
    desc.m_Properties.clear();

    // Multiplier
    Property* mulProp(NEW Property());
    mulProp->init<float>(HEFS::strMultiplier, 1.0f);
    desc.m_Properties.add(PropertyDesc(mulProp, "Multiplier", "Sets the intensity of the light"));

    // Color
    Property* attProp(NEW Property());
    mulProp->init<vec2>(HEFS::strAttenuation, vec2(1.0f, 10.0f));
    desc.m_Properties.add(PropertyDesc(attProp, "Attenuation", "Sets the range of the light"));

    // Attenuation
    Property* colorProp(NEW Property());
    mulProp->init<vec3>(HEFS::strColor, vec3(1, 1, 1));
    desc.m_Properties.add(PropertyDesc(colorProp, "Color", "Sets the color of the light", PropertyFeel_Color));


}

bool PointLightComponent::setProperty( const Property* const inProperty )
{
    bool result(false);
    if (EntityComponent::setProperty(inProperty) == false)
    {
        const he::FixedString& id(inProperty->getName());
        if (id == HEFS::strMultiplier)
        {
            setMultiplier(inProperty->get<float>());
            result = true;
        }
        else if (id == HEFS::strAttenuation)
        {
            setAttenuation(inProperty->get<vec2>());
            result = true;
        }
        else if (id == HEFS::strColor)
        {
            setColor(inProperty->get<vec3>());
            result = true;
        }
    }
    else
    {
        result = true;
    }
    return result;
}

bool PointLightComponent::getProperty( Property* const inOutProperty )
{
    bool result(false);
    if (EntityComponent::getProperty(inOutProperty) == false)
    {
        const he::FixedString& id(inOutProperty->getName());
        if (id == HEFS::strMultiplier)
        {
            inOutProperty->set(getMultiplier());
            result = true;
        }
        else if (id == HEFS::strAttenuation)
        {
            inOutProperty->set(vec2(getBeginAttenuation(), getEndAttenuation()));
            result = true;
        }
        else if (id == HEFS::strColor)
        {
            inOutProperty->set(getColor());
            result = true;
        }
    }
    else
    {
        result = true;
    }
    return result;
}

#pragma endregion

#pragma region SpotLight

SpotLightComponent::SpotLightComponent()
    : m_SpotLight(nullptr)
    , m_Parent(nullptr)
    , m_Multiplier(1.0f)
    , m_Color(1.0f, 1.0f, 1.0f)
    , m_Attenuation(1.0f, 10.0f)
    , m_Fov(he::pi / 3.0f)
    , m_Direction(0, -1, 0)
    , m_ShadowResolution(gfx::ShadowResolution_None)
{

}

SpotLightComponent::~SpotLightComponent()
{
}

void SpotLightComponent::init( Entity* parent )
{
    HE_ASSERT(parent != nullptr, "Parent can not be nullptr! - fatal crash");
    m_Parent = parent;
}

void SpotLightComponent::activate()
{
    HE_IF_ASSERT(m_SpotLight == nullptr, "SpotLightComponent is already active!")
    HE_IF_ASSERT(m_Parent != nullptr, "SpotLightComponent is not initialized!")
    {
        m_SpotLight = m_Parent->getScene()->getLightManager()->addSpotLight();
        m_SpotLight->setMultiplier(m_Multiplier);
        m_SpotLight->setColor(m_Color);
        m_SpotLight->setAttenuation(m_Attenuation.x, m_Attenuation.y);
        m_SpotLight->setFov(m_Fov);
        m_SpotLight->setDirection(m_Direction);
        m_SpotLight->setShadowResolution(m_ShadowResolution);
        attach(m_SpotLight);
    }
}

void SpotLightComponent::deactivate()
{
    HE_IF_ASSERT(m_SpotLight != nullptr, "SpotLightComponent is not active!")
    {
        const ObjectHandle spotlightHandle(m_SpotLight->getHandle());
        detach(m_SpotLight);
        m_SpotLight = nullptr;
        m_Parent->getScene()->getLightManager()->remove(spotlightHandle);
    }
}

void SpotLightComponent::setMultiplier( float multiplier )
{
    m_Multiplier = multiplier;
    if (m_SpotLight != nullptr)
        m_SpotLight->setMultiplier(multiplier);
}

void SpotLightComponent::setDirection( const vec3& direction )
{
    m_Direction = direction;
    if (m_SpotLight != nullptr)
        m_SpotLight->setDirection(direction);
}

void SpotLightComponent::setAttenuation( float begin, float end )
{
    m_Attenuation.x = begin;
    m_Attenuation.y = end;
    if (m_SpotLight != nullptr)
        m_SpotLight->setAttenuation(begin, end);
}

void SpotLightComponent::setColor( const vec3& color )
{
    m_Color = color;
    if (m_SpotLight != nullptr)
        m_SpotLight->setColor(color);  
}

void SpotLightComponent::setColor( const Color& color )
{
    m_Color = color.rgb();
    if (m_SpotLight != nullptr)
        m_SpotLight->setColor(color);
}

void SpotLightComponent::setFov( float angle )
{
    m_Fov = angle;
    if (m_SpotLight != nullptr)
        m_SpotLight->setFov(angle);
}

void SpotLightComponent::setShadow( const gfx::ShadowResolution shadow )
{
    m_ShadowResolution = shadow;
    if (m_SpotLight != nullptr)
        m_SpotLight->setShadowResolution(shadow);
}

float SpotLightComponent::getMultiplier() const
{
    return m_Multiplier;
}

const vec3& SpotLightComponent::getDirection() const
{
    return m_Direction;
}

float SpotLightComponent::getBeginAttenuation() const
{
    return m_Attenuation.x;
}

float SpotLightComponent::getEndAttenuation() const
{
    return m_Attenuation.y;
}

const vec3& SpotLightComponent::getColor() const
{
    return m_Color;
}

float SpotLightComponent::getFov() const
{
    return m_Fov;
}

gfx::ShadowResolution SpotLightComponent::getShadow() const
{
    return m_ShadowResolution;
}

#pragma endregion

} } //end namespace
