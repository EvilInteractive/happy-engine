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
//Created: 17/10/2011
#include "HappyPCH.h" 

#include "DirectionalLight.h"
#include "HappyNew.h"
#include "MathFunctions.h"
#include "HappyEngine.h"
#include "Deferred3DRenderer.h"
#include "Entity.h"
#include "ContentManager.h"

namespace he {
namespace gfx {

DirectionalLight::DirectionalLight(): m_Multiplier(1.0f), m_Color(1.0f, 1.0f, 1.0f)
{
}


DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::setMultiplier(float multiplier)
{
    m_Multiplier = multiplier;
}
void DirectionalLight::setColor(const vec3& color)
{
    m_Color = color;
}
void DirectionalLight::setColor(const Color& color)
{
    m_Color = color.rgb();
}

float DirectionalLight::getMultiplier() const
{
    return m_Multiplier;
}
const vec3& DirectionalLight::getColor() const
{
    return m_Color;
}

void DirectionalLight::setDirection( const vec3& direction )
{
	m_Direction = direction;
}

const vec3& DirectionalLight::getDirection() const
{
	return m_Direction;
}

void DirectionalLight::setShadowMap( int index, const Texture2D::pointer& map )
{
    ASSERT(index < CASCADES, "setting shadowmap with index >= CASCADES");
    m_pShadowMap[index] = map;
}

const Texture2D::pointer& DirectionalLight::getShadowMap(int index) const
{
    ASSERT(index < CASCADES, "getting shadowmap with index >= CASCADES");
    return m_pShadowMap[index];
}

void DirectionalLight::setShadowMatrix( int index, const mat44& mtx )
{
    ASSERT(index < CASCADES, "setting shadowmatrix with index >= CASCADES");
    m_ShadowMatrix[index] = mtx;
}

const mat44& DirectionalLight::getShadowMatrix(int index) const
{
    ASSERT(index < CASCADES, "getting shadowmatrix with index >= CASCADES");
    return m_ShadowMatrix[index];
}

} } //end namespace