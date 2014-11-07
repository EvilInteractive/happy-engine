//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
#include "HappySandBoxPCH.h"
#include "MaterialGeneratorNodeParam.h"

#include "../forms/MaterialGraph.h"
#include "MaterialGeneratorNode.h"

#include <ShaderGenerator.h>
#include <ShaderGeneratorVariableFactory.h>

namespace hs {

MaterialGeneratorNodeParam::MaterialGeneratorNodeParam()
    : m_Name(""), m_Type(Type_Unknown)
{
    he_memset(&m_Data, 0, sizeof(Data));
}

MaterialGeneratorNodeParam::MaterialGeneratorNodeParam(const he::String& name, const Type type)
    : m_Name(name), m_Type(type)
{
    he_memset(&m_Data, 0, sizeof(Data));
}

MaterialGeneratorNodeParam::~MaterialGeneratorNodeParam()
{
    HE_ASSERT(m_Variable == he::ObjectHandle::unassigned, "MaterialGeneratorNodeParam was not destroyed!");
}

MaterialGeneratorNodeParam::MaterialGeneratorNodeParam( MaterialGeneratorNodeParam&& other )
    : m_Name(std::move(other.m_Name)), m_Type(other.m_Type), m_Data(other.m_Data), m_Variable(other.m_Variable)
{
    other.m_Variable = he::ObjectHandle::unassigned;
    other.m_Type = Type_Unknown;
}

MaterialGeneratorNodeParam& MaterialGeneratorNodeParam::operator=( MaterialGeneratorNodeParam&& other )
{
    m_Name = std::move(other.m_Name);
    m_Type = other.m_Type;
    m_Data = other.m_Data;
    std::swap(m_Variable, other.m_Variable);
    other.m_Type = Type_Unknown;
    return *this;
}

void MaterialGeneratorNodeParam::Init( MaterialGeneratorNode* node )
{
    switch (m_Type)
    {
    case hs::MaterialGeneratorNodeParam::Type_Float:
    case hs::MaterialGeneratorNodeParam::Type_Float2:
    case hs::MaterialGeneratorNodeParam::Type_Float3:
    case hs::MaterialGeneratorNodeParam::Type_Float4:
        {
            m_Variable = node->getParent()->getShaderGenerator()->addVariable();
        } break;
    default:
        break;
    }
}

void MaterialGeneratorNodeParam::Destroy( MaterialGeneratorNode* node )
{
    if (m_Variable != he::ObjectHandle::unassigned)
    {
        node->getParent()->getShaderGenerator()->removeVariable(m_Variable);
        m_Variable = he::ObjectHandle::unassigned;
    }
}

void MaterialGeneratorNodeParam::setFloat( const float val )
{
    HE_ASSERT(m_Type == Type_Float, "Type mismatch setting Float when var is not");
    he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());
    he::ct::ShaderGeneratorVariable* var(factory->get(m_Variable));
    var->setConstant(val);
    m_Data.m_Float[0] = val;
}

void MaterialGeneratorNodeParam::setFloat2( const he::vec2& val )
{
    HE_ASSERT(m_Type == Type_Float2, "Type mismatch setting Float2 when var is not");
    he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());
    he::ct::ShaderGeneratorVariable* var(factory->get(m_Variable));
    var->setConstant(val);
    m_Data.m_Float[0] = val.x;
    m_Data.m_Float[1] = val.y;
}

void MaterialGeneratorNodeParam::setFloat3( const he::vec3& val )
{
    HE_ASSERT(m_Type == Type_Float3, "Type mismatch setting Float3 when var is not");
    he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());
    he::ct::ShaderGeneratorVariable* var(factory->get(m_Variable));
    var->setConstant(val);
    m_Data.m_Float[0] = val.x;
    m_Data.m_Float[1] = val.y;
    m_Data.m_Float[2] = val.z;
}

void MaterialGeneratorNodeParam::setFloat4( const he::vec4& val )
{
    HE_ASSERT(m_Type == Type_Float4, "Type mismatch setting Float4 when var is not");
    he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());
    he::ct::ShaderGeneratorVariable* var(factory->get(m_Variable));
    var->setConstant(val);
    m_Data.m_Float[0] = val.x;
    m_Data.m_Float[1] = val.y;
    m_Data.m_Float[2] = val.z;
    m_Data.m_Float[3] = val.w;
}

void MaterialGeneratorNodeParam::setSwizzleMask( const he::ct::ShaderGeneratorSwizzleMask val )
{
    HE_ASSERT(m_Type == Type_SwizzleMask, "Type mismatch setting SwizzleMask when var is not");
    m_Data.m_Mask = val;
}

void MaterialGeneratorNodeParam::setBool( const bool val )
{
    HE_ASSERT(m_Type == Type_Bool, "Type mismatch setting Bool when var is not");
    he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());
    he::ct::ShaderGeneratorVariable* var(factory->get(m_Variable));
    var->setConstant(val);
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
    return he::vec2(m_Data.m_Float[0], m_Data.m_Float[1]);
}

he::vec3 MaterialGeneratorNodeParam::getFloat3() const
{
    HE_ASSERT(m_Type == Type_Float3, "Type mismatch getting Float3 when var is not");
    return he::vec3(m_Data.m_Float[0], m_Data.m_Float[1], m_Data.m_Float[2]);
}

he::vec4 MaterialGeneratorNodeParam::getFloat4() const
{
    HE_ASSERT(m_Type == Type_Float4, "Type mismatch getting Float4 when var is not");
    return he::vec4(m_Data.m_Float[0], m_Data.m_Float[1], m_Data.m_Float[2], m_Data.m_Float[3]);
}

bool MaterialGeneratorNodeParam::getBool() const
{
    HE_ASSERT(m_Type == Type_Bool, "Type mismatch getting Bool when var is not");
    return m_Data.m_Bool;
}

he::ct::ShaderGeneratorSwizzleMask MaterialGeneratorNodeParam::getSwizzleMask() const
{
    HE_ASSERT(m_Type == Type_SwizzleMask, "Type mismatch getting SwizzleMask when var is not");
    return m_Data.m_Mask;
}

he::ObjectHandle MaterialGeneratorNodeParam::getVar() const
{
    return m_Variable;
}

} //end namespace
