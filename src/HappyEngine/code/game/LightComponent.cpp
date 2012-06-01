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

namespace he {
namespace ge {

Random PointLightComponent::s_Random;

#pragma region Pointlight
PointLightComponent::PointLightComponent(): m_PointLight(ObjectHandle::unassigned)
{
}

PointLightComponent::~PointLightComponent()
{
    GRAPHICS->getLightManager()->remove(m_PointLight);
    GAME->removeFromTickList(this);
}

void PointLightComponent::init(Entity* pParent)
{
    m_Parent = pParent;
    m_PointLight = GRAPHICS->getLightManager()->addPointLight(m_Offset,
                                                               Color(1.0f, 1.0f, 1.0f),
                                                               1.0f,
                                                               0.0f,
                                                               5.0f);
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

void PointLightComponent::tick( float /*dTime*/ )
{
    if (m_Parent != nullptr)
    {
        gfx::PointLight* light(gfx::LightFactory::getInstance()->getPointLight(m_PointLight));
        mat44 parentWorld(m_Parent->getWorldMatrix());
        light->setPosition(parentWorld * m_mtxLocalTransform * m_Offset);
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

void PointLightComponent::setOffset( const vec3& offset )
{
    m_Offset = offset;
    if (m_Parent == nullptr)
    {
        HE_ASSERT(m_PointLight != ObjectHandle::unassigned, "Attach light component first!");
        gfx::PointLight* light(gfx::LightFactory::getInstance()->getPointLight(m_PointLight));
        light->setPosition(m_Offset);
    }
}

void PointLightComponent::setMultiplier( float multiplier )
{
    HE_ASSERT(m_PointLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::PointLight* light(gfx::LightFactory::getInstance()->getPointLight(m_PointLight));
    light->setMultiplier(multiplier);
}

void PointLightComponent::setAttenuation( float begin, float end )
{
    HE_ASSERT(m_PointLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::PointLight* light(gfx::LightFactory::getInstance()->getPointLight(m_PointLight));
    light->setAttenuation(begin, end);
}

void PointLightComponent::setColor( const vec3& color )
{
    HE_ASSERT(m_PointLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::PointLight* light(gfx::LightFactory::getInstance()->getPointLight(m_PointLight));
    light->setColor(color);
}

void PointLightComponent::setColor( const Color& color )
{
    HE_ASSERT(m_PointLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::PointLight* light(gfx::LightFactory::getInstance()->getPointLight(m_PointLight));
    light->setColor(color);
}

const vec3& PointLightComponent::getOffset() const
{
    HE_ASSERT(m_PointLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::PointLight* light(gfx::LightFactory::getInstance()->getPointLight(m_PointLight));
    return light->getPosition();
}

float PointLightComponent::getMultiplier() const
{
    HE_ASSERT(m_PointLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::PointLight* light(gfx::LightFactory::getInstance()->getPointLight(m_PointLight));
    return light->getMultiplier();
}

float PointLightComponent::getBeginAttenuation() const
{
    HE_ASSERT(m_PointLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::PointLight* light(gfx::LightFactory::getInstance()->getPointLight(m_PointLight));
    return light->getBeginAttenuation();
}

float PointLightComponent::getEndAttenuation() const
{
    HE_ASSERT(m_PointLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::PointLight* light(gfx::LightFactory::getInstance()->getPointLight(m_PointLight));
    return light->getEndAttenuation();
}

const vec3& PointLightComponent::getColor() const
{
    HE_ASSERT(m_PointLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::PointLight* light(gfx::LightFactory::getInstance()->getPointLight(m_PointLight));
    return light->getColor();
}

#pragma endregion

#pragma region SpotLight
SpotLightComponent::SpotLightComponent(): m_SpotLight(ObjectHandle::unassigned)
{

}

SpotLightComponent::~SpotLightComponent()
{
    if (m_SpotLight != ObjectHandle::unassigned)
    {
        GRAPHICS->getLightManager()->remove(m_SpotLight);
        GAME->removeFromTickList(this);
    }
}

void SpotLightComponent::init( Entity* parent )
{
    m_Parent = parent;
    m_SpotLight = GRAPHICS->getLightManager()->addSpotLight(
        m_Offset,
        m_Direction,
        Color(1.0f, 1.0f, 1.0f),
        1.0f,
        piOverFour,
        0,
        5.0f);

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
    if (m_Parent != nullptr)
    {
        mat44 parentWorld(m_Parent->getWorldMatrix());
        gfx::SpotLight* light(gfx::LightFactory::getInstance()->getSpotLight(m_SpotLight));
        light->setPosition(parentWorld * m_mtxLocalTransform * m_Offset);
        light->setDirection((parentWorld * m_mtxLocalTransform * vec4(m_Direction, 0.0f)).xyz());
    }
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
    m_Offset = position;
    if (m_Parent == nullptr)
    {
        gfx::SpotLight* light(gfx::LightFactory::getInstance()->getSpotLight(m_SpotLight));
        light->setPosition(position);
    }
}

void SpotLightComponent::setMultiplier( float multiplier )
{
    HE_ASSERT(m_SpotLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::SpotLight* light(gfx::LightFactory::getInstance()->getSpotLight(m_SpotLight));
    light->setMultiplier(multiplier);
}

void SpotLightComponent::setDirection( const vec3& direction )
{
    m_Direction = direction;
    if (m_Parent == nullptr)
    {
        gfx::SpotLight* light(gfx::LightFactory::getInstance()->getSpotLight(m_SpotLight));
        light->setDirection(direction);
    }
}

void SpotLightComponent::setAttenuation( float begin, float end )
{
    HE_ASSERT(m_SpotLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::SpotLight* light(gfx::LightFactory::getInstance()->getSpotLight(m_SpotLight));
    light->setAttenuation(begin, end);
}

void SpotLightComponent::setColor( const vec3& color )
{
    HE_ASSERT(m_SpotLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::SpotLight* light(gfx::LightFactory::getInstance()->getSpotLight(m_SpotLight));
    light->setColor(color);  
}

void SpotLightComponent::setColor( const Color& color )
{
    HE_ASSERT(m_SpotLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::SpotLight* light(gfx::LightFactory::getInstance()->getSpotLight(m_SpotLight));
    light->setColor(color);
}

void SpotLightComponent::setFov( float angle )
{
    HE_ASSERT(m_SpotLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::SpotLight* light(gfx::LightFactory::getInstance()->getSpotLight(m_SpotLight));
    light->setFov(angle);
}

const vec3& SpotLightComponent::getOffset() const
{
    return m_Offset;
}

float SpotLightComponent::getMultiplier() const
{
    HE_ASSERT(m_SpotLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::SpotLight* light(gfx::LightFactory::getInstance()->getSpotLight(m_SpotLight));
    return light->getMultiplier();
}

const vec3& SpotLightComponent::getDirection() const
{
    return m_Direction;
}

float SpotLightComponent::getBeginAttenuation() const
{
    HE_ASSERT(m_SpotLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::SpotLight* light(gfx::LightFactory::getInstance()->getSpotLight(m_SpotLight));
    return light->getBeginAttenuation();
}

float SpotLightComponent::getEndAttenuation() const
{
    HE_ASSERT(m_SpotLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::SpotLight* light(gfx::LightFactory::getInstance()->getSpotLight(m_SpotLight));
    return light->getEndAttenuation();
}

const vec3& SpotLightComponent::getColor() const
{
    HE_ASSERT(m_SpotLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::SpotLight* light(gfx::LightFactory::getInstance()->getSpotLight(m_SpotLight));
    return light->getColor();
}

float SpotLightComponent::getCosCutoff() const
{
    HE_ASSERT(m_SpotLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::SpotLight* light(gfx::LightFactory::getInstance()->getSpotLight(m_SpotLight));
    return light->getCosCutoff();
}

float SpotLightComponent::getFov() const
{
    HE_ASSERT(m_SpotLight != ObjectHandle::unassigned, "Attach light component first!");
    gfx::SpotLight* light(gfx::LightFactory::getInstance()->getSpotLight(m_SpotLight));
    return light->getFov();
}

#pragma endregion

} } //end namespace