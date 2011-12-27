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

#include "FxParticleScale.h"
#include "FxParticle.h"

namespace he {
namespace gfx {

FxParticleScale::FxParticleScale()
{
}


FxParticleScale::~FxParticleScale()
{
}

void FxParticleScale::init( FxParticle* pParticle )
{
    pParticle->m_Scale = m_Scale->getValue(0);
}
void FxParticleScale::transform( FxParticle* pParticle, float currentTime, float /*dTime*/ )
{
    pParticle->m_Scale = m_Scale->getValue(currentTime);
}

void FxParticleScale::setValue( const IFxVariable<vec3>::pointer& value )
{
    m_Scale = value;
}

const IFxVariable<vec3>::pointer& FxParticleScale::getValue() const
{
    return m_Scale;
}

} } //end namespace