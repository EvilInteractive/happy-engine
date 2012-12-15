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
//Created: 17/11/2012
#include "HappyPCH.h" 

#include "ShaderGeneratorVariableFactory.h"
#include "ShaderGeneratorVariable.h"

namespace he {
namespace ct {

ShaderGenerator::ShaderGenerator()
    : m_LocalVarUID(0)
    , m_State(State_Idle)
    , m_VertexShader("")
    , m_FragmentShader("")
    , m_FragmentOutput(ObjectHandle::unassigned)
    , m_VertexOutput(ObjectHandle::unassigned)
{

}

ShaderGenerator::~ShaderGenerator()
{

}

he::ObjectHandle ShaderGenerator::addVariable()
{
    ShaderGeneratorVariableFactory* factory(ShaderGeneratorVariableFactory::getInstance());
    ObjectHandle result(factory->create());
    m_Variables.add(result);
    return result;
}

bool ShaderGenerator::compile()
{
    m_VertexShader.clear();
    m_FragmentShader.clear();
    m_ShaderFile.clear();
    m_ShaderFile.str("");

    ShaderGeneratorVariableFactory* factory(ShaderGeneratorVariableFactory::getInstance());

    m_PassVariables.clear();
    m_Variables.forEach([this, factory](const ObjectHandle& handle)
    {
        ShaderGeneratorVariable* var(factory->get(handle));
        if (var->getVertUseCount() >= 1 && var->getOperation().type == ShaderGeneratorVariableOperationType_Global)
        {
            createLocalVar(var);
        }
    });


    //////////////////////////////////////////////////////////////////////////
    // Vertex
    //////////////////////////////////////////////////////////////////////////
    m_State = State_VertexShader;

    // Vertex
    writeHeader();
    writeNewLine();
    writeVertexInputVars();
    writeNewLine();
    writeVertexGlobalVars();

    writeNewLine();
    writeOpenMain();

    m_Variables.forEach([this, factory](const ObjectHandle& handle)
    {
        ShaderGeneratorVariable* var(factory->get(handle));
        if (var->getVertUseCount() > 1 && var->getOperation().type != ShaderGeneratorVariableOperationType_Constant)
            createLocalVar(var);
    });
    writeVertexOutput();

    writeCloseMain();
    writeNewLine();

    m_VertexShader = m_ShaderFile.str();
    m_ShaderFile.clear();
    m_ShaderFile.str("");

    //////////////////////////////////////////////////////////////////////////
    // Fragment
    //////////////////////////////////////////////////////////////////////////
    m_State = State_FragmentShader;
    writeHeader();
    writeNewLine();
    writeVertexPassVars();
    writeNewLine();
    writeFragmentGlobalVars();

    writeNewLine();
    writeOpenMain();

    m_Variables.forEach([this, factory](const ObjectHandle& handle)
    {
        ShaderGeneratorVariable* var(factory->get(handle));
        if (var->getFragUseCount() > 1 && var->getOperation().type != ShaderGeneratorVariableOperationType_Constant)
            createLocalVar(var);
    });
    writeFragmentOutput();

    writeCloseMain();
    writeNewLine();

    m_FragmentShader = m_ShaderFile.str();
    m_ShaderFile.clear();
    m_ShaderFile.str("");
    m_State = State_Idle;

    return true;
}

void ShaderGenerator::createLocalVar( ShaderGeneratorVariable* const var )
{
    uint32 id(m_LocalVarUID++);
    char name[20];
    sprintf(name, "localvar%d\0", id);
    var->setLocalName(name);

    writeVariableDeclaration(var);
    writeOperation(var->getOperation());
    writeEndLine();
    writeNewLine();
}

void ShaderGenerator::writeVariableDeclaration( const ShaderGeneratorVariable* const var )
{
    if (var->getOperation().type == ShaderGeneratorVariableOperationType_Constant)
        m_ShaderFile << "const ";
    writeTypeName(var->getType());
    m_ShaderFile << " ";
    writeVariable(var);
}

void ShaderGenerator::writeTypeName( const ShaderGeneratorVariableType type )
{
    switch (type)
    {
        case ShaderVariableType_Unknown: { m_ShaderFile << "Unknown"; break; }
        case ShaderVariableType_Int: { m_ShaderFile << "int"; break; }
        case ShaderVariableType_Int2: { m_ShaderFile << "ivec2"; break; }
        case ShaderVariableType_Int3: { m_ShaderFile << "ivec3"; break; }
        case ShaderVariableType_Int4: { m_ShaderFile << "ivec4"; break; }
        case ShaderVariableType_Uint: { m_ShaderFile << "uint"; break; }
        case ShaderVariableType_Float: { m_ShaderFile << "float"; break; }
        case ShaderVariableType_Float2: { m_ShaderFile << "vec2"; break; }
        case ShaderVariableType_Float3: { m_ShaderFile << "vec3"; break; }
        case ShaderVariableType_Float4: { m_ShaderFile << "vec4"; break; }
        case ShaderVariableType_Mat44: { m_ShaderFile << "mat4"; break; }
        case ShaderVariableType_Texture2D: { m_ShaderFile << "sampler2D"; break; }
        case ShaderVariableType_TextureCube: { m_ShaderFile << "samplerCube"; break; }
        default: LOG(LogType_ProgrammerAssert, "Unknown type! %d", type); break;
    }
}

void ShaderGenerator::writeOperation( const ShaderGeneratorVariableOperation& operation )
{
#pragma region defines
    #define GLSL_FUNC1(type, func)\
    case type: \
    { \
        m_ShaderFile << func << "(";\
        writeVariable(factory->get(operation.params[0]));\
        m_ShaderFile << ")";\
    } break;
    #define GLSL_FUNC2(type, func)\
    case type: \
    { \
        m_ShaderFile << func << "(";\
        writeVariable(factory->get(operation.params[0]));\
        m_ShaderFile << ", ";\
        writeVariable(factory->get(operation.params[1]));\
        m_ShaderFile << ")";\
    } break;
    #define GLSL_FUNC3(type, func)\
    case type: \
    { \
        m_ShaderFile << func << "(";\
        writeVariable(factory->get(operation.params[0]));\
        m_ShaderFile << ", ";\
        writeVariable(factory->get(operation.params[1]));\
        m_ShaderFile << ", ";\
        writeVariable(factory->get(operation.params[3]));\
        m_ShaderFile << ")";\
    } break;
    #define GLSL_OPERATOR(type, op)\
    case type: \
    { \
        m_ShaderFile << "(";\
        writeVariable(factory->get(operation.params[0]));\
        m_ShaderFile << " "op" "; \
        writeVariable(factory->get(operation.params[1]));\
        m_ShaderFile << ")";\
    } break;
#pragma endregion

    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    switch (operation.type)
    {
    case ShaderGeneratorVariableOperationType_Constant:
    {
        LOG(LogType_ProgrammerAssert, "Should not get here");
    } break;
    // Operators
    GLSL_OPERATOR(ShaderGeneratorVariableOperationType_Add, "+" );
    GLSL_OPERATOR(ShaderGeneratorVariableOperationType_Div, "/" );
    GLSL_OPERATOR(ShaderGeneratorVariableOperationType_Mul, "*" );
    GLSL_OPERATOR(ShaderGeneratorVariableOperationType_Sub, "-" );

    // Func1
    GLSL_FUNC1(ShaderGeneratorVariableOperationType_Sin, "sin");
    GLSL_FUNC1(ShaderGeneratorVariableOperationType_Cos, "cos");
    GLSL_FUNC1(ShaderGeneratorVariableOperationType_Tan, "tan");
    GLSL_FUNC1(ShaderGeneratorVariableOperationType_ASin, "asin");
    GLSL_FUNC1(ShaderGeneratorVariableOperationType_ACos, "acos");
    GLSL_FUNC1(ShaderGeneratorVariableOperationType_ATan, "atan");

    GLSL_FUNC1(ShaderGeneratorVariableOperationType_Log, "log");
    GLSL_FUNC1(ShaderGeneratorVariableOperationType_Log2, "log2");
    GLSL_FUNC1(ShaderGeneratorVariableOperationType_Sqrt, "sqrt");
    GLSL_FUNC1(ShaderGeneratorVariableOperationType_InvSqrt, "inversesqrt");

    GLSL_FUNC1(ShaderGeneratorVariableOperationType_Abs, "abs");
    GLSL_FUNC1(ShaderGeneratorVariableOperationType_Sign, "sign");
    GLSL_FUNC1(ShaderGeneratorVariableOperationType_Floor, "floor");
    GLSL_FUNC1(ShaderGeneratorVariableOperationType_Ceil, "ceil");
    GLSL_FUNC1(ShaderGeneratorVariableOperationType_Round, "round");
    GLSL_FUNC1(ShaderGeneratorVariableOperationType_Frac, "frac");
    GLSL_FUNC1(ShaderGeneratorVariableOperationType_Length, "length");
    GLSL_FUNC1(ShaderGeneratorVariableOperationType_Normalize, "normalize");

    // Fuc2
    GLSL_FUNC2(ShaderGeneratorVariableOperationType_Pow, "pow");
    GLSL_FUNC2(ShaderGeneratorVariableOperationType_Mod, "mod");
    GLSL_FUNC2(ShaderGeneratorVariableOperationType_Min, "min");
    GLSL_FUNC2(ShaderGeneratorVariableOperationType_Max, "max");
    GLSL_FUNC2(ShaderGeneratorVariableOperationType_Step, "step");
    GLSL_FUNC2(ShaderGeneratorVariableOperationType_Dot, "dot");
    GLSL_FUNC2(ShaderGeneratorVariableOperationType_Cross, "cross");
    GLSL_FUNC2(ShaderGeneratorVariableOperationType_Reflect, "reflect");
    GLSL_FUNC2(ShaderGeneratorVariableOperationType_Distance, "distance");
    GLSL_FUNC2(ShaderGeneratorVariableOperationType_Texture, "texture");

    // Func 3
    GLSL_FUNC3(ShaderGeneratorVariableOperationType_Clamp, "clamp");
    GLSL_FUNC3(ShaderGeneratorVariableOperationType_Lerp, "mix");
    GLSL_FUNC3(ShaderGeneratorVariableOperationType_SmoothStep, "smoothstep");
    GLSL_FUNC3(ShaderGeneratorVariableOperationType_Refract, "refract");
    GLSL_FUNC3(ShaderGeneratorVariableOperationType_TextureLod, "textureLod");
    GLSL_FUNC3(ShaderGeneratorVariableOperationType_TextureOffset, "textureOffset");

    default:
        {
            LOG(LogType_ProgrammerAssert, "Unknown ShaderGeneratorVariableOperationType");
        }
        break;
    }

    #undef GLSL_FUNC1
    #undef GLSL_FUNC2
    #undef GLSL_FUNC3
    #undef GLSL_OPERATOR
}

void ShaderGenerator::writeVariable( const ShaderGeneratorVariable* const var, const bool forceInline )
{
    HE_ASSERT(var != nullptr, "Writing nullptr var");
    HE_ASSERT(m_State == State_Idle, "Writing in invalid state");
    const uint16 usageCount(m_State == State_VertexShader? var->getVertUseCount() : var->getFragUseCount());
    if (usageCount > 1 && forceInline == false)
    {
        HE_ASSERT(var->getLocalName().empty() == false, "ShaderVar does not have a name"); 
        m_ShaderFile << var->getLocalName();
    }
    else
    {
        if (var->getOperation().type == ShaderGeneratorVariableOperationType_Constant)
        {
            writeConstant(var);
        }
        else
        {
            writeOperation(var->getOperation());
        }
    }
}

void ShaderGenerator::writeHeader()
{
    m_ShaderFile << 
        "//HappyEngine Copyright (C) 2012 Evil Interactive                                \n"
        "//                                                                               \n"
        "//This file is part of HappyEngine.                                              \n"
        "//                                                                               \n"
        "//    HappyEngine is free software: you can redistribute it and/or modify        \n"
        "//    it under the terms of the GNU Lesser General Public License as published by\n"
        "//    the Free Software Foundation, either version 3 of the License, or          \n"
        "//    (at your option) any later version.                                        \n"
        "//                                                                               \n"
        "//    HappyEngine is distributed in the hope that it will be useful,             \n"
        "//    but WITHOUT ANY WARRANTY; without even the implied warranty of             \n"
        "//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              \n"
        "//    GNU Lesser General Public License for more details.                        \n"
        "//                                                                               \n"
        "//    You should have received a copy of the GNU Lesser General Public License   \n"
        "//    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.       \n"
        "//                                                                               \n"
        "//This file was was generated with the HappyMaterialEditor                       \n";
}

void ShaderGenerator::writeOpenMain()
{
    m_ShaderFile << "void main() \n{\n";
}

void ShaderGenerator::writeCloseMain()
{
    m_ShaderFile << "}\n";
}

void ShaderGenerator::writeConstant( const ShaderGeneratorVariable* const variable )
{
    switch (variable->getType())
    {
    case ShaderVariableType_Float:
        {
            m_ShaderFile << variable->getFloatData();
        } break;
    case ShaderVariableType_Float2:
        {
            const vec2& float2(variable->getFloat2Data());
            m_ShaderFile << "vec2(" << float2.x << ", " << float2.y << ")";
        } break;
    case ShaderVariableType_Float3:
        {
            const vec3& float3(variable->getFloat3Data());
            m_ShaderFile << "vec3(" << float3.x << ", " << float3.y << ", " << float3.z << ")";
        } break;
    case ShaderVariableType_Float4:
        {
            const vec4& float4(variable->getFloat4Data());
            m_ShaderFile << "vec4(" << float4.x << ", " << float4.y << ", " << float4.z << ", " << float4.w << ")";
        } break;
    default:
        {
            LOG(LogType_ProgrammerAssert, "Not implemented type");
        }
    }
}

void ShaderGenerator::setFragmentOutput( const ObjectHandle& var )
{
    m_FragmentOutput = var;
}

void ShaderGenerator::setVertexOutput( const ObjectHandle& var )
{
    m_VertexOutput = var;
}

} } //end namespace
