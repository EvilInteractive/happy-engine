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

const char* const g_Tab = "    ";

ShaderGenerator::ShaderGenerator()
    : m_LocalVarUID(0)
    , m_DiffuseVar(ObjectHandle::unassigned)
    , m_EmissiveVar(ObjectHandle::unassigned)
    , m_SpecularVar(ObjectHandle::unassigned)
    , m_GlossVar(ObjectHandle::unassigned)
    , m_OpacityVar(ObjectHandle::unassigned)
    , m_NormalVar(ObjectHandle::unassigned)
    , m_WorldPositionOffsetVar(ObjectHandle::unassigned)
    , m_AlphaTestValue(ObjectHandle::unassigned)
    , m_ScopeDepth(0)
{
    createGlobalVariables();
}

ShaderGenerator::~ShaderGenerator()
{
    clearShader();
    reset();
    destroyGlobalVariables();
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
    for (size_t i(0); i < ShaderGeneratorOutVariableType_MAX; ++i)
    {
        m_GlobalOutVariables[i] = factory->get(factory->create());
        m_GlobalOutVariables[i]->setGlobal(static_cast<ShaderGeneratorOutVariableType>(i));
    }
}
void ShaderGenerator::destroyGlobalVariables()
{
    ShaderGeneratorVariableFactory* factory(ShaderGeneratorVariableFactory::getInstance());
    for (size_t i(0); i < ShaderGeneratorGlobalInputVariableType_MAX; ++i)
    {
        factory->destroyObject(m_GlobalInputVariables[i]->getHandle());
        m_GlobalInputVariables[i] = nullptr;
    }
    for (size_t i(0); i < ShaderGeneratorGlobalFragmentVariableType_MAX; ++i)
    {
        factory->destroyObject(m_GlobalFragmentVariables[i]->getHandle());
        m_GlobalFragmentVariables[i] = nullptr;
    }
    for (size_t i(0); i < ShaderGeneratorGlobalCodeVariableType_MAX; ++i)
    {
        factory->destroyObject(m_GlobalCodeVariables[i]->getHandle());
        m_GlobalCodeVariables[i] = nullptr;
    }
    for (size_t i(0); i < ShaderGeneratorOutVariableType_MAX; ++i)
    {
        factory->destroyObject(m_GlobalOutVariables[i]->getHandle());
        m_GlobalOutVariables[i] = nullptr;
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

    m_GlobalSwitches.m_UseNormalMap = m_NormalVar != ObjectHandle::unassigned;
    m_GlobalSwitches.m_UseTexcoord = false; // fragment generator will tell us this

    // Fragment Shader
    result &= generateShader(ShaderGeneratorType_Fragment, ShadingType_Forward);
    saveShader(shaderPath.append(shaderName + "_F.frag"));

    result &= generateShader(ShaderGeneratorType_Fragment, ShadingType_Deferred);
    saveShader(shaderPath.append(shaderName + "_D.frag"));


    // Vertex shader
    result &= generateShader(ShaderGeneratorType_Vertex, ShadingType_Forward, DrawType_Single);
    saveShader(shaderPath.append(shaderName + "_F_SI.vert"));

    //result &= generateVertexShader(ShadingType_Forward, ShadingType_Skinned);
    //saveShader(shaderPath.append(shaderName + "_F_SK.vert"));

    result &= generateShader(ShaderGeneratorType_Vertex, ShadingType_Forward, DrawType_Instanced);
    saveShader(shaderPath.append(shaderName + "_F_IN.vert"));

    result &= generateShader(ShaderGeneratorType_Vertex, ShadingType_Deferred, DrawType_Single);
    saveShader(shaderPath.append(shaderName + "_D_SI.vert"));

    //result &= generateVertexShader(ShadingType_Deferred, ShadingType_Skinned);
    //saveShader(shaderPath.append(shaderName + "_D_SK.vert"));

    result &= generateShader(ShaderGeneratorType_Vertex, ShadingType_Deferred, DrawType_Instanced);
    saveShader(shaderPath.append(shaderName + "_D_IN.vert"));

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
    for (size_t i(0); i < ShaderGeneratorOutVariableType_MAX; ++i)
    {
        m_GlobalOutVariables[i]->resetRefcounter();
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
    if (m_AlphaTestValue != ObjectHandle::unassigned)
        factory->get(m_AlphaTestValue)->resetRefcounter();
}

void ShaderGenerator::analyseVariable( ShaderGeneratorVariable* const var )
{
    var->incrementRefcounter();

    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());

    const ShaderGeneratorVariableOperation& operation(var->getOperation());
    for (size_t i(0); i < ShaderGeneratorVariableOperation::MAX_PARAMS; ++i)
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

void ShaderGenerator::analyseVariables()
{
    // Reset
    resetAnalyse();

    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    m_OutVariables.forEach([this, factory](const ObjectHandle& handle)
    {
        analyseVariable(factory->get(handle));
    });

    // Alpha test
    if (m_AlphaTestValue != ObjectHandle::unassigned && m_OpacityVar != ObjectHandle::unassigned)
    {
        analyseVariable(factory->get(m_AlphaTestValue));
        analyseVariable(factory->get(m_OpacityVar));
    }
}

void ShaderGenerator::inititalizeInternalVertexVars( const ShadingType /*shadingType*/, const DrawType drawType )
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
    outPosition->setLocalName(getOutVariableName(ShaderGeneratorOutVariableType_GLPosition));

    switch (drawType)
    {
        case DrawType_Single:
        {
            ObjectHandle wvp(getVariable(ShaderGeneratorGlobalCodeVariableType_WorldViewProjection));
            outPosition->setMultiply(wvp, localPos4_1->getHandle());
        } break;
        case DrawType_Skinned:
        {
            LOG(LogType_ProgrammerAssert, "Not implemented");
        } break;
        case DrawType_Instanced:
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

void ShaderGenerator::inititalizeInternalFragmentVars( const ShadingType shadingType )
{
    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());

    ShaderGeneratorVariable* const one(addInternalVariable());
    one->setConstant(1.0f);

    if (shadingType == ShadingType_Deferred)
    {
        const ObjectHandle outColor(getVariable(ShaderGeneratorOutVariableType_OutColor));
        const ObjectHandle outNormalDepth(getVariable(ShaderGeneratorOutVariableType_OutNormalDepth));
        const ObjectHandle outSG(getVariable(ShaderGeneratorOutVariableType_OutSG));

        ShaderGeneratorVariable* const outColorVar(factory->get(outColor));
        ShaderGeneratorVariable* const outNormalDepthVar(factory->get(outNormalDepth));
        ShaderGeneratorVariable* const outSGVar(factory->get(outSG));

        m_OutVariables.add(outColor);
        m_OutVariables.add(outNormalDepth);
        m_OutVariables.add(outSG);
        
        // Color
        outColorVar->setComposeFloat4(m_DiffuseVar, one->getHandle());

        // Depth
        const ObjectHandle passDepth(getVariable(ShaderGeneratorGlobalFragmentVariableType_Depth));
        ShaderGeneratorVariable* const depth(addInternalVariable());
        {
            const ObjectHandle camNearFar(getVariable(ShaderGeneratorGlobalCodeVariableType_CameraNearFar));
            ShaderGeneratorVariable* const camNear(addInternalVariable());
            camNear->setSwizzle(camNearFar, ShaderGeneratorSwizzleMask_X);
            ShaderGeneratorVariable* const camFar(addInternalVariable());
            camFar->setSwizzle(camNearFar, ShaderGeneratorSwizzleMask_Y);

            ShaderGeneratorVariable* const camRange(addInternalVariable());
            camRange->setSubtract(camFar->getHandle(), camNear->getHandle());

            ShaderGeneratorVariable* const depthRange(addInternalVariable());
            depthRange->setSubtract(passDepth, camNear->getHandle());

            depth->setDivide(depthRange->getHandle(), camRange->getHandle());
            depth->setForceDeclare(true);
        }

        // Normal
        const ObjectHandle passNormal(getVariable(ShaderGeneratorGlobalFragmentVariableType_ViewNormal));
        ShaderGeneratorVariable* const normal(addInternalVariable());
        if (m_NormalVar != ObjectHandle::unassigned)
        {
            const ObjectHandle passTangent(getVariable(ShaderGeneratorGlobalFragmentVariableType_ViewTangent));
            m_GlobalSwitches.m_UseNormalMap = true;
            ShaderGeneratorVariable* const calcNormal(addInternalVariable());
            calcNormal->setCalculateNormal(passNormal, passTangent, m_NormalVar);
            normal->setEncodeNormal(calcNormal->getHandle());
            normal->setForceDeclare(true);
        }
        else
        {
            normal->setEncodeNormal(passNormal);
        }

        // Normal - Depth
        outNormalDepthVar->setComposeFloat3(normal->getHandle(), depth->getHandle());

        // SG
        outSGVar->setComposeFloat4(m_SpecularVar, m_GlossVar);
    }
    else
    {

    }
}

bool ShaderGenerator::generateShader( const ShaderGeneratorType genType, const ShadingType shadingType, const DrawType drawType )
{
    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());

    clearShader();

    if (genType == ShaderGeneratorType_Vertex)
    {
        inititalizeInternalVertexVars(shadingType, drawType);
    }
    else
    {
        inititalizeInternalFragmentVars(shadingType);
    }

    analyseVariables();

    if (genType == ShaderGeneratorType_Fragment)
    {
        m_GlobalSwitches.m_UseTexcoord |= m_GlobalFragmentVariables[ShaderGeneratorGlobalFragmentVariableType_TexCoord]->getRefCount() > 0;
    }
    
    writeHeader();

    if (genType == ShaderGeneratorType_Vertex)
    {
        writeGlobalVertexVariablesDeclarations();
    }
    else
    {
        writeGlobalFragmentVariablesDeclarations();
    }

    writeNewLine();

    if (genType == ShaderGeneratorType_Fragment && m_GlobalSwitches.m_UseNormalMap)
    {
        writeNormalFunction();
    }

    writeNewLine();

    writeOpenMain();

    if (m_AlphaTestValue != ObjectHandle::unassigned && m_OpacityVar != ObjectHandle::unassigned)
    {
        ShaderGeneratorVariable* const alphaVar(factory->get(m_OpacityVar));
        ShaderGeneratorVariable* const alphaTestValue(factory->get(m_AlphaTestValue));
        writeVariableDeclaration(alphaVar);
        writeVariableDeclaration(alphaTestValue);
        writeDiscardTest(alphaVar, alphaTestValue);
    }

    writeVariableDeclarations();

    m_OutVariables.forEach([this, factory](const ObjectHandle& handle)
    {
        ShaderGeneratorVariable* const var(factory->get(handle));

        write(var->getLocalName());
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
    m_Variables.forEach([this, factory](const ObjectHandle& handle)
    {
        writeVariableDeclaration(factory->get(handle));
    });
    m_InternalVariables.forEach([this, factory](const ObjectHandle& handle)
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
            for (size_t i(0); i < ShaderGeneratorVariableOperation::MAX_PARAMS; ++i)
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
        else if (var->declareVar())
        {
            write("const ");
        }

        if (var->declareVar())
        {
            writeTypeName(var->getType());
            write(" ");
            write(var->getLocalName());
            writeAssignment();
            writeVariable(var, true);
            writeEndLine();
            writeNewLine();
        }
    }
}

void ShaderGenerator::clearShader()
{
    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    m_OutVariables.clear();
    m_InternalVariables.forEach([factory](const ObjectHandle& handle)
    {
        factory->destroyObject(handle);
    });
    m_InternalVariables.clear();

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
    m_AlphaTestValue = ObjectHandle::unassigned;

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
        if (var->getRefCount() > 0)
        {
            writeInVarDeclaration(var);
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
    writeNewLine();
    for (size_t i(0); i < ShaderGeneratorOutVariableType_MAX; ++i)
    {
        const ShaderGeneratorVariable* const var(m_GlobalOutVariables[i]);
        if (var->getRefCount() > 0)
        {
            writeOutVarDeclaration(var);
        }
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
        writeVariable(factory->get(operation.params[2]));\
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

    GLSL_FUNC1(ShaderGeneratorVariableOperationType_EncodeNormal, "encodeNormal");

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
    GLSL_FUNC3(ShaderGeneratorVariableOperationType_CalcNormal, "calcNormal");

    // Multiple
    case ShaderGeneratorVariableOperationType_ComposeFloat3:
    {
        write("vec3(");
        writeVariable(factory->get(operation.params[0]));
        write(", ");
        writeVariable(factory->get(operation.params[1]));
        if (operation.params[2] != ObjectHandle::unassigned)
        {
            write(", ");
            writeVariable(factory->get(operation.params[2]));
        }
        write(")");
    } break;
    case ShaderGeneratorVariableOperationType_ComposeFloat4:
    {
        write("vec4(");
        writeVariable(factory->get(operation.params[0]));
        write(", ");
        writeVariable(factory->get(operation.params[1]));
        if (operation.params[2] != ObjectHandle::unassigned)
        {
            write(", ");
            writeVariable(factory->get(operation.params[2]));
            if (operation.params[3] != ObjectHandle::unassigned)
            {
                write(", ");
                writeVariable(factory->get(operation.params[3]));
            }
        }
        write(")");
    } break;
    case ShaderGeneratorVariableOperationType_Swizzle:
    {
        writeVariable(factory->get(operation.params[0]));
        write(".");
        write(shaderGeneratorSwizzleMaskToString(operation.swizzleParams[0]));
        write(shaderGeneratorSwizzleMaskToString(operation.swizzleParams[1]));
        write(shaderGeneratorSwizzleMaskToString(operation.swizzleParams[2]));
        write(shaderGeneratorSwizzleMaskToString(operation.swizzleParams[3]));
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
    const ShaderGeneratorVariableOperation& operation(var->getOperation());

    if (var->declareVar() && forceInline == false)
    {
        HE_ASSERT(var->getLocalName().empty() == false, "ShaderVar does not have a name"); 
        write(var->getLocalName());
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
        "//This file was was generated with the HappyMaterialEditor                       \n"
        "                                                                                 \n"
        "#version 150 core                                                                \n"
        "#pragma optionNV(fastmath on)                                                    \n"
        "//#pragma optionNV(fastprecision on)                                             \n"
        "#pragma optionNV(ifcvt none)                                                     \n"
        "#pragma optionNV(inline all)                                                     \n"
        "#pragma optionNV(strict on)                                                      \n"
        "#pragma optionNV(unroll all)                                                     \n"
        "                                                                                 \n";
}

void ShaderGenerator::writeNormalFunction()
{
    m_ShaderFile << 
        "vec3 calcNormal(in vec3 normal, in vec3 tangent, in vec3 rgb)            \n"
        "{                                                                        \n"
        "    //NormalMap                                                          \n"
        "    tangent = normalize(tangent);                                        \n"
        "    normal = normalize(normal);                                          \n"
        "                                                                         \n"
        "    vec3 binormal = cross(tangent, normal);                              \n"
        "                                                                         \n"
        "    mat3 assenstelsel = mat3(binormal, tangent, normal);                 \n"
        "                                                                         \n"
        "    vec3 xyz = vec3(rgb.x * 2 - 1, (1 - rgb.y) * 2 - 1, rgb.z * 2 - 1);  \n"
        "                                                                         \n"
        "    return normalize(assenstelsel * xyz);                                \n"
        "}                                                                        \n"
        "                                                                         \n"
        "vec2 encodeNormal(in vec3 normal)                                        \n"
        "{                                                                        \n"
        "    float f = sqrt(-normal.z * 8.0f + 8.0f);                             \n"
        "    return normal.xy / f + 0.5f;                                         \n"
        "}                                                                        \n"
        "                                                                         \n";
}

void ShaderGenerator::writeOpenMain()
{
    write("void main()");
    writeNewLine();
    writeOpenScope();
}

void ShaderGenerator::writeCloseMain()
{
    writeCloseScope();
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
    write("in ");
    writeTypeName(var->getType());
    write(" ");
    write(var->getLocalName());
    writeEndLine();
    writeNewLine();
}

void ShaderGenerator::writeOutVarDeclaration( const ShaderGeneratorVariable* const var )
{
    write("out ");
    writeTypeName(var->getType());
    write(" ");
    write(var->getLocalName());
    writeEndLine();
    writeNewLine();
}

void ShaderGenerator::writeCodeVarDeclaration( const ShaderGeneratorVariable* const var )
{
    write("uniform ");
    writeTypeName(var->getType());
    write(" ");
    write(var->getLocalName());
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

void ShaderGenerator::writeDiscardTest( const ShaderGeneratorVariable* const value, const ShaderGeneratorVariable* const test )
{
    write("if (");
    writeVariable(value);
    write(" < ");
    writeVariable(test);
    write(")");
    writeNewLine();
    writeOpenScope();
    write("discard;");
    writeCloseScope();
    writeNewLine();
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

const ObjectHandle& ShaderGenerator::getVariable( const ShaderGeneratorOutVariableType type )
{
    return m_GlobalOutVariables[type]->getHandle();
}

void ShaderGenerator::write(const char* const text)
{
    m_ShaderFile << text;
}

void ShaderGenerator::write(const std::string& text)
{
    m_ShaderFile << text;
}

void ShaderGenerator::writeIndend()
{
    m_ShaderFile << g_Tab;
}

void ShaderGenerator::writeOpenScope()
{
    write("{");
    ++m_ScopeDepth;
    writeNewLine();
}

void ShaderGenerator::writeCloseScope()
{
    HE_ASSERT(m_ScopeDepth > 0, "scope going sub zero");
    --m_ScopeDepth;
    writeNewLine();
    write("}");
    writeNewLine();
}

void ShaderGenerator::writeNewLine()
{
    write("\n");
    for (uint8 i(0); i < m_ScopeDepth; ++i)
    {
        writeIndend();
    }
}


} } //end namespace
