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

void MaterialGeneratorNodeComposeVector::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc(HSFS::strIn1, "In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc(HSFS::strIn2, "In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc(HSFS::strIn3, "In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));
    addInput(MaterialGeneratorNodeConnectorDesc(HSFS::strIn4, "In", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    addOutput(MaterialGeneratorNodeConnectorDesc(HSFS::strOut, "Out", he::Color(1.0f, 0.5f, 0.0f, 1.0f)));

    MaterialGeneratorNode::init();
}

bool MaterialGeneratorNodeComposeVector::evaluate()
{
    bool result(false);
    he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());

    he::ObjectHandle vars[4];
    int components(0);
    for (size_t i(0); i < 4; ++i)
    {
        vars[i] = getInputConnector(i).getVar();
        if (vars[i] != he::ObjectHandle::unassigned)
        {
            he::ct::ShaderGeneratorVariable* const var(factory->get(vars[i]));
            switch (var->getType())
            {
            case he::ct::eShaderGeneratorVariableType_Float: components += 1; break;
            case he::ct::eShaderGeneratorVariableType_Float2: components += 2; break;
            case he::ct::eShaderGeneratorVariableType_Float3: components += 3; break;
            case he::ct::eShaderGeneratorVariableType_Float4: components += 4; break;
            }
        }
    }

    const he::ObjectHandle varResultHandle(getOutputConnector(0).getVar());
    he::ct::ShaderGeneratorVariable* const varResult(factory->get(varResultHandle));
    varResult->reset();

    if (components > 0 && components <= 4)
    {
        he::ct::EShaderGeneratorVariableType type(he::checked_numcast<he::ct::EShaderGeneratorVariableType>(
            he::ct::eShaderGeneratorVariableType_Float + components - 1));
        
        size_t actualindex(0);
        // Shift everything so we have no holes
        for (size_t i(0); i < 4; ++i)
        {
            if (vars[i] != he::ObjectHandle::unassigned)
            {
                if (i != actualindex)
                {
                    vars[actualindex] = vars[i];
                    vars[i] = he::ObjectHandle::unassigned;
                }
                ++actualindex;
            }
        }

        switch (type)
        {
        case he::ct::eShaderGeneratorVariableType_Float2: result = varResult->setComposeFloat2(vars[0], vars[1]); break;
        case he::ct::eShaderGeneratorVariableType_Float3: result = varResult->setComposeFloat3(vars[0], vars[1], vars[2]); break;
        case he::ct::eShaderGeneratorVariableType_Float4: result = varResult->setComposeFloat4(vars[0], vars[1], vars[2], vars[3]); break;
        }
    }
    result &= MaterialGeneratorNode::evaluate();
    setCompileState(result);
    return result;
}

#pragma endregion

#pragma region Swizzle

void MaterialGeneratorNodeSwizzle::init()
{
    addInput(MaterialGeneratorNodeConnectorDesc(HSFS::strIn, "In", he::Color(1.0f, 0.5f, 0.0f)));
    addOutput(MaterialGeneratorNodeConnectorDesc(HSFS::strOut, "Out", he::Color(1.0f, 0.5f, 0.0f)));

    addParam(MaterialGeneratorNodeParam(HSFS::strA, MaterialGeneratorNodeParam::Type_SwizzleMask));
    addParam(MaterialGeneratorNodeParam(HSFS::strB, MaterialGeneratorNodeParam::Type_SwizzleMask));
    addParam(MaterialGeneratorNodeParam(HSFS::strC, MaterialGeneratorNodeParam::Type_SwizzleMask));
    addParam(MaterialGeneratorNodeParam(HSFS::strD, MaterialGeneratorNodeParam::Type_SwizzleMask));

    getParam(0).setSwizzleMask(he::ct::eShaderGeneratorSwizzleMask_X);
    getParam(1).setSwizzleMask(he::ct::eShaderGeneratorSwizzleMask_Y);
    getParam(2).setSwizzleMask(he::ct::eShaderGeneratorSwizzleMask_Z);
    getParam(3).setSwizzleMask(he::ct::eShaderGeneratorSwizzleMask_W);

    MaterialGeneratorNode::init();
}

bool MaterialGeneratorNodeSwizzle::evaluate()
{
    bool result(false);
    he::ct::EShaderGeneratorSwizzleMask mask[4];
    for (size_t i(0); i < 4; ++i)
    {
        mask[i] = he::ct::eShaderGeneratorSwizzleMask_None;
    }

    size_t components(0);
    for (size_t i(0); i < 4; ++i)
    {
        const MaterialGeneratorNodeParam& param(getParam(i));
        if (param.getSwizzleMask() != he::ct::eShaderGeneratorSwizzleMask_None)
        {
            mask[components] = param.getSwizzleMask();
            ++components;
        }
    }

    he::ct::ShaderGeneratorVariableFactory* const factory(he::ct::ShaderGeneratorVariableFactory::getInstance());
    const he::ObjectHandle resultHandle(getOutputConnector(0).getVar());
    he::ct::ShaderGeneratorVariable* const varResult(factory->get(resultHandle));
    varResult->reset();

    if (components > 0 && components <= 4)
    {
        const he::ObjectHandle a(getInputConnector(0).getVar());
        if (a != he::ObjectHandle::unassigned)
        {
           result = varResult->setSwizzle(a, mask[0], mask[1], mask[2], mask[3]);
        }
    }
    result &= MaterialGeneratorNode::evaluate();
    setCompileState(result);
    return result;
}

#pragma endregion
} //end namespace
