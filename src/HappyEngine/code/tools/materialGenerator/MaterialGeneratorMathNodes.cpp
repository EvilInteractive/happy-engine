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
//Created: 14/11/2012
#include "HappyPCH.h" 

#include "MaterialGeneratorMathNodes.h"
#include "ShaderGeneratorVariableFactory.h"
#include "MaterialGeneratorGraph.h"

#pragma region defines
#define IMPLEMENT_EVALUATE1(class_, func)\
bool class_::evaluate( MaterialGeneratorError& error )\
{\
    const bool result(MaterialGeneratorNode::evaluate(error));\
    if (result)\
    {\
        const ObjectHandle a(getInputConnection(0).getConnection().getVar());\
        \
        ct::ShaderGeneratorVariableFactory* const factory(ct::ShaderGeneratorVariableFactory::getInstance());\
        const he::ObjectHandle handle(m_Parent->getGenerator()->addVariable());\
        ct::ShaderGeneratorVariable* const varResult(factory->get(handle));\
        varResult->func(a);\
        \
        getOutput(0).setVar(handle);\
    }\
    return result;\
}

#define IMPLEMENT_EVALUATE2(class_, func)\
    bool class_::evaluate( MaterialGeneratorError& error )\
{\
    const bool result(MaterialGeneratorNode::evaluate(error));\
    if (result)\
    {\
        const ObjectHandle a(getInputConnection(0).getConnection().getVar());\
        const ObjectHandle b(getInputConnection(1).getConnection().getVar());\
        \
        ct::ShaderGeneratorVariableFactory* const factory(ct::ShaderGeneratorVariableFactory::getInstance());\
        const he::ObjectHandle handle(m_Parent->getGenerator()->addVariable());\
        ct::ShaderGeneratorVariable* const varResult(factory->get(handle));\
        varResult->func(a, b);\
        \
        getOutput(0).setVar(handle);\
    }\
    return result;\
}

#define IMPLEMENT_EVALUATE3(class_, func)\
    bool class_::evaluate( MaterialGeneratorError& error )\
{\
    const bool result(MaterialGeneratorNode::evaluate(error));\
    if (result)\
    {\
        const ObjectHandle a(getInputConnection(0).getConnection().getVar());\
        const ObjectHandle b(getInputConnection(1).getConnection().getVar());\
        const ObjectHandle c(getInputConnection(2).getConnection().getVar());\
        \
        ct::ShaderGeneratorVariableFactory* const factory(ct::ShaderGeneratorVariableFactory::getInstance());\
        const he::ObjectHandle handle(m_Parent->getGenerator()->addVariable());\
        ct::ShaderGeneratorVariable* const varResult(factory->get(handle));\
        varResult->func(a, b, c);\
        \
        getOutput(0).setVar(handle);\
    }\
    return result;\
}
#pragma endregion

namespace he {
namespace tools {
    
// One Param

#pragma region Abs
MaterialGeneratorNodeAbs::MaterialGeneratorNodeAbs()
{
    addOverload(1, 1, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 1, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 1, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 1, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeAbs, setAbs)

#pragma endregion

#pragma region Cos
MaterialGeneratorNodeCos::MaterialGeneratorNodeCos()
{
    addOverload(1, 1, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 1, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 1, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 1, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) } );
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeCos, setCos)

#pragma endregion

#pragma region Sin
MaterialGeneratorNodeSin::MaterialGeneratorNodeSin()
{
    addOverload(1, 1, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 1, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 1, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 1, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeSin, setSin)

#pragma endregion

#pragma region Ceil
MaterialGeneratorNodeCeil::MaterialGeneratorNodeCeil()
{
    addOverload(1, 1, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 1, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 1, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 1, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeCeil, setCeil)

#pragma endregion

#pragma region Floor
MaterialGeneratorNodeFloor::MaterialGeneratorNodeFloor()
{
    addOverload(1, 1, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 1, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 1, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 1, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeFloor, setFloor)

#pragma endregion

#pragma region Frac
MaterialGeneratorNodeFrac::MaterialGeneratorNodeFrac()
{
    addOverload(1, 1, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 1, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 1, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 1, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeFrac, setFrac)
#pragma endregion

#pragma region OneMin
MaterialGeneratorNodeOneMin::MaterialGeneratorNodeOneMin()
{
    addOverload(1, 1, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 1, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 1, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 1, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}
bool MaterialGeneratorNodeOneMin::evaluate( MaterialGeneratorError& error )
{
    const bool result(MaterialGeneratorNode::evaluate(error));
    if (result)
    {
        const ObjectHandle a(getInputConnection(0).getConnection().getVar());

        ct::ShaderGeneratorVariableFactory* const factory(ct::ShaderGeneratorVariableFactory::getInstance());

        const he::ObjectHandle oneHandle(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varOne(factory->get(oneHandle));
        switch (factory->get(a)->getType())
        {
        case ct::ShaderGeneratorVariableType_Float: varOne->setConstant(1.0f); break;
        case ct::ShaderGeneratorVariableType_Float2: varOne->setConstant(vec2(1.0f, 1.0f)); break;
        case ct::ShaderGeneratorVariableType_Float3: varOne->setConstant(vec3(1.0f, 1.0f, 1.0f)); break;
        case ct::ShaderGeneratorVariableType_Float4: varOne->setConstant(vec4(1.0f, 1.0f, 1.0f, 1.0f)); break;
        default:
            LOG(LogType_ProgrammerAssert, "Error unknown/unsupported variable type"); break;
        }

        const he::ObjectHandle resultHandle(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varResult(factory->get(resultHandle));
        varResult->setSubtract(oneHandle, a);

        getOutput(0).setVar(resultHandle);
    }
    return result;
}

#pragma endregion

#pragma region Normalize
MaterialGeneratorNodeNormalize::MaterialGeneratorNodeNormalize()
{
    addOverload(1, 1, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 1, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 1, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 1, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeNormalize, setNormalize)
#pragma endregion

#pragma region Sign
MaterialGeneratorNodeSign::MaterialGeneratorNodeSign()
{
    addOverload(1, 1, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 1, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 1, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 1, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}
IMPLEMENT_EVALUATE1(MaterialGeneratorNodeSign, setSign)
#pragma endregion

// Two Params

#pragma region Add
MaterialGeneratorNodeAdd::MaterialGeneratorNodeAdd()
{
    addOverload(1, 2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("A", Color(1.0f, 0.5f, 0.0f, 1.0f)), ConnecterDesc("B", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeAdd, setAdd)

#pragma endregion

#pragma region Divide
MaterialGeneratorNodeDivide::MaterialGeneratorNodeDivide()
{
    addOverload(1, 2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addOverload(1, 2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float);
    addOverload(1, 2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float);
    addOverload(1, 2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("A", Color(1.0f, 0.5f, 0.0f, 1.0f)), ConnecterDesc("B", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeDivide, setDivide)

#pragma endregion

#pragma region Min
MaterialGeneratorNodeMin::MaterialGeneratorNodeMin()
{
    addOverload(1, 2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("A", Color(1.0f, 0.5f, 0.0f, 1.0f)), ConnecterDesc("B", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeMin, setMin)

#pragma endregion

#pragma region Max
    MaterialGeneratorNodeMax::MaterialGeneratorNodeMax()
{
    addOverload(1, 2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("A", Color(1.0f, 0.5f, 0.0f, 1.0f)), ConnecterDesc("B", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeMax, setMax)

#pragma endregion

#pragma region Multiply
MaterialGeneratorNodeMultiply::MaterialGeneratorNodeMultiply()
{
    addOverload(1, 2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addOverload(1, 2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float2);
    addOverload(1, 2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float3);
    addOverload(1, 2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float4);
    addOverload(1, 2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float);
    addOverload(1, 2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float);
    addOverload(1, 2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("A", Color(1.0f, 0.5f, 0.0f, 1.0f)), ConnecterDesc("B", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeMultiply, setMultiply)

#pragma endregion

#pragma region Subtract
MaterialGeneratorNodeSubtract::MaterialGeneratorNodeSubtract()
{
    addOverload(1, 2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("A", Color(1.0f, 0.5f, 0.0f, 1.0f)), ConnecterDesc("B", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeSubtract, setSubtract)

#pragma endregion

#pragma region Cross
MaterialGeneratorNodeCross::MaterialGeneratorNodeCross()
{
    addOverload(1, 2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("A", Color(1.0f, 0.5f, 0.0f, 1.0f)), ConnecterDesc("B", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeCross, setCross)

#pragma endregion

#pragma region Distance
MaterialGeneratorNodeDistance::MaterialGeneratorNodeDistance()
{
    addOverload(1, 2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("A", Color(1.0f, 0.5f, 0.0f, 1.0f)), ConnecterDesc("B", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeDistance, setDistance)

#pragma endregion

#pragma region DistanceSqr
MaterialGeneratorNodeDistanceSqr::MaterialGeneratorNodeDistanceSqr()
{
    addOverload(1, 2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("A", Color(1.0f, 0.5f, 0.0f, 1.0f)), ConnecterDesc("B", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

bool MaterialGeneratorNodeDistanceSqr::evaluate( MaterialGeneratorError& error )
{
    const bool result(MaterialGeneratorNode::evaluate(error));
    if (result)
    {
        const ObjectHandle a(getInputConnection(0).getConnection().getVar());
        const ObjectHandle b(getInputConnection(1).getConnection().getVar());

        ct::ShaderGeneratorVariableFactory* const factory(ct::ShaderGeneratorVariableFactory::getInstance());

        const he::ObjectHandle subHandle(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varSub(factory->get(subHandle));
        varSub->setSubtract(b, a);

        const he::ObjectHandle resultHandle(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varResult(factory->get(resultHandle));
        varResult->setDot(subHandle, subHandle);

        getOutput(0).setVar(resultHandle);
    }
    return result;
}

#pragma endregion

#pragma region Dot
MaterialGeneratorNodeDot::MaterialGeneratorNodeDot()
{
    addOverload(1, 2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("A", Color(1.0f, 0.5f, 0.0f, 1.0f)), ConnecterDesc("B", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeDot, setDot)

#pragma endregion

#pragma region Power
MaterialGeneratorNodePower::MaterialGeneratorNodePower()
{
    addOverload(1, 2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addOverload(1, 2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float);
    addOverload(1, 2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float);
    addOverload(1, 2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("A", Color(1.0f, 0.5f, 0.0f, 1.0f)), ConnecterDesc("B", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodePower, setPower)

#pragma endregion

#pragma region Reflect
MaterialGeneratorNodeReflect::MaterialGeneratorNodeReflect()
{
    addOverload(1, 2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("A", Color(1.0f, 0.5f, 0.0f, 1.0f)), ConnecterDesc("B", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeReflect, setReflect)

#pragma endregion

// Three Params

#pragma region Clamp
MaterialGeneratorNodeClamp::MaterialGeneratorNodeClamp()
{
    addOverload(1, 3, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 3, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 3, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addOverload(1, 3, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float);
    addOverload(1, 3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float);
    addOverload(1, 3, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("X", Color(1.0f, 0.5f, 0.0f, 1.0f)), ConnecterDesc("Min", Color(1.0f, 0.5f, 0.0f, 1.0f)), ConnecterDesc("Max", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE3(MaterialGeneratorNodeClamp, setClamp)

#pragma endregion

#pragma region Lerp
MaterialGeneratorNodeLerp::MaterialGeneratorNodeLerp()
{
    addOverload(1, 3, MaterialGeneratorVariableType_Float,  MaterialGeneratorVariableType_Float,  MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 3, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
    addOverload(1, 3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
    addOverload(1, 3, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
    addOverload(1, 3, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float);
    addOverload(1, 3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float);
    addOverload(1, 3, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float);
    addConnecters( { ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)) }, { ConnecterDesc("X", Color(1.0f, 0.5f, 0.0f, 1.0f)), ConnecterDesc("Y", Color(1.0f, 0.5f, 0.0f, 1.0f)), ConnecterDesc("A", Color(1.0f, 0.5f, 0.0f, 1.0f)) });
}

IMPLEMENT_EVALUATE3(MaterialGeneratorNodeLerp, setLerp)

#pragma endregion


#undef IMPLEMENT_EVALUATE1
#undef IMPLEMENT_EVALUATE2
#undef IMPLEMENT_EVALUATE3
} } //end namespace
