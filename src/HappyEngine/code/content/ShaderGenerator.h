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
//Created: 04/11/2012

#ifndef _HE_ShaderGenerator_H_
#define _HE_ShaderGenerator_H_
#pragma once

#include "ShaderGeneratorEnums.h"

namespace he {
namespace ct {

struct ShaderGeneratorVariableOperation
{
    static const int MAX_PARAMS = 4;

    ShaderGeneratorVariableOperation() : type(eShaderGeneratorVariableOperationType_Invalid), scope(eShaderGeneratorVariableScope_Local)
    {
        for (size_t i(0); i < MAX_PARAMS; ++i)
        {
            params[i] = ObjectHandle::unassigned;
        }
        for (size_t i(0); i < 4; ++i)
        {
            swizzleParams[i] = eShaderGeneratorSwizzleMask_None;
        }
    }
    EShaderGeneratorVariableOperationType type;
    
    ObjectHandle params[MAX_PARAMS];
    EShaderGeneratorSwizzleMask swizzleParams[4];
    EShaderGeneratorVariableScope scope;
};

class ShaderGeneratorVariable;

class HAPPY_ENTRY ShaderGenerator
{
public:
    ShaderGenerator();
    ~ShaderGenerator();

    bool compile(const Path& shaderPath, const he::String& shaderName);

    ObjectHandle addVariable(const char* nameHint);
    void removeVariable(const ObjectHandle& var);

    ObjectHandle getVariable(const EShaderGeneratorGlobalInputVariableType type);
    ObjectHandle getVariable(const EShaderGeneratorGlobalCameraVariableType type);
    ObjectHandle getVariable(const EShaderGeneratorGlobalFrameVariableType type);
    ObjectHandle getVariable(const EShaderGeneratorGlobalSamplerVariableType type);
    ObjectHandle getVariable(const EShaderGeneratorGlobalSceneVariableType type);
    ObjectHandle getVariable(const EShaderGeneratorGlobalOutVariableType type);
    
    void clearOutputVariables();
    void addOutputVariable(const ObjectHandle handle);

private:
    void resetShader();
    void startShader();
    void endShader();
    void saveShader(const he::Path& path);

    void createGlobalVariables();
    void destroyGlobalVariables();

    void calculateUseCounts();
    void recureCalculateUseCountVariable(const he::ObjectHandle handle);

    void recurseDeclareDependencies( const ObjectHandle handle );
    void recurseWriteDeclarations( const ObjectHandle handle );
    void recurseWriteOutput( const ObjectHandle handle );

    void writeHeader();
    void writeLineFeed( he::StringStream& body, const bool indent = true );
    void writeGlobalVariableDeclaration( const ShaderGeneratorVariable& var );
    void writeType( he::StringStream& stream, EShaderGeneratorVariableType type );
    void writeOperation( he::StringStream& stream, const ShaderGeneratorVariable& var );
    void writeGLFunc(he::StringStream& stream, const int params, const ShaderGeneratorVariableOperation& op, const char* glName);
    void writeGLSwizzle(he::StringStream& stream, const ShaderGeneratorVariableOperation& op);
    void writeGLOperator(he::StringStream& stream, const ShaderGeneratorVariableOperation& op, const char* glOperator);
    void writeConstant(he::StringStream& stream, const ShaderGeneratorVariable& var);
    void writeInclude(const char* include);
    void IncludeNormalCalc();
    void IncludeEncoding();

    he::ObjectList<ObjectHandle> m_Variables;
    he::ObjectList<ObjectHandle> m_OutputVariables;

    he::StringStream m_IncludeSection;
    he::StringStream m_GlobalSection;
    he::StringStream m_BodySection;

    ObjectHandle m_GlobalInputVariables[eShaderGeneratorGlobalInputVariableType_MAX];
    ObjectHandle m_GlobalCameraVariables[eShaderGeneratorGlobalCameraVariableType_MAX];
    ObjectHandle m_GlobalFrameVariables[eShaderGeneratorGlobalFrameVariableType_MAX];
    ObjectHandle m_GlobalSamplerVariables[eShaderGeneratorGlobalSamplerVariableType_MAX];
    ObjectHandle m_GlobalSceneVariables[eShaderGeneratorGlobalSceneVariableType_MAX];
    ObjectHandle m_GlobalOutVariables[eShaderGeneratorGlobalOutVariableType_MAX];

    EShaderGeneratorType m_ShaderType;
    bool m_CalcNormalIncluded;
    bool m_EncodeIncluded;

};                 

} } //end namespace

#endif
