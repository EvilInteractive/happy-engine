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
//Created: 17/11/2012
#include "HappyPCH.h" 

#include "ShaderGeneratorVariableFactory.h"
#include "ShaderGeneratorVariable.h"
#include "FileWriter.h"

namespace he {
namespace ct {

const char* const g_Tab = "    ";


ShaderGenerator::ShaderGenerator()
    : m_ShaderType(eShaderGeneratorType_Fragment)
    , m_CalcNormalIncluded(false)
    , m_EncodeIncluded(false)
{
    createGlobalVariables();
}

ShaderGenerator::~ShaderGenerator()
{
    resetShader();
    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    m_Variables.forEach([factory](he::ObjectHandle handle)
    {
        factory->destroyObject(handle);
    });
    destroyGlobalVariables();
}

void ShaderGenerator::createGlobalVariables()
{
    ShaderGeneratorVariableFactory* factory(ShaderGeneratorVariableFactory::getInstance());
    for (size_t i(0); i < eShaderGeneratorGlobalInputVariableType_MAX; ++i)
    {
        m_GlobalInputVariables[i] = factory->create();
        factory->get(m_GlobalInputVariables[i])->setGlobal(static_cast<EShaderGeneratorGlobalInputVariableType>(i));
    }
    for (size_t i(0); i < eShaderGeneratorGlobalCameraVariableType_MAX; ++i)
    {
        m_GlobalCameraVariables[i] = factory->create();
        factory->get(m_GlobalCameraVariables[i])->setGlobal(static_cast<EShaderGeneratorGlobalCameraVariableType>(i));
    }
    for (size_t i(0); i < eShaderGeneratorGlobalFrameVariableType_MAX; ++i)
    {
        m_GlobalFrameVariables[i] = factory->create();
        factory->get(m_GlobalFrameVariables[i])->setGlobal(static_cast<EShaderGeneratorGlobalFrameVariableType>(i));
    }
    for (size_t i(0); i < eShaderGeneratorGlobalSamplerVariableType_MAX; ++i)
    {
        m_GlobalSamplerVariables[i] = factory->create();
        factory->get(m_GlobalSamplerVariables[i])->setGlobal(static_cast<EShaderGeneratorGlobalSamplerVariableType>(i));
    }
    for (size_t i(0); i < eShaderGeneratorGlobalSceneVariableType_MAX; ++i)
    {
        m_GlobalSceneVariables[i] = factory->create();
        factory->get(m_GlobalSceneVariables[i])->setGlobal(static_cast<EShaderGeneratorGlobalSceneVariableType>(i));
    }
    for (size_t i(0); i < eShaderGeneratorGlobalOutVariableType_MAX; ++i)
    {
        m_GlobalOutVariables[i] = factory->create();
        factory->get(m_GlobalOutVariables[i])->setGlobal(static_cast<EShaderGeneratorGlobalOutVariableType>(i));
    }
}
void ShaderGenerator::destroyGlobalVariables()
{
    ShaderGeneratorVariableFactory* factory(ShaderGeneratorVariableFactory::getInstance());
    for (size_t i(0); i < eShaderGeneratorGlobalInputVariableType_MAX; ++i)
    {
        factory->destroyObject(m_GlobalInputVariables[i]);
        m_GlobalInputVariables[i] = he::ObjectHandle::unassigned;
    }
    for (size_t i(0); i < eShaderGeneratorGlobalCameraVariableType_MAX; ++i)
    {
        factory->destroyObject(m_GlobalCameraVariables[i]);
        m_GlobalCameraVariables[i] = he::ObjectHandle::unassigned;
    }
    for (size_t i(0); i < eShaderGeneratorGlobalFrameVariableType_MAX; ++i)
    {
        factory->destroyObject(m_GlobalFrameVariables[i]);
        m_GlobalFrameVariables[i] = he::ObjectHandle::unassigned;
    }
    for (size_t i(0); i < eShaderGeneratorGlobalSamplerVariableType_MAX; ++i)
    {
        factory->destroyObject(m_GlobalSamplerVariables[i]);
        m_GlobalSamplerVariables[i] = he::ObjectHandle::unassigned;
    }
    for (size_t i(0); i < eShaderGeneratorGlobalSceneVariableType_MAX; ++i)
    {
        factory->destroyObject(m_GlobalSceneVariables[i]);
        m_GlobalSceneVariables[i] = he::ObjectHandle::unassigned;
    }
    for (size_t i(0); i < eShaderGeneratorGlobalOutVariableType_MAX; ++i)
    {
        factory->destroyObject(m_GlobalOutVariables[i]);
        m_GlobalOutVariables[i] = he::ObjectHandle::unassigned;
    }
}

he::ObjectHandle ShaderGenerator::getVariable( const EShaderGeneratorGlobalInputVariableType type )
{
    return m_GlobalInputVariables[type];
}
he::ObjectHandle ShaderGenerator::getVariable( const EShaderGeneratorGlobalCameraVariableType type )
{
    return m_GlobalCameraVariables[type];
}
he::ObjectHandle ShaderGenerator::getVariable( const EShaderGeneratorGlobalFrameVariableType type )
{
    return m_GlobalFrameVariables[type];
}
he::ObjectHandle ShaderGenerator::getVariable( const EShaderGeneratorGlobalSamplerVariableType type )
{
    return m_GlobalSamplerVariables[type];
}
he::ObjectHandle ShaderGenerator::getVariable( const EShaderGeneratorGlobalSceneVariableType type )
{
    return m_GlobalSceneVariables[type];
}
he::ObjectHandle ShaderGenerator::getVariable( const EShaderGeneratorGlobalOutVariableType type )
{
    return m_GlobalOutVariables[type];
}

he::ObjectHandle ShaderGenerator::addVariable(const char* nameHint)
{
    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    const ObjectHandle result(factory->create());
    ShaderGeneratorVariable* const var(factory->get(result));

    const uint32 id(m_Variables.size());
    char name[64];
    name[63] = '\0';
    hesnprintf(name, 63, "localvar_%s_%d", nameHint, id);
    var->setLocalName(he::String(name));

    m_Variables.add(result);

    return result;
}

void ShaderGenerator::removeVariable( const ObjectHandle& var )
{
    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    factory->destroyObject(var);
    m_Variables.remove(var);
}

bool ShaderGenerator::compile(const Path& shaderPath, const he::String& shaderName)
{
    bool result(false);
    if (m_OutputVariables.size() > 0)
    {
        resetShader();

        // TODO change dependent on vertex / fragment
        ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
        for (size_t i(0); i < eShaderGeneratorGlobalInputVariableType_MAX; ++i)
        {
            factory->get(m_GlobalInputVariables[i])->setLocalName(he::String("pass") +
                getGlobalInputVariableName(checked_numcast<EShaderGeneratorGlobalInputVariableType>(i)));
        }
        startShader();
        calculateUseCounts();
        m_OutputVariables.forEach([this](const ObjectHandle handle)
        {
            recurseDeclareDependencies(handle);
            recurseWriteDeclarations(handle);
            recurseWriteOutput(handle);
            writeLineFeed(m_BodySection);
        });
        endShader();
        saveShader(shaderPath.append(shaderName + ".frag"));
    }
    return result;
}

void ShaderGenerator::saveShader( const Path& shaderPath )
{
    io::FileWriter writer;
    if (writer.open(shaderPath.str(), false))
    {
        writer << m_IncludeSection.str();
        writer << m_GlobalSection.str();
        writer << m_BodySection.str();
        writer.close();
    }
}

void ShaderGenerator::clearOutputVariables()
{
    m_OutputVariables.clear();
}

void ShaderGenerator::addOutputVariable( const ObjectHandle handle )
{
    m_OutputVariables.add(handle);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ShaderGenerator::calculateUseCounts()
{
    m_Variables.forEach([this](const ObjectHandle handle)
    {
        ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
        ShaderGeneratorVariable* var(factory->get(handle));
        var->resetUseCount();
    });
    m_OutputVariables.forEach([this](const ObjectHandle handle)
    {
        recureCalculateUseCountVariable(handle);
    });
}
void ShaderGenerator::recureCalculateUseCountVariable( const he::ObjectHandle handle )
{
    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    ShaderGeneratorVariable* var(factory->get(handle));
    if (var)
    {
        var->increaseUseCount();
    
        const ShaderGeneratorVariableOperation& operation(var->getOperation());
        for (size_t i(0); i < ShaderGeneratorVariableOperation::MAX_PARAMS; ++i)
        {
            recureCalculateUseCountVariable(operation.params[i]);
        }
    }
}

void ShaderGenerator::writeGLFunc( he::StringStream& stream, const int params, const ShaderGeneratorVariableOperation& op, const char* glName )
{
    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    stream << glName << "(";
    const size_t loopMax(params > 0? checked_numcast<size_t>(params) : 4);
    for (size_t i(0); i < loopMax; ++i)
    {
        ShaderGeneratorVariable* param(factory->get(op.params[i]));
        HE_ASSERT(params <= 0 || param, "Param %u for this operation cannot be NULL!", i);
        if (!param)
            break;

        if (i != 0)
            stream << ", ";

        if (param->hasDeclaration())
            stream << param->getLocalName(); 
        else
            writeOperation(stream, *param);
    }
    stream << ")";
}

void ShaderGenerator::writeGLSwizzle( he::StringStream& stream, const ShaderGeneratorVariableOperation& op )
{
    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    ShaderGeneratorVariable* param0(factory->get(op.params[0]));
    if (param0->hasDeclaration())
        stream << param0->getLocalName(); 
    else
        writeOperation(stream, *param0);
    stream << ".";
    for (size_t i(0); i < 4; ++i)
    {
        EShaderGeneratorSwizzleMask mask(op.swizzleParams[i]);
        switch (mask)
        {
        case he::ct::eShaderGeneratorSwizzleMask_X: stream << "x"; break;
        case he::ct::eShaderGeneratorSwizzleMask_Y: stream << "y"; break;
        case he::ct::eShaderGeneratorSwizzleMask_Z: stream << "z"; break;
        case he::ct::eShaderGeneratorSwizzleMask_W: stream << "w"; break;
        }
    }
}

void ShaderGenerator::writeGLOperator( he::StringStream& stream, const ShaderGeneratorVariableOperation& op, const char* glOperator )
{
    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    stream << "(";
    ShaderGeneratorVariable* param0(factory->get(op.params[0]));
    if (param0->hasDeclaration())
        stream << param0->getLocalName(); 
    else
        writeOperation(stream, *param0);

    stream << " " << glOperator << " ";

    ShaderGeneratorVariable* param1(factory->get(op.params[1]));
    if (param1->hasDeclaration())
        stream << param1->getLocalName(); 
    else
        writeOperation(stream, *param1);

    stream << ")"; 
}

void ShaderGenerator::writeConstant( he::StringStream& stream, const ShaderGeneratorVariable& var )
{
    switch (var.getType())
    {
    case eShaderGeneratorVariableType_Float: 
        {
            stream << var.getFloatData(); 
        } break;
    case eShaderGeneratorVariableType_Float2: 
        {
            const he::vec2& data(var.getFloat2Data());
            stream << "vec2(" << data.x << ", " << data.y << ")"; 
        } break;
    case eShaderGeneratorVariableType_Float3: 
        {
            const he::vec3& data(var.getFloat3Data());
            stream << "vec3(" << data.x << ", " << data.y << ", " << data.z << ")"; 
        } break;
    case eShaderGeneratorVariableType_Float4: 
        {
            const he::vec4& data(var.getFloat4Data());
            stream << "vec4(" << data.x << ", " << data.y << ", " << data.z << ", " << data.w << ")"; 
        } break;
    default: LOG(LogType_ProgrammerAssert, "Unsupported constant type %d", var.getType());
    }
}

void ShaderGenerator::writeOperation( he::StringStream& stream, const ShaderGeneratorVariable& var )
{
    const ShaderGeneratorVariableOperation& operation(var.getOperation());
    switch (operation.type)
    {
    // Const
    case eShaderGeneratorVariableOperationType_Constant : writeConstant(stream, var); break;

    // Operators
    case eShaderGeneratorVariableOperationType_Add : writeGLOperator(stream, operation, "+"); break;
    case eShaderGeneratorVariableOperationType_Div : writeGLOperator(stream, operation, "/"); break;
    case eShaderGeneratorVariableOperationType_Mul : writeGLOperator(stream, operation, "*"); break;
    case eShaderGeneratorVariableOperationType_Sub : writeGLOperator(stream, operation, "-"); break;

    // Swizzle
    case eShaderGeneratorVariableOperationType_Swizzle : writeGLSwizzle(stream, operation); break;

    // Func1
    case eShaderGeneratorVariableOperationType_Sin : writeGLFunc(stream, 1, operation, "sin"); break;
    case eShaderGeneratorVariableOperationType_Cos : writeGLFunc(stream, 1, operation, "cos"); break;
    case eShaderGeneratorVariableOperationType_Tan : writeGLFunc(stream, 1, operation, "tan"); break;
    case eShaderGeneratorVariableOperationType_ASin : writeGLFunc(stream, 1, operation, "asin"); break;
    case eShaderGeneratorVariableOperationType_ACos : writeGLFunc(stream, 1, operation, "acos"); break;
    case eShaderGeneratorVariableOperationType_ATan : writeGLFunc(stream, 1, operation, "atan"); break;
        
    case eShaderGeneratorVariableOperationType_Log : writeGLFunc(stream, 1, operation, "log"); break;
    case eShaderGeneratorVariableOperationType_Log2 : writeGLFunc(stream, 1, operation, "log2"); break;
    case eShaderGeneratorVariableOperationType_Sqrt : writeGLFunc(stream, 1, operation, "sqrt"); break;
    case eShaderGeneratorVariableOperationType_InvSqrt : writeGLFunc(stream, 1, operation, "inversesqrt"); break;
         
    case eShaderGeneratorVariableOperationType_Abs : writeGLFunc(stream, 1, operation, "abs"); break;
    case eShaderGeneratorVariableOperationType_Sign : writeGLFunc(stream, 1, operation, "sign"); break;
    case eShaderGeneratorVariableOperationType_Floor : writeGLFunc(stream, 1, operation, "floor"); break;
    case eShaderGeneratorVariableOperationType_Ceil : writeGLFunc(stream, 1, operation, "ceil"); break;
    case eShaderGeneratorVariableOperationType_Round : writeGLFunc(stream, 1, operation, "round"); break;
    case eShaderGeneratorVariableOperationType_Frac : writeGLFunc(stream, 1, operation, "frac"); break;
    case eShaderGeneratorVariableOperationType_Length : writeGLFunc(stream, 1, operation, "length"); break;
    case eShaderGeneratorVariableOperationType_Normalize : writeGLFunc(stream, 1, operation, "normalize"); break;
                    
    case eShaderGeneratorVariableOperationType_EncodeNormal : writeGLFunc(stream, 1, operation, "encodeNormal"); break;

    // Fuc2
    case eShaderGeneratorVariableOperationType_Pow : writeGLFunc(stream, 2, operation, "pow"); break;
    case eShaderGeneratorVariableOperationType_Mod : writeGLFunc(stream, 2, operation, "mod"); break;
    case eShaderGeneratorVariableOperationType_Min : writeGLFunc(stream, 2, operation, "min"); break;
    case eShaderGeneratorVariableOperationType_Max : writeGLFunc(stream, 2, operation, "max"); break;
    case eShaderGeneratorVariableOperationType_Step : writeGLFunc(stream, 2, operation, "step"); break;
    case eShaderGeneratorVariableOperationType_Dot : writeGLFunc(stream, 2, operation, "dot"); break;
    case eShaderGeneratorVariableOperationType_Cross : writeGLFunc(stream, 2, operation, "cross"); break;
    case eShaderGeneratorVariableOperationType_Reflect : writeGLFunc(stream, 2, operation, "reflect"); break;
    case eShaderGeneratorVariableOperationType_Distance : writeGLFunc(stream, 2, operation, "distance"); break;
    case eShaderGeneratorVariableOperationType_Texture : writeGLFunc(stream, 2, operation, "texture"); break;
    case eShaderGeneratorVariableOperationType_ComposeFloat2 : writeGLFunc(stream, 2, operation, "vec2"); break;

    // Func 3
    case eShaderGeneratorVariableOperationType_Clamp : writeGLFunc(stream, 3, operation, "clamp"); break;
    case eShaderGeneratorVariableOperationType_Lerp : writeGLFunc(stream, 3, operation, "mix"); break;
    case eShaderGeneratorVariableOperationType_SmoothStep : writeGLFunc(stream, 3, operation, "smoothstep"); break;
    case eShaderGeneratorVariableOperationType_Refract : writeGLFunc(stream, 3, operation, "refract"); break;
    case eShaderGeneratorVariableOperationType_TextureLod : writeGLFunc(stream, 3, operation, "textureLod"); break;
    case eShaderGeneratorVariableOperationType_TextureOffset : writeGLFunc(stream, 3, operation, "textureOffset"); break;
    case eShaderGeneratorVariableOperationType_CalcNormal : writeGLFunc(stream, 3, operation, "calcNormal"); break;
    
    // Func variable
    case eShaderGeneratorVariableOperationType_ComposeFloat3 : writeGLFunc(stream, -1, operation, "vec3"); break;
    case eShaderGeneratorVariableOperationType_ComposeFloat4 : writeGLFunc(stream, -1, operation, "vec4"); break;


    default: LOG(LogType_ProgrammerAssert, "Unknown ShaderGeneratorVariableOperationType %d", operation.type);  break;
    }
}

void ShaderGenerator::writeGlobalVariableDeclaration( const ShaderGeneratorVariable& var )
{
    switch (var.getOperation().scope)
    {
    case eShaderGeneratorVariableScope_Local:
        LOG(LogType_ProgrammerAssert, "writeGlobalVariableDeclaration should never receive a local variable!");
        break;
    case eShaderGeneratorVariableScope_Exposed:
    case eShaderGeneratorVariableScope_GlobalSampler:
        {
            m_GlobalSection << "uniform ";
            writeType(m_GlobalSection, var.getType());
            m_GlobalSection << " " << var.getLocalName() << ";";
            writeLineFeed(m_GlobalSection, false);
        } break;
    case eShaderGeneratorVariableScope_GlobalInput:
        {
            m_GlobalSection << "in ";
            writeType(m_GlobalSection, var.getType());
            m_GlobalSection << " " << var.getLocalName() << ";";
            writeLineFeed(m_GlobalSection, false);

        } break;
    case eShaderGeneratorVariableScope_GlobalOutput:
        {
            m_GlobalSection << "out ";
            writeType(m_GlobalSection, var.getType());
            m_GlobalSection << " " << var.getLocalName() << ";";
            writeLineFeed(m_GlobalSection, false);
        } break;
    case eShaderGeneratorVariableScope_GlobalCamera:
    case eShaderGeneratorVariableScope_GlobalFrame:
    case eShaderGeneratorVariableScope_GlobalScene:
        {
            writeInclude(getGlobalIncludeFile(var.getOperation().scope));
        } break;
    default:
        LOG(LogType_ProgrammerAssert, "writeGlobalVariableDeclaration does not know a variable scope %d", var.getOperation().scope);
        break;
    }
}

void ShaderGenerator::recurseDeclareDependencies( const ObjectHandle handle )
{
    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    ShaderGeneratorVariable* var(factory->get(handle));
    const ShaderGeneratorVariableOperation& op(var->getOperation());
    if (op.type == eShaderGeneratorVariableOperationType_Named)
    {
        if (!var->hasDeclaration())
        {
            writeGlobalVariableDeclaration(*var);
            var->setHasDeclaration(true);
        }
    }
    else 
    {
        switch (op.type)
        {
        case eShaderGeneratorVariableOperationType_EncodeNormal: IncludeEncoding(); break;
        case eShaderGeneratorVariableOperationType_CalcNormal: IncludeNormalCalc(); break;
        }
        for (size_t i(0); i < ShaderGeneratorVariableOperation::MAX_PARAMS; ++i)
        {
            if (op.params[i] != he::ObjectHandle::unassigned)
                recurseDeclareDependencies(op.params[i]);
        }   
    }
}

void ShaderGenerator::recurseWriteDeclarations( const ObjectHandle handle )
{
    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    ShaderGeneratorVariable* var(factory->get(handle));
    const ShaderGeneratorVariableOperation& op(var->getOperation());
    if (op.type != eShaderGeneratorVariableOperationType_Named)
    {
        for (size_t i(0); i < ShaderGeneratorVariableOperation::MAX_PARAMS; ++i)
        {
            if (op.params[i] != he::ObjectHandle::unassigned)
                recurseWriteDeclarations(op.params[i]);
        }   

        if (var->needsDeclaration() && !var->hasDeclaration())
        {
            writeType(m_BodySection, var->getType());
            m_BodySection << " " << var->getLocalName() << " = ";
            writeOperation(m_BodySection, *var);
            m_BodySection << ";";
            writeLineFeed(m_BodySection);
            var->setHasDeclaration(true);
        } 
    }
}

void ShaderGenerator::recurseWriteOutput( const ObjectHandle handle )
{
    ShaderGeneratorVariableFactory* const factory(ShaderGeneratorVariableFactory::getInstance());
    ShaderGeneratorVariable* var(factory->get(handle));

    m_BodySection << var->getLocalName() << " = ";
    writeOperation(m_BodySection, *var);
    m_BodySection << ";";
    writeLineFeed(m_BodySection);
    writeLineFeed(m_BodySection);
}

void ShaderGenerator::writeType( he::StringStream& stream, EShaderGeneratorVariableType type )
{
    switch (type)
    {
    case he::ct::eShaderGeneratorVariableType_Int: stream << "int"; break;
    case he::ct::eShaderGeneratorVariableType_Int2: stream << "int2"; break;
    case he::ct::eShaderGeneratorVariableType_Int3: stream << "int3"; break;
    case he::ct::eShaderGeneratorVariableType_Int4: stream << "int4"; break;
    case he::ct::eShaderGeneratorVariableType_Uint: stream << "uint"; break;
    case he::ct::eShaderGeneratorVariableType_Float: stream << "float"; break;
    case he::ct::eShaderGeneratorVariableType_Float2: stream << "vec2"; break;
    case he::ct::eShaderGeneratorVariableType_Float3: stream << "vec3"; break;
    case he::ct::eShaderGeneratorVariableType_Float4: stream << "vec4"; break;
    case he::ct::eShaderGeneratorVariableType_Mat44: stream << "mat4"; break;
    case he::ct::eShaderGeneratorVariableType_Texture1D: stream << "sampler1D"; break;
    case he::ct::eShaderGeneratorVariableType_Texture2D: stream << "sampler2D"; break;
    case he::ct::eShaderGeneratorVariableType_Texture3D: stream << "sampler3D"; break;
    case he::ct::eShaderGeneratorVariableType_TextureCube: stream << "samplerCube"; break;
    default: LOG(LogType_ProgrammerAssert, "Not implemented or unknown EShaderGeneratorVariableType %d", type);
    }
}

void ShaderGenerator::writeHeader()
{
    m_IncludeSection << 
        "//HappyEngine Copyright (C) 2012 - 2014 Evil Interactive                         \n"
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

void ShaderGenerator::writeLineFeed( he::StringStream& strstr, bool indend /*= true*/ )
{
    strstr << "\n";
    if (indend)
    {
        strstr << g_Tab;
    }
}

void ShaderGenerator::IncludeNormalCalc()
{
    if (!m_CalcNormalIncluded)
    {
        m_CalcNormalIncluded = true;
        writeInclude("shared/calcNormal.frag");
    }
}

void ShaderGenerator::IncludeEncoding()
{
    if (!m_EncodeIncluded)
    {
        m_EncodeIncluded = true;
        writeInclude("packing/encode.frag");
    }
}

void ShaderGenerator::writeInclude( const char* include )
{
    m_IncludeSection << "#include \"" << include << "\"";
    writeLineFeed(m_IncludeSection, false);
}

void ShaderGenerator::resetShader()
{
    m_CalcNormalIncluded = false;
    m_EncodeIncluded = false;

    m_IncludeSection.clear();
    m_IncludeSection.str("");
    m_IncludeSection.precision(4);
    m_GlobalSection.clear();
    m_GlobalSection.str("");
    m_GlobalSection.precision(4);
    m_BodySection.clear();
    m_BodySection.str("");
    m_BodySection.precision(4);
}

void ShaderGenerator::startShader()
{
    writeHeader();
    m_BodySection << "void main()";
    writeLineFeed(m_BodySection, false);
    m_BodySection << "{";
    writeLineFeed(m_BodySection, true);
}

void ShaderGenerator::endShader()
{
    writeLineFeed(m_IncludeSection, false);
    writeLineFeed(m_GlobalSection, false);
    writeLineFeed(m_BodySection, false);
    m_BodySection << "}";
    writeLineFeed(m_BodySection, false);
    writeLineFeed(m_BodySection, false);
    writeLineFeed(m_BodySection, false);
}

} } //end namespace
