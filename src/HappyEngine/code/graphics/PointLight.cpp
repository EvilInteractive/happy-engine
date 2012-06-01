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
//Created: 07/10/2011
#include "HappyPCH.h" 

#include "PointLight.h"

#include "ContentManager.h"
#include "ModelMesh.h"

namespace he {
namespace gfx {

PointLight::PointLight(): m_Position(0, 0, 0), m_Multiplier(1.0f), m_BeginAttenuation(0.0f),
    m_Color(1.0f, 1.0f, 1.0f), m_EndAttenuation(10.0f)
{
    calculateWorld();
    m_Material = CONTENT->loadMaterial("engine/light/debuglight.material");
    BufferLayout vertexLayout;
    vertexLayout.addElement(BufferElement(0, BufferElement::Type_Vec3, BufferElement::Usage_Position, 12, 0));
    m_LightVolume = CONTENT->asyncLoadModelMesh("engine/lightvolume/pointlight.binobj", "M_PointLight", vertexLayout);

    ResourceFactory<gfx::ModelMesh>::getInstance()->instantiate(m_LightVolume->getHandle());
    m_Model = m_LightVolume;
}

PointLight::PointLight( const PointLight& other )
: m_Position(other.m_Position)
, m_Multiplier(other.m_Multiplier)
, m_Color(other.m_Color)
, m_BeginAttenuation(other.m_BeginAttenuation)
, m_EndAttenuation(other.m_EndAttenuation)
, m_mtxWorld(other.m_mtxWorld)
, m_Material(other.m_Material)
, m_Model(other.m_Model)
, m_LightVolume(other.m_LightVolume)
{
    ResourceFactory<gfx::ModelMesh>::getInstance()->instantiate(m_Model->getHandle());
    ResourceFactory<gfx::ModelMesh>::getInstance()->instantiate(m_LightVolume->getHandle());
}

PointLight& PointLight::operator=( const PointLight& other )
{
    m_Position = other.m_Position;
    m_Multiplier = other.m_Multiplier;
    m_Color = other.m_Color;
    m_BeginAttenuation = other.m_BeginAttenuation;
    m_EndAttenuation = other.m_EndAttenuation;

    m_mtxWorld = other.m_mtxWorld;

    m_Material = other.m_Material;

    if (m_Model != nullptr)
        m_Model->release();
    if (m_LightVolume != nullptr)
        m_LightVolume->release();
    m_Model = other.m_Model;
    ResourceFactory<gfx::ModelMesh>::getInstance()->instantiate(m_Model->getHandle());
    m_LightVolume = other.m_LightVolume;
    ResourceFactory<gfx::ModelMesh>::getInstance()->instantiate(m_LightVolume->getHandle());

    return *this;
}

PointLight::~PointLight()
{
    m_Model->release();
    m_LightVolume->release();
}


void PointLight::calculateWorld()
{
    m_mtxWorld = mat44::createTranslation(m_Position) * mat44::createScale(m_EndAttenuation + 1);
}



void PointLight::setPosition(const vec3& position)
{
    if (m_Position != position)
    {
        m_Position = position;
        calculateWorld();
    }
}
void PointLight::setMultiplier(float multiplier)
{
    m_Multiplier = multiplier;
}
void PointLight::setAttenuation(float begin, float end)
{
    if (begin != m_BeginAttenuation || end != m_EndAttenuation)
    {
        m_BeginAttenuation = begin;
        m_EndAttenuation = end;
        calculateWorld();
    }
}
void PointLight::setColor(const vec3& color)
{
    m_Color = color;
}
void PointLight::setColor(const Color& color)
{
    m_Color = color.rgb();
}

const vec3& PointLight::getPosition() const
{
    return m_Position;
}
float PointLight::getMultiplier() const
{
    return m_Multiplier;
}
float PointLight::getBeginAttenuation() const
{
    return m_BeginAttenuation;
}
float PointLight::getEndAttenuation() const
{
    return m_EndAttenuation;
}
const vec3& PointLight::getColor() const
{
    return m_Color;
}
    
mat44 PointLight::getWorldMatrix() const
{
    return m_mtxWorld;
}
const ModelMesh* PointLight::getLightVolume() const
{
    return m_LightVolume;
}
const Material& PointLight::getMaterial() const
{
    return m_Material;
}
const ModelMesh* PointLight::getModelMesh() const
{
    return m_Model;
}

} } //end namespace