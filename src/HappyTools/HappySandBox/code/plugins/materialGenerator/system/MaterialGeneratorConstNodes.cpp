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

void MaterialGeneratorNodeFloat1::init()
{
    addOutput(MaterialGeneratorNodeConnectorDesc(HSFS::strX, "X", he::Color(1.0f, 0.0f, 0.0f, 1.0f)));

    addParam(MaterialGeneratorNodeParam(HSFS::strFloat, MaterialGeneratorNodeParam::Type_Float));

    MaterialGeneratorNode::init();
}
bool MaterialGeneratorNodeFloat1::evaluate()
{
    bool result(false);

    he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());
    he::ct::ShaderGeneratorVariable* const varResult(factory->get(getOutputConnector(0).getVar()));
    varResult->setConstant(getParam(0).getFloat());

    result = MaterialGeneratorNode::evaluate();
    setCompileState(result);

    return result;
}

#pragma endregion

#pragma region Float2

void MaterialGeneratorNodeFloat2::init()
{
    addOutput(MaterialGeneratorNodeConnectorDesc(HSFS::strXY, "XY", he::Color(1.0f, 1.0f, 1.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc(HSFS::strX, "X", he::Color(1.0f, 0.0f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc(HSFS::strY, "Y", he::Color(0.0f, 1.0f, 0.0f, 1.0f)));

    addParam(MaterialGeneratorNodeParam(HSFS::strFloat2, MaterialGeneratorNodeParam::Type_Float2));

    MaterialGeneratorNode::init();
}
bool MaterialGeneratorNodeFloat2::evaluate()
{
    bool result(false);
    he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());

    const he::vec2 constant(getParam(0).getFloat2());

    // XY
    he::ct::ShaderGeneratorVariable* const varResultXY(factory->get(getOutputConnector(0).getVar()));
    varResultXY->setConstant(constant);

    // X
    he::ct::ShaderGeneratorVariable* const varResultX(factory->get(getOutputConnector(1).getVar()));
    varResultX->setConstant(constant.x);

    // Y
    he::ct::ShaderGeneratorVariable* const varResultY(factory->get(getOutputConnector(2).getVar()));
    varResultY->setConstant(constant.y);

    result = MaterialGeneratorNode::evaluate();
    setCompileState(result);

    return result;
}

#pragma endregion

#pragma region Float3

void MaterialGeneratorNodeFloat3::init()
{
    addOutput(MaterialGeneratorNodeConnectorDesc(HSFS::strXYZ, "XYZ", he::Color(1.0f, 1.0f, 1.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc(HSFS::strX, "X", he::Color(1.0f, 0.0f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc(HSFS::strY, "Y", he::Color(0.0f, 1.0f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc(HSFS::strZ, "Z", he::Color(0.0f, 0.0f, 1.0f, 1.0f)));

    addParam(MaterialGeneratorNodeParam(HSFS::strFloat3, MaterialGeneratorNodeParam::Type_Float3));

    MaterialGeneratorNode::init();
}
bool MaterialGeneratorNodeFloat3::evaluate()
{
    bool result(false);

    he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());
    const he::vec3 constant(getParam(0).getFloat3());

    // XYZ
    he::ct::ShaderGeneratorVariable* const varResultXYZ(factory->get(getOutputConnector(0).getVar()));
    varResultXYZ->setConstant(constant);

    // X
    he::ct::ShaderGeneratorVariable* const varResultX(factory->get(getOutputConnector(1).getVar()));
    varResultX->setConstant(constant.x);

    // Y
    he::ct::ShaderGeneratorVariable* const varResultY(factory->get(getOutputConnector(2).getVar()));
    varResultY->setConstant(constant.y);

    // Z
    he::ct::ShaderGeneratorVariable* const varResultZ(factory->get(getOutputConnector(3).getVar()));
    varResultZ->setConstant(constant.z);

    result = MaterialGeneratorNode::evaluate();
    setCompileState(result);

    return result;
}

#pragma endregion

#pragma region Float4

void MaterialGeneratorNodeFloat4::init()
{
    addOutput(MaterialGeneratorNodeConnectorDesc(HSFS::strXYZW, "XYZW", he::Color(1.0f, 1.0f, 1.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc(HSFS::strX, "X", he::Color(1.0f, 0.0f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc(HSFS::strY, "Y", he::Color(0.0f, 1.0f, 0.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc(HSFS::strZ, "Z", he::Color(0.0f, 0.0f, 1.0f, 1.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc(HSFS::strW, "W", he::Color(0.5f, 0.0f, 1.0f, 1.0f)));

    addParam(MaterialGeneratorNodeParam(HSFS::strFloat4, MaterialGeneratorNodeParam::Type_Float4));

    MaterialGeneratorNode::init();
}
bool MaterialGeneratorNodeFloat4::evaluate()
{
    bool result(false);

    he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());

    const he::vec4 constant(getParam(0).getFloat4());

    // RGBA
    he::ct::ShaderGeneratorVariable* const varResultRGBA(factory->get(getOutputConnector(0).getVar()));
    varResultRGBA->setConstant(constant);

    // R
    he::ct::ShaderGeneratorVariable* const varResultR(factory->get(getOutputConnector(1).getVar()));
    varResultR->setConstant(constant.x);

    // G
    he::ct::ShaderGeneratorVariable* const varResultG(factory->get(getOutputConnector(2).getVar()));
    varResultG->setConstant(constant.y);

    // B
    he::ct::ShaderGeneratorVariable* const varResultB(factory->get(getOutputConnector(3).getVar()));
    varResultB->setConstant(constant.z);

    // A
    he::ct::ShaderGeneratorVariable* const varResultA(factory->get(getOutputConnector(4).getVar()));
    varResultA->setConstant(constant.w);

    result = MaterialGeneratorNode::evaluate();
    setCompileState(result);

    return result;
}

#pragma endregion

} //end namespace
