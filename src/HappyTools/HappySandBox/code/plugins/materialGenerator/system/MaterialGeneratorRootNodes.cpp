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

#include "MaterialGeneratorRootNodes.h"
#include "ShaderGeneratorVariableFactory.h"
#include "ContentManager.h"
#include "MaterialGeneratorNodeConnector.h"
#include "../forms/MaterialGraph.h"

namespace hs {
    
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
    addInput(MaterialGeneratorNodeConnectorDesc("Diffuse", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("Emissive", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("Specular", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("Gloss", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("Opacity", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("Normal", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("WorldPositionOffset", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MGO_ADD_OVERLOAD_I(MGO_IN(7, MaterialGeneratorVariableType_Float3,      // Diffuse
        MaterialGeneratorVariableType_Float3,                               // Emissive
        MaterialGeneratorVariableType_Float3,                               // Specular
        MaterialGeneratorVariableType_Float,                                // Gloss
        MaterialGeneratorVariableType_Float,                                // Opacity
        MaterialGeneratorVariableType_Float3,                               // Normal
        MaterialGeneratorVariableType_Float3));                             // WorldPositionOffset
    
}
bool MaterialGeneratorNodeRootNormalDraw::evaluate()
{
    const bool result(MaterialGeneratorNode::evaluate());
    if (result)
    {
        he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());
        he::ct::ShaderGenerator* const shaderGenerator(getParent()->getShaderGenerator());
        
        const auto& diffuseConnection(getInputConnector(RootNodeNormalDrawConnection_Diffuse));
        const auto& emissiveConnection(getInputConnector(RootNodeNormalDrawConnection_Emissive));
        const auto& specularConnection(getInputConnector(RootNodeNormalDrawConnection_Specular));
        const auto& glossConnection(getInputConnector(RootNodeNormalDrawConnection_Gloss));
        const auto& opacityConnection(getInputConnector(RootNodeNormalDrawConnection_Opacity));
        const auto& normalConnection(getInputConnector(RootNodeNormalDrawConnection_Normal));
        const auto& worldPosOffsetConnection(getInputConnector(RootNodeNormalDrawConnection_WorldPositionOffset));
        if (diffuseConnection.isConnected())
        {
            shaderGenerator->setDiffuse(diffuseConnection.getInputConnection()->getVar());
        }
        if (emissiveConnection.isConnected())
        {
            shaderGenerator->setEmissive(emissiveConnection.getInputConnection()->getVar());
        }
        if (specularConnection.isConnected())
        {
            shaderGenerator->setSpecular(specularConnection.getInputConnection()->getVar());
        }
        if (glossConnection.isConnected())
        {
            shaderGenerator->setGloss(glossConnection.getInputConnection()->getVar());
        }
        if (opacityConnection.isConnected())
        {
            shaderGenerator->setOpacity(opacityConnection.getInputConnection()->getVar());
            he::ct::ShaderGeneratorVariable* const testValue(factory->get(shaderGenerator->addVariable()));
            testValue->setConstant(0.5f);
            shaderGenerator->setAlphaTestValue(testValue->getHandle());
        }
        if (normalConnection.isConnected())
        {
            shaderGenerator->setNormal(normalConnection.getInputConnection()->getVar());
        }
        if (worldPosOffsetConnection.isConnected())
        {
            shaderGenerator->setWorldPositionOffset(worldPosOffsetConnection.getInputConnection()->getVar());
        }

        shaderGenerator->compile(he::Path(CONTENT->getShaderFolderPath().append("generateTest")), "testShader");
        shaderGenerator->reset();
    }
    return result;
}

#pragma endregion

} //end namespace
