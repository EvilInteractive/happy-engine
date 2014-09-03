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

    ShaderGeneratorVariableOperation() : type(ShaderGeneratorVariableOperationType_Invalid)
    {
        for (size_t i(0); i < MAX_PARAMS; ++i)
        {
            params[i] = ObjectHandle::unassigned;
        }
        for (size_t i(0); i < 4; ++i)
        {
            swizzleParams[i] = ShaderGeneratorSwizzleMask_None;
        }
    }
    ShaderGeneratorVariableOperationType type;

    ObjectHandle params[MAX_PARAMS];
    ShaderGeneratorSwizzleMask swizzleParams[4];
};

class ShaderGeneratorVariable;

class ShaderGenerator
{
public:
    ShaderGenerator();
    ~ShaderGenerator();

    bool compile(const Path& shaderPath, const he::String& shaderName);
    void reset();

    ObjectHandle addVariable();
    const ObjectHandle& getVariable(const ShaderGeneratorGlobalInputVariableType type);
    const ObjectHandle& getVariable(const ShaderGeneratorGlobalFragmentVariableType type);
    const ObjectHandle& getVariable(const ShaderGeneratorGlobalCodeVariableType type);
    const ObjectHandle& getVariable(const ShaderGeneratorOutVariableType type);

    void setDiffuse(const ObjectHandle& var) { m_DiffuseVar = var; }
    void setEmissive(const ObjectHandle& var) { m_EmissiveVar = var; }
    void setSpecular(const ObjectHandle& var) { m_SpecularVar = var; }
    void setGloss(const ObjectHandle& var) { m_GlossVar = var; }
    void setOpacity(const ObjectHandle& var) { m_OpacityVar = var; }
    void setNormal(const ObjectHandle& var) { m_NormalVar = var; }
    void setWorldPositionOffset(const ObjectHandle& var) { m_WorldPositionOffsetVar = var; }
    void setAlphaTestValue(const ObjectHandle& var) { m_AlphaTestValue = var; }

private:
    struct GlobalGeneratorSwitches
    {
        GlobalGeneratorSwitches()
          : m_UseNormalMap(false)
          , m_UseTexcoord(false) 
          , m_AlphaTest(false)
        {}

        bool m_UseNormalMap;
        bool m_UseTexcoord;
        bool m_AlphaTest;
    };
    enum ShadingType
    {
        ShadingType_Forward,
        ShadingType_Deferred
    };
    enum DrawType
    {
        DrawType_Single,
        DrawType_Skinned,
        DrawType_Instanced,
        DrawType_Unused
    };
    void clearShader();

    bool generateShader(const ShaderGeneratorType genType, const ShadingType shadingType, const DrawType drawType = DrawType_Unused);
    void saveShader(const Path& shaderPath);

    void inititalizeInternalVertexVars(const ShadingType shadingType, const DrawType drawType);
    void inititalizeInternalFragmentVars(const ShadingType shadingType);
    ShaderGeneratorVariable* addInternalVariable();

    void resetAnalyse();
    void analyseVariables();
    void analyseVariable( ShaderGeneratorVariable* const var);

    void writeTypeName(const ShaderGeneratorVariableType type);
    void writeOperation(const ShaderGeneratorVariableOperation& operation);

    void writeHeader();

    void writeOpenMain();
    void writeCloseMain();

    void writeGlobalVertexVariablesDeclarations();
    void writeGlobalFragmentVariablesDeclarations();
    void writeInVarDeclaration(const ShaderGeneratorVariable* const var);
    void writeOutVarDeclaration(const ShaderGeneratorVariable* const var);
    void writeCodeVarDeclaration(const ShaderGeneratorVariable* const var);

    void writeVariableDeclarations();
    void writeVariableDeclaration(ShaderGeneratorVariable* const var);
    void writeVariable(const ShaderGeneratorVariable* const var, const bool forceInline = false);
    void writeConstant(const ShaderGeneratorVariable* const var);
    
    void writeFloat(const float value);
    void writeFloat2(const vec2& value);
    void writeFloat3(const vec3& value);
    void writeFloat4(const vec4& value);

    void writeAssignment() { m_ShaderFile << " = "; }
    void writeEndLine() { m_ShaderFile << ";"; }
    void writeNewLine();

    void write(const char* const text);
    void write(const he::String& text);
    void writeIndend();

    void writeOpenScope();
    void writeCloseScope();

    void writeNormalFunction();
    void writeDiscardTest(const ShaderGeneratorVariable* const value, const ShaderGeneratorVariable* const test);

    uint32 m_LocalVarUID;

    uint8 m_ScopeDepth;

    he::StringStream m_ShaderFile;
    
    he::ObjectList<ObjectHandle> m_Variables;
    he::ObjectList<ObjectHandle> m_InternalVariables;
    he::ObjectList<ObjectHandle> m_OutVariables;

    void createGlobalVariables();
    void destroyGlobalVariables();

    // not a handle because lifetime == my lifetime
    ShaderGeneratorVariable* m_GlobalInputVariables[ShaderGeneratorGlobalInputVariableType_MAX];
    ShaderGeneratorVariable* m_GlobalFragmentVariables[ShaderGeneratorGlobalFragmentVariableType_MAX];
    ShaderGeneratorVariable* m_GlobalCodeVariables[ShaderGeneratorGlobalCodeVariableType_MAX];
    ShaderGeneratorVariable* m_GlobalOutVariables[ShaderGeneratorOutVariableType_MAX];

    ObjectHandle m_DiffuseVar;
    ObjectHandle m_EmissiveVar;
    ObjectHandle m_SpecularVar;
    ObjectHandle m_GlossVar;
    ObjectHandle m_OpacityVar;
    ObjectHandle m_NormalVar;
    ObjectHandle m_WorldPositionOffsetVar;

    ObjectHandle m_AlphaTestValue;

    GlobalGeneratorSwitches m_GlobalSwitches;
};                 

} } //end namespace

#endif
