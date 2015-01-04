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
#include "MaterialGeneratorNodeConnector.h"
#include "../forms/MaterialGraph.h"

#include <ShaderGeneratorVariableFactory.h>
#include <ShaderGeneratorVariable.h>

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

MaterialGeneratorNodeRootNormalDraw::MaterialGeneratorNodeRootNormalDraw(MaterialGraph* const parent)
    : MaterialGeneratorNode(parent)
{

}

void MaterialGeneratorNodeRootNormalDraw::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc(HSFS::strDiffuse, "Diffuse", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc(HSFS::strEmissive, "Emissive", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc(HSFS::strSpecular, "Specular", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc(HSFS::strGloss, "Gloss", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc(HSFS::strOpacity, "Opacity", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc(HSFS::strNormal, "Normal", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc(HSFS::strWorldPositionOffset, "WorldPositionOffset", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    addParam(MaterialGeneratorNodeParam(HSFS::strAlphaTestValue, MaterialGeneratorNodeParam::Type_Float));
    
    getParam(eRootNodeNormalDrawParam_AlphaTest).setFloat(0.5f);

    MaterialGeneratorNode::init();
}

void MaterialGeneratorNodeRootNormalDraw::destroy()
{
    removeAllTempVars();
    MaterialGeneratorNode::destroy();
}

bool MaterialGeneratorNodeRootNormalDraw::areConnectionsValid()
{
    he::ct::ShaderGeneratorVariableFactory* factory(he::ct::ShaderGeneratorVariableFactory::getInstance());
    he::ct::ShaderGeneratorVariable* diffuseConnection(factory->get(getInputConnector(RootNodeNormalDrawConnection_Diffuse).getVar()));
    // At least we need a diffuse connection
    bool result(false);
    if (diffuseConnection && diffuseConnection->getType() == he::ct::eShaderGeneratorVariableType_Float3)
    {
        result = true;
        he::ct::ShaderGeneratorVariable* emissiveConnection(factory->get(getInputConnector(RootNodeNormalDrawConnection_Emissive).getVar()));
        result &= !emissiveConnection || emissiveConnection->getType() == he::ct::eShaderGeneratorVariableType_Float;

        he::ct::ShaderGeneratorVariable* specularConnection(factory->get(getInputConnector(RootNodeNormalDrawConnection_Specular).getVar()));
        result &= !specularConnection || specularConnection->getType() == he::ct::eShaderGeneratorVariableType_Float || specularConnection->getType() == he::ct::eShaderGeneratorVariableType_Float3;

        he::ct::ShaderGeneratorVariable* glossConnection(factory->get(getInputConnector(RootNodeNormalDrawConnection_Gloss).getVar()));
        result &= !glossConnection || glossConnection->getType() == he::ct::eShaderGeneratorVariableType_Float;

        he::ct::ShaderGeneratorVariable* opacityConnection(factory->get(getInputConnector(RootNodeNormalDrawConnection_Opacity).getVar()));
        result &= !opacityConnection || opacityConnection->getType() == he::ct::eShaderGeneratorVariableType_Float;

        he::ct::ShaderGeneratorVariable* normalConnection(factory->get(getInputConnector(RootNodeNormalDrawConnection_Normal).getVar()));
        result &= !normalConnection || normalConnection->getType() == he::ct::eShaderGeneratorVariableType_Float3;

        he::ct::ShaderGeneratorVariable* worldPosOffsetConnection(factory->get(getInputConnector(RootNodeNormalDrawConnection_WorldPositionOffset).getVar()));
        result &= !worldPosOffsetConnection || worldPosOffsetConnection->getType() == he::ct::eShaderGeneratorVariableType_Float3;
    }
    return result;
}

bool MaterialGeneratorNodeRootNormalDraw::evaluate()
{
    bool result(false);        
    const he::ObjectHandle diffuseConnection(getInputConnector(RootNodeNormalDrawConnection_Diffuse).getVar());
    if (areConnectionsValid())
    {
        he::ct::ShaderGeneratorVariableFactory* factory(he::ct::ShaderGeneratorVariableFactory::getInstance());
        he::ct::ShaderGenerator* const shaderGenerator(he::checked_cast<MaterialGraph*>(getParent())->getShaderGenerator());

        removeAllTempVars();

        he::ct::ShaderGeneratorVariable* diffuseConnection(factory->get(getInputConnector(RootNodeNormalDrawConnection_Diffuse).getVar()));
        he::ct::ShaderGeneratorVariable* emissiveConnection(factory->get(getInputConnector(RootNodeNormalDrawConnection_Emissive).getVar()));
        he::ct::ShaderGeneratorVariable* specularConnection(factory->get(getInputConnector(RootNodeNormalDrawConnection_Specular).getVar()));
        he::ct::ShaderGeneratorVariable* glossConnection(factory->get(getInputConnector(RootNodeNormalDrawConnection_Gloss).getVar()));
        //he::ct::ShaderGeneratorVariable* opacityConnection(factory->get(getInputConnector(RootNodeNormalDrawConnection_Opacity).getVar()));
        he::ct::ShaderGeneratorVariable* normalConnection(factory->get(getInputConnector(RootNodeNormalDrawConnection_Normal).getVar()));
        //he::ct::ShaderGeneratorVariable* worldPosOffsetConnection(factory->get(getInputConnector(RootNodeNormalDrawConnection_WorldPositionOffset).getVar()));
        //const he::ObjectHandle alphaTest(getParam(eRootNodeNormalDrawParam_AlphaTest).getVar());
        
        he::ct::ShaderGeneratorVariable* outColor(factory->get(shaderGenerator->getVariable(he::ct::eShaderGeneratorGlobalOutVariableType_OutColor)));
        he::ct::ShaderGeneratorVariable* outSpecGloss(factory->get(shaderGenerator->getVariable(he::ct::eShaderGeneratorGlobalOutVariableType_OutSG)));
        he::ct::ShaderGeneratorVariable* outNormalDepth(factory->get(shaderGenerator->getVariable(he::ct::eShaderGeneratorGlobalOutVariableType_OutNormalDepth)));

        he::ct::ShaderGeneratorVariable* zero(addTempVar("zero"));
        zero->setConstant(0.0f);
        he::ct::ShaderGeneratorVariable* one(addTempVar("one"));
        one->setConstant(1.0f);

        // Color
        he::ObjectHandle illumination(emissiveConnection? emissiveConnection->getHandle() : zero->getHandle());
        outColor->setComposeFloat4(diffuseConnection->getHandle(), illumination);
        
        // SG
        he::ObjectHandle spec(he::ObjectHandle::unassigned);
        if (specularConnection)
        {
            if (specularConnection->getType() == he::ct::eShaderGeneratorVariableType_Float)
            {
                he::ct::ShaderGeneratorVariable* composedSpec(addTempVar("spec"));
                composedSpec->setComposeFloat3(specularConnection->getHandle(), specularConnection->getHandle(), specularConnection->getHandle());
                spec = composedSpec->getHandle();
            }
            else
            {
                spec = specularConnection->getHandle();
            }
        }
        else
        {
            he::ct::ShaderGeneratorVariable* composedSpec(addTempVar("spec"));
            composedSpec->setComposeFloat3(zero->getHandle(), zero->getHandle(), zero->getHandle());
            spec = composedSpec->getHandle();
        }
        he::ObjectHandle gloss(glossConnection? glossConnection->getHandle() : one->getHandle());
        outSpecGloss->setComposeFloat4(spec, gloss);

        // Normal
        he::ct::ShaderGeneratorVariable* encodedNormal(addTempVar("encodedNormal"));
        if (normalConnection)
        {
            he::ct::ShaderGeneratorVariable* calcNormal(addTempVar("calcNormal"));
            calcNormal->setCalculateNormal(
                shaderGenerator->getVariable(he::ct::eShaderGeneratorGlobalInputVariableType_Normal),
                shaderGenerator->getVariable(he::ct::eShaderGeneratorGlobalInputVariableType_Tangent),
                normalConnection->getHandle());
            encodedNormal->setEncodeNormal(calcNormal->getHandle());
        }
        else
        {
            encodedNormal->setEncodeNormal(shaderGenerator->getVariable(he::ct::eShaderGeneratorGlobalInputVariableType_Normal));
        }

        // Depth
        he::ct::ShaderGeneratorVariable* camNear(addTempVar("camNear"));
        he::ct::ShaderGeneratorVariable* camFar(addTempVar("camFar"));
        he::ct::ShaderGeneratorVariable* depthMinusNear(addTempVar("depthMinusNear"));
        he::ct::ShaderGeneratorVariable* depthRange(addTempVar("depthRange"));
        he::ct::ShaderGeneratorVariable* linDepth(addTempVar("linDepth"));
        camNear->setSwizzle(
            shaderGenerator->getVariable(he::ct::eShaderGeneratorGlobalCameraVariableType_CameraNearFar), 
            he::ct::eShaderGeneratorSwizzleMask_X);
        camFar->setSwizzle(
            shaderGenerator->getVariable(he::ct::eShaderGeneratorGlobalCameraVariableType_CameraNearFar), 
            he::ct::eShaderGeneratorSwizzleMask_Y);
        depthMinusNear->setSubtract(
            shaderGenerator->getVariable(he::ct::eShaderGeneratorGlobalInputVariableType_Depth),
            camNear->getHandle());
        depthRange->setSubtract(camFar->getHandle(), camNear->getHandle());
        linDepth->setDivide(depthMinusNear->getHandle(), depthRange->getHandle());

        // OutNormalDepth
        outNormalDepth->setComposeFloat3(encodedNormal->getHandle(), linDepth->getHandle());

        shaderGenerator->clearOutputVariables();
        shaderGenerator->addOutputVariable(outColor->getHandle());
        shaderGenerator->addOutputVariable(outSpecGloss->getHandle());
        shaderGenerator->addOutputVariable(outNormalDepth->getHandle());
        
        result = true;
    }
    result &= MaterialGeneratorNode::evaluate();
    setCompileState(result);
    return result;
}

void MaterialGeneratorNodeRootNormalDraw::removeAllTempVars()
{
    he::ct::ShaderGenerator* const shaderGenerator(he::checked_cast<MaterialGraph*>(getParent())->getShaderGenerator());
    m_TempVars.forEach([&](const he::ObjectHandle handle)
    {
        shaderGenerator->removeVariable(handle);
    });
    m_TempVars.clear();
}

he::ct::ShaderGeneratorVariable* MaterialGeneratorNodeRootNormalDraw::addTempVar( const char* nameHint )
{
    he::ct::ShaderGeneratorVariableFactory* factory(he::ct::ShaderGeneratorVariableFactory::getInstance());
    he::ct::ShaderGenerator* const shaderGenerator(he::checked_cast<MaterialGraph*>(getParent())->getShaderGenerator());
    he::ct::ShaderGeneratorVariable* var(factory->get(shaderGenerator->addVariable(nameHint)));
    m_TempVars.add(var->getHandle());
    return var;
}

#pragma endregion

} //end namespace
