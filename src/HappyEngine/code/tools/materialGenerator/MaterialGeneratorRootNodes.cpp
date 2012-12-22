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

namespace he {
namespace tools {
    
#pragma region RootNormalDraw
MaterialGeneratorNodeRootNormalDraw::MaterialGeneratorNodeRootNormalDraw()
{
    addOverload(0, 7, 
        MaterialGeneratorVariableType_Float3, 
        MaterialGeneratorVariableType_Float3, 
        MaterialGeneratorVariableType_Float3, 
        MaterialGeneratorVariableType_Float3, 
        MaterialGeneratorVariableType_Float, 
        MaterialGeneratorVariableType_Float3, 
        MaterialGeneratorVariableType_Float3);
    addConnecters(0, 7, 
        ConnecterDesc("Diffuse", Color(1.0f, 0.5f, 0.0f, 1.0f)), 
        ConnecterDesc("Emissive", Color(1.0f, 0.5f, 0.0f, 1.0f)),
        ConnecterDesc("Specular", Color(1.0f, 0.5f, 0.0f, 1.0f)),
        ConnecterDesc("Gloss", Color(1.0f, 0.5f, 0.0f, 1.0f)),
        ConnecterDesc("Opacity", Color(1.0f, 0.5f, 0.0f, 1.0f)),
        ConnecterDesc("Normal", Color(1.0f, 0.5f, 0.0f, 1.0f)),
        ConnecterDesc("WorldPositionOffset", Color(1.0f, 0.5f, 0.0f, 1.0f)));
    setSize(vec2(128, 256));
    setCanBeSelected(false);
}
bool MaterialGeneratorNodeRootNormalDraw::evaluate( MaterialGeneratorError& error )
{
    const bool result(MaterialGeneratorNode::evaluate(error));
    if (result)
    {
        const ObjectHandle a(getInputConnection(0).getConnection().getVar());

        ct::ShaderGeneratorVariableFactory* const factory(ct::ShaderGeneratorVariableFactory::getInstance());

        const he::ObjectHandle oneHandle(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varOne(factory->get(oneHandle));
        switch (factory->get(a)->getType())
        {
        case MaterialGeneratorVariableType_Float: varOne->setConstant(1.0f); break;
        case MaterialGeneratorVariableType_Float2: varOne->setConstant(vec2(1.0f, 1.0f)); break;
        case MaterialGeneratorVariableType_Float3: varOne->setConstant(vec3(1.0f, 1.0f, 1.0f)); break;
        case MaterialGeneratorVariableType_Float4: varOne->setConstant(vec4(1.0f, 1.0f, 1.0f, 1.0f)); break;
        }

        const he::ObjectHandle resultHandle(m_Parent->getGenerator()->addVariable());
        ct::ShaderGeneratorVariable* const varResult(factory->get(resultHandle));
        varResult->setSubtract(oneHandle, a);

        getOutput(0).setVar(resultHandle);
    }
    return result;
}

#pragma endregion

} } //end namespace
