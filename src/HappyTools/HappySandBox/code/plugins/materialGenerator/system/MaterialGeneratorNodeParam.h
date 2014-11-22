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

#ifndef _HE_MaterialGeneratorNodeParam_H_
#define _HE_MaterialGeneratorNodeParam_H_
#pragma once

#include "ShaderGeneratorEnums.h"
#include <Property.h>
#include <PropertyDesc.h>
#include <PropertyFeel.h>
#include <PropertyConverter.h>

namespace hs {
class MaterialGeneratorNode;
class MaterialGeneratorNodeParam
{
public:
    enum Type
    {
        Type_Float,
        Type_Float2,
        Type_Float3,
        Type_Float4,
        Type_Bool,
        Type_SwizzleMask,

        Type_Unknown
    };

    MaterialGeneratorNodeParam();
    MaterialGeneratorNodeParam(const he::FixedString& name, const Type type);

    ~MaterialGeneratorNodeParam();

    MaterialGeneratorNodeParam(MaterialGeneratorNodeParam&& other);
    MaterialGeneratorNodeParam& operator=(MaterialGeneratorNodeParam&& other);
    
    // Setters
    void setFloat(const float val);
    void setFloat2(const he::vec2& val);
    void setFloat3(const he::vec3& val);
    void setFloat4(const he::vec4& val);
    void setBool(const bool val);
    void setSwizzleMask(const he::ct::EShaderGeneratorSwizzleMask val);

    // Getters
    float getFloat() const;
    he::vec2  getFloat2() const;
    he::vec3  getFloat3() const;
    he::vec4  getFloat4() const;
    bool  getBool() const;
    he::ct::EShaderGeneratorSwizzleMask  getSwizzleMask() const;
    he::ge::PropertyDesc getPropertyDesc();

    Type getType() const { return m_Type; }

    static void sdmInit();
    static void sdmDestroy();

private:
    static he::ge::PropertyConverterBool* s_BoolConverter;
    static he::ge::PropertyConverterFloat* s_FloatConverter;
    static he::ge::PropertyConverterVec2* s_Vec2Converter;
    static he::ge::PropertyConverterVec3* s_Vec3Converter;
    static he::ge::PropertyConverterVec4* s_Vec4Converter;
    static he::ge::PropertyConverterEnum<he::ct::EShaderGeneratorSwizzleMask>* s_SwizzleConverter;

    static he::ge::PropertyFeelDefault* s_DefaultFeel;
    static he::ge::PropertyFeelCheckBox* s_CheckBoxFeel;
    static he::ge::PropertyFeelColor* s_ColorFeel;
    static he::ge::PropertyFeelDropDown* s_SwizzleFeel;

    he::ge::Property m_Property;
    Type m_Type;

    MaterialGeneratorNodeParam(MaterialGeneratorNodeParam&);
    MaterialGeneratorNodeParam& operator=(const MaterialGeneratorNodeParam&);
};

} //end namespace

#endif
