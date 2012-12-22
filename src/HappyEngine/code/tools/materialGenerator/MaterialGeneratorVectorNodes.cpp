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

#include "MaterialGeneratorMathNodes.h"
#include "ShaderGeneratorVariableFactory.h"
#include "MaterialGeneratorGraph.h"

#pragma region defines
#define IMPLEMENT_EVALUATE1(class_, func)\
bool class_::evaluate( MaterialGeneratorError& error )\
{\
    const bool result(MaterialGeneratorNode::evaluate(error));\
    if (result)\
    {\
        const ObjectHandle a(getInputConnection(0).getConnection().getVar());\
        \
        ct::ShaderGeneratorVariableFactory* const factory(ct::ShaderGeneratorVariableFactory::getInstance());\
        const he::ObjectHandle handle(m_Parent->getGenerator()->addVariable());\
        ct::ShaderGeneratorVariable* const varResult(factory->get(handle));\
        varResult->func(a);\
        \
        getOutput(0).setVar(handle);\
    }\
    return result;\
}

#define IMPLEMENT_EVALUATE2(class_, func)\
    bool class_::evaluate( MaterialGeneratorError& error )\
{\
    const bool result(MaterialGeneratorNode::evaluate(error));\
    if (result)\
    {\
        const ObjectHandle a(getInputConnection(0).getConnection().getVar());\
        const ObjectHandle b(getInputConnection(1).getConnection().getVar());\
        \
        ct::ShaderGeneratorVariableFactory* const factory(ct::ShaderGeneratorVariableFactory::getInstance());\
        const he::ObjectHandle handle(m_Parent->getGenerator()->addVariable());\
        ct::ShaderGeneratorVariable* const varResult(factory->get(handle));\
        varResult->func(a, b);\
        \
        getOutput(0).setVar(handle);\
    }\
    return result;\
}

#define IMPLEMENT_EVALUATE3(class_, func)\
    bool class_::evaluate( MaterialGeneratorError& error )\
{\
    const bool result(MaterialGeneratorNode::evaluate(error));\
    if (result)\
    {\
        const ObjectHandle a(getInputConnection(0).getConnection().getVar());\
        const ObjectHandle b(getInputConnection(1).getConnection().getVar());\
        const ObjectHandle c(getInputConnection(2).getConnection().getVar());\
        \
        ct::ShaderGeneratorVariableFactory* const factory(ct::ShaderGeneratorVariableFactory::getInstance());\
        const he::ObjectHandle handle(m_Parent->getGenerator()->addVariable());\
        ct::ShaderGeneratorVariable* const varResult(factory->get(handle));\
        varResult->func(a, b, c);\
        \
        getOutput(0).setVar(handle);\
    }\
    return result;\
}
#pragma endregion

namespace he {
namespace tools {
    
// One Param

#pragma region Abs
//MaterialGeneratorNodeAbs::MaterialGeneratorNodeAbs()
//{
//    addOverload(1, 1, MaterialGeneratorVariableType_Float, MaterialGeneratorVariableType_Float);
//    addOverload(1, 1, MaterialGeneratorVariableType_Float2, MaterialGeneratorVariableType_Float2);
//    addOverload(1, 1, MaterialGeneratorVariableType_Float3, MaterialGeneratorVariableType_Float3);
//    addOverload(1, 1, MaterialGeneratorVariableType_Float4, MaterialGeneratorVariableType_Float4);
//    addConnecters(1, 1, ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)), ConnecterDesc("", Color(1.0f, 0.5f, 0.0f, 1.0f)));
//}
//
//IMPLEMENT_EVALUATE1(MaterialGeneratorNodeAbs, setAbs)

#pragma endregion

#undef IMPLEMENT_EVALUATE1
#undef IMPLEMENT_EVALUATE2
#undef IMPLEMENT_EVALUATE3
} } //end namespace
