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
//Created: 04/11/2012

#ifndef _HE_ShaderGenerator_H_
#define _HE_ShaderGenerator_H_
#pragma once

namespace he {
namespace ct {

enum ShaderGeneratorVariableOperationType
{
    ShaderGeneratorVariableOperationType_Constant,
    ShaderGeneratorVariableOperationType_Global,
    ShaderGeneratorVariableOperationType_Exposed,

    // Operators
    ShaderGeneratorVariableOperationType_Add,
    ShaderGeneratorVariableOperationType_Div,
    ShaderGeneratorVariableOperationType_Mul,
    ShaderGeneratorVariableOperationType_Sub,

    // Func
    ShaderGeneratorVariableOperationType_Sin,
    ShaderGeneratorVariableOperationType_Cos,
    ShaderGeneratorVariableOperationType_Tan,
    ShaderGeneratorVariableOperationType_ASin,
    ShaderGeneratorVariableOperationType_ACos,
    ShaderGeneratorVariableOperationType_ATan,

    ShaderGeneratorVariableOperationType_Pow,
    ShaderGeneratorVariableOperationType_Log,
    ShaderGeneratorVariableOperationType_Log2,
    ShaderGeneratorVariableOperationType_Sqrt,
    ShaderGeneratorVariableOperationType_InvSqrt,

    ShaderGeneratorVariableOperationType_Abs,
    ShaderGeneratorVariableOperationType_Sign,
    ShaderGeneratorVariableOperationType_Floor,
    ShaderGeneratorVariableOperationType_Ceil,
    ShaderGeneratorVariableOperationType_Round,
    ShaderGeneratorVariableOperationType_Frac,
    ShaderGeneratorVariableOperationType_Mod,
    ShaderGeneratorVariableOperationType_Min,
    ShaderGeneratorVariableOperationType_Max,
    ShaderGeneratorVariableOperationType_Clamp,
    ShaderGeneratorVariableOperationType_Lerp,
    ShaderGeneratorVariableOperationType_Step,

    ShaderGeneratorVariableOperationType_Length,
    ShaderGeneratorVariableOperationType_Distance,
    ShaderGeneratorVariableOperationType_Dot,
    ShaderGeneratorVariableOperationType_Cross,
    ShaderGeneratorVariableOperationType_Normalize,
    ShaderGeneratorVariableOperationType_Reflect,
    ShaderGeneratorVariableOperationType_Refract,

    // Texture
    ShaderGeneratorVariableOperationType_Texture,
    ShaderGeneratorVariableOperationType_TextureLod,
    ShaderGeneratorVariableOperationType_TextureOffset,

    ShaderGeneratorVariableOperationType_Invalid,
    ShaderGeneratorVariableOperationType_MAX = ShaderGeneratorVariableOperationType_Invalid
};
bool isConstantOperation(const ShaderGeneratorVariableOperationType type);

enum ShaderGeneratorVariableType
{
    ShaderVariableType_Unknown,
    ShaderVariableType_Int,
    ShaderVariableType_Int2,
    ShaderVariableType_Int3,
    ShaderVariableType_Int4,
    ShaderVariableType_Uint,
    ShaderVariableType_Float,
    ShaderVariableType_Float2,
    ShaderVariableType_Float3,
    ShaderVariableType_Float4,
    ShaderVariableType_Mat44,
    ShaderVariableType_Texture2D,
    ShaderVariableType_TextureCube
};

enum ShaderGeneratorGlobalVariableType
{
    // Pass
    ShaderGeneratorGlobalVariableType_TexCoord,

    ShaderGeneratorGlobalVariableType_CameraViewDirection,  // always (0, 0, 1)
    ShaderGeneratorGlobalVariableType_CameraViewPosition,   // always (0, 0, 0)
    ShaderGeneratorGlobalVariableType_CameraNearFar,

    ShaderGeneratorGlobalVariableType_DepthMap,
    ShaderGeneratorGlobalVariableType_ColorMap,

    ShaderGeneratorGlobalVariableType_ViewPosition,
    ShaderGeneratorGlobalVariableType_ViewNormal,

    ShaderGeneratorGlobalVariableType_Time,

    ShaderGeneratorGlobalVariableType_ScreenPosition,
    ShaderGeneratorGlobalVariableType_ScreenSize,
};

enum ShaderGeneratorFlag
{
    ShaderGeneratorFlag_Single,
    ShaderGeneratorFlag_Skinned,
    ShaderGeneratorFlag_Instanced,
    ShaderGeneratorFlag_PreDeffered,
    ShaderGeneratorFlag_PostDeffered,
    ShaderGeneratorFlag_Forward,
};

struct ShaderGeneratorVariableOperation
{
    ShaderGeneratorVariableOperationType type;
    ObjectHandle params[4];
};

class ShaderGeneratorVariable;

class ShaderGenerator
{
    enum State
    {
        State_Idle,
        State_VertexShader,
        State_FragmentShader
    };
public:
    ShaderGenerator();
    virtual ~ShaderGenerator();

    //void init(const ShaderGeneratorType type);
    bool compile();

    ObjectHandle addVariable();
    void setFragmentOutput(const ObjectHandle& var);
    void setVertexOutput(const ObjectHandle& var);



private:
    virtual void writeTypeName(const ShaderGeneratorVariableType type);
    virtual void writeOperation(const ShaderGeneratorVariableOperation& operation);
    virtual void writeHeader();
    virtual void writeOpenMain();
    virtual void writeCloseMain();
    virtual void writeVertexOutput();
    virtual void writeVertexInputVars();
    virtual void writeVertexGlobalVars();
    virtual void writeVertexPassVars();
    virtual void writeFragmentGlobalVars();
    virtual void writeFragmentOutput();
    virtual void writeVariableDeclaration(const ShaderGeneratorVariable* const var);
    virtual void writeVariable(const ShaderGeneratorVariable* const var, const bool forceInline = false);
    virtual void writeConstant(const ShaderGeneratorVariable* const var);
    virtual void writeFloat(const float value);
    virtual void writeFloat2(const vec2& value);
    virtual void writeFloat3(const vec3& value);
    virtual void writeFloat4(const vec4& value);
    virtual void writeAssignment() { m_ShaderFile << " = "; }
    virtual void writeEndLine() { m_ShaderFile << ";"; }
    virtual void writeNewLine() { m_ShaderFile << "\n"; }

    void createLocalVar(ShaderGeneratorVariable* const var);
    uint32 m_LocalVarUID;

    std::stringstream m_ShaderFile;

    std::string m_VertexShader;
    std::string m_FragmentShader;

    he::ObjectList<ObjectHandle> m_PassVariables;

    he::ObjectList<ObjectHandle> m_Variables;
    ObjectHandle m_FragmentOutput;
    ObjectHandle m_VertexOutput;

    State m_State;
};

} } //end namespace

#endif
