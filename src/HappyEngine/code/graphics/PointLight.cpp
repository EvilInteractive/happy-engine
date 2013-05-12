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

PointLight::PointLight()
    : m_Multiplier(1.0f)
    , m_Attenuation(0.0f, 10.0f)
    , m_ScaledAttenuation(0.0f, 10.0f)
    , m_Color(1.0f, 1.0f, 1.0f)
    , m_Material(nullptr)
    , m_Model(nullptr)
    , m_LightVolume(nullptr)
{
    m_Material = ResourceFactory<gfx::Material>::getInstance()->get(CONTENT->loadMaterial("engine/light/debuglight.material"));
    BufferLayout vertexLayout;
    vertexLayout.addElement(BufferElement(0, BufferElement::Type_Vec3, BufferElement::Usage_Position, 12, 0));
    m_LightVolume = CONTENT->asyncLoadModelMesh("engine/lightvolume/pointlight.binobj", "M_PointLight", vertexLayout);

    ResourceFactory<gfx::ModelMesh>::getInstance()->instantiate(m_LightVolume->getHandle());
    m_Model = m_LightVolume;
}

PointLight::PointLight( const PointLight& other )
: m_Multiplier(other.m_Multiplier)
, m_Color(other.m_Color)
, m_Attenuation(other.m_Attenuation)
, m_ScaledAttenuation(other.m_ScaledAttenuation)
, m_Material(other.m_Material)
, m_Model(other.m_Model)
, m_LightVolume(other.m_LightVolume)
{
    ResourceFactory<gfx::ModelMesh>::getInstance()->instantiate(m_Model->getHandle());
    ResourceFactory<gfx::ModelMesh>::getInstance()->instantiate(m_LightVolume->getHandle());
    ResourceFactory<gfx::Material>::getInstance()->instantiate(m_Material->getHandle());
}

PointLight& PointLight::operator=( const PointLight& other )
{
    m_Multiplier = other.m_Multiplier;
    m_Color = other.m_Color;
    m_Attenuation = other.m_Attenuation;
    m_ScaledAttenuation = other.m_ScaledAttenuation;

    if (m_Model != nullptr)
        m_Model->release();
    if (m_LightVolume != nullptr)
        m_LightVolume->release();
    if (m_Material != nullptr)
        m_Material->release();
    m_Model = other.m_Model;
    ResourceFactory<gfx::ModelMesh>::getInstance()->instantiate(m_Model->getHandle());
    m_LightVolume = other.m_LightVolume;
    ResourceFactory<gfx::ModelMesh>::getInstance()->instantiate(m_LightVolume->getHandle());
    m_Material = other.m_Material;
    ResourceFactory<gfx::Material>::getInstance()->instantiate(m_Material->getHandle());

    return *this;
}

PointLight::~PointLight()
{
    if (m_Material != nullptr)
        m_Model->release();
    if (m_Material != nullptr)
        m_LightVolume->release();
    if (m_Material != nullptr)
        m_Material->release();
}

void PointLight::setMultiplier(float multiplier)
{
    m_Multiplier = multiplier;
}
void PointLight::setAttenuation(float begin, float end)
{
    if (m_Attenuation.x != begin || m_Attenuation.y != end)
    {
        m_Attenuation.x = begin;
        m_Attenuation.y = end;
        setLocalScale(vec3(end, end, end));
        setWorldMatrixDirty(Object3D::DirtyFlag_Scale);
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

float PointLight::getMultiplier() const
{
    return m_Multiplier;
}
float PointLight::getBeginAttenuation() const
{
    return m_Attenuation.x;
}
float PointLight::getEndAttenuation() const
{
    return m_Attenuation.y;
}
float PointLight::getScaledBeginAttenuation() const
{
    return m_ScaledAttenuation.x;
}
float PointLight::getScaledEndAttenuation() const
{
    return m_ScaledAttenuation.y;
}
const vec3& PointLight::getColor() const
{
    return m_Color;
}
  
const ModelMesh* PointLight::getLightVolume() const
{
    return m_LightVolume;
}
const Material* PointLight::getMaterial() const
{
    return m_Material;
}
const ModelMesh* PointLight::getModelMesh() const
{
    return m_Model;
}

void PointLight::calculateWorldMatrix()
{
    Drawable::calculateWorldMatrix();
    float scale(length(vec3(m_WorldMatrix(0, 0), m_WorldMatrix(1, 0), m_WorldMatrix(2, 0)))); // takes x as uniform scale
    m_ScaledAttenuation = m_Attenuation * (scale / m_Attenuation.y);
}

} } //end namespace