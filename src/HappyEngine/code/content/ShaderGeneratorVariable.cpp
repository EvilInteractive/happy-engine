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
#include "HappyPCH.h" 

#include "ShaderGeneratorVariable.h"
#include "ShaderGeneratorVariableFactory.h"

namespace he {
namespace ct {

IMPLEMENT_OBJECT(ShaderGeneratorVariable)

ShaderGeneratorVariable::ShaderGeneratorVariable()
: m_Type(eShaderGeneratorVariableType_Unknown)
, m_LocalName("")
, m_UseCount(0)
, m_HasDeclaration(false)
, m_Inline(false)
{
    he_memset(&m_ConstantData, 0, sizeof(m_ConstantData));
}


ShaderGeneratorVariable::~ShaderGeneratorVariable()
{
}

void ShaderGeneratorVariable::reset()
{
    m_Type = eShaderGeneratorVariableType_Unknown;
    he_memset(&m_ConstantData, 0, sizeof(m_ConstantData));
}

bool ShaderGeneratorVariable::setConstant( const float a )
{
    m_Operation.type = eShaderGeneratorVariableOperationType_Constant;
    m_Operation.scope = eShaderGeneratorVariableScope_Local;
    m_Type = eShaderGeneratorVariableType_Float;
    m_ConstantData.floatData[0] = a;
    m_Inline = true;
    return true;
}
bool ShaderGeneratorVariable::setConstant( const vec2& a )
{
    m_Operation.type = eShaderGeneratorVariableOperationType_Constant;
    m_Operation.scope = eShaderGeneratorVariableScope_Local;
    m_Type = eShaderGeneratorVariableType_Float2;
    m_ConstantData.floatData[0] = a.x;
    m_ConstantData.floatData[1] = a.y;
    return true;
}
bool ShaderGeneratorVariable::setConstant( const vec3& a )
{
    m_Operation.type = eShaderGeneratorVariableOperationType_Constant;
    m_Operation.scope = eShaderGeneratorVariableScope_Local;
    m_Type = eShaderGeneratorVariableType_Float3;
    m_ConstantData.floatData[0] = a.x;
    m_ConstantData.floatData[1] = a.y;
    m_ConstantData.floatData[2] = a.z;
    return true;
}
bool ShaderGeneratorVariable::setConstant( const vec4& a )
{
    m_Operation.type = eShaderGeneratorVariableOperationType_Constant;
    m_Operation.scope = eShaderGeneratorVariableScope_Local;
    m_Type = eShaderGeneratorVariableType_Float4;
    m_ConstantData.floatData[0] = a.x;
    m_ConstantData.floatData[1] = a.y;
    m_ConstantData.floatData[2] = a.z;
    m_ConstantData.floatData[3] = a.w;
    return true;
}

bool ShaderGeneratorVariable::setExposed( const he::String& name )
{
    if (m_Operation.type == eShaderGeneratorVariableOperationType_Constant)
    {
        setLocalName(name);
        m_Operation.type = eShaderGeneratorVariableOperationType_Named;
        m_Operation.scope = eShaderGeneratorVariableScope_Exposed;
        return true;
    }
    else
    {
        LOG(he::LogType_ProgrammerAssert, "Variable must be constant before making it exposed!");
        return false;
    }
}

bool ShaderGeneratorVariable::setGlobal(const EShaderGeneratorGlobalInputVariableType type)
{
    setLocalName(he::String(getGlobalInputVariableName(type)));
    setType(getGlobalInputVariableType(type));
    m_Operation.type = eShaderGeneratorVariableOperationType_Named;
    m_Operation.scope = eShaderGeneratorVariableScope_GlobalInput;
    return true;
}
bool ShaderGeneratorVariable::setGlobal(const EShaderGeneratorGlobalCameraVariableType type)
{
    setLocalName(he::String(getGlobalCameraVariableName(type)));
    setType(getGlobalCameraVariableType(type));
    m_Operation.type = eShaderGeneratorVariableOperationType_Named;
    m_Operation.scope = eShaderGeneratorVariableScope_GlobalCamera;
    return true;
}
bool ShaderGeneratorVariable::setGlobal(const EShaderGeneratorGlobalFrameVariableType type)
{
    setLocalName(he::String(getGlobalFrameVariableName(type)));
    setType(getGlobalFrameVariableType(type));
    m_Operation.type = eShaderGeneratorVariableOperationType_Named;
    m_Operation.scope = eShaderGeneratorVariableScope_GlobalFrame;
    return true;
}
bool ShaderGeneratorVariable::setGlobal(const EShaderGeneratorGlobalSamplerVariableType type)
{
    setLocalName(he::String(getGlobalSamplerVariableName(type)));
    setType(getGlobalSamplerVariableType(type));
    m_Operation.type = eShaderGeneratorVariableOperationType_Named;
    m_Operation.scope = eShaderGeneratorVariableScope_GlobalSampler;
    return true;
}
bool ShaderGeneratorVariable::setGlobal(const EShaderGeneratorGlobalSceneVariableType type)
{
    setLocalName(he::String(getGlobalSceneVariableName(type)));
    setType(getGlobalSceneVariableType(type));
    m_Operation.type = eShaderGeneratorVariableOperationType_Named;
    m_Operation.scope = eShaderGeneratorVariableScope_GlobalScene;
    return true;
}
bool ShaderGeneratorVariable::setGlobal(const EShaderGeneratorGlobalOutVariableType type)
{
    setLocalName(he::String(getOutVariableName(type)));
    setType(getOutVariableType(type));
    m_Operation.type = eShaderGeneratorVariableOperationType_Named;
    m_Operation.scope = eShaderGeneratorVariableScope_GlobalOutput;
    return true;
}


#define FUNC1(name, oper) \
bool ShaderGeneratorVariable::name( const ObjectHandle& a )\
{\
    m_Operation.type = oper; \
    m_Operation.scope = eShaderGeneratorVariableScope_Local;\
    m_Operation.params[0] = a; \
    m_Operation.params[1] = ObjectHandle::unassigned; \
    m_Operation.params[2] = ObjectHandle::unassigned; \
    m_Operation.params[3] = ObjectHandle::unassigned; \
    return setTypeFromOther(a);\
}

#define FUNC2(name, oper) \
bool ShaderGeneratorVariable::name( const ObjectHandle& a, const ObjectHandle& b )\
{\
    m_Operation.type = oper; \
    m_Operation.scope = eShaderGeneratorVariableScope_Local;\
    m_Operation.params[0] = a; \
    m_Operation.params[1] = b; \
    m_Operation.params[2] = ObjectHandle::unassigned; \
    m_Operation.params[3] = ObjectHandle::unassigned; \
    return setTypeFromOther(a); \
}

#define FUNC3(name, oper) \
bool ShaderGeneratorVariable::name( const ObjectHandle& a, const ObjectHandle& b, const ObjectHandle& c )\
{\
    m_Operation.type = oper; \
    m_Operation.scope = eShaderGeneratorVariableScope_Local;\
    m_Operation.params[0] = a; \
    m_Operation.params[1] = b; \
    m_Operation.params[2] = c; \
    m_Operation.params[3] = ObjectHandle::unassigned; \
    return setTypeFromOther(a); \
}

#define FUNC4(name, oper) \
bool ShaderGeneratorVariable::name( const ObjectHandle& a, const ObjectHandle& b, const ObjectHandle& c, const ObjectHandle& d )\
{\
    m_Operation.type = oper; \
    m_Operation.scope = eShaderGeneratorVariableScope_Local;\
    m_Operation.params[0] = a; \
    m_Operation.params[1] = b; \
    m_Operation.params[2] = c; \
    m_Operation.params[3] = d; \
    return setTypeFromOther(a); \
}

FUNC1(setAbs, eShaderGeneratorVariableOperationType_Abs)
FUNC1(setCeil, eShaderGeneratorVariableOperationType_Ceil)
FUNC1(setCos, eShaderGeneratorVariableOperationType_Cos)
FUNC1(setFloor, eShaderGeneratorVariableOperationType_Floor)
FUNC1(setFrac, eShaderGeneratorVariableOperationType_Frac)
FUNC1(setNormalize, eShaderGeneratorVariableOperationType_Normalize)
FUNC1(setSign, eShaderGeneratorVariableOperationType_Sign)
FUNC1(setSin, eShaderGeneratorVariableOperationType_Sin)
FUNC1(setEncodeNormal, eShaderGeneratorVariableOperationType_EncodeNormal)

FUNC2(setAdd, eShaderGeneratorVariableOperationType_Add)
FUNC2(setCross, eShaderGeneratorVariableOperationType_Cross)
FUNC2(setDistance, eShaderGeneratorVariableOperationType_Distance)
FUNC2(setDivide, eShaderGeneratorVariableOperationType_Div)
FUNC2(setDot, eShaderGeneratorVariableOperationType_Dot)
FUNC2(setMultiply, eShaderGeneratorVariableOperationType_Mul)
FUNC2(setMin, eShaderGeneratorVariableOperationType_Min)
FUNC2(setMax, eShaderGeneratorVariableOperationType_Max)
FUNC2(setPower, eShaderGeneratorVariableOperationType_Pow)
FUNC2(setReflect, eShaderGeneratorVariableOperationType_Reflect)
FUNC2(setSubtract, eShaderGeneratorVariableOperationType_Sub)
FUNC2(setStep, eShaderGeneratorVariableOperationType_Step)

FUNC3(setClamp, eShaderGeneratorVariableOperationType_Clamp)
FUNC3(setLerp, eShaderGeneratorVariableOperationType_Lerp)
FUNC3(setSmoothStep, eShaderGeneratorVariableOperationType_SmoothStep)
FUNC3(setCalculateNormal, eShaderGeneratorVariableOperationType_CalcNormal)

bool ShaderGeneratorVariable::setComposeFloat2( const ObjectHandle& a, const ObjectHandle& b )\
{
    m_Operation.type = eShaderGeneratorVariableOperationType_ComposeFloat2;
    m_Operation.scope = eShaderGeneratorVariableScope_Local;
    m_Operation.params[0] = a;
    m_Operation.params[1] = b; 
    m_Operation.params[2] = ObjectHandle::unassigned;
    m_Operation.params[3] = ObjectHandle::unassigned;
    m_Type = eShaderGeneratorVariableType_Float2;
    return true;
}
bool ShaderGeneratorVariable::setComposeFloat3( const ObjectHandle& a, const ObjectHandle& b, const ObjectHandle& c )\
{
    m_Operation.type = eShaderGeneratorVariableOperationType_ComposeFloat3;
    m_Operation.scope = eShaderGeneratorVariableScope_Local;
    m_Operation.params[0] = a;
    m_Operation.params[1] = b; 
    m_Operation.params[2] = c;
    m_Operation.params[3] = ObjectHandle::unassigned;
    m_Type = eShaderGeneratorVariableType_Float3;
    return true;
}
bool ShaderGeneratorVariable::setComposeFloat4( const ObjectHandle& a, const ObjectHandle& b, const ObjectHandle& c, const ObjectHandle& d )\
{
    m_Operation.type = eShaderGeneratorVariableOperationType_ComposeFloat4;
    m_Operation.scope = eShaderGeneratorVariableScope_Local;
    m_Operation.params[0] = a;
    m_Operation.params[1] = b; 
    m_Operation.params[2] = c;
    m_Operation.params[3] = d;
    m_Type = eShaderGeneratorVariableType_Float4;
    return true;
}

bool ShaderGeneratorVariable::setSwizzle( const ObjectHandle& a, const EShaderGeneratorSwizzleMask maskA, const EShaderGeneratorSwizzleMask maskB /*= ShaderGeneratorSwizzleMask_None*/, const EShaderGeneratorSwizzleMask maskC /*= ShaderGeneratorSwizzleMask_None*/, const EShaderGeneratorSwizzleMask maskD /*= ShaderGeneratorSwizzleMask_None*/ )
{
    m_Inline = true;
    m_Operation.type = eShaderGeneratorVariableOperationType_Swizzle;
    m_Operation.scope = eShaderGeneratorVariableScope_Local;
    m_Operation.params[0] = a;
    m_Operation.swizzleParams[0] = maskA; 
    m_Operation.swizzleParams[1] = maskB;
    m_Operation.swizzleParams[2] = maskC;
    m_Operation.swizzleParams[3] = maskD;
    if (maskD == eShaderGeneratorSwizzleMask_None)
    {
        if (maskC == eShaderGeneratorSwizzleMask_None)
        {
            if (maskB == eShaderGeneratorSwizzleMask_None)
            {
                m_Type = eShaderGeneratorVariableType_Float;
            }
            else
            {
                m_Type = eShaderGeneratorVariableType_Float2;
            }
        }
        else
        {
            m_Type = eShaderGeneratorVariableType_Float3;
        }
    }
    else
    {
        m_Type = eShaderGeneratorVariableType_Float4;
    }
    return true;
}

#undef FUNC1                                           
#undef FUNC2
#undef FUNC3

bool ShaderGeneratorVariable::setTypeFromOther( const ObjectHandle& handle )
{
    const ShaderGeneratorVariableFactory* factory(ShaderGeneratorVariableFactory::getInstance());
    m_Type = factory->get(handle)->getType();
    return m_Type != eShaderGeneratorVariableType_Unknown;
}

float ShaderGeneratorVariable::getFloatData() const
{
    return *reinterpret_cast<const float*>(m_ConstantData.floatData);
}
const vec2& ShaderGeneratorVariable::getFloat2Data() const
{
    return *reinterpret_cast<const vec2*>(m_ConstantData.floatData);
}
const vec3& ShaderGeneratorVariable::getFloat3Data() const
{
    return *reinterpret_cast<const vec3*>(m_ConstantData.floatData);
}
const vec4& ShaderGeneratorVariable::getFloat4Data() const
{
    return *reinterpret_cast<const vec4*>(m_ConstantData.floatData);
}


} } //end namespace
