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
    bool setExposedVar(const ShaderGeneratorVariableType type);
    bool setGlobal(const ShaderGeneratorGlobalInputVariableType type);
    bool setGlobal(const ShaderGeneratorGlobalFragmentVariableType type);
    bool setGlobal(const ShaderGeneratorGlobalCodeVariableType type);
    bool setGlobal(const ShaderGeneratorOutVariableType type);

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
    bool setSwizzle(const ObjectHandle& a, const ShaderGeneratorSwizzleMask maskA, const ShaderGeneratorSwizzleMask maskB = ShaderGeneratorSwizzleMask_None, const ShaderGeneratorSwizzleMask maskC = ShaderGeneratorSwizzleMask_None, const ShaderGeneratorSwizzleMask maskD = ShaderGeneratorSwizzleMask_None);
    
    // Custom
    bool setCalculateNormal(const ObjectHandle& a, const ObjectHandle& b, const ObjectHandle& c);
    bool setEncodeNormal(const ObjectHandle& a);

    // Getters
    float getFloatData() const;
    const vec2& getFloat2Data() const;
    const vec3& getFloat3Data() const;
    const vec4& getFloat4Data() const;

    const ShaderGeneratorVariableOperation& getOperation() const { return m_Operation; }
    const ShaderGeneratorVariableType& getType() const { return m_Type; }
    
    const he::String& getLocalName() const { return m_LocalName; }
    void setLocalName(const he::String& name) { m_LocalName = name; }

    void setType(const ShaderGeneratorVariableType type) { m_Type = type; }
    ShaderGeneratorVariableType getType() { return m_Type; }

    bool hasDeclaration() const { return m_HasDeclaration; }
    void setHasDeclaration(const bool has) { m_HasDeclaration = has; }

    bool getForceInline() const { return m_ForceInline; }
    void setForceInline(const bool force) { m_ForceInline = force; }

    bool getForceDeclare() const { return m_ForceDeclare; }
    void setForceDeclare(const bool force) { m_ForceDeclare = force; }

    void resetRefcounter() { m_RefCounter = 0; }
    void incrementRefcounter() { ++m_RefCounter; }
    uint32 getRefCount() const { return m_RefCounter; }

    bool declareVar() const { return (m_RefCounter > 1 || m_ForceDeclare) && !m_ForceInline; }

private:
    ShaderGeneratorVariableOperation m_Operation;
    ShaderGeneratorVariableType m_Type;
    union
    {
        float floatData[4];
        uint64 handleData;
    } m_ConstantData;

    he::String m_LocalName;

    uint32 m_RefCounter;
    bool m_HasDeclaration;
    bool m_ForceInline;
    bool m_ForceDeclare;

    bool setTypeFromOther(const ObjectHandle& handle);

    //Disable default copy constructor and default assignment operator
    ShaderGeneratorVariable(const ShaderGeneratorVariable&);
    ShaderGeneratorVariable& operator=(const ShaderGeneratorVariable&);
};

} } //end namespace

#endif
