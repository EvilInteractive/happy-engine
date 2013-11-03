//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 16/12/2012
#include "HappyPCH.h"

#include "MaterialGeneratorNodeParam.h"

namespace he {
namespace tools {

MaterialGeneratorNodeParam::MaterialGeneratorNodeParam()
    : m_Name("Unknown param"), m_Type(Type_Unknown)
{
    he_memset(&m_Data, 0, sizeof(Data));
}

MaterialGeneratorNodeParam::MaterialGeneratorNodeParam(const he::String& name, const Type type)
    : m_Name(name), m_Type(type)
{
    he_memset(&m_Data, 0, sizeof(Data));
}

void MaterialGeneratorNodeParam::setFloat( const float val )
{
    HE_ASSERT(m_Type == Type_Float, "Type mismatch setting Float when var is not");
    m_Data.m_Float[0] = val;
}

void MaterialGeneratorNodeParam::setFloat2( const vec2& val )
{
    HE_ASSERT(m_Type == Type_Float2, "Type mismatch setting Float2 when var is not");
    m_Data.m_Float[0] = val.x;
    m_Data.m_Float[1] = val.y;
}

void MaterialGeneratorNodeParam::setFloat3( const vec3& val )
{
    HE_ASSERT(m_Type == Type_Float3, "Type mismatch setting Float3 when var is not");
    m_Data.m_Float[0] = val.x;
    m_Data.m_Float[1] = val.y;
    m_Data.m_Float[2] = val.z;
}

void MaterialGeneratorNodeParam::setFloat4( const vec4& val )
{
    HE_ASSERT(m_Type == Type_Float4, "Type mismatch setting Float4 when var is not");
    m_Data.m_Float[0] = val.x;
    m_Data.m_Float[1] = val.y;
    m_Data.m_Float[2] = val.z;
    m_Data.m_Float[3] = val.w;
}

void MaterialGeneratorNodeParam::setSwizzleMask( const ct::ShaderGeneratorSwizzleMask val )
{
    HE_ASSERT(m_Type == Type_SwizzleMask, "Type mismatch setting Float4 when var is not");
    m_Data.m_Mask = val;
}

void MaterialGeneratorNodeParam::setBool( const bool val )
{
    HE_ASSERT(m_Type == Type_Bool, "Type mismatch setting Bool when var is not");
    m_Data.m_Bool = val;
}

float MaterialGeneratorNodeParam::getFloat() const
{
    HE_ASSERT(m_Type == Type_Float, "Type mismatch getting Float when var is not");
    return m_Data.m_Float[0];
}

he::vec2 MaterialGeneratorNodeParam::getFloat2() const
{
    HE_ASSERT(m_Type == Type_Float2, "Type mismatch getting Float2 when var is not");
    return vec2(m_Data.m_Float[0], m_Data.m_Float[1]);
}

he::vec3 MaterialGeneratorNodeParam::getFloat3() const
{
    HE_ASSERT(m_Type == Type_Float3, "Type mismatch getting Float3 when var is not");
    return vec3(m_Data.m_Float[0], m_Data.m_Float[1], m_Data.m_Float[2]);
}

he::vec4 MaterialGeneratorNodeParam::getFloat4() const
{
    HE_ASSERT(m_Type == Type_Float4, "Type mismatch getting Float4 when var is not");
    return vec4(m_Data.m_Float[0], m_Data.m_Float[1], m_Data.m_Float[2], m_Data.m_Float[3]);
}

bool MaterialGeneratorNodeParam::getBool() const
{
    HE_ASSERT(m_Type == Type_Bool, "Type mismatch getting Bool when var is not");
    return m_Data.m_Bool;
}

ct::ShaderGeneratorSwizzleMask MaterialGeneratorNodeParam::getSwizzleMask() const
{
    HE_ASSERT(m_Type == Type_SwizzleMask, "Type mismatch getting SwizzleMask when var is not");
    return m_Data.m_Mask;
}

} } //end namespace
