//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#include "SpotLight.h"
#include "HappyNew.h"
#include "MathFunctions.h"
#include "HappyEngine.h"
#include "Deferred3DRenderer.h"
#include "Entity.h"
#include "ContentManager.h"

namespace he {
namespace gfx {

SpotLight::SpotLight(): m_Position(0, 0, 0), m_Multiplier(1.0f), m_Direction(0, -1, 0), m_BeginAttenuation(0.0f),
    m_Color(1.0f, 1.0f, 1.0f), m_EndAttenuation(10.0f), m_CosCutoff(0.5f)
{
    calculateWorld();

    m_Material = CONTENT->loadMaterial("engine/light/debuglight.material");
    BufferLayout vertexLayout;
    vertexLayout.addElement(BufferElement(0, BufferElement::Type_Vec3, BufferElement::Usage_Position, 12, 0));
    m_pLightVolume = CONTENT->asyncLoadModelMesh("engine/lightvolume/spotLight.binobj", "M_SpotLight", vertexLayout);
    m_pModel = m_pLightVolume;
}
void SpotLight::calculateWorld()
{
    float rangeScale = (sqrtf(1 - sqr(m_CosCutoff)) / m_CosCutoff) * m_EndAttenuation;
    m_mtxWorld = mat44::createTranslation(m_Position) * 
        mat44::createRotation(vec3::up, -atan2f(m_Direction.z, m_Direction.x)) *
        mat44::createRotation(vec3::right, -atan2f(m_Direction.y, m_Direction.z)) *   
        mat44::createScale(rangeScale, rangeScale, m_EndAttenuation);
}

SpotLight::~SpotLight()
{
}

void SpotLight::setPosition(const vec3& position)
{
    m_Position = position;
    calculateWorld();
}
void SpotLight::setMultiplier(float multiplier)
{
    m_Multiplier = multiplier;
}
void SpotLight::setDirection(const vec3& direction)
{
    m_Direction = direction;
    calculateWorld();
}
void SpotLight::setAttenuation(float begin, float end)
{
    m_BeginAttenuation = begin;
    m_EndAttenuation = end;
    calculateWorld();
}
void SpotLight::setColor(const vec3& color)
{
    m_Color = color;
}
void SpotLight::setColor(const Color& color)
{
    m_Color = color.rgb();
}
void SpotLight::setFov(float angle)
{
    m_CosCutoff = sinf(angle/2.0f);
    calculateWorld();
}

const vec3& SpotLight::getPosition() const
{
    return m_Position;
}
float SpotLight::getMultiplier() const
{
    return m_Multiplier;
}
const vec3& SpotLight::getDirection() const
{
    return m_Direction;
}
float SpotLight::getBeginAttenuation() const
{
    return m_BeginAttenuation;
}
float SpotLight::getEndAttenuation() const
{
    return m_EndAttenuation;
}
const vec3& SpotLight::getColor() const
{
    return m_Color;
}
float SpotLight::getCosCutoff() const
{
    return m_CosCutoff;
}
    
mat44 SpotLight::getWorldMatrix() const
{
    return m_mtxWorld;
}
const ModelMesh::pointer& SpotLight::getLightVolume() const
{
    return m_pLightVolume;
}
const Material& SpotLight::getMaterial() const
{
    return m_Material;
}
const ModelMesh::pointer& SpotLight::getModelMesh() const
{
    return m_pModel;
}

} } //end namespace