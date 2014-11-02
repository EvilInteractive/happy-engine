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
    const bool result(MaterialGeneratorNode::evaluate());\
    if (result)\
    {\
        const he::ObjectHandle a(getInputConnector(0).getInputConnection()->getVar());\
        \
        he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());\
        const he::ObjectHandle handle(getParent()->getShaderGenerator()->addVariable());\
        he::ct::ShaderGeneratorVariable* const varResult(factory->get(handle));\
        varResult->func(a);\
        \
        getOutputConnector(0).setVar(handle);\
    }\
    return result;\
}

#define IMPLEMENT_EVALUATE2(class_, func)\
    bool class_::evaluate()\
{\
    const bool result(MaterialGeneratorNode::evaluate());\
    if (result)\
    {\
        const he::ObjectHandle a(getInputConnector(0).getInputConnection()->getVar());\
        const he::ObjectHandle b(getInputConnector(1).getInputConnection()->getVar());\
        \
        he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());\
        const he::ObjectHandle handle(getParent()->getShaderGenerator()->addVariable());\
        he::ct::ShaderGeneratorVariable* const varResult(factory->get(handle));\
        varResult->func(a, b);\
        \
        getOutputConnector(0).setVar(handle);\
    }\
    return result;\
}

#define IMPLEMENT_EVALUATE3(class_, func)\
    bool class_::evaluate()\
{\
    const bool result(MaterialGeneratorNode::evaluate());\
    if (result)\
    {\
        const he::ObjectHandle a(getInputConnector(0).getInputConnection()->getVar());\
        const he::ObjectHandle b(getInputConnector(1).getInputConnection()->getVar());\
        const he::ObjectHandle c(getInputConnector(2).getInputConnection()->getVar());\
        \
        he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());\
        const he::ObjectHandle handle(getParent()->getShaderGenerator()->addVariable());\
        he::ct::ShaderGeneratorVariable* const varResult(factory->get(handle));\
        varResult->func(a, b, c);\
        \
        getOutputConnector(0).setVar(handle);\
    }\
    return result;\
}
#pragma endregion

namespace hs {
    
// One Param

#pragma region Abs
MaterialGeneratorNodeAbs::MaterialGeneratorNodeAbs()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float),  MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float2), MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float3), MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float4), MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeAbs, setAbs)

#pragma endregion

#pragma region Cos
MaterialGeneratorNodeCos::MaterialGeneratorNodeCos()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float),   MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeCos, setCos)

#pragma endregion

#pragma region Sin
MaterialGeneratorNodeSin::MaterialGeneratorNodeSin()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float),   MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeSin, setSin)

#pragma endregion

#pragma region Ceil
MaterialGeneratorNodeCeil::MaterialGeneratorNodeCeil()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float),   MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeCeil, setCeil)

#pragma endregion

#pragma region Floor
MaterialGeneratorNodeFloor::MaterialGeneratorNodeFloor()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float),   MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeFloor, setFloor)

#pragma endregion

#pragma region Frac
MaterialGeneratorNodeFrac::MaterialGeneratorNodeFrac()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float),   MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeFrac, setFrac)
#pragma endregion

#pragma region OneMin
MaterialGeneratorNodeOneMin::MaterialGeneratorNodeOneMin()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float),   MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}
bool MaterialGeneratorNodeOneMin::evaluate()
{
    const bool result(MaterialGeneratorNode::evaluate());
    if (result)
    {
        const he::ObjectHandle a(getInputConnector(0).getInputConnection()->getVar());

        he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());

        const he::ObjectHandle oneHandle(getParent()->getShaderGenerator()->addVariable());
        he::ct::ShaderGeneratorVariable* const varOne(factory->get(oneHandle));
        switch (factory->get(a)->getType())
        {
        case he::ct::ShaderGeneratorVariableType_Float: varOne->setConstant(1.0f); break;
        case he::ct::ShaderGeneratorVariableType_Float2: varOne->setConstant(he::vec2(1.0f, 1.0f)); break;
        case he::ct::ShaderGeneratorVariableType_Float3: varOne->setConstant(he::vec3(1.0f, 1.0f, 1.0f)); break;
        case he::ct::ShaderGeneratorVariableType_Float4: varOne->setConstant(he::vec4(1.0f, 1.0f, 1.0f, 1.0f)); break;
        default:
            LOG(he::LogType_ProgrammerAssert, "Error unknown/unsupported variable type"); break;
        }

        const he::ObjectHandle resultHandle(getParent()->getShaderGenerator()->addVariable());
        he::ct::ShaderGeneratorVariable* const varResult(factory->get(resultHandle));
        varResult->setSubtract(oneHandle, a);

        getOutputConnector(0).setVar(resultHandle);
    }
    return result;
}

#pragma endregion

#pragma region Normalize
MaterialGeneratorNodeNormalize::MaterialGeneratorNodeNormalize()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float),   MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

IMPLEMENT_EVALUATE1(MaterialGeneratorNodeNormalize, setNormalize)
#pragma endregion

#pragma region Sign
MaterialGeneratorNodeSign::MaterialGeneratorNodeSign()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float),   MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}
IMPLEMENT_EVALUATE1(MaterialGeneratorNodeSign, setSign)
#pragma endregion

// Two Params

#pragma region Add
MaterialGeneratorNodeAdd::MaterialGeneratorNodeAdd()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float),   MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeAdd, setAdd)

#pragma endregion

#pragma region Divide
MaterialGeneratorNodeDivide::MaterialGeneratorNodeDivide()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float),   MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeDivide, setDivide)

#pragma endregion

#pragma region Min
MaterialGeneratorNodeMin::MaterialGeneratorNodeMin()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float),   MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeMin, setMin)

#pragma endregion

#pragma region Max
    MaterialGeneratorNodeMax::MaterialGeneratorNodeMax()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float),   MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeMax, setMax)

#pragma endregion

#pragma region Multiply
MaterialGeneratorNodeMultiply::MaterialGeneratorNodeMultiply()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float),   MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeMultiply, setMultiply)

#pragma endregion

#pragma region Subtract
MaterialGeneratorNodeSubtract::MaterialGeneratorNodeSubtract()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float),   MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeSubtract, setSubtract)

#pragma endregion

#pragma region Cross
MaterialGeneratorNodeCross::MaterialGeneratorNodeCross()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(1, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeCross, setCross)

#pragma endregion

#pragma region Distance
MaterialGeneratorNodeDistance::MaterialGeneratorNodeDistance()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeDistance, setDistance)

#pragma endregion

#pragma region DistanceSqr
MaterialGeneratorNodeDistanceSqr::MaterialGeneratorNodeDistanceSqr()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

bool MaterialGeneratorNodeDistanceSqr::evaluate()
{
    const bool result(MaterialGeneratorNode::evaluate());
    if (result)
    {
        const he::ObjectHandle a(getInputConnector(0).getInputConnection()->getVar());
        const he::ObjectHandle b(getInputConnector(1).getInputConnection()->getVar());

        he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());

        const he::ObjectHandle subHandle(getParent()->getShaderGenerator()->addVariable());
        he::ct::ShaderGeneratorVariable* const varSub(factory->get(subHandle));
        varSub->setSubtract(b, a);

        const he::ObjectHandle resultHandle(getParent()->getShaderGenerator()->addVariable());
        he::ct::ShaderGeneratorVariable* const varResult(factory->get(resultHandle));
        varResult->setDot(subHandle, subHandle);

        getOutputConnector(0).setVar(resultHandle);
    }
    return result;
}

#pragma endregion

#pragma region Dot
MaterialGeneratorNodeDot::MaterialGeneratorNodeDot()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float),   MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeDot, setDot)

#pragma endregion

#pragma region Power
MaterialGeneratorNodePower::MaterialGeneratorNodePower()
{
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("B", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float),   MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodePower, setPower)

#pragma endregion

#pragma region Reflect
MaterialGeneratorNodeReflect::MaterialGeneratorNodeReflect()
{
    addInput(MaterialGeneratorNodeConnectorDesc("Direction", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("Normal", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(2, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3), MGO_OUT(1, MaterialGeneratorVariableType_Float3));
}

IMPLEMENT_EVALUATE2(MaterialGeneratorNodeReflect, setReflect)

#pragma endregion

// Three Params

#pragma region Clamp
MaterialGeneratorNodeClamp::MaterialGeneratorNodeClamp()
{
    addInput(MaterialGeneratorNodeConnectorDesc("X", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("Min", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("Max", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(3, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float),   MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(3, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(3, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
    MGO_ADD_OVERLOAD_IO(MGO_IN(3, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(3, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

IMPLEMENT_EVALUATE3(MaterialGeneratorNodeClamp, setClamp)

#pragma endregion

#pragma region Lerp
MaterialGeneratorNodeLerp::MaterialGeneratorNodeLerp()
{
    addInput(MaterialGeneratorNodeConnectorDesc("X", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("Y", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("A", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_IO(MGO_IN(3, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float),   MGO_OUT(1, MaterialGeneratorVariableType_Float));
    MGO_ADD_OVERLOAD_IO(MGO_IN(3, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(3, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
    MGO_ADD_OVERLOAD_IO(MGO_IN(3, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float),  MGO_OUT(1, MaterialGeneratorVariableType_Float2));
    MGO_ADD_OVERLOAD_IO(MGO_IN(3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float),  MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(MGO_IN(3, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float),  MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

IMPLEMENT_EVALUATE3(MaterialGeneratorNodeLerp, setLerp)

#pragma endregion


#undef IMPLEMENT_EVALUATE1
#undef IMPLEMENT_EVALUATE2
#undef IMPLEMENT_EVALUATE3
} //end namespace
