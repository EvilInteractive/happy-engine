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
//Created: 23/12/2012

#ifndef _HE_ShaderGeneratorEnums_H_
#define _HE_ShaderGeneratorEnums_H_
#pragma once

namespace he {
namespace ct {

enum EShaderGeneratorType
{
    eShaderGeneratorType_Unknown = -1,

    eShaderGeneratorType_Vertex,
    eShaderGeneratorType_Fragment
};

enum EShaderGeneratorDependency
{
    eShaderGeneratorDependeny_PerSceneBuffer = BIT(0),
    eShaderGeneratorDependeny_PerCameraBuffer = BIT(1),
    eShaderGeneratorDependeny_Tonemap = BIT(2),
    eShaderGeneratorDependeny_NormalEncodeFunctions = BIT(3)
};

enum EShaderGeneratorFsVsDependency
{
    eShaderGeneratorDependeny_Texcoord = BIT(0),
    eShaderGeneratorDependeny_ViewDepth = BIT(1),
    eShaderGeneratorDependeny_ViewPosition = BIT(2),
    eShaderGeneratorDependeny_Color = BIT(3)
};

enum EShaderGeneratorVariableScope
{
    eShaderGeneratorVariableScope_Local,
    eShaderGeneratorVariableScope_Exposed,
    eShaderGeneratorVariableScope_GlobalInput,
    eShaderGeneratorVariableScope_GlobalOutput,
    eShaderGeneratorVariableScope_GlobalCamera,
    eShaderGeneratorVariableScope_GlobalScene,
    eShaderGeneratorVariableScope_GlobalFrame,
    eShaderGeneratorVariableScope_GlobalSampler
};

enum EShaderGeneratorVariableOperationType
{
    eShaderGeneratorVariableOperationType_Invalid = -1,

    // Const
    eShaderGeneratorVariableOperationType_Constant,
    eShaderGeneratorVariableOperationType_Named,
    
    // Operators
    eShaderGeneratorVariableOperationType_Add,
    eShaderGeneratorVariableOperationType_Div,
    eShaderGeneratorVariableOperationType_Mul,
    eShaderGeneratorVariableOperationType_Sub,
    
    // Vector
    eShaderGeneratorVariableOperationType_ComposeFloat2,
    eShaderGeneratorVariableOperationType_ComposeFloat3,
    eShaderGeneratorVariableOperationType_ComposeFloat4,
    eShaderGeneratorVariableOperationType_Swizzle,
    
    // Func
    eShaderGeneratorVariableOperationType_Sin,
    eShaderGeneratorVariableOperationType_Cos,
    eShaderGeneratorVariableOperationType_Tan,
    eShaderGeneratorVariableOperationType_ASin,
    eShaderGeneratorVariableOperationType_ACos,
    eShaderGeneratorVariableOperationType_ATan,
    
    eShaderGeneratorVariableOperationType_Pow,
    eShaderGeneratorVariableOperationType_Log,
    eShaderGeneratorVariableOperationType_Log2,
    eShaderGeneratorVariableOperationType_Sqrt,
    eShaderGeneratorVariableOperationType_InvSqrt,
    
    eShaderGeneratorVariableOperationType_Abs,
    eShaderGeneratorVariableOperationType_Sign,
    eShaderGeneratorVariableOperationType_Floor,
    eShaderGeneratorVariableOperationType_Ceil,
    eShaderGeneratorVariableOperationType_Round,
    eShaderGeneratorVariableOperationType_Frac,
    eShaderGeneratorVariableOperationType_Mod,
    eShaderGeneratorVariableOperationType_Min,
    eShaderGeneratorVariableOperationType_Max,
    eShaderGeneratorVariableOperationType_Clamp,
    eShaderGeneratorVariableOperationType_Lerp,
    eShaderGeneratorVariableOperationType_Step,
    eShaderGeneratorVariableOperationType_SmoothStep,
    
    eShaderGeneratorVariableOperationType_Length,
    eShaderGeneratorVariableOperationType_Distance,
    eShaderGeneratorVariableOperationType_Dot,
    eShaderGeneratorVariableOperationType_Cross,
    eShaderGeneratorVariableOperationType_Normalize,
    eShaderGeneratorVariableOperationType_Reflect,
    eShaderGeneratorVariableOperationType_Refract,
    
    // Custom func
    eShaderGeneratorVariableOperationType_CalcNormal,
    eShaderGeneratorVariableOperationType_EncodeNormal,
    
    // Texture
    eShaderGeneratorVariableOperationType_Texture,
    eShaderGeneratorVariableOperationType_TextureLod,
    eShaderGeneratorVariableOperationType_TextureOffset,
    
    eShaderGeneratorVariableOperationType_MAX
};

enum EShaderGeneratorVariableType
{
    eShaderGeneratorVariableType_Unknown = -1,

    eShaderGeneratorVariableType_Int,
    eShaderGeneratorVariableType_Int2,
    eShaderGeneratorVariableType_Int3,
    eShaderGeneratorVariableType_Int4,
    eShaderGeneratorVariableType_Uint,
    eShaderGeneratorVariableType_Float,
    eShaderGeneratorVariableType_Float2,
    eShaderGeneratorVariableType_Float3,
    eShaderGeneratorVariableType_Float4,
    eShaderGeneratorVariableType_Mat44,
    eShaderGeneratorVariableType_Texture1D,
    eShaderGeneratorVariableType_Texture2D,
    eShaderGeneratorVariableType_Texture3D,
    eShaderGeneratorVariableType_TextureCube
};

enum EShaderGeneratorSwizzleMask
{
    eShaderGeneratorSwizzleMask_X,
    eShaderGeneratorSwizzleMask_Y,
    eShaderGeneratorSwizzleMask_Z,
    eShaderGeneratorSwizzleMask_W,
    eShaderGeneratorSwizzleMask_None
};

enum EShaderGeneratorGlobalInputVariableType
{
    eShaderGeneratorGlobalInputVariableType_Position,
    eShaderGeneratorGlobalInputVariableType_TexCoord,
    eShaderGeneratorGlobalInputVariableType_Normal,
    eShaderGeneratorGlobalInputVariableType_Tangent,
    eShaderGeneratorGlobalInputVariableType_World,
    eShaderGeneratorGlobalInputVariableType_BoneId,
    eShaderGeneratorGlobalInputVariableType_BoneWeight,
    eShaderGeneratorGlobalInputVariableType_Depth,

    eShaderGeneratorGlobalInputVariableType_MAX
};

enum EShaderGeneratorGlobalFrameVariableType
{
    eShaderGeneratorGlobalFrameVariableType_Time,

    eShaderGeneratorGlobalFrameVariableType_MAX
};

enum EShaderGeneratorGlobalSceneVariableType
{
    eShaderGeneratorGlobalSceneVariableType_AmbientLightColor,
    eShaderGeneratorGlobalSceneVariableType_DirectionalLightColor,
    eShaderGeneratorGlobalSceneVariableType_DirectionalLightDirection,

    eShaderGeneratorGlobalSceneVariableType_MAX
};

enum EShaderGeneratorGlobalSamplerVariableType
{
    eShaderGeneratorGlobalSamplerVariableType_DepthMap,
    eShaderGeneratorGlobalSamplerVariableType_ColorMap,
    eShaderGeneratorGlobalSamplerVariableType_Skybox,

    eShaderGeneratorGlobalSamplerVariableType_MAX
};

enum EShaderGeneratorGlobalCameraVariableType
{
    eShaderGeneratorGlobalCameraVariableType_CameraNearFar,
    
    eShaderGeneratorGlobalCameraVariableType_CameraPostion,
    eShaderGeneratorGlobalCameraVariableType_ViewportSize,
    
    eShaderGeneratorGlobalCameraVariableType_ViewProjection,
    eShaderGeneratorGlobalCameraVariableType_View,
    eShaderGeneratorGlobalCameraVariableType_Projection,
    
    eShaderGeneratorGlobalCameraVariableType_MAX
};

enum EShaderGeneratorGlobalOutVariableType
{
    eShaderGeneratorGlobalOutVariableType_GLPosition,
    eShaderGeneratorGlobalOutVariableType_OutColor,
    eShaderGeneratorGlobalOutVariableType_OutNormalDepth,
    eShaderGeneratorGlobalOutVariableType_OutSG,
    eShaderGeneratorGlobalOutVariableType_MAX
};

HAPPY_ENTRY const char* shaderGeneratorSwizzleMaskToString(const EShaderGeneratorSwizzleMask mask);
HAPPY_ENTRY EShaderGeneratorSwizzleMask shaderGeneratorSwizzleMaskFromString(const char* str);

const char* getGlobalInputVariableName(const EShaderGeneratorGlobalInputVariableType type);
const char* getGlobalFrameVariableName(const EShaderGeneratorGlobalFrameVariableType type);
const char* getGlobalSceneVariableName(const EShaderGeneratorGlobalSceneVariableType type);
const char* getGlobalCameraVariableName(const EShaderGeneratorGlobalCameraVariableType type);
const char* getGlobalSamplerVariableName(const EShaderGeneratorGlobalSamplerVariableType type);

const char* getOutVariableName(const EShaderGeneratorGlobalOutVariableType type);

EShaderGeneratorVariableType getGlobalInputVariableType(const EShaderGeneratorGlobalInputVariableType type);
EShaderGeneratorVariableType getGlobalFrameVariableType(const EShaderGeneratorGlobalFrameVariableType type);
EShaderGeneratorVariableType getGlobalSceneVariableType(const EShaderGeneratorGlobalSceneVariableType type);
EShaderGeneratorVariableType getGlobalCameraVariableType(const EShaderGeneratorGlobalCameraVariableType type);
EShaderGeneratorVariableType getGlobalSamplerVariableType(const EShaderGeneratorGlobalSamplerVariableType type);

EShaderGeneratorVariableType getOutVariableType(const EShaderGeneratorGlobalOutVariableType type);

const char* getGlobalIncludeFile(const EShaderGeneratorVariableScope scope);

} } //end namespace

#endif
