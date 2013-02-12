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

#include "MaterialGeneratorRootNodes.h"
#include "ShaderGeneratorVariableFactory.h"
#include "MaterialGeneratorGraph.h"
#include "ContentManager.h"

namespace he {
namespace tools {
    
#pragma region RootNormalDraw

enum RootNodeNormalDrawConnection
{
    RootNodeNormalDrawConnection_Diffuse,
    RootNodeNormalDrawConnection_Emissive,
    RootNodeNormalDrawConnection_Specular,
    RootNodeNormalDrawConnection_Gloss,
    RootNodeNormalDrawConnection_Opacity,
    RootNodeNormalDrawConnection_Normal,
    RootNodeNormalDrawConnection_WorldPositionOffset
};

MaterialGeneratorNodeRootNormalDraw::MaterialGeneratorNodeRootNormalDraw()
{
    addOverload(0, 7, 
        MaterialGeneratorVariableType_Float3, 
        MaterialGeneratorVariableType_Float3, 
        MaterialGeneratorVariableType_Float3, 
        MaterialGeneratorVariableType_Float, 
        MaterialGeneratorVariableType_Float, 
        MaterialGeneratorVariableType_Float3, 
        MaterialGeneratorVariableType_Float3);

    FixedSizeList<ConnecterDesc, 7> inputs;
    inputs[0] = ConnecterDesc("Diffuse", Color(1.0f, 0.5f, 0.0f, 1.0f));
    inputs[1] = ConnecterDesc("Emissive", Color(1.0f, 0.5f, 0.0f, 1.0f));
    inputs[2] = ConnecterDesc("Specular", Color(1.0f, 0.5f, 0.0f, 1.0f));
    inputs[3] = ConnecterDesc("Gloss", Color(1.0f, 0.5f, 0.0f, 1.0f));
    inputs[4] = ConnecterDesc("Opacity", Color(1.0f, 0.5f, 0.0f, 1.0f));
    inputs[5] = ConnecterDesc("Normal", Color(1.0f, 0.5f, 0.0f, 1.0f));
    inputs[6] = ConnecterDesc("WorldPositionOffset", Color(1.0f, 0.5f, 0.0f, 1.0f));
    addConnecterInputs( inputs );

    setSize(vec2(128, 256));
    setCanBeSelected(false);
}
bool MaterialGeneratorNodeRootNormalDraw::evaluate( MaterialGeneratorError& error )
{
    const bool result(MaterialGeneratorNode::evaluate(error));
    if (result)
    {
        ct::ShaderGeneratorVariableFactory* const factory(ct::ShaderGeneratorVariableFactory::getInstance());
        ct::ShaderGenerator* const shaderGenerator(m_Parent->getGenerator());
        
        const auto& diffuseConnection(getInputConnection(RootNodeNormalDrawConnection_Diffuse));
        const auto& emissiveConnection(getInputConnection(RootNodeNormalDrawConnection_Emissive));
        const auto& specularConnection(getInputConnection(RootNodeNormalDrawConnection_Specular));
        const auto& glossConnection(getInputConnection(RootNodeNormalDrawConnection_Gloss));
        const auto& opacityConnection(getInputConnection(RootNodeNormalDrawConnection_Opacity));
        const auto& normalConnection(getInputConnection(RootNodeNormalDrawConnection_Normal));
        const auto& worldPosOffsetConnection(getInputConnection(RootNodeNormalDrawConnection_WorldPositionOffset));
        if (diffuseConnection.isConnected())
        {
            shaderGenerator->setDiffuse(diffuseConnection.getConnection().getVar());
        }
        if (emissiveConnection.isConnected())
        {
            shaderGenerator->setEmissive(emissiveConnection.getConnection().getVar());
        }
        if (specularConnection.isConnected())
        {
            shaderGenerator->setSpecular(specularConnection.getConnection().getVar());
        }
        if (glossConnection.isConnected())
        {
            shaderGenerator->setGloss(glossConnection.getConnection().getVar());
        }
        if (opacityConnection.isConnected())
        {
            shaderGenerator->setOpacity(opacityConnection.getConnection().getVar());
            ct::ShaderGeneratorVariable* const testValue(factory->get(shaderGenerator->addVariable()));
            testValue->setConstant(0.5f);
            shaderGenerator->setAlphaTestValue(testValue->getHandle());
        }
        if (normalConnection.isConnected())
        {
            shaderGenerator->setNormal(normalConnection.getConnection().getVar());
        }
        if (worldPosOffsetConnection.isConnected())
        {
            shaderGenerator->setWorldPositionOffset(worldPosOffsetConnection.getConnection().getVar());
        }

        shaderGenerator->compile(Path(CONTENT->getShaderFolderPath().append("generateTest")), "testShader");
        shaderGenerator->reset();
    }
    return result;
}

#pragma endregion

} } //end namespace
