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
//Created: 27/12/2011
#include "HappyPCH.h" 

#include "FxParticleSpeed.h"
#include "FxParticle.h"

namespace he {
namespace gfx {

FxParticleSpeed::FxParticleSpeed()
{
}


FxParticleSpeed::~FxParticleSpeed()
{
}

void FxParticleSpeed::init( FxParticle* pParticle, const mat44& parentWorld )
{
    pParticle->m_Velocity = (parentWorld * vec4(m_Speed->getValue(0.0f), 0.0f)).xyz();
}
void FxParticleSpeed::transform( FxParticle* pParticle, float currentTime, float /*dTime*/, const mat44& parentWorld )
{
    pParticle->m_Velocity = (parentWorld * vec4(m_Speed->getValue(currentTime), 0.0f)).xyz();
}

void FxParticleSpeed::setValue( const IFxVariable<vec3>::pointer& value )
{
    m_Speed = value;
}
const IFxVariable<vec3>::pointer& FxParticleSpeed::getValue() const
{
    return m_Speed;
}

} } //end namespace