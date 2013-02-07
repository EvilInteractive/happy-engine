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

#include "MaterialGeneratorConstNodes.h"
#include "ShaderGeneratorVariableFactory.h"
#include "MaterialGeneratorGraph.h"

namespace he {
namespace tools {

#pragma region Float

MaterialGeneratorNodeFloat1::MaterialGeneratorNodeFloat1()
{
    addOverload(1, 0, MaterialGeneratorVariableType_Float);
    addConnecters({ ConnecterDesc("X", Color(1.0f, 0.0f, 0.0f, 1.0f)) }, {});
    addParam(MaterialGeneratorNodeParam("Float", MaterialGeneratorNodeParam::Type_Float));
}
bool MaterialGeneratorNodeFloat1::evaluate( MaterialGeneratorError& error )
{
    const bool result(MaterialGeneratorNode::evaluate(error));
    if (result)
    {
        ct::ShaderGeneratorVariableFactory* const factory(ct::ShaderGeneratorVariableFactory::getInstance());
        const he::ObjectHandle resultHandle(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varResult(factory->get(resultHandle));
        varResult->setConstant(getParam(0).getFloat());

        getOutput(0).setVar(resultHandle);
    }
    return result;
}

#pragma endregion

#pragma region Float2

MaterialGeneratorNodeFloat2::MaterialGeneratorNodeFloat2()
{
    addOverload(3, 0, 
        MaterialGeneratorVariableType_Float2, 
        MaterialGeneratorVariableType_Float, 
        MaterialGeneratorVariableType_Float);
    addConnecters({
        ConnecterDesc("XY", Color(1.0f, 1.0f, 1.0f, 1.0f)), 
        ConnecterDesc("X", Color(1.0f, 0.0f, 0.0f, 1.0f)), 
        ConnecterDesc("Y", Color(0.0f, 1.0f, 0.0f, 1.0f)) }, {});
    addParam(MaterialGeneratorNodeParam("Float2", MaterialGeneratorNodeParam::Type_Float2));
}
bool MaterialGeneratorNodeFloat2::evaluate( MaterialGeneratorError& error )
{
    const bool result(MaterialGeneratorNode::evaluate(error));
    if (result)
    {
        ct::ShaderGeneratorVariableFactory* const factory(ct::ShaderGeneratorVariableFactory::getInstance());

        const vec2 constant(getParam(0).getFloat2());

        // XY
        const he::ObjectHandle resultHandleXY(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varResultXY(factory->get(resultHandleXY));
        varResultXY->setConstant(constant);
        getOutput(0).setVar(resultHandleXY);

        // X
        const he::ObjectHandle resultHandleX(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varResultX(factory->get(resultHandleX));
        varResultX->setConstant(constant.x);
        getOutput(1).setVar(resultHandleX);

        // Y
        const he::ObjectHandle resultHandleY(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varResultY(factory->get(resultHandleY));
        varResultY->setConstant(constant.y);
        getOutput(2).setVar(resultHandleY);
    }
    return result;
}

#pragma endregion

#pragma region Float3

MaterialGeneratorNodeFloat3::MaterialGeneratorNodeFloat3()
{
    addOverload(4, 0, 
        MaterialGeneratorVariableType_Float3, 
        MaterialGeneratorVariableType_Float, 
        MaterialGeneratorVariableType_Float, 
        MaterialGeneratorVariableType_Float);
    addConnecters({
        ConnecterDesc("XYZ", Color(1.0f, 1.0f, 1.0f, 1.0f)), 
        ConnecterDesc("X", Color(1.0f, 0.0f, 0.0f, 1.0f)), 
        ConnecterDesc("Y", Color(0.0f, 1.0f, 0.0f, 1.0f)),
        ConnecterDesc("Z", Color(0.0f, 0.0f, 1.0f, 1.0f)) }, {});
    addParam(MaterialGeneratorNodeParam("Float3", MaterialGeneratorNodeParam::Type_Float3));
}
bool MaterialGeneratorNodeFloat3::evaluate( MaterialGeneratorError& error )
{
    const bool result(MaterialGeneratorNode::evaluate(error));
    if (result)
    {
        ct::ShaderGeneratorVariableFactory* const factory(ct::ShaderGeneratorVariableFactory::getInstance());

        const vec3 constant(getParam(0).getFloat3());

        // XYZ
        const he::ObjectHandle resultHandleXYZ(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varResultXYZ(factory->get(resultHandleXYZ));
        varResultXYZ->setConstant(constant);
        getOutput(0).setVar(resultHandleXYZ);

        // X
        const he::ObjectHandle resultHandleX(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varResultX(factory->get(resultHandleX));
        varResultX->setConstant(constant.x);
        getOutput(1).setVar(resultHandleX);

        // Y
        const he::ObjectHandle resultHandleY(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varResultY(factory->get(resultHandleY));
        varResultY->setConstant(constant.y);
        getOutput(2).setVar(resultHandleY);

        // Z
        const he::ObjectHandle resultHandleZ(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varResultZ(factory->get(resultHandleZ));
        varResultZ->setConstant(constant.z);
        getOutput(3).setVar(resultHandleZ);
    }
    return result;
}

#pragma endregion

#pragma region Float4

MaterialGeneratorNodeFloat4::MaterialGeneratorNodeFloat4()
{
    addOverload(5, 0, 
        MaterialGeneratorVariableType_Float4, 
        MaterialGeneratorVariableType_Float, 
        MaterialGeneratorVariableType_Float, 
        MaterialGeneratorVariableType_Float, 
        MaterialGeneratorVariableType_Float);
    addConnecters({
        ConnecterDesc("RGBA", Color(1.0f, 1.0f, 1.0f, 1.0f)), 
        ConnecterDesc("R", Color(1.0f, 0.0f, 0.0f, 1.0f)), 
        ConnecterDesc("G", Color(0.0f, 1.0f, 0.0f, 1.0f)),
        ConnecterDesc("B", Color(0.0f, 0.0f, 1.0f, 1.0f)),
        ConnecterDesc("A", Color(0.5f, 0.0f, 1.0f, 1.0f)) }, {});
    addParam(MaterialGeneratorNodeParam("Float4", MaterialGeneratorNodeParam::Type_Float4));
}
bool MaterialGeneratorNodeFloat4::evaluate( MaterialGeneratorError& error )
{
    const bool result(MaterialGeneratorNode::evaluate(error));
    if (result)
    {
        ct::ShaderGeneratorVariableFactory* const factory(ct::ShaderGeneratorVariableFactory::getInstance());

        const vec4 constant(getParam(0).getFloat4());

        // RGBA
        const he::ObjectHandle resultHandleRGBA(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varResultRGBA(factory->get(resultHandleRGBA));
        varResultRGBA->setConstant(constant);
        getOutput(0).setVar(resultHandleRGBA);

        // R
        const he::ObjectHandle resultHandleR(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varResultR(factory->get(resultHandleR));
        varResultR->setConstant(constant.x);
        getOutput(1).setVar(resultHandleR);

        // G
        const he::ObjectHandle resultHandleG(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varResultG(factory->get(resultHandleG));
        varResultG->setConstant(constant.y);
        getOutput(2).setVar(resultHandleG);

        // B
        const he::ObjectHandle resultHandleB(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varResultB(factory->get(resultHandleB));
        varResultB->setConstant(constant.z);
        getOutput(3).setVar(resultHandleB);

        // A
        const he::ObjectHandle resultHandleA(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varResultA(factory->get(resultHandleA));
        varResultA->setConstant(constant.w);
        getOutput(4).setVar(resultHandleA);
    }
    return result;
}

#pragma endregion

} } //end namespace
