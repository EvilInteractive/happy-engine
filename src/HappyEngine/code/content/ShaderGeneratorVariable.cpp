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
#include "HappyPCH.h" 

#include "ShaderGeneratorVariable.h"
#include "ShaderGeneratorVariableFactory.h"

namespace he {
namespace ct {

IMPLEMENT_OBJECT(ShaderGeneratorVariable)

ShaderGeneratorVariable::ShaderGeneratorVariable()
: m_Type(ShaderVariableType_Unknown)
, m_LocalName("")
, m_FragUsageCount(0)
, m_VertUsageCount(0)
{
    he_memset(&m_ConstantData, 0, sizeof(m_ConstantData));
}


ShaderGeneratorVariable::~ShaderGeneratorVariable()
{
}

void ShaderGeneratorVariable::setConstant( const float a )
{
    m_Operation.type = ShaderGeneratorVariableOperationType_Constant;
    m_Type = ShaderVariableType_Float;
    m_ConstantData.floatData[0] = a;
}
void ShaderGeneratorVariable::setConstant( const vec2& a )
{
    m_Operation.type = ShaderGeneratorVariableOperationType_Constant;
    m_Type = ShaderVariableType_Float2;
    m_ConstantData.floatData[0] = a.x;
    m_ConstantData.floatData[1] = a.y;
}
void ShaderGeneratorVariable::setConstant( const vec3& a )
{
    m_Operation.type = ShaderGeneratorVariableOperationType_Constant;
    m_Type = ShaderVariableType_Float3;
    m_ConstantData.floatData[0] = a.x;
    m_ConstantData.floatData[1] = a.y;
    m_ConstantData.floatData[2] = a.z;
}
void ShaderGeneratorVariable::setConstant( const vec4& a )
{
    m_Operation.type = ShaderGeneratorVariableOperationType_Constant;
    m_Type = ShaderVariableType_Float4;
    m_ConstantData.floatData[0] = a.x;
    m_ConstantData.floatData[1] = a.y;
    m_ConstantData.floatData[2] = a.z;
    m_ConstantData.floatData[3] = a.w;
}
void ShaderGeneratorVariable::setGlobalVar( const ShaderGeneratorGlobalVariableType type )
{
    setTypeFromGlobal(type);
    m_Operation.type = ShaderGeneratorVariableOperationType_Global;
}

void ShaderGeneratorVariable::setExposedVar( const ShaderGeneratorVariableType type )
{
    m_Type = type;
    m_Operation.type = ShaderGeneratorVariableOperationType_Exposed;
}

#define FUNC1(name, oper) \
void ShaderGeneratorVariable::name( const ObjectHandle& a )\
{\
    m_Operation.type = oper; \
    m_Operation.params[0] = a; \
    setTypeFromOther(a);\
}

#define FUNC2(name, oper) \
void ShaderGeneratorVariable::name( const ObjectHandle& a, const ObjectHandle& b )\
{\
    m_Operation.type = oper; \
    m_Operation.params[0] = a; \
    m_Operation.params[1] = b; \
    setTypeFromOther(a); \
}

#define FUNC3(name, oper) \
void ShaderGeneratorVariable::name( const ObjectHandle& a, const ObjectHandle& b, const ObjectHandle& c )\
{\
    m_Operation.type = oper; \
    m_Operation.params[0] = a; \
    m_Operation.params[1] = b; \
    m_Operation.params[2] = c; \
    setTypeFromOther(a); \
}

FUNC1(setAbs, ShaderGeneratorVariableOperationType_Abs)
FUNC1(setCeil, ShaderGeneratorVariableOperationType_Ceil)
FUNC1(setCos, ShaderGeneratorVariableOperationType_Cos)
FUNC1(setFloor, ShaderGeneratorVariableOperationType_Floor)
FUNC1(setFrac, ShaderGeneratorVariableOperationType_Frac)
FUNC1(setNormalize, ShaderGeneratorVariableOperationType_Normalize)
FUNC1(setSign, ShaderGeneratorVariableOperationType_Sign)
FUNC1(setSin, ShaderGeneratorVariableOperationType_Sin)

FUNC2(setAdd, ShaderGeneratorVariableOperationType_Add)
FUNC2(setCross, ShaderGeneratorVariableOperationType_Cross)
FUNC2(setDistance, ShaderGeneratorVariableOperationType_Distance)
FUNC2(setDivide, ShaderGeneratorVariableOperationType_Div)
FUNC2(setDot, ShaderGeneratorVariableOperationType_Dot)
FUNC2(setMultiply, ShaderGeneratorVariableOperationType_Mul)
FUNC2(setMin, ShaderGeneratorVariableOperationType_Min)
FUNC2(setPower, ShaderGeneratorVariableOperationType_Pow)
FUNC2(setReflect, ShaderGeneratorVariableOperationType_Reflect)
FUNC2(setSubtract, ShaderGeneratorVariableOperationType_Sub)
FUNC2(setStep, ShaderGeneratorVariableOperationType_Step)

FUNC3(setClamp, ShaderGeneratorVariableOperationType_Clamp)
FUNC3(setLerp, ShaderGeneratorVariableOperationType_Lerp)
FUNC3(setSmoothStep, ShaderGeneratorVariableOperationType_SmoothStep)

#undef FUNC1                                           
#undef FUNC2
#undef FUNC3

void ShaderGeneratorVariable::setTypeFromOther( const ObjectHandle& handle )
{
    const ShaderGeneratorVariableFactory* factory(ShaderGeneratorVariableFactory::getInstance());
    m_Type = factory->get(handle)->getType();
    HE_ASSERT(m_Type != ShaderVariableType_Unknown, "Variable type is Unknown!");
}

void ShaderGeneratorVariable::setTypeFromGlobal( const ShaderGeneratorGlobalVariableType /*type*/ )
{
    LOG(LogType_ProgrammerAssert, "Not implemented");
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
