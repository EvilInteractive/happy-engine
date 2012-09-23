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

namespace he {
namespace ge {
    
#pragma region Pointlight
PointLightComponent::PointLightComponent(): m_PointLight(nullptr)
{
}

PointLightComponent::~PointLightComponent()
{
    detach(m_PointLight);
    m_Parent->getScene()->getLightManager()->remove(m_PointLight->getHandle());
}

void PointLightComponent::init(Entity* parent)
{
    HE_ASSERT(parent != nullptr, "Parent can not be nullptr! - fatal crash");
    m_Parent = parent;
    m_PointLight =  m_Parent->getScene()->getLightManager()->addPointLight();
    attach(m_PointLight);
}

void PointLightComponent::serialize(SerializerStream& /*stream*/)
{
}

void PointLightComponent::deserialize(const SerializerStream& /*stream*/)
{
}

void PointLightComponent::setMultiplier( float multiplier )
{
    HE_ASSERT(m_PointLight != nullptr, "Attach light component first!");
    m_PointLight->setMultiplier(multiplier);
}

void PointLightComponent::setAttenuation( float begin, float end )
{
    HE_ASSERT(m_PointLight != nullptr, "Attach light component first!");
    m_PointLight->setAttenuation(begin, end);
}

void PointLightComponent::setColor( const vec3& color )
{
    HE_ASSERT(m_PointLight != nullptr, "Attach light component first!");
    m_PointLight->setColor(color);
}

void PointLightComponent::setColor( const Color& color )
{
    HE_ASSERT(m_PointLight != nullptr, "Attach light component first!");    
    m_PointLight->setColor(color);
}

float PointLightComponent::getMultiplier() const
{
    HE_ASSERT(m_PointLight != nullptr, "Attach light component first!");   
    return m_PointLight->getMultiplier();
}

float PointLightComponent::getBeginAttenuation() const
{
    HE_ASSERT(m_PointLight != nullptr, "Attach light component first!");    
    return m_PointLight->getBeginAttenuation();
}

float PointLightComponent::getEndAttenuation() const
{
    HE_ASSERT(m_PointLight != nullptr, "Attach light component first!");    
    return m_PointLight->getEndAttenuation();
}

const vec3& PointLightComponent::getColor() const
{
    HE_ASSERT(m_PointLight != nullptr, "Attach light component first!");   
    return m_PointLight->getColor();
}

#pragma endregion

#pragma region SpotLight
SpotLightComponent::SpotLightComponent(): m_SpotLight(nullptr)
{

}

SpotLightComponent::~SpotLightComponent()
{
    detach(m_SpotLight);
     m_Parent->getScene()->getLightManager()->remove(m_SpotLight->getHandle());
}

void SpotLightComponent::init( Entity* parent )
{
    HE_ASSERT(parent != nullptr, "Parent can not be nullptr! - fatal crash");
    m_Parent = parent;
    m_SpotLight =  m_Parent->getScene()->getLightManager()->addSpotLight();
    attach(m_SpotLight);
}

void SpotLightComponent::serialize( SerializerStream& /*stream*/ )
{
}

void SpotLightComponent::deserialize( const SerializerStream& /*stream*/ )
{
}

void SpotLightComponent::setMultiplier( float multiplier )
{
    HE_ASSERT(m_SpotLight != nullptr, "Attach light component first!");
    m_SpotLight->setMultiplier(multiplier);
}

void SpotLightComponent::setDirection( const vec3& direction )
{
    HE_ASSERT(m_SpotLight != nullptr, "Attach light component first!");
    m_SpotLight->setDirection(direction);
}

void SpotLightComponent::setAttenuation( float begin, float end )
{
    HE_ASSERT(m_SpotLight != nullptr, "Attach light component first!");
    m_SpotLight->setAttenuation(begin, end);
}

void SpotLightComponent::setColor( const vec3& color )
{
    HE_ASSERT(m_SpotLight != nullptr, "Attach light component first!");
    m_SpotLight->setColor(color);  
}

void SpotLightComponent::setColor( const Color& color )
{
    HE_ASSERT(m_SpotLight != nullptr, "Attach light component first!");
    m_SpotLight->setColor(color);
}

void SpotLightComponent::setFov( float angle )
{
    HE_ASSERT(m_SpotLight != nullptr, "Attach light component first!");
    m_SpotLight->setFov(angle);
}

float SpotLightComponent::getMultiplier() const
{
    HE_ASSERT(m_SpotLight != nullptr, "Attach light component first!");
    return m_SpotLight->getMultiplier();
}

const vec3& SpotLightComponent::getDirection() const
{
    HE_ASSERT(m_SpotLight != nullptr, "Attach light component first!");
    return m_SpotLight->getLocalDirection();
}

float SpotLightComponent::getBeginAttenuation() const
{
    HE_ASSERT(m_SpotLight != nullptr, "Attach light component first!");
    return m_SpotLight->getBeginAttenuation();
}

float SpotLightComponent::getEndAttenuation() const
{
    HE_ASSERT(m_SpotLight != nullptr, "Attach light component first!");
    return m_SpotLight->getEndAttenuation();
}

const vec3& SpotLightComponent::getColor() const
{
    HE_ASSERT(m_SpotLight != nullptr, "Attach light component first!");
    return m_SpotLight->getColor();
}

float SpotLightComponent::getCosCutoff() const
{
    HE_ASSERT(m_SpotLight != nullptr, "Attach light component first!");
    return m_SpotLight->getCosCutoff();
}

float SpotLightComponent::getFov() const
{
    HE_ASSERT(m_SpotLight != nullptr, "Attach light component first!");
    return m_SpotLight->getFov();
}

#pragma endregion

} } //end namespace
