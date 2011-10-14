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
#include "StdAfx.h" 

#include "PointLight.h"
#include "HappyNew.h"
#include "MathFunctions.h"
#include "HappyEngine.h"
#include "Deferred3DRenderer.h"
#include "Entity.h"
#include "ContentManager.h"

namespace he {
namespace gfx {

PointLight::PointLight(): m_Position(0, 0, 0), m_Multiplier(1.0f), m_BeginAttenuation(0.0f),
    m_Color(1.0f, 1.0f, 1.0f), m_EndAttenuation(10.0f)
{
    calculateWorld();
    game::EntityDesc desc(CONTENT->loadEntity("engine/light/pointlight.entity"));
    m_Material = desc.material;
    m_pLightVolume = CONTENT->asyncLoadModel("engine/lightvolume/pointlight.binobj", Deferred3DRenderer::getVertexLayoutLightVolume());
    m_pModel = desc.pModel;
}
void PointLight::calculateWorld()
{
    m_mtxWorld =  mat44::createTranslation(m_Position) * mat44::createScale(m_EndAttenuation);
}


PointLight::~PointLight()
{
}

void PointLight::setPosition(const vec3& position)
{
    m_Position = position;
    calculateWorld();
}
void PointLight::setMultiplier(float multiplier)
{
    m_Multiplier = multiplier;
}
void PointLight::setAttenuation(float begin, float end)
{
    m_BeginAttenuation = begin;
    m_EndAttenuation = end;
    calculateWorld();
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
    
const mat44& PointLight::getWorldMatrix() const
{
    return m_mtxWorld;
}
const Model::pointer& PointLight::getLightVolume() const
{
    return m_pLightVolume;
}
const Material& PointLight::getMaterial() const
{
    return m_Material;
}
const Model::pointer PointLight::getModel() const
{
    return m_pModel;
}

} } //end namespace