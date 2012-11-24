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
//Created: 17/11/2012

#ifndef _HE_ShaderGeneratorVariable_H_
#define _HE_ShaderGeneratorVariable_H_
#pragma once

#include "ShaderGenerator.h"

namespace he {
namespace ct {

class ShaderGeneratorVariable
{
DECLARE_OBJECT(ShaderGeneratorVariable)
public:
    ShaderGeneratorVariable();
    ~ShaderGeneratorVariable();

    void setConstant(const float a);
    void setConstant(const vec2& a);
    void setConstant(const vec3& a);
    void setConstant(const vec4& a);
    void setGlobalVar(const ShaderGeneratorGlobalVariableType type);
    void setExposedVar(const ShaderGeneratorVariableType type);

    float getFloatData() const;
    const vec2& getFloat2Data() const;
    const vec3& getFloat3Data() const;
    const vec4& getFloat4Data() const;

    void setAbs(const ObjectHandle& a);
    void setAdd(const ObjectHandle& a, const ObjectHandle& b);

    const ShaderGeneratorVariableOperation& getOperation() const { return m_Operation; }
    const ShaderGeneratorVariableType& getType() const { return m_Type; }

    uint16 getFragUseCount() const { return m_VertUsageCount; }
    uint16 getVertUseCount() const { return m_FragUsageCount; }
    void IncrementVertUseCount() { ++m_VertUsageCount; }
    void IncrementFragUseCount() { ++m_FragUsageCount; }

    const std::string& getLocalName() const { return m_LocalName; }
    void setLocalName(const std::string& name) { m_LocalName = name; }

private:
    ShaderGeneratorVariableOperation m_Operation;
    ShaderGeneratorVariableType m_Type;
    union
    {
        ShaderGeneratorGlobalVariableType globalData;
        float floatData[4];
        uint64 handleData;
    } m_ConstantData;

    uint16 m_FragUsageCount;
    uint16 m_VertUsageCount;
    std::string m_LocalName;

    void setTypeFromOther(const ObjectHandle& handle);
    void setTypeFromGlobal(const ShaderGeneratorGlobalVariableType type);

    //Disable default copy constructor and default assignment operator
    ShaderGeneratorVariable(const ShaderGeneratorVariable&);
    ShaderGeneratorVariable& operator=(const ShaderGeneratorVariable&);
};

} } //end namespace

#endif
