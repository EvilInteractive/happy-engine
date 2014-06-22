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

enum ShaderGeneratorType
{
    ShaderGeneratorType_Unknown,

    ShaderGeneratorType_Vertex,
    ShaderGeneratorType_Fragment
};

enum ShaderGeneratorVariableOperationType
{
    ShaderGeneratorVariableOperationType_Constant,
    ShaderGeneratorVariableOperationType_Global,
    ShaderGeneratorVariableOperationType_Exposed,
    ShaderGeneratorVariableOperationType_Output,

    // Operators
    ShaderGeneratorVariableOperationType_Add,
    ShaderGeneratorVariableOperationType_Div,
    ShaderGeneratorVariableOperationType_Mul,
    ShaderGeneratorVariableOperationType_Sub,

    // Vector
    ShaderGeneratorVariableOperationType_ComposeFloat2,
    ShaderGeneratorVariableOperationType_ComposeFloat3,
    ShaderGeneratorVariableOperationType_ComposeFloat4,
    ShaderGeneratorVariableOperationType_Swizzle,

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
    ShaderGeneratorVariableOperationType_SmoothStep,

    ShaderGeneratorVariableOperationType_Length,
    ShaderGeneratorVariableOperationType_Distance,
    ShaderGeneratorVariableOperationType_Dot,
    ShaderGeneratorVariableOperationType_Cross,
    ShaderGeneratorVariableOperationType_Normalize,
    ShaderGeneratorVariableOperationType_Reflect,
    ShaderGeneratorVariableOperationType_Refract,

    // Custom func
    ShaderGeneratorVariableOperationType_CalcNormal,
    ShaderGeneratorVariableOperationType_EncodeNormal,

    // Texture
    ShaderGeneratorVariableOperationType_Texture,
    ShaderGeneratorVariableOperationType_TextureLod,
    ShaderGeneratorVariableOperationType_TextureOffset,

    ShaderGeneratorVariableOperationType_Invalid,
    ShaderGeneratorVariableOperationType_MAX = ShaderGeneratorVariableOperationType_Invalid
};

enum ShaderGeneratorVariableType
{
    ShaderGeneratorVariableType_Unknown,
    ShaderGeneratorVariableType_Int,
    ShaderGeneratorVariableType_Int2,
    ShaderGeneratorVariableType_Int3,
    ShaderGeneratorVariableType_Int4,
    ShaderGeneratorVariableType_Uint,
    ShaderGeneratorVariableType_Float,
    ShaderGeneratorVariableType_Float2,
    ShaderGeneratorVariableType_Float3,
    ShaderGeneratorVariableType_Float4,
    ShaderGeneratorVariableType_Mat44,
    ShaderGeneratorVariableType_Texture2D,
    ShaderGeneratorVariableType_TextureCube
};

enum ShaderGeneratorSwizzleMask
{
    ShaderGeneratorSwizzleMask_X,
    ShaderGeneratorSwizzleMask_Y,
    ShaderGeneratorSwizzleMask_Z,
    ShaderGeneratorSwizzleMask_W,
    ShaderGeneratorSwizzleMask_None
};

enum ShaderGeneratorGlobalInputVariableType
{
    ShaderGeneratorGlobalInputVariableType_Position,
    ShaderGeneratorGlobalInputVariableType_TexCoord,
    ShaderGeneratorGlobalInputVariableType_Normal,
    ShaderGeneratorGlobalInputVariableType_Tangent,
    ShaderGeneratorGlobalInputVariableType_World,
    ShaderGeneratorGlobalInputVariableType_BoneId,
    ShaderGeneratorGlobalInputVariableType_BoneWeight,

    ShaderGeneratorGlobalInputVariableType_MAX
};

enum ShaderGeneratorGlobalFragmentVariableType
{
    ShaderGeneratorGlobalFragmentVariableType_ViewPosition,
    ShaderGeneratorGlobalFragmentVariableType_ViewNormal,
    ShaderGeneratorGlobalFragmentVariableType_ViewTangent,
    ShaderGeneratorGlobalFragmentVariableType_TexCoord,
    ShaderGeneratorGlobalFragmentVariableType_Depth,

    ShaderGeneratorGlobalFragmentVariableType_MAX
};

enum ShaderGeneratorGlobalCodeVariableType
{
    ShaderGeneratorGlobalCodeVariableType_CameraViewDirection,  // always (0, 0, 1)
    ShaderGeneratorGlobalCodeVariableType_CameraViewPosition,   // always (0, 0, 0)
    ShaderGeneratorGlobalCodeVariableType_CameraNearFar,

    ShaderGeneratorGlobalCodeVariableType_Time,

    ShaderGeneratorGlobalCodeVariableType_ScreenPosition,
    ShaderGeneratorGlobalCodeVariableType_ScreenSize,

    ShaderGeneratorGlobalCodeVariableType_World,
    ShaderGeneratorGlobalCodeVariableType_WorldView,
    ShaderGeneratorGlobalCodeVariableType_WorldViewProjection,
    ShaderGeneratorGlobalCodeVariableType_ViewProjection,
    ShaderGeneratorGlobalCodeVariableType_View,
    ShaderGeneratorGlobalCodeVariableType_Projection,

    ShaderGeneratorGlobalSamplerVariableType_DepthMap,
    ShaderGeneratorGlobalSamplerVariableType_ColorMap,
    ShaderGeneratorGlobalSamplerVariableType_Skybox,

    ShaderGeneratorGlobalCodeVariableType_MAX
};

enum ShaderGeneratorOutVariableType
{
    ShaderGeneratorOutVariableType_GLPosition,
    ShaderGeneratorOutVariableType_OutColor,
    ShaderGeneratorOutVariableType_OutNormalDepth,
    ShaderGeneratorOutVariableType_OutSG,
    ShaderGeneratorOutVariableType_MAX
};

const char* shaderGeneratorSwizzleMaskToString(const ShaderGeneratorSwizzleMask mask);

const char* getGlobalInputVariableName(const ShaderGeneratorGlobalInputVariableType type);
const char* getGlobalFragmentVariableName(const ShaderGeneratorGlobalFragmentVariableType type);
const char* getGlobalCodeVariableName(const ShaderGeneratorGlobalCodeVariableType type);

const char* getOutVariableName(const ShaderGeneratorOutVariableType type);

ShaderGeneratorVariableType getGlobalInputVariableType(const ShaderGeneratorGlobalInputVariableType type);
ShaderGeneratorVariableType getGlobalFragmentVariableType(const ShaderGeneratorGlobalFragmentVariableType type);
ShaderGeneratorVariableType getGlobalCodeVariableType(const ShaderGeneratorGlobalCodeVariableType type);

ShaderGeneratorVariableType getOutVariableType(const ShaderGeneratorOutVariableType type);

} } //end namespace

#endif
