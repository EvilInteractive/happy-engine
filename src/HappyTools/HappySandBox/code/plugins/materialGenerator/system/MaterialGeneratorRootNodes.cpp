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

enum RootNodeNormalDrawParam
{
    eRootNodeNormalDrawParam_AlphaTest
};

void MaterialGeneratorNodeRootNormalDraw::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc("Diffuse", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("Emissive", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("Specular", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("Gloss", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("Opacity", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("Normal", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("WorldPositionOffset", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    addParam(MaterialGeneratorNodeParam("Alpha Test Value", MaterialGeneratorNodeParam::Type_Float));
    
    getParam(eRootNodeNormalDrawParam_AlphaTest).setFloat(0.5f);

    MaterialGeneratorNode::init();
}
bool MaterialGeneratorNodeRootNormalDraw::evaluate()
{
    bool result(false);
    he::ct::ShaderGenerator* const shaderGenerator(getParent()->getShaderGenerator());
        
    const he::ObjectHandle diffuseConnection(getInputConnector(RootNodeNormalDrawConnection_Diffuse).getVar());
    if (diffuseConnection != he::ObjectHandle::unassigned)
    {
        const he::ObjectHandle emissiveConnection(getInputConnector(RootNodeNormalDrawConnection_Emissive).getVar());
        const he::ObjectHandle specularConnection(getInputConnector(RootNodeNormalDrawConnection_Specular).getVar());
        const he::ObjectHandle glossConnection(getInputConnector(RootNodeNormalDrawConnection_Gloss).getVar());
        const he::ObjectHandle opacityConnection(getInputConnector(RootNodeNormalDrawConnection_Opacity).getVar());
        const he::ObjectHandle normalConnection(getInputConnector(RootNodeNormalDrawConnection_Normal).getVar());
        const he::ObjectHandle worldPosOffsetConnection(getInputConnector(RootNodeNormalDrawConnection_WorldPositionOffset).getVar());
        const he::ObjectHandle alphaTest(getParam(eRootNodeNormalDrawParam_AlphaTest).getVar());
        
        shaderGenerator->setDiffuse(diffuseConnection);
        shaderGenerator->setEmissive(emissiveConnection);
        shaderGenerator->setSpecular(specularConnection);
        shaderGenerator->setGloss(glossConnection);

        shaderGenerator->setOpacity(opacityConnection);
        shaderGenerator->setAlphaTestValue(alphaTest);

        shaderGenerator->setNormal(normalConnection);
        shaderGenerator->setWorldPositionOffset(worldPosOffsetConnection);

        result = true;
    }
    result &= MaterialGeneratorNode::evaluate();
    setCompileState(result);
    return result;
}

#pragma endregion

} //end namespace
