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
//Created: 14/11/2012
#include "HappySandBoxPCH.h" 
#include "MaterialGeneratorMathNodes.h"

#include <ShaderGeneratorVariableFactory.h>
#include "MaterialGeneratorNodeConnector.h"
#include "../forms/MaterialGraph.h"

#pragma region defines
#define IMPLEMENT_EVALUATE1(class_, func)\
bool class_::evaluate()\
{\
    bool result(false);\
    he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance()); \
    he::ct::ShaderGeneratorVariable* const varResult(factory->get(getOutputConnector(0).getVar())); \
    varResult->reset();\
    \
    const he::ObjectHandle input(getInputConnector(0).getVar());\
    if (input != he::ObjectHandle::unassigned) \
    { \
        result = varResult->func(input);\
    } \
    result &= MaterialGeneratorNode::evaluate(); \
    setCompileState(result);\
    return result;\
}

#define IMPLEMENT_EVALUATE2(class_, func)\
bool class_::evaluate()\
{\
    bool result(false);\
    he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance()); \
    he::ct::ShaderGeneratorVariable* const varResult(factory->get(getOutputConnector(0).getVar())); \
    varResult->reset();\
    \
    const he::ObjectHandle a(getInputConnector(0).getVar());\
    const he::ObjectHandle b(getInputConnector(1).getVar());\
    if (a != he::ObjectHandle::unassigned && b != he::ObjectHandle::unassigned)\
    {\
        result = varResult->func(a, b);\
    }\
    result &= MaterialGeneratorNode::evaluate(); \
    setCompileState(result);\
    return result;\
}

#define IMPLEMENT_EVALUATE3(class_, func)\
    bool class_::evaluate()\
{\
    bool result(false);\
    he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance()); \
    he::ct::ShaderGeneratorVariable* const varResult(factory->get(getOutputConnector(0).getVar())); \
    varResult->reset();\
    \
    const he::ObjectHandle a(getInputConnector(0).getVar());\
    const he::ObjectHandle b(getInputConnector(1).getVar());\
    const he::ObjectHandle c(getInputConnector(2).getVar());\
    if (a != he::ObjectHandle::unassigned && b != he::ObjectHandle::unassigned && c != he::ObjectHandle::unassigned)\
    {\
        result = varResult->func(a, b, c);\
    }\
    result &= MaterialGeneratorNode::evaluate(); \
    setCompileState(result);\
    return result;\
}
#pragma endregion

namespace hs {
    
// One Param

#pragma region Abs
void MaterialGeneratorNodeAbs::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeAbs, setAbs)

#pragma endregion

#pragma region Cos
void MaterialGeneratorNodeCos::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
                     
    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeCos, setCos)

#pragma endregion

#pragma region Sin
void MaterialGeneratorNodeSin::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
                     
    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeSin, setSin)

#pragma endregion

#pragma region Ceil
void MaterialGeneratorNodeCeil::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
                     
    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeCeil, setCeil)

#pragma endregion

#pragma region Floor
void MaterialGeneratorNodeFloor::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
                     
    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeFloor, setFloor)

#pragma endregion

#pragma region Frac
void MaterialGeneratorNodeFrac::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
                     
    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeFrac, setFrac)
#pragma endregion

#pragma region OneMin
void MaterialGeneratorNodeOneMin::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
                     
    m_One = getParent()->getShaderGenerator()->addVariable();

    MaterialGeneratorNode::init();
}
void MaterialGeneratorNodeOneMin::destroy()
{
    if (m_One != he::ObjectHandle::unassigned)
    {
        getParent()->getShaderGenerator()->removeVariable(m_One);
        m_One = he::ObjectHandle::unassigned;
    }
    MaterialGeneratorNode::destroy();
}
bool MaterialGeneratorNodeOneMin::evaluate()
{
    bool result(false);

    const he::ObjectHandle resultHandle(getOutputConnector(0).getVar());
    he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());
    he::ct::ShaderGeneratorVariable* const varResult(factory->get(resultHandle));
    varResult->reset();

    const he::ObjectHandle a(getInputConnector(0).getVar());
    if (a != he::ObjectHandle::unassigned)
    {
        he::ct::ShaderGeneratorVariable* const varOne(factory->get(m_One));
        switch (factory->get(a)->getType())
        {
        case he::ct::ShaderGeneratorVariableType_Float: varOne->setConstant(1.0f); break;
        case he::ct::ShaderGeneratorVariableType_Float2: varOne->setConstant(he::vec2(1.0f, 1.0f)); break;
        case he::ct::ShaderGeneratorVariableType_Float3: varOne->setConstant(he::vec3(1.0f, 1.0f, 1.0f)); break;
        case he::ct::ShaderGeneratorVariableType_Float4: varOne->setConstant(he::vec4(1.0f, 1.0f, 1.0f, 1.0f)); break;
        default:
            LOG(he::LogType_ProgrammerAssert, "Error unknown/unsupported variable type"); break;
        }

        result = varResult->setSubtract(m_One, a);
    }
    result &= MaterialGeneratorNode::evaluate();
    setCompileState(result);
    return result;
}

#pragma endregion

#pragma region Normalize
void MaterialGeneratorNodeNormalize::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
                     
    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeNormalize, setNormalize)
#pragma endregion

#pragma region Sign
void MaterialGeneratorNodeSign::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
                     
    MaterialGeneratorNode::init();
}
IMPLEMENT_EVALUATE1(MaterialGeneratorNodeSign, setSign)
#pragma endregion

// Two Params

#pragma region Add
void MaterialGeneratorNodeAdd::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
                     
    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeAdd, setAdd)

#pragma endregion

#pragma region Divide
void MaterialGeneratorNodeDivide::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
                                 
    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeDivide, setDivide)

#pragma endregion

#pragma region Min
void MaterialGeneratorNodeMin::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
                     
    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeMin, setMin)

#pragma endregion

#pragma region Max
    void MaterialGeneratorNodeMax::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
                     
    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeMax, setMax)

#pragma endregion

#pragma region Multiply
void MaterialGeneratorNodeMultiply::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
                                 
    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeMultiply, setMultiply)

#pragma endregion

#pragma region Subtract
void MaterialGeneratorNodeSubtract::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
                     
    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeSubtract, setSubtract)

#pragma endregion

#pragma region Cross
void MaterialGeneratorNodeCross::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
         
    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeCross, setCross)

#pragma endregion

#pragma region Distance
void MaterialGeneratorNodeDistance::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeDistance, setDistance)

#pragma endregion

#pragma region DistanceSqr
void MaterialGeneratorNodeDistanceSqr::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    m_TempVar = getParent()->getShaderGenerator()->addVariable();

    MaterialGeneratorNode::init();
}

void MaterialGeneratorNodeDistanceSqr::destroy()
{
    if (m_TempVar != he::ObjectHandle::unassigned)
    {
        getParent()->getShaderGenerator()->removeVariable(m_TempVar);
        m_TempVar = he::ObjectHandle::unassigned;
    }

    MaterialGeneratorNode::destroy();
}

bool MaterialGeneratorNodeDistanceSqr::evaluate()
{
    bool result(false);

    const he::ObjectHandle resultHandle(getOutputConnector(0).getVar());
    he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());
    he::ct::ShaderGeneratorVariable* const varResult(factory->get(resultHandle));
    varResult->reset();

    const he::ObjectHandle a(getInputConnector(0).getVar());
    const he::ObjectHandle b(getInputConnector(1).getVar());
    if (a != he::ObjectHandle::unassigned && b != he::ObjectHandle::unassigned)
    {
        he::ct::ShaderGeneratorVariable* const varSub(factory->get(m_TempVar));
        if (varSub->setSubtract(b, a))
        {
            result = varResult->setDot(m_TempVar, m_TempVar);
        }
    }
    result &= MaterialGeneratorNode::evaluate();
    setCompileState(result);
    return result;
}

#pragma endregion

#pragma region Dot
void MaterialGeneratorNodeDot::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
                     
    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeDot, setDot)

#pragma endregion

#pragma region Power
void MaterialGeneratorNodePower::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
                                 
    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodePower, setPower)

#pragma endregion

#pragma region Reflect
void MaterialGeneratorNodeReflect::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("Direction", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("Normal", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeReflect, setReflect)

#pragma endregion

// Three Params

#pragma region Clamp
void MaterialGeneratorNodeClamp::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("X", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("Min", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("Max", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE3(MaterialGeneratorNodeClamp, setClamp)

#pragma endregion

#pragma region Lerp
void MaterialGeneratorNodeLerp::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("X", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("Y", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
                                 
    MaterialGeneratorNode::init();
}

IMPLEMENT_EVALUATE3(MaterialGeneratorNodeLerp, setLerp)

#pragma endregion


#undef IMPLEMENT_EVALUATE1
#undef IMPLEMENT_EVALUATE2
#undef IMPLEMENT_EVALUATE3
} //end namespace
