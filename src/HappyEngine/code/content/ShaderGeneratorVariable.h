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

    // Const
    void setConstant(const float a);
    void setConstant(const vec2& a);
    void setConstant(const vec3& a);
    void setConstant(const vec4& a);
    void setExposedVar(const ShaderGeneratorVariableType type);
    void setGlobal(const ShaderGeneratorGlobalInputVariableType type);
    void setGlobal(const ShaderGeneratorGlobalFragmentVariableType type);
    void setGlobal(const ShaderGeneratorGlobalCodeVariableType type);

    // One param
    void setAbs(const ObjectHandle& a);
    void setCeil(const ObjectHandle& a);
    void setCos(const ObjectHandle& a);
    void setFloor(const ObjectHandle& a);
    void setFrac(const ObjectHandle& a);
    void setNormalize(const ObjectHandle& a);
    void setSign(const ObjectHandle& a);
    void setSin(const ObjectHandle& a);

    // Two params
    void setAdd(const ObjectHandle& a, const ObjectHandle& b);
    void setCross(const ObjectHandle& a, const ObjectHandle& b);
    void setDistance(const ObjectHandle& a, const ObjectHandle& b);
    void setDivide(const ObjectHandle& a, const ObjectHandle& b);
    void setDot(const ObjectHandle& a, const ObjectHandle& b);
    void setMultiply(const ObjectHandle& a, const ObjectHandle& b);
    void setMin(const ObjectHandle& a, const ObjectHandle& b);
    void setPower(const ObjectHandle& a, const ObjectHandle& b);
    void setReflect(const ObjectHandle& in, const ObjectHandle& normal);
    void setSubtract(const ObjectHandle& a, const ObjectHandle& b);
    void setStep(const ObjectHandle& edge, const ObjectHandle& a);
    void setComposeFloat2(const ObjectHandle& a, const ObjectHandle& b);

    // Three params
    void setClamp(const ObjectHandle& a, const ObjectHandle& min, const ObjectHandle& max);
    void setLerp(const ObjectHandle& x, const ObjectHandle& y, const ObjectHandle& a);
    void setSmoothStep(const ObjectHandle& edge1, const ObjectHandle& edge2, const ObjectHandle& a);

    // Mutiple
    void setComposeFloat3(const ObjectHandle& a, const ObjectHandle& b, const ObjectHandle& c = ObjectHandle::unassigned);
    void setComposeFloat4(const ObjectHandle& a, const ObjectHandle& b, const ObjectHandle& c = ObjectHandle::unassigned, const ObjectHandle& d = ObjectHandle::unassigned);
    void setSwizzle(const ObjectHandle& a, const ShaderGeneratorSwizzleMask maskA, const ShaderGeneratorSwizzleMask maskB = ShaderGeneratorSwizzleMask_None, const ShaderGeneratorSwizzleMask maskC = ShaderGeneratorSwizzleMask_None, const ShaderGeneratorSwizzleMask maskD = ShaderGeneratorSwizzleMask_None);
    
    // Getters
    float getFloatData() const;
    const vec2& getFloat2Data() const;
    const vec3& getFloat3Data() const;
    const vec4& getFloat4Data() const;

    const ShaderGeneratorVariableOperation& getOperation() const { return m_Operation; }
    const ShaderGeneratorVariableType& getType() const { return m_Type; }
    
    const std::string& getLocalName() const { return m_LocalName; }
    void setLocalName(const std::string& name) { m_LocalName = name; }

    void setType(const ShaderGeneratorVariableType type) { m_Type = type; }
    ShaderGeneratorVariableType getType() { return m_Type; }

    bool hasDeclaration() const { return m_HasDeclaration; }
    void setHasDeclaration(const bool has) { m_HasDeclaration = has; }

    void resetRefcounter() { m_RefCounter = 0; }
    void incrementRefcounter() { ++m_RefCounter; }
    uint32 getRefCount() const { return m_RefCounter; }

private:
    ShaderGeneratorVariableOperation m_Operation;
    ShaderGeneratorVariableType m_Type;
    union
    {
        float floatData[4];
        uint64 handleData;
    } m_ConstantData;

    std::string m_LocalName;

    uint32 m_RefCounter;
    bool m_HasDeclaration;

    void setTypeFromOther(const ObjectHandle& handle);

    //Disable default copy constructor and default assignment operator
    ShaderGeneratorVariable(const ShaderGeneratorVariable&);
    ShaderGeneratorVariable& operator=(const ShaderGeneratorVariable&);
};

} } //end namespace

#endif
