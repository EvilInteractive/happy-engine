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

#ifndef _HE_MaterialGeneratorNodeParam_H_
#define _HE_MaterialGeneratorNodeParam_H_
#pragma once

#include "ShaderGeneratorEnums.h"

namespace he {
namespace tools {

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
    MaterialGeneratorNodeParam(const he::String& name, const Type type);
    // Default copy and assignment are OK

    // Setters
    void setFloat(const float val);
    void setFloat2(const vec2& val);
    void setFloat3(const vec3& val);
    void setFloat4(const vec4& val);
    void setBool(const bool val);
    void setSwizzleMask(const ct::ShaderGeneratorSwizzleMask val);

    // Getters
    float getFloat() const;
    vec2  getFloat2() const;
    vec3  getFloat3() const;
    vec4  getFloat4() const;
    bool  getBool() const;
    ct::ShaderGeneratorSwizzleMask  getSwizzleMask() const;

    Type getType() const { return m_Type; }

private:
    he::String m_Name;
    Type m_Type;
    union Data
    {
        bool m_Bool;
        float m_Float[4];
        ct::ShaderGeneratorSwizzleMask m_Mask;
    } m_Data;
};

} } //end namespace

#endif
