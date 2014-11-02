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

#include "MaterialGeneratorVectorNodes.h"
#include "ShaderGeneratorVariableFactory.h"
#include "MaterialGeneratorNodeConnector.h"
#include "../forms/MaterialGraph.h"

namespace hs {
    
// One Param

#pragma region ComposeVector

MaterialGeneratorNodeComposeVector::MaterialGeneratorNodeComposeVector()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    // FLOAT2
    MGO_ADD_OVERLOAD_IO(
        MGO_IN(4, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Unknown, MaterialGeneratorVariableType_Unknown),
        MGO_OUT(1, MaterialGeneratorVariableType_Float2));

    // FLOAT3

    // float2 + float
    MGO_ADD_OVERLOAD_IO(
        MGO_IN(4, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Unknown, MaterialGeneratorVariableType_Unknown),
        MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    MGO_ADD_OVERLOAD_IO(
        MGO_IN(4, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Unknown, MaterialGeneratorVariableType_Unknown),
        MGO_OUT(1, MaterialGeneratorVariableType_Float3));
    // float + float + float
    MGO_ADD_OVERLOAD_IO(
        MGO_IN(4, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Unknown),
        MGO_OUT(1, MaterialGeneratorVariableType_Float3));

    // FLOAT4

    // float + float + float + float
    MGO_ADD_OVERLOAD_IO(
        MGO_IN(4, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float),
        MGO_OUT(1, MaterialGeneratorVariableType_Float4));

    // float2 + float + float
    MGO_ADD_OVERLOAD_IO(
        MGO_IN(4, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Unknown),
        MGO_OUT(1, MaterialGeneratorVariableType_Float4));
    MGO_ADD_OVERLOAD_IO(
        MGO_IN(4, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Unknown),
        MGO_OUT(1, MaterialGeneratorVariableType_Float4));
    MGO_ADD_OVERLOAD_IO(
        MGO_IN(4, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Unknown),
        MGO_OUT(1, MaterialGeneratorVariableType_Float4));

    // float2 + float2
    MGO_ADD_OVERLOAD_IO(
        MGO_IN(4, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Unknown, MaterialGeneratorVariableType_Unknown),
        MGO_OUT(1, MaterialGeneratorVariableType_Float4));

    // float3 + float
    MGO_ADD_OVERLOAD_IO(
        MGO_IN(4, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Unknown, MaterialGeneratorVariableType_Unknown),
        MGO_OUT(1, MaterialGeneratorVariableType_Float4));
    MGO_ADD_OVERLOAD_IO(
        MGO_IN(4, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Unknown, MaterialGeneratorVariableType_Unknown),
        MGO_OUT(1, MaterialGeneratorVariableType_Float4));
}

bool MaterialGeneratorNodeComposeVector::evaluate()
{
    const bool result(MaterialGeneratorNode::evaluate());
    if (result)
    {
        he::ObjectHandle vars[4];
        for (size_t i(0); i < 4; ++i)
        {
            MaterialGeneratorNodeConnector* c(getInputConnector(i).getInputConnection());
            if (c)
                vars[i] = c->getVar();
            else
                vars[i] = he::ObjectHandle::unassigned;
        }

        he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());
        const he::ObjectHandle varResultHandle(getParent()->getShaderGenerator()->addVariable());
        he::ct::ShaderGeneratorVariable* const varResult(factory->get(varResultHandle));

        MaterialGeneratorNodeConnector& output(getOutputConnector(0));
        const MaterialGeneratorVariableType outputType(getOutputConnector(0).getVariableType());
        switch (outputType)
        {
            case MaterialGeneratorVariableType_Float2: varResult->setComposeFloat2(vars[0], vars[1]); break;
            case MaterialGeneratorVariableType_Float3: varResult->setComposeFloat3(vars[0], vars[1], vars[2]); break;
            case MaterialGeneratorVariableType_Float4: varResult->setComposeFloat4(vars[0], vars[1], vars[2], vars[3]); break;
            default: LOG(he::LogType_ProgrammerAssert, "Unsupported Type!"); break;
        }

        output.setVar(varResultHandle);
    }
    return result;
}

#pragma endregion

#pragma region Swizzle

MaterialGeneratorNodeSwizzle::MaterialGeneratorNodeSwizzle()
{
    addInput(MaterialGeneratorNodeConnectorDesc("In", he::Color(1.0f, 0.5f, 0.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc("Out", he::Color(1.0f, 0.5f, 0.0f)));

    MGO_ADD_OVERLOAD_IO(
        MGO_IN(1, MaterialGeneratorVariableType_Float),
        MGO_OUT(1, MaterialGeneratorVariableType_Float));
           
    addParam(MaterialGeneratorNodeParam("A", MaterialGeneratorNodeParam::Type_SwizzleMask));
    addParam(MaterialGeneratorNodeParam("B", MaterialGeneratorNodeParam::Type_SwizzleMask));
    addParam(MaterialGeneratorNodeParam("C", MaterialGeneratorNodeParam::Type_SwizzleMask));
    addParam(MaterialGeneratorNodeParam("D", MaterialGeneratorNodeParam::Type_SwizzleMask));
}

bool MaterialGeneratorNodeSwizzle::evaluate()
{
    bool result(MaterialGeneratorNode::evaluate());
    if (result)
    {
        he::ct::ShaderGeneratorSwizzleMask mask[4];
        for (size_t i(0); i < 4; ++i)
        {
            mask[i] = he::ct::ShaderGeneratorSwizzleMask_None;
        }

        size_t components(0);
        for (size_t i(0); i < 4; ++i)
        {
            const MaterialGeneratorNodeParam& param(getParam(i));
            if (param.getSwizzleMask() != he::ct::ShaderGeneratorSwizzleMask_None)
            {
                mask[components] = param.getSwizzleMask();
                ++components;
            }
        }

        MaterialGeneratorVariableType outputType(MaterialGeneratorVariableType_Unknown);
        switch (components)
        {
        case 0: break;
        case 1: outputType = MaterialGeneratorVariableType_Float; break;
        case 2: outputType = MaterialGeneratorVariableType_Float2; break;
        case 3: outputType = MaterialGeneratorVariableType_Float3; break;
        case 4: outputType = MaterialGeneratorVariableType_Float4; break;
        default: LOG(he::LogType_ProgrammerAssert, "Unsupported component count"); break;
        }

        if (outputType != MaterialGeneratorVariableType_Unknown)
        {
            result = false;
        }
        else
        {
            MaterialGeneratorNodeConnector* connection(getInputConnector(0).getInputConnection());
            if (connection)
            {
                const he::ObjectHandle a(connection->getVar());

                he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());
                const he::ObjectHandle resultHandle(getParent()->getShaderGenerator()->addVariable());
                he::ct::ShaderGeneratorVariable* const varResult(factory->get(resultHandle));

                varResult->setSwizzle(a, mask[0], mask[1], mask[2], mask[3]);

                getOutputConnector(0).setVar(resultHandle);
            }
        }
    }
    return result;
}

#pragma endregion
} //end namespace
