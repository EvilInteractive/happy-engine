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

#include "FxParticleRotation.h"
#include "FxParticle.h"

namespace he {
namespace gfx {

FxParticleRotation::FxParticleRotation()
{
}


FxParticleRotation::~FxParticleRotation()
{
}

void FxParticleRotation::init( FxParticle* pParticle, const mat44& /*parentWorld*/ )
{
    pParticle->m_Rotation = m_Rotation->getValue(0);
}
void FxParticleRotation::transform( FxParticle* pParticle, float currentTime, float /*dTime*/ )
{
    pParticle->m_Rotation = m_Rotation->getValue(currentTime);
}

void FxParticleRotation::setValue( const IFxVariable<float>::pointer& value )
{
    m_Rotation = value;
}
const IFxVariable<float>::pointer& FxParticleRotation::getValue() const
{
    return m_Rotation;
}


} } //end namespace