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

#include "MaterialGeneratorConstNodes.h"
#include "ShaderGeneratorVariableFactory.h"
#include "MaterialGeneratorNodeConnector.h"
#include "MaterialGeneratorNode.h"
#include "../forms/MaterialGraph.h"

#include <ShaderGenerator.h>

namespace hs {

#pragma region Float

MaterialGeneratorNodeFloat1::MaterialGeneratorNodeFloat1()
{
    addOutput(MaterialGeneratorNodeConnectorDesc("X", he::Color(1.0f, 0.0f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_O(
        MGO_OUT(1, MaterialGeneratorVariableType_Float));

    addParam(MaterialGeneratorNodeParam("Float", MaterialGeneratorNodeParam::Type_Float));
}
bool MaterialGeneratorNodeFloat1::evaluate()
{
    const bool result(MaterialGeneratorNode::evaluate());
    if (result)
    {
        he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());
        const he::ObjectHandle resultHandle(getParent()->getShaderGenerator()->addVariable());
        he::ct::ShaderGeneratorVariable* const varResult(factory->get(resultHandle));
        varResult->setConstant(getParam(0).getFloat());

        getOutputConnector(0).setVar(resultHandle);
    }
    return result;
}

#pragma endregion

#pragma region Float2

MaterialGeneratorNodeFloat2::MaterialGeneratorNodeFloat2()
{
    addOutput(MaterialGeneratorNodeConnectorDesc("XY", he::Color(1.0f, 1.0f, 1.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("X", he::Color(1.0f, 0.0f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Y", he::Color(0.0f, 1.0f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_O(
        MGO_OUT(3, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float));

    addParam(MaterialGeneratorNodeParam("Float2", MaterialGeneratorNodeParam::Type_Float2));
}
bool MaterialGeneratorNodeFloat2::evaluate()
{
    const bool result(MaterialGeneratorNode::evaluate());
    if (result)
    {
        he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());

        const he::vec2 constant(getParam(0).getFloat2());

        // XY
        const he::ObjectHandle resultHandleXY(getParent()->getShaderGenerator()->addVariable());
        he::ct::ShaderGeneratorVariable* const varResultXY(factory->get(resultHandleXY));
        varResultXY->setConstant(constant);
        getOutputConnector(0).setVar(resultHandleXY);

        // X
        const he::ObjectHandle resultHandleX(getParent()->getShaderGenerator()->addVariable());
        he::ct::ShaderGeneratorVariable* const varResultX(factory->get(resultHandleX));
        varResultX->setConstant(constant.x);
        getOutputConnector(1).setVar(resultHandleX);

        // Y
        const he::ObjectHandle resultHandleY(getParent()->getShaderGenerator()->addVariable());
        he::ct::ShaderGeneratorVariable* const varResultY(factory->get(resultHandleY));
        varResultY->setConstant(constant.y);
        getOutputConnector(2).setVar(resultHandleY);
    }
    return result;
}

#pragma endregion

#pragma region Float3

MaterialGeneratorNodeFloat3::MaterialGeneratorNodeFloat3()
{
    addOutput(MaterialGeneratorNodeConnectorDesc("XYZ", he::Color(1.0f, 1.0f, 1.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("X", he::Color(1.0f, 0.0f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Y", he::Color(0.0f, 1.0f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Z", he::Color(0.0f, 0.0f, 1.0f, 1.0f)));

    MGO_ADD_OVERLOAD_O(
        MGO_OUT(4, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float));

    addParam(MaterialGeneratorNodeParam("Float3", MaterialGeneratorNodeParam::Type_Float3));
}
bool MaterialGeneratorNodeFloat3::evaluate()
{
    const bool result(MaterialGeneratorNode::evaluate());
    if (result)
    {
        he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());

        const he::vec3 constant(getParam(0).getFloat3());

        // XYZ
        const he::ObjectHandle resultHandleXYZ(getParent()->getShaderGenerator()->addVariable());
        he::ct::ShaderGeneratorVariable* const varResultXYZ(factory->get(resultHandleXYZ));
        varResultXYZ->setConstant(constant);
        getOutputConnector(0).setVar(resultHandleXYZ);

        // X
        const he::ObjectHandle resultHandleX(getParent()->getShaderGenerator()->addVariable());
        he::ct::ShaderGeneratorVariable* const varResultX(factory->get(resultHandleX));
        varResultX->setConstant(constant.x);
        getOutputConnector(1).setVar(resultHandleX);

        // Y
        const he::ObjectHandle resultHandleY(getParent()->getShaderGenerator()->addVariable());
        he::ct::ShaderGeneratorVariable* const varResultY(factory->get(resultHandleY));
        varResultY->setConstant(constant.y);
        getOutputConnector(2).setVar(resultHandleY);

        // Z
        const he::ObjectHandle resultHandleZ(getParent()->getShaderGenerator()->addVariable());
        he::ct::ShaderGeneratorVariable* const varResultZ(factory->get(resultHandleZ));
        varResultZ->setConstant(constant.z);
        getOutputConnector(3).setVar(resultHandleZ);
    }
    return result;
}

#pragma endregion

#pragma region Float4

MaterialGeneratorNodeFloat4::MaterialGeneratorNodeFloat4()
{
    addOutput(MaterialGeneratorNodeConnectorDesc("XYZW", he::Color(1.0f, 1.0f, 1.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("X", he::Color(1.0f, 0.0f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Y", he::Color(0.0f, 1.0f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Z", he::Color(0.0f, 0.0f, 1.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("W", he::Color(0.5f, 0.0f, 1.0f, 1.0f)));

    MGO_ADD_OVERLOAD_O(
        MGO_OUT(5, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float));

    addParam(MaterialGeneratorNodeParam("Float4", MaterialGeneratorNodeParam::Type_Float4));
}
bool MaterialGeneratorNodeFloat4::evaluate()
{
    const bool result(MaterialGeneratorNode::evaluate());
    if (result)
    {
        he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());

        const he::vec4 constant(getParam(0).getFloat4());

        // RGBA
        const he::ObjectHandle resultHandleRGBA(getParent()->getShaderGenerator()->addVariable());
        he::ct::ShaderGeneratorVariable* const varResultRGBA(factory->get(resultHandleRGBA));
        varResultRGBA->setConstant(constant);
        getOutputConnector(0).setVar(resultHandleRGBA);

        // R
        const he::ObjectHandle resultHandleR(getParent()->getShaderGenerator()->addVariable());
        he::ct::ShaderGeneratorVariable* const varResultR(factory->get(resultHandleR));
        varResultR->setConstant(constant.x);
        getOutputConnector(1).setVar(resultHandleR);

        // G
        const he::ObjectHandle resultHandleG(getParent()->getShaderGenerator()->addVariable());
        he::ct::ShaderGeneratorVariable* const varResultG(factory->get(resultHandleG));
        varResultG->setConstant(constant.y);
        getOutputConnector(2).setVar(resultHandleG);

        // B
        const he::ObjectHandle resultHandleB(getParent()->getShaderGenerator()->addVariable());
        he::ct::ShaderGeneratorVariable* const varResultB(factory->get(resultHandleB));
        varResultB->setConstant(constant.z);
        getOutputConnector(3).setVar(resultHandleB);

        // A
        const he::ObjectHandle resultHandleA(getParent()->getShaderGenerator()->addVariable());
        he::ct::ShaderGeneratorVariable* const varResultA(factory->get(resultHandleA));
        varResultA->setConstant(constant.w);
        getOutputConnector(4).setVar(resultHandleA);
    }
    return result;
}

#pragma endregion

} //end namespace
