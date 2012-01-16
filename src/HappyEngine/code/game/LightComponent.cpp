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
#include "HappyNew.h"
#include "Entity.h"
#include "HappyEngine.h"
#include "GraphicsEngine.h"
#include "LightManager.h"
#include "Game.h"

namespace he {
namespace game {

Random PointLightComponent::s_Random;

#pragma region Pointlight
PointLightComponent::PointLightComponent(): m_Broken(false), m_IsOn(true), m_BrokenCounter(0)
{
}

PointLightComponent::~PointLightComponent()
{
    GRAPHICS->getLightManager()->remove(m_pPointLight);
    GAME->removeFromTickList(this);
}

void PointLightComponent::init(Entity* pParent)
{
    m_pParent = pParent;
    m_pPointLight = GRAPHICS->getLightManager()->addPointLight(m_OriginalPointLight.getPosition(),
                                                               Color(vec4(m_OriginalPointLight.getColor(), 1.0f)),
                                                               m_OriginalPointLight.getMultiplier(),
                                                               m_OriginalPointLight.getBeginAttenuation(),
                                                               m_OriginalPointLight.getEndAttenuation());
    GAME->addToTickList(this);
}

void PointLightComponent::serialize(SerializerStream& stream)
{
    stream << getOffset() << getMultiplier() << getBeginAttenuation() << getEndAttenuation() << 
              getColor() << getLocalTransform();
}

void PointLightComponent::deserialize(const SerializerStream& stream)
{
    vec3 off, color;
    float mult, beginAtt, endAtt;
    stream >> off >> mult >> beginAtt >> endAtt >> color >> m_mtxLocalTransform;
    
    setOffset(off);
    setColor(color);
    setMultiplier(mult);
    setAttenuation(beginAtt, endAtt);
}

void PointLightComponent::tick( float dTime )
{
    m_pPointLight->setPosition(m_pParent->getWorldMatrix() * m_mtxLocalTransform * m_OriginalPointLight.getPosition());
    if (m_Broken)
    {
        m_BrokenCounter -= dTime;
        if (m_BrokenCounter <= 0)
        {
            m_IsOn = !m_IsOn;
            m_pPointLight->setMultiplier(m_IsOn? m_OriginalPointLight.getMultiplier() : 0);
            m_BrokenCounter = s_Random.nextFloat(0, 1);
        }
    }
}

void PointLightComponent::setLocalTransform( const mat44& mtxWorld )
{
    m_mtxLocalTransform = mtxWorld;
}

const mat44& PointLightComponent::getLocalTransform() const
{
    return m_mtxLocalTransform;
}

void PointLightComponent::setOffset( const vec3& position )
{
    m_OriginalPointLight.setPosition(position);
}

void PointLightComponent::setMultiplier( float multiplier )
{
    m_OriginalPointLight.setMultiplier(multiplier);
    m_pPointLight->setMultiplier(multiplier);
}

void PointLightComponent::setAttenuation( float begin, float end )
{
    m_OriginalPointLight.setAttenuation(begin, end);
    m_pPointLight->setAttenuation(begin, end);
}

void PointLightComponent::setColor( const vec3& color )
{
    m_OriginalPointLight.setColor(color);
    m_pPointLight->setColor(color);
}

void PointLightComponent::setColor( const Color& color )
{
    m_OriginalPointLight.setColor(color);
    m_pPointLight->setColor(color);
}

const vec3& PointLightComponent::getOffset() const
{
    return m_OriginalPointLight.getPosition();
}

float PointLightComponent::getMultiplier() const
{
    return m_OriginalPointLight.getMultiplier();
}

float PointLightComponent::getBeginAttenuation() const
{
    return m_OriginalPointLight.getBeginAttenuation();
}

float PointLightComponent::getEndAttenuation() const
{
    return m_OriginalPointLight.getEndAttenuation();
}

const vec3& PointLightComponent::getColor() const
{
    return m_OriginalPointLight.getColor();
}

void he::game::PointLightComponent::setBroken( bool broken )
{
    m_Broken = broken;
}

#pragma endregion

#pragma region SpotLight
SpotLightComponent::SpotLightComponent()
{

}

SpotLightComponent::~SpotLightComponent()
{
    GRAPHICS->getLightManager()->remove(m_pSpotLight);
    GAME->removeFromTickList(this);
}

void SpotLightComponent::init( Entity* pParent )
{
    m_pParent = pParent;
    m_pSpotLight = GRAPHICS->getLightManager()->addSpotLight(
        m_OriginalSpotLight.getPosition(),
        m_OriginalSpotLight.getDirection(),
        Color(vec4(m_OriginalSpotLight.getColor(), 1.0f)),
        m_OriginalSpotLight.getMultiplier(),
        m_OriginalSpotLight.getFov(),
        m_OriginalSpotLight.getBeginAttenuation(),
        m_OriginalSpotLight.getEndAttenuation());

    GAME->addToTickList(this);
}

void SpotLightComponent::serialize( SerializerStream& stream )
{
    stream << getOffset() << getDirection() << getColor() <<
              getMultiplier() << getFov() << getBeginAttenuation() << 
              getEndAttenuation() << getLocalTransform();
}

void SpotLightComponent::deserialize( const SerializerStream& stream )
{
    vec3 off, dir, color;
    float mult, fov, bAtt, eAtt;

    stream >> off >> dir >> color >> mult >> fov >> bAtt >> eAtt >> m_mtxLocalTransform;
    
    setOffset(off);
    setDirection(dir);
    setColor(color);
    setMultiplier(mult);
    setFov(fov);
    setAttenuation(bAtt, eAtt);
}

void SpotLightComponent::tick( float /*dTime*/ )
{
    m_pSpotLight->setPosition(m_pParent->getWorldMatrix() * m_mtxLocalTransform * m_OriginalSpotLight.getPosition());
    m_pSpotLight->setDirection((m_pParent->getWorldMatrix() * m_mtxLocalTransform * vec4(m_OriginalSpotLight.getDirection(), 0.0f)).xyz());
}

void SpotLightComponent::setLocalTransform( const mat44& mtxWorld )
{
    m_mtxLocalTransform = mtxWorld;
}

const mat44& SpotLightComponent::getLocalTransform() const
{
    return m_mtxLocalTransform;
}

void SpotLightComponent::setOffset( const vec3& position )
{
    m_OriginalSpotLight.setPosition(position);
}

void SpotLightComponent::setMultiplier( float multiplier )
{
    m_OriginalSpotLight.setMultiplier(multiplier);
    m_pSpotLight->setMultiplier(multiplier);
}

void SpotLightComponent::setDirection( const vec3& direction )
{
    m_OriginalSpotLight.setDirection(direction);
}

void SpotLightComponent::setAttenuation( float begin, float end )
{
    m_OriginalSpotLight.setAttenuation(begin, end);
    m_pSpotLight->setAttenuation(begin, end);
}

void SpotLightComponent::setColor( const vec3& color )
{
    m_OriginalSpotLight.setColor(color);
    m_pSpotLight->setColor(color);    
}

void SpotLightComponent::setColor( const Color& color )
{
    m_OriginalSpotLight.setColor(color);
    m_pSpotLight->setColor(color);  
}

void SpotLightComponent::setFov( float angle )
{
    m_OriginalSpotLight.setFov(angle);
    m_pSpotLight->setFov(angle);  
}

const vec3& SpotLightComponent::getOffset() const
{
    return m_OriginalSpotLight.getPosition();
}

float SpotLightComponent::getMultiplier() const
{
    return m_OriginalSpotLight.getMultiplier();
}

const vec3& SpotLightComponent::getDirection() const
{
    return m_OriginalSpotLight.getDirection();
}

float SpotLightComponent::getBeginAttenuation() const
{
    return m_OriginalSpotLight.getBeginAttenuation();
}

float SpotLightComponent::getEndAttenuation() const
{
    return m_OriginalSpotLight.getEndAttenuation();
}

const vec3& SpotLightComponent::getColor() const
{
    return m_OriginalSpotLight.getColor();
}

float SpotLightComponent::getCosCutoff() const
{
    return m_OriginalSpotLight.getCosCutoff();
}

float SpotLightComponent::getFov() const
{
    return m_OriginalSpotLight.getFov();
}

#pragma endregion

} } //end namespace