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
#include "FileWriter.h"

namespace he {
namespace ct {

ShaderGenerator::ShaderGenerator()
    : m_LocalVarUID(0)
    , m_DiffuseVar(ObjectHandle::unassigned)
    , m_EmissiveVar(ObjectHandle::unassigned)
    , m_SpecularVar(ObjectHandle::unassigned)
    , m_GlossVar(ObjectHandle::unassigned)
    , m_OpacityVar(ObjectHandle::unassigned)
    , m_NormalVar(ObjectHandle::unassigned)
    , m_WorldPositionOffsetVar(ObjectHandle::unassigned)
{
    createGlobalVariables();
}

ShaderGenerator::~ShaderGenerator()
{

}

void ShaderGenerator::createGlobalVariables()
{
    ShaderGeneratorVariableFactory* factory(ShaderGeneratorVariableFactory::getInstance());
    for (size_t i(0); i < ShaderGeneratorGlobalInputVariableType_MAX; ++i)
    {
        m_GlobalInputVariables[i] = factory->get(factory->create());
        m_GlobalInputVariables[i]->setGlobal(static_cast<ShaderGeneratorGlobalInputVariableType>(i));
    }
    for (size_t i(0); i < ShaderGeneratorGlobalFragmentVariableType_MAX; ++i)
    {
        m_GlobalFragmentVariables[i] = factory->get(factory->create());
        m_GlobalFragmentVariables[i]->setGlobal(static_cast<ShaderGeneratorGlobalFragmentVariableType>(i));
    }
    for (size_t i(0); i < ShaderGeneratorGlobalCodeVariableType_MAX; ++i)
    {
        m_GlobalCodeVariables[i] = factory->get(factory->create());
        m_GlobalCodeVariables[i]->setGlobal(static_cast<ShaderGeneratorGlobalCodeVariableType>(i));
    }
}

he::ObjectHandle ShaderGenerator::addVariable()
{
    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    ObjectHandle result(factory->create());
    m_Variables.add(result);

    ShaderGeneratorVariable* const var(factory->get(result));

    const uint32 id(m_LocalVarUID++);
    char name[20];
    sprintf(name, "localvar%d\0", id);
    var->setLocalName(name);

    return result;
}

ShaderGeneratorVariable* ShaderGenerator::addInternalVariable()
{
    ShaderGeneratorVariableFactory* factory(ShaderGeneratorVariableFactory::getInstance());
    ObjectHandle result(factory->create());
    m_InternalVariables.add(result);

    ShaderGeneratorVariable* const var(factory->get(result));

    const uint32 id(m_LocalVarUID++);
    char name[20];
    sprintf(name, "localvar%d\0", id);
    var->setLocalName(name);

    return var;
}


bool ShaderGenerator::compile(const Path& shaderPath, const std::string& shaderName)
{
    bool result(true);

    // Vertex shader
    result &= generateVertexShader(ShadingType_Forward, ShadingType_Single);
    saveShader(shaderPath.append(shaderName + "_F_SI.vert"));

    //result &= generateVertexShader(ShadingType_Forward, ShadingType_Skinned);
    //saveShader(shaderPath.append(shaderName + "_F_SK.vert"));

    result &= generateVertexShader(ShadingType_Forward, ShadingType_Instanced);
    saveShader(shaderPath.append(shaderName + "_F_IN.vert"));

    result &= generateVertexShader(ShadingType_Deferred, ShadingType_Single);
    saveShader(shaderPath.append(shaderName + "_D_SI.vert"));

    //result &= generateVertexShader(ShadingType_Deferred, ShadingType_Skinned);
    //saveShader(shaderPath.append(shaderName + "_D_SK.vert"));

    result &= generateVertexShader(ShadingType_Deferred, ShadingType_Instanced);
    saveShader(shaderPath.append(shaderName + "_D_IN.vert"));


    // Fragment Shader
    result &= generateFragmentShader(ShadingType_Forward);
    saveShader(shaderPath.append(shaderName + "_F.frag"));

    result &= generateFragmentShader(ShadingType_Deferred);
    saveShader(shaderPath.append(shaderName + "_D.frag"));

    return result;
}

void ShaderGenerator::saveShader( const Path& shaderPath )
{
    io::FileWriter writer;
    if (writer.open(shaderPath.str(), false))
    {
        writer << m_ShaderFile.str();
        writer.close();
    }
}

void ShaderGenerator::resetAnalyse()
{
    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    m_Variables.forEach([factory](const ObjectHandle& handle)
    {
        ShaderGeneratorVariable* const var(factory->get(handle));
        var->resetRefcounter();
        var->setHasDeclaration(false);
    });
    for (size_t i(0); i < ShaderGeneratorGlobalInputVariableType_MAX; ++i)
    {
        m_GlobalInputVariables[i]->resetRefcounter();
    }
    for (size_t i(0); i < ShaderGeneratorGlobalFragmentVariableType_MAX; ++i)
    {
        m_GlobalFragmentVariables[i]->resetRefcounter();
    }
    for (size_t i(0); i < ShaderGeneratorGlobalCodeVariableType_MAX; ++i)
    {
        m_GlobalCodeVariables[i]->resetRefcounter();
    }
    if (m_DiffuseVar != ObjectHandle::unassigned)
        factory->get(m_DiffuseVar)->resetRefcounter();
    if (m_EmissiveVar != ObjectHandle::unassigned)
        factory->get(m_EmissiveVar)->resetRefcounter();
    if (m_SpecularVar != ObjectHandle::unassigned)
        factory->get(m_SpecularVar)->resetRefcounter();
    if (m_GlossVar != ObjectHandle::unassigned)
        factory->get(m_GlossVar)->resetRefcounter();
    if (m_OpacityVar != ObjectHandle::unassigned)
        factory->get(m_OpacityVar)->resetRefcounter();
    if (m_NormalVar != ObjectHandle::unassigned)
        factory->get(m_NormalVar)->resetRefcounter();
    if (m_WorldPositionOffsetVar != ObjectHandle::unassigned)
        factory->get(m_WorldPositionOffsetVar)->resetRefcounter();
}

void ShaderGenerator::analyseVariable( ShaderGeneratorVariable* const var )
{
    var->incrementRefcounter();

    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());

    const ShaderGeneratorVariableOperation& operation(var->getOperation());
    for (size_t i(0); i < 4; ++i)
    {
        ObjectHandle p(operation.params[i]);
        if (p != ObjectHandle::unassigned)
        {
            analyseVariable(factory->get(p));
        }
        else // no holes in parameters allowed
        {
            break;
        }
    }
}

void ShaderGenerator::analyseVertexVariables(const ShadingType shadingType, const DrawType /*drawType*/)
{
    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());

    // Reset
    resetAnalyse();
    
    switch (shadingType)
    {
        case ShadingType_Forward:
        {
            m_OutVariables.forEach([this, factory](const ObjectHandle& handle)
            {
                analyseVariable(factory->get(handle));
            });
        } break;
        case ShadingType_Deferred:
        {

        } break;
    }
}

void ShaderGenerator::analyseFragmentVariables( const ShadingType /*shadingType*/, const DrawType /*drawType*/ )
{
    // Reset
    resetAnalyse();

}

void ShaderGenerator::intitializeInternalVertexVars( const ShadingType /*shadingType*/, const DrawType drawType )
{
    ShaderGeneratorVariable* const const0(addInternalVariable());
    const0->setConstant(0.0f);
    ShaderGeneratorVariable* const const1(addInternalVariable());
    const1->setConstant(1.0f);

    // Normal
    ObjectHandle localNormal(getVariable(ShaderGeneratorGlobalInputVariableType_Normal));
    ObjectHandle worldView(getVariable(ShaderGeneratorGlobalCodeVariableType_WorldView));
    
    ShaderGeneratorVariable* localNormal4_0(addInternalVariable());
    localNormal4_0->setComposeFloat4(localNormal, const0->getHandle());

    ShaderGeneratorVariable* viewNormal4(addInternalVariable());
    viewNormal4->setMultiply(worldView, localNormal4_0->getHandle());

    ShaderGeneratorVariable* passNormal(addInternalVariable());
    passNormal->setLocalName(getGlobalFragmentVariableName(ShaderGeneratorGlobalFragmentVariableType_ViewNormal));
    passNormal->setSwizzle(viewNormal4->getHandle(), ShaderGeneratorSwizzleMask_X, ShaderGeneratorSwizzleMask_Y, ShaderGeneratorSwizzleMask_Z);
    m_OutVariables.add(passNormal->getHandle());

    // Position
    ObjectHandle inPos3(getVariable(ShaderGeneratorGlobalInputVariableType_Position));

    ObjectHandle localPos(inPos3);
    if (m_WorldPositionOffsetVar != ObjectHandle::unassigned)
    {
        ShaderGeneratorVariable* const localPosVar(addInternalVariable());
        localPosVar->setAdd(inPos3, m_WorldPositionOffsetVar);
        localPos = localPosVar->getHandle();
    }

    ShaderGeneratorVariable* const localPos4_0(addInternalVariable());
    localPos4_0->setComposeFloat4(localPos, const0->getHandle());

    ShaderGeneratorVariable* const localPos4_1(addInternalVariable());
    localPos4_1->setComposeFloat4(localPos, const1->getHandle());

    ShaderGeneratorVariable* outPosition(addInternalVariable());
    outPosition->setLocalName("gl_Position");

    switch (drawType)
    {
        case ShadingType_Single:
        {
            ObjectHandle wvp(getVariable(ShaderGeneratorGlobalCodeVariableType_WorldViewProjection));
            outPosition->setMultiply(wvp, localPos4_1->getHandle());
        } break;
        case ShadingType_Skinned:
        {
            LOG(LogType_ProgrammerAssert, "Not implemented");
        } break;
        case ShadingType_Instanced:
        {
            ObjectHandle inWorld(getVariable(ShaderGeneratorGlobalInputVariableType_World));
            ObjectHandle view(getVariable(ShaderGeneratorGlobalCodeVariableType_View));
            ObjectHandle proj(getVariable(ShaderGeneratorGlobalCodeVariableType_Projection));

            ShaderGeneratorVariable* viewWorld(addInternalVariable());
            viewWorld->setMultiply(view, inWorld);

            ShaderGeneratorVariable* viewPosition(addInternalVariable());
            viewPosition->setMultiply(viewWorld->getHandle(), localPos4_1->getHandle());

            outPosition->setMultiply(proj, viewPosition->getHandle());
        } break;
    }
    
    m_OutVariables.add(outPosition->getHandle());
}

bool ShaderGenerator::generateVertexShader( const ShadingType shadingType, const DrawType drawType )
{
    clearShader();

    intitializeInternalVertexVars(shadingType, drawType);

    analyseVertexVariables(shadingType, drawType);
    
    writeHeader();

    writeGlobalVertexVariablesDeclarations();

    writeNewLine();

    writeOpenMain();

    writeVariableDeclarations();

    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    m_OutVariables.forEach([this, factory](const ObjectHandle& handle)
    {
        ShaderGeneratorVariable* const var(factory->get(handle));

        m_ShaderFile << var->getLocalName();
        writeAssignment();
        writeVariable(var);
        writeEndLine();
        writeNewLine();
    });

    writeCloseMain();
    writeNewLine();

    return true;
}

void ShaderGenerator::writeVariableDeclarations()
{
    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    m_OutVariables.forEach([this, factory](const ObjectHandle& handle)
    {
        writeVariableDeclaration(factory->get(handle));
    });
}

void ShaderGenerator::writeVariableDeclaration( ShaderGeneratorVariable* const var )
{
    if (var->hasDeclaration() == false)
    {
        var->setHasDeclaration(true);

        ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
        const ShaderGeneratorVariableOperation& operation(var->getOperation());
        if (operation.type != ShaderGeneratorVariableOperationType_Constant)
        {
            for (size_t i(0); i < 4; ++i)
            {
                ObjectHandle p(operation.params[i]);
                if (p != ObjectHandle::unassigned)
                {
                    writeVariableDeclaration(factory->get(p));
                }
                else // no holes in parameters allowed
                {
                    break;
                }
            }
        }
        else if (var->getRefCount() > 1)
        {
            m_ShaderFile << "const ";
        }

        if (var->getRefCount() > 1)
        {
            writeTypeName(var->getType());
            m_ShaderFile << " " << var->getLocalName();
            writeAssignment();
            writeVariable(var, true);
            writeEndLine();
            writeNewLine();
        }
    }
}

bool ShaderGenerator::generateFragmentShader( const ShadingType /*shadingType*/ )
{
    return false;
}

void ShaderGenerator::clearShader()
{
    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    m_InternalVariables.forEach([factory](const ObjectHandle& handle)
    {
        factory->destroyObject(handle);
    });
    m_InternalVariables.clear();
    m_OutVariables.clear();

    m_ShaderFile.clear();
    m_ShaderFile.str("");
}

void ShaderGenerator::reset()
{
    clearShader();

    m_DiffuseVar = ObjectHandle::unassigned;
    m_EmissiveVar = ObjectHandle::unassigned;
    m_SpecularVar = ObjectHandle::unassigned;
    m_GlossVar = ObjectHandle::unassigned;
    m_OpacityVar = ObjectHandle::unassigned;
    m_NormalVar = ObjectHandle::unassigned;
    m_WorldPositionOffsetVar = ObjectHandle::unassigned;

    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    m_Variables.forEach([factory](const ObjectHandle& variable)
    {
        factory->destroyObject(variable);
    });
    m_Variables.clear();

    m_LocalVarUID = 0;
}


void ShaderGenerator::writeGlobalVertexVariablesDeclarations()
{
    for (size_t i(0); i < ShaderGeneratorGlobalInputVariableType_MAX; ++i)
    {
        const ShaderGeneratorVariable* const var(m_GlobalInputVariables[i]);
        if (var->getRefCount() > 0)
        {
            writeInVarDeclaration(var);
        }
    }
    writeNewLine();
    for (size_t i(0); i < ShaderGeneratorGlobalFragmentVariableType_MAX; ++i)
    {
        const ShaderGeneratorVariable* const var(m_GlobalFragmentVariables[i]);
        if (var->getRefCount() > 0)
        {
            writeOutVarDeclaration(var);
        }
    }
    writeNewLine();
    for (size_t i(0); i < ShaderGeneratorGlobalCodeVariableType_MAX; ++i)
    {
        const ShaderGeneratorVariable* const var(m_GlobalCodeVariables[i]);
        if (var->getRefCount() > 0)
        {
            writeCodeVarDeclaration(var);
        }
    }
}
void ShaderGenerator::writeGlobalFragmentVariablesDeclarations()
{
    for (size_t i(0); i < ShaderGeneratorGlobalFragmentVariableType_MAX; ++i)
    {
        const ShaderGeneratorVariable* const var(m_GlobalFragmentVariables[i]);
        writeInVarDeclaration(var);
    }
    writeNewLine();
    for (size_t i(0); i < ShaderGeneratorGlobalCodeVariableType_MAX; ++i)
    {
        const ShaderGeneratorVariable* const var(m_GlobalCodeVariables[i]);
        writeCodeVarDeclaration(var);
    }
}

void ShaderGenerator::writeTypeName( const ShaderGeneratorVariableType type )
{
    switch (type)
    {
        case ShaderGeneratorVariableType_Unknown: { m_ShaderFile << "Unknown"; break; }
        case ShaderGeneratorVariableType_Int: { m_ShaderFile << "int"; break; }
        case ShaderGeneratorVariableType_Int2: { m_ShaderFile << "ivec2"; break; }
        case ShaderGeneratorVariableType_Int3: { m_ShaderFile << "ivec3"; break; }
        case ShaderGeneratorVariableType_Int4: { m_ShaderFile << "ivec4"; break; }
        case ShaderGeneratorVariableType_Uint: { m_ShaderFile << "uint"; break; }
        case ShaderGeneratorVariableType_Float: { m_ShaderFile << "float"; break; }
        case ShaderGeneratorVariableType_Float2: { m_ShaderFile << "vec2"; break; }
        case ShaderGeneratorVariableType_Float3: { m_ShaderFile << "vec3"; break; }
        case ShaderGeneratorVariableType_Float4: { m_ShaderFile << "vec4"; break; }
        case ShaderGeneratorVariableType_Mat44: { m_ShaderFile << "mat4"; break; }
        case ShaderGeneratorVariableType_Texture2D: { m_ShaderFile << "sampler2D"; break; }
        case ShaderGeneratorVariableType_TextureCube: { m_ShaderFile << "samplerCube"; break; }
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
    case ShaderGeneratorVariableOperationType_Exposed:
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
    GLSL_FUNC2(ShaderGeneratorVariableOperationType_ComposeFloat2, "vec2");

    // Func 3
    GLSL_FUNC3(ShaderGeneratorVariableOperationType_Clamp, "clamp");
    GLSL_FUNC3(ShaderGeneratorVariableOperationType_Lerp, "mix");
    GLSL_FUNC3(ShaderGeneratorVariableOperationType_SmoothStep, "smoothstep");
    GLSL_FUNC3(ShaderGeneratorVariableOperationType_Refract, "refract");
    GLSL_FUNC3(ShaderGeneratorVariableOperationType_TextureLod, "textureLod");
    GLSL_FUNC3(ShaderGeneratorVariableOperationType_TextureOffset, "textureOffset");

    // Multiple
    case ShaderGeneratorVariableOperationType_ComposeFloat3:
    {
        m_ShaderFile << "vec3(";
        writeVariable(factory->get(operation.params[0]));
        m_ShaderFile << ", ";
        writeVariable(factory->get(operation.params[1]));
        if (operation.params[2] != ObjectHandle::unassigned)
        {
            m_ShaderFile << ", ";
            writeVariable(factory->get(operation.params[2]));
        }
        m_ShaderFile << ")";
    } break;
    case ShaderGeneratorVariableOperationType_ComposeFloat4:
    {
        m_ShaderFile << "vec4(";
        writeVariable(factory->get(operation.params[0]));
        m_ShaderFile << ", ";
        writeVariable(factory->get(operation.params[1]));
        if (operation.params[2] != ObjectHandle::unassigned)
        {
            m_ShaderFile << ", ";
            writeVariable(factory->get(operation.params[2]));
            if (operation.params[3] != ObjectHandle::unassigned)
            {
                m_ShaderFile << ", ";
                writeVariable(factory->get(operation.params[3]));
            }
        }
        m_ShaderFile << ")";
    } break;
    case ShaderGeneratorVariableOperationType_Swizzle:
    {
        writeVariable(factory->get(operation.params[0]));
        m_ShaderFile << ".";
        m_ShaderFile << shaderGeneratorSwizzleMaskToString(operation.swizzleParams[0]);
        m_ShaderFile << shaderGeneratorSwizzleMaskToString(operation.swizzleParams[1]);
        m_ShaderFile << shaderGeneratorSwizzleMaskToString(operation.swizzleParams[2]);
        m_ShaderFile << shaderGeneratorSwizzleMaskToString(operation.swizzleParams[3]);
    } break;

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
    const uint32 usageCount(var->getRefCount());
    const ShaderGeneratorVariableOperation& operation(var->getOperation());

    if ((usageCount > 1 && forceInline == false) || operation.type == ShaderGeneratorVariableOperationType_Exposed)
    {
        HE_ASSERT(var->getLocalName().empty() == false, "ShaderVar does not have a name"); 
        m_ShaderFile << var->getLocalName();
    }
    else
    {
        if (operation.type == ShaderGeneratorVariableOperationType_Constant)
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
        "//This file was was generated with the HappyMaterialEditor                       \n\n";
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
    case ShaderGeneratorVariableType_Float:
        {
            writeFloat(variable->getFloatData());
        } break;
    case ShaderGeneratorVariableType_Float2:
        {
            writeFloat2(variable->getFloat2Data());
        } break;
    case ShaderGeneratorVariableType_Float3:
        {
            writeFloat3(variable->getFloat3Data());
        } break;
    case ShaderGeneratorVariableType_Float4:
        {
            writeFloat4(variable->getFloat4Data());
        } break;
    default:
        {
            LOG(LogType_ProgrammerAssert, "Not implemented type");
        }
    }
}

void ShaderGenerator::writeInVarDeclaration( const ShaderGeneratorVariable* const var )
{
    m_ShaderFile << "in ";
    writeTypeName(var->getType());
    m_ShaderFile << " " << var->getLocalName();
    writeEndLine();
    writeNewLine();
}

void ShaderGenerator::writeOutVarDeclaration( const ShaderGeneratorVariable* const var )
{
    m_ShaderFile << "out ";
    writeTypeName(var->getType());
    m_ShaderFile << " " << var->getLocalName();
    writeEndLine();
    writeNewLine();
}

void ShaderGenerator::writeCodeVarDeclaration( const ShaderGeneratorVariable* const var )
{
    m_ShaderFile << "uniform ";
    writeTypeName(var->getType());
    m_ShaderFile << " " << var->getLocalName();
    writeEndLine();
    writeNewLine();
}

void ShaderGenerator::writeFloat( const float value )
{
    m_ShaderFile << value;
}

void ShaderGenerator::writeFloat2( const vec2& value )
{
    m_ShaderFile << "vec2(" << value.x << ", " << value.y << ")";
}

void ShaderGenerator::writeFloat3( const vec3& value )
{
    m_ShaderFile << "vec3(" << value.x << ", " << value.y << ", " << value.z << ")";
}

void ShaderGenerator::writeFloat4( const vec4& value )
{
    m_ShaderFile << "vec4(" << value.x << ", " << value.y << ", " << value.z << ", " << value.w << ")";
}

void ShaderGenerator::writeOutPosition()
{
    m_ShaderFile << "gl_Position";
}

const ObjectHandle& ShaderGenerator::getVariable( const ShaderGeneratorGlobalInputVariableType type )
{
    return m_GlobalInputVariables[type]->getHandle();
}

const ObjectHandle& ShaderGenerator::getVariable( const ShaderGeneratorGlobalFragmentVariableType type )
{
    return m_GlobalFragmentVariables[type]->getHandle();
}

const ObjectHandle& ShaderGenerator::getVariable( const ShaderGeneratorGlobalCodeVariableType type )
{
    return m_GlobalCodeVariables[type]->getHandle();
}


} } //end namespace
