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
#include <ShaderGeneratorEnums.h>
#include <Property.h>

namespace hs {

he::ge::PropertyFeelDropDown* MaterialGeneratorNodeParam::s_SwizzleFeel(nullptr);
he::ge::PropertyFeelColor* MaterialGeneratorNodeParam::s_ColorFeel(nullptr);
he::ge::PropertyFeelCheckBox* MaterialGeneratorNodeParam::s_CheckBoxFeel(nullptr);
he::ge::PropertyFeelDefault* MaterialGeneratorNodeParam::s_DefaultFeel(nullptr);
he::ge::PropertyConverterEnum<he::ct::EShaderGeneratorSwizzleMask>* MaterialGeneratorNodeParam::s_SwizzleConverter(nullptr);
he::ge::PropertyConverterVec4* MaterialGeneratorNodeParam::s_Vec4Converter(nullptr);
he::ge::PropertyConverterVec3* MaterialGeneratorNodeParam::s_Vec3Converter(nullptr);
he::ge::PropertyConverterVec2* MaterialGeneratorNodeParam::s_Vec2Converter(nullptr);
he::ge::PropertyConverterFloat* MaterialGeneratorNodeParam::s_FloatConverter(nullptr);
he::ge::PropertyConverterBool* MaterialGeneratorNodeParam::s_BoolConverter(nullptr);

void MaterialGeneratorNodeParam::sdmInit()
{
    s_SwizzleFeel = HENew(he::ge::PropertyFeelDropDown)();
    s_SwizzleFeel->addItem(he::ct::shaderGeneratorSwizzleMaskToString(he::ct::eShaderGeneratorSwizzleMask_X));
    s_SwizzleFeel->addItem(he::ct::shaderGeneratorSwizzleMaskToString(he::ct::eShaderGeneratorSwizzleMask_Y));
    s_SwizzleFeel->addItem(he::ct::shaderGeneratorSwizzleMaskToString(he::ct::eShaderGeneratorSwizzleMask_Z));
    s_SwizzleFeel->addItem(he::ct::shaderGeneratorSwizzleMaskToString(he::ct::eShaderGeneratorSwizzleMask_W));
    s_SwizzleFeel->addItem(he::ct::shaderGeneratorSwizzleMaskToString(he::ct::eShaderGeneratorSwizzleMask_None));

    s_ColorFeel = HENew(he::ge::PropertyFeelColor)();
    s_CheckBoxFeel = HENew(he::ge::PropertyFeelCheckBox)();
    s_DefaultFeel = HENew(he::ge::PropertyFeelDefault)();
    
    s_SwizzleConverter = HENew(he::ge::PropertyConverterEnum<he::ct::EShaderGeneratorSwizzleMask>)(he::ct::shaderGeneratorSwizzleMaskFromString, he::ct::shaderGeneratorSwizzleMaskToString);
    s_Vec4Converter = HENew(he::ge::PropertyConverterVec4)();
    s_Vec3Converter = HENew(he::ge::PropertyConverterVec3)();
    s_Vec2Converter = HENew(he::ge::PropertyConverterVec2)();
    s_FloatConverter = HENew(he::ge::PropertyConverterFloat)();
    s_BoolConverter = HENew(he::ge::PropertyConverterBool)();
}

void MaterialGeneratorNodeParam::sdmDestroy()
{
    HEDelete(s_SwizzleFeel);
    HEDelete(s_ColorFeel);
    HEDelete(s_CheckBoxFeel);
    HEDelete(s_DefaultFeel);
    HEDelete(s_SwizzleConverter);
    HEDelete(s_Vec4Converter);
    HEDelete(s_Vec3Converter); 
    HEDelete(s_Vec2Converter); 
    HEDelete(s_FloatConverter);
    HEDelete(s_BoolConverter); 
}

MaterialGeneratorNodeParam::MaterialGeneratorNodeParam()
    : m_Type(Type_Unknown)
{
}

MaterialGeneratorNodeParam::MaterialGeneratorNodeParam(const he::FixedString& name, const Type type)
    : m_Type(type)
{
    switch (type)
    {
    case Type_Float: m_Property.init(name, 0.0f); break;
    case Type_Float2: m_Property.init(name, he::vec2(0.0f)); break;
    case Type_Float3: m_Property.init(name, he::vec3(0.0f)); break;
    case Type_Float4: m_Property.init(name, he::vec4(0.0f)); break;
    case Type_Bool: m_Property.init(name, false); break;
    case Type_SwizzleMask: m_Property.init(name, he::ct::eShaderGeneratorSwizzleMask_None); break;
    default: LOG(he::LogType_ProgrammerAssert, "Unknown material parameter type %d", type);
    }
}

MaterialGeneratorNodeParam::~MaterialGeneratorNodeParam()
{
}

MaterialGeneratorNodeParam::MaterialGeneratorNodeParam( MaterialGeneratorNodeParam&& other )
    : m_Type(other.m_Type), m_Property(std::move(other.m_Property))
{
    other.m_Type = Type_Unknown;
}

MaterialGeneratorNodeParam& MaterialGeneratorNodeParam::operator=( MaterialGeneratorNodeParam&& other )
{
    m_Type = other.m_Type;
    m_Property = std::move(other.m_Property);
    other.m_Type = Type_Unknown;
    return *this;
}
void MaterialGeneratorNodeParam::setFloat( const float val )
{
    HE_ASSERT(m_Type == Type_Float, "Type mismatch setting Float when var is not");
    m_Property.set(val);
}

void MaterialGeneratorNodeParam::setFloat2( const he::vec2& val )
{
    HE_ASSERT(m_Type == Type_Float2, "Type mismatch setting Float2 when var is not");
    m_Property.set(val);
}

void MaterialGeneratorNodeParam::setFloat3( const he::vec3& val )
{
    HE_ASSERT(m_Type == Type_Float3, "Type mismatch setting Float3 when var is not");
    m_Property.set(val);
}

void MaterialGeneratorNodeParam::setFloat4( const he::vec4& val )
{
    HE_ASSERT(m_Type == Type_Float4, "Type mismatch setting Float4 when var is not");
    m_Property.set(val);
}

void MaterialGeneratorNodeParam::setSwizzleMask( const he::ct::EShaderGeneratorSwizzleMask val )
{
    HE_ASSERT(m_Type == Type_SwizzleMask, "Type mismatch setting SwizzleMask when var is not");
    m_Property.set(val);
}

void MaterialGeneratorNodeParam::setBool( const bool val )
{
    HE_ASSERT(m_Type == Type_Bool, "Type mismatch setting Bool when var is not");
    m_Property.set(val);
}

float MaterialGeneratorNodeParam::getFloat() const
{
    HE_ASSERT(m_Type == Type_Float, "Type mismatch getting Float when var is not");
    return m_Property.get<float>();
}

he::vec2 MaterialGeneratorNodeParam::getFloat2() const
{
    HE_ASSERT(m_Type == Type_Float2, "Type mismatch getting Float2 when var is not");
    return m_Property.get<he::vec2>();
}

he::vec3 MaterialGeneratorNodeParam::getFloat3() const
{
    HE_ASSERT(m_Type == Type_Float3, "Type mismatch getting Float3 when var is not");
    return m_Property.get<he::vec3>();
}

he::vec4 MaterialGeneratorNodeParam::getFloat4() const
{
    HE_ASSERT(m_Type == Type_Float4, "Type mismatch getting Float4 when var is not");
    return m_Property.get<he::vec4>();
}

bool MaterialGeneratorNodeParam::getBool() const
{
    HE_ASSERT(m_Type == Type_Bool, "Type mismatch getting Bool when var is not");
    return m_Property.get<bool>();
}

he::ct::EShaderGeneratorSwizzleMask MaterialGeneratorNodeParam::getSwizzleMask() const
{
    HE_ASSERT(m_Type == Type_SwizzleMask, "Type mismatch getting SwizzleMask when var is not");
    return m_Property.get<he::ct::EShaderGeneratorSwizzleMask>();
}

he::ge::PropertyDesc MaterialGeneratorNodeParam::getPropertyDesc()
{
    he::ge::PropertyConverter* conv(nullptr);
    he::ge::PropertyFeel* feel(nullptr);
    switch (m_Type)
    {
    case hs::MaterialGeneratorNodeParam::Type_Float:
        conv = s_FloatConverter;
        feel = s_DefaultFeel;
        break;
    case hs::MaterialGeneratorNodeParam::Type_Float2:
        conv = s_Vec2Converter;
        feel = s_DefaultFeel;
        break;
    case hs::MaterialGeneratorNodeParam::Type_Float3:
        conv = s_Vec3Converter;
        feel = s_DefaultFeel;
        break;
    case hs::MaterialGeneratorNodeParam::Type_Float4:
        conv = s_Vec4Converter;
        feel = s_DefaultFeel;
        break;
    case hs::MaterialGeneratorNodeParam::Type_Bool:
        conv = s_BoolConverter;
        feel = s_CheckBoxFeel;
        break;
    case hs::MaterialGeneratorNodeParam::Type_SwizzleMask:
        conv = s_SwizzleConverter;
        feel = s_SwizzleFeel;
        break;
    default: LOG(he::LogType_ProgrammerAssert, "Unknown material parameter type %d", m_Type); break;
    }
    he::ge::PropertyDesc desc(&m_Property, m_Property.getName().c_str(), "", conv, feel);
    return std::move(desc);
}


} //end namespace
