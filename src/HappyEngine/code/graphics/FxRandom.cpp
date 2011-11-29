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
//Created: 27/11/2011
#include "StdAfx.h" 

#include "FxRandom.h"
#include "HappyNew.h"

namespace he {
namespace gfx {

FxRandomFloat::FxRandomFloat(): m_Min(0), m_Max(1), m_Random()
{
}
FxRandomFloat::~FxRandomFloat()
{
}
float FxRandomFloat::getValue( float /*normTime*/ ) const
{
    return m_Random.nextFloat(m_Min, m_Max);
}
void FxRandomFloat::setMin( float min )
{
    m_Min = min;
}
void FxRandomFloat::setMax( float max )
{
    m_Max = max;
}
void FxRandomFloat::setSeed( uint seed )
{
    m_Random.setSeed(seed);
}

FxRandomVec3::FxRandomVec3(): m_Min(0, 0, 0), m_Max(1, 1, 1), m_Random()
{
}
FxRandomVec3::~FxRandomVec3()
{
}
vec3 FxRandomVec3::getValue( float /*normTime*/ )  const
{
    return vec3(m_Random.nextFloat(m_Min.x, m_Max.x),
                m_Random.nextFloat(m_Min.y, m_Max.y),
                m_Random.nextFloat(m_Min.z, m_Max.z));
}
void FxRandomVec3::setMin( vec3 min )
{
    m_Min = min;
}
void FxRandomVec3::setMax( vec3 max )
{
    m_Max = max;
}
void FxRandomVec3::setSeed( uint seed )
{
    m_Random.setSeed(seed);
}

} } //end namespace