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
//Created: 17/11/2012

#ifndef _HE_ShaderGeneratorVariable_H_
#define _HE_ShaderGeneratorVariable_H_
#pragma once

#include "ShaderGenerator.h"

namespace he {
namespace ct {

class HAPPY_ENTRY ShaderGeneratorVariable
{
DECLARE_OBJECT(ShaderGeneratorVariable)
public:
    ShaderGeneratorVariable();
    ~ShaderGeneratorVariable();

    void reset();

    // Const
    bool setConstant(const float a);
    bool setConstant(const vec2& a);
    bool setConstant(const vec3& a);
    bool setConstant(const vec4& a);
    bool setConstantTex2D(const he::String& texDefault);
    bool setConstantTexCube(const he::String& texDefault);
    bool setExposed(const he::String& name); // must be already constant
    bool setGlobal(const EShaderGeneratorGlobalInputVariableType type);
    bool setGlobal(const EShaderGeneratorGlobalCameraVariableType type);
    bool setGlobal(const EShaderGeneratorGlobalFrameVariableType type);
    bool setGlobal(const EShaderGeneratorGlobalSamplerVariableType type);
    bool setGlobal(const EShaderGeneratorGlobalSceneVariableType type);
    bool setGlobal(const EShaderGeneratorGlobalOutVariableType type);

    // One param
    bool setAbs(const ObjectHandle& a);
    bool setCeil(const ObjectHandle& a);
    bool setCos(const ObjectHandle& a);
    bool setFloor(const ObjectHandle& a);
    bool setFrac(const ObjectHandle& a);
    bool setNormalize(const ObjectHandle& a);
    bool setSign(const ObjectHandle& a);
    bool setSin(const ObjectHandle& a);

    // Two params
    bool setAdd(const ObjectHandle& a, const ObjectHandle& b);
    bool setCross(const ObjectHandle& a, const ObjectHandle& b);
    bool setDistance(const ObjectHandle& a, const ObjectHandle& b);
    bool setDivide(const ObjectHandle& a, const ObjectHandle& b);
    bool setDot(const ObjectHandle& a, const ObjectHandle& b);
    bool setMultiply(const ObjectHandle& a, const ObjectHandle& b);
    bool setMin(const ObjectHandle& a, const ObjectHandle& b);
    bool setMax(const ObjectHandle& a, const ObjectHandle& b);
    bool setPower(const ObjectHandle& a, const ObjectHandle& b);
    bool setReflect(const ObjectHandle& in, const ObjectHandle& normal);
    bool setSubtract(const ObjectHandle& a, const ObjectHandle& b);
    bool setStep(const ObjectHandle& edge, const ObjectHandle& a);
    bool setComposeFloat2(const ObjectHandle& a, const ObjectHandle& b);

    // Three params
    bool setClamp(const ObjectHandle& a, const ObjectHandle& min, const ObjectHandle& max);
    bool setLerp(const ObjectHandle& x, const ObjectHandle& y, const ObjectHandle& a);
    bool setSmoothStep(const ObjectHandle& edge1, const ObjectHandle& edge2, const ObjectHandle& a);

    // Mutiple
    bool setComposeFloat3(const ObjectHandle& a, const ObjectHandle& b, const ObjectHandle& c = ObjectHandle::unassigned);
    bool setComposeFloat4(const ObjectHandle& a, const ObjectHandle& b, const ObjectHandle& c = ObjectHandle::unassigned, const ObjectHandle& d = ObjectHandle::unassigned);
    bool setSwizzle(const ObjectHandle& a, const EShaderGeneratorSwizzleMask maskA, const EShaderGeneratorSwizzleMask maskB = eShaderGeneratorSwizzleMask_None, const EShaderGeneratorSwizzleMask maskC = eShaderGeneratorSwizzleMask_None, const EShaderGeneratorSwizzleMask maskD = eShaderGeneratorSwizzleMask_None);
    
    // Custom
    bool setCalculateNormal(const ObjectHandle& a, const ObjectHandle& b, const ObjectHandle& c);
    bool setEncodeNormal(const ObjectHandle& a);

    // Getters
    float getFloatData() const;
    const vec2& getFloat2Data() const;
    const vec3& getFloat3Data() const;
    const vec4& getFloat4Data() const;

    const ShaderGeneratorVariableOperation& getOperation() const { return m_Operation; }
    
    const he::String& getLocalName() const { return m_LocalName; }
    void setLocalName(const he::String& name) { m_LocalName = name; }

    void setType(const EShaderGeneratorVariableType type) { m_Type = type; }
    EShaderGeneratorVariableType getType() const { return m_Type; }

    bool hasDeclaration() const { return m_HasDeclaration; }
    void setHasDeclaration(const bool has) { m_HasDeclaration = has; }

    void increaseUseCount() { ++m_UseCount; }
    void resetUseCount() { m_UseCount = 0; }
    uint32 getUseCount() const { return m_UseCount; }
    bool needsDeclaration() const { return m_UseCount > 1 && !m_Inline; }
    
private:
    ShaderGeneratorVariableOperation m_Operation;
    EShaderGeneratorVariableType m_Type;
    union
    {
        float floatData[4];
        uint64 handleData;
    } m_ConstantData;

    he::String m_LocalName;

    uint32 m_UseCount;
    bool m_HasDeclaration;
    bool m_Inline;

    bool setTypeFromOther(const ObjectHandle& handle);

    //Disable default copy constructor and default assignment operator
    ShaderGeneratorVariable(const ShaderGeneratorVariable&);
    ShaderGeneratorVariable& operator=(const ShaderGeneratorVariable&);
};

} } //end namespace

#endif
