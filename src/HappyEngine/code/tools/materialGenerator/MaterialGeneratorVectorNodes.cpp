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

#include "MaterialGeneratorVectorNodes.h"
#include "ShaderGeneratorVariableFactory.h"
#include "MaterialGeneratorGraph.h"

namespace he {
namespace tools {
    
// One Param

#pragma region ComposeFloat2

MaterialGeneratorNodeComposeVector::MaterialGeneratorNodeComposeVector()
{
    addOverload(1, 4, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Unknown, MaterialGeneratorVariableType_Unknown);

    addOverload(1, 4, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Unknown);
    addOverload(1, 4, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Unknown, MaterialGeneratorVariableType_Unknown);
    addOverload(1, 4, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Unknown, MaterialGeneratorVariableType_Unknown);

    addOverload(1, 4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
    addOverload(1, 4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Unknown);
    addOverload(1, 4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Unknown);
    addOverload(1, 4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Unknown);
    addOverload(1, 4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Unknown, MaterialGeneratorVariableType_Unknown);
    addOverload(1, 4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Unknown, MaterialGeneratorVariableType_Unknown);
    addOverload(1, 4, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Unknown, MaterialGeneratorVariableType_Unknown);

    addConnecters(1, 4, ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)), 
                        ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)), 
                        ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)), 
                        ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)), 
                        ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)));
}

bool MaterialGeneratorNodeComposeVector::evaluate( MaterialGeneratorError& error )
{
    const bool result(MaterialGeneratorNode::evaluate(error));
    if (result)
    {
        const ObjectHandle a(getInputConnection(0).getConnection().getVar());
        const ObjectHandle b(getInputConnection(1).getConnection().getVar());
        const ObjectHandle c(getInputConnection(2).getConnection().getVar());
        const ObjectHandle d(getInputConnection(3).getConnection().getVar());

        ct::ShaderGeneratorVariableFactory* const factory(ct::ShaderGeneratorVariableFactory::getInstance());
        const he::ObjectHandle handle(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varResult(factory->get(handle));

        const MaterialGeneratorVariableType outputType(getOutput(0).getType());

        switch (outputType)
        {
            case MaterialGeneratorVariableType_Float2: varResult->setComposeFloat2(a, b); break;
            case MaterialGeneratorVariableType_Float3: varResult->setComposeFloat3(a, b, c); break;
            case MaterialGeneratorVariableType_Float4: varResult->setComposeFloat4(a, b, c, d); break;
            default: LOG(LogType_ProgrammerAssert, "Unsupported Type!"); break;
        }

        getOutput(0).setVar(handle);
    }
    return result;
}

#pragma endregion

#pragma region Swizzle

MaterialGeneratorNodeSwizzle::MaterialGeneratorNodeSwizzle()
{
    addOverload(1, 1, MaterialGeneratorVariableType_Unknown, MaterialGeneratorVariableType_Float2);
    addOverload(1, 1, MaterialGeneratorVariableType_Unknown, MaterialGeneratorVariableType_Float3);
    addOverload(1, 1, MaterialGeneratorVariableType_Unknown, MaterialGeneratorVariableType_Float4);

    addConnecters(1, 1, ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)), 
                        ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)));

    addParam(MaterialGeneratorNodeParam("A", MaterialGeneratorNodeParam::Type_SwizzleMask));
    addParam(MaterialGeneratorNodeParam("B", MaterialGeneratorNodeParam::Type_SwizzleMask));
    addParam(MaterialGeneratorNodeParam("C", MaterialGeneratorNodeParam::Type_SwizzleMask));
    addParam(MaterialGeneratorNodeParam("D", MaterialGeneratorNodeParam::Type_SwizzleMask));
}

bool MaterialGeneratorNodeSwizzle::evaluate( MaterialGeneratorError& error )
{
    bool result(MaterialGeneratorNode::evaluate(error));
    if (result)
    {
        ct::ShaderGeneratorSwizzleMask mask[4];
        for (uint8 i(0); i < 4; ++i)
        {
            mask[i] = ct::ShaderGeneratorSwizzleMask_None;
        }

        uint8 components(0);

        const MaterialGeneratorNodeParam& paramA(getParam(0));
        const MaterialGeneratorNodeParam& paramB(getParam(1));
        const MaterialGeneratorNodeParam& paramC(getParam(2));
        const MaterialGeneratorNodeParam& paramD(getParam(3));

        if (paramA.getSwizzleMask() != ct::ShaderGeneratorSwizzleMask_None)
        {
            mask[components] = paramA.getSwizzleMask();
            ++components;
        }
        if (paramB.getSwizzleMask() != ct::ShaderGeneratorSwizzleMask_None)
        {
            mask[components] = paramB.getSwizzleMask();
            ++components;
        }
        if (paramC.getSwizzleMask() != ct::ShaderGeneratorSwizzleMask_None)
        {
            mask[components] = paramC.getSwizzleMask();
            ++components;
        }
        if (paramD.getSwizzleMask() != ct::ShaderGeneratorSwizzleMask_None)
        {
            mask[components] = paramD.getSwizzleMask();
            ++components;
        }

        MaterialGeneratorVariableType outputType(MaterialGeneratorVariableType_Unknown);
        switch (components)
        {
        case 0: break;
        case 1: outputType = MaterialGeneratorVariableType_Float; break;
        case 2: outputType = MaterialGeneratorVariableType_Float2; break;
        case 3: outputType = MaterialGeneratorVariableType_Float3; break;
        case 4: outputType = MaterialGeneratorVariableType_Float4; break;
        default: LOG(LogType_ProgrammerAssert, "Unsupported component count"); break;
        }

        if (outputType != MaterialGeneratorVariableType_Unknown || outputType > getInput(0).getType())
        {
            result = false;
            error.setMessage("Invalid swizzle");
        }
        else
        {
            const ObjectHandle a(getInputConnection(0).getConnection().getVar());

            ct::ShaderGeneratorVariableFactory* const factory(ct::ShaderGeneratorVariableFactory::getInstance());
            const he::ObjectHandle handle(m_Parent->getGenerator()->addVariable());
            ct::ShaderGeneratorVariable* const varResult(factory->get(handle));

            varResult->setSwizzle(a, mask[0], mask[1], mask[2], mask[3]);

            getOutput(0).setVar(handle);
        }
    }
    return result;
}

#pragma endregion
} } //end namespace
