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
    MaterialGeneratorNodeParam(const he::String& name, const Type type);

    ~MaterialGeneratorNodeParam();

    MaterialGeneratorNodeParam(MaterialGeneratorNodeParam&& other);
    MaterialGeneratorNodeParam& operator=(MaterialGeneratorNodeParam&& other);

    void Init(MaterialGeneratorNode* node);
    void Destroy(MaterialGeneratorNode* node);

    // Setters
    void setFloat(const float val);
    void setFloat2(const he::vec2& val);
    void setFloat3(const he::vec3& val);
    void setFloat4(const he::vec4& val);
    void setBool(const bool val);
    void setSwizzleMask(const he::ct::ShaderGeneratorSwizzleMask val);

    // Getters
    float getFloat() const;
    he::vec2  getFloat2() const;
    he::vec3  getFloat3() const;
    he::vec4  getFloat4() const;
    bool  getBool() const;
    he::ct::ShaderGeneratorSwizzleMask  getSwizzleMask() const;

    he::ObjectHandle getVar() const;

    Type getType() const { return m_Type; }

private:
    he::String m_Name;
    he::ObjectHandle m_Variable;
    Type m_Type;
    union Data
    {
        bool m_Bool;
        float m_Float[4];
        he::ct::ShaderGeneratorSwizzleMask m_Mask;
    } m_Data;


    MaterialGeneratorNodeParam(MaterialGeneratorNodeParam&);
    MaterialGeneratorNodeParam& operator=(const MaterialGeneratorNodeParam&);
};

} //end namespace

#endif
