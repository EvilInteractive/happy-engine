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
//Created: 13/11/2012

#ifndef _HE_MaterialGeneratorNodeTypes_H_
#define _HE_MaterialGeneratorNodeTypes_H_
#pragma once

namespace hs {
    
enum MaterialGeneratorVariableType
{
    MaterialGeneratorVariableType_Float,
    MaterialGeneratorVariableType_Float2,
    MaterialGeneratorVariableType_Float3,
    MaterialGeneratorVariableType_Float4,
    MaterialGeneratorVariableType_Texture2D,
    MaterialGeneratorVariableType_TextureCube,
    MaterialGeneratorVariableType_Unknown,
};
const char* materialGeneratorVariableTypeToString(const MaterialGeneratorVariableType type);

enum MaterialGeneratorNodeType
{
    // Root
    MaterialGeneratorNodeType_RootNormalDraw,

    // Const
    MaterialGeneratorNodeType_CONST,
    MaterialGeneratorNodeType_Float1 = MaterialGeneratorNodeType_CONST,
    MaterialGeneratorNodeType_Float2,
    MaterialGeneratorNodeType_Float3,
    MaterialGeneratorNodeType_Float4,
    MaterialGeneratorNodeType_CONST_MAX,

    // Globals
    MaterialGeneratorNodeType_GLOBALS = MaterialGeneratorNodeType_CONST_MAX,
    MaterialGeneratorNodeType_WorldViewPosition = MaterialGeneratorNodeType_GLOBALS,
    MaterialGeneratorNodeType_WorldViewNormal,
    MaterialGeneratorNodeType_GLOBALS_MAX,

    // Vector
    MaterialGeneratorNodeType_VECTOR = MaterialGeneratorNodeType_GLOBALS_MAX,
    MaterialGeneratorNodeType_Swizzle = MaterialGeneratorNodeType_VECTOR,
    MaterialGeneratorNodeType_ComposeVector,
    MaterialGeneratorNodeType_VECTOR_MAX,

    // Math
    MaterialGeneratorNodeType_MATH = MaterialGeneratorNodeType_VECTOR_MAX,
        // Basic
        MaterialGeneratorNodeType_Abs = MaterialGeneratorNodeType_MATH,
        MaterialGeneratorNodeType_Add,
        MaterialGeneratorNodeType_Cos,
        MaterialGeneratorNodeType_Divide,
        MaterialGeneratorNodeType_Multiply,
        MaterialGeneratorNodeType_Sin,
        MaterialGeneratorNodeType_Subtract,
        // Func
        MaterialGeneratorNodeType_Ceil,
        MaterialGeneratorNodeType_Clamp,
        MaterialGeneratorNodeType_Cross,
        MaterialGeneratorNodeType_Distance,
        MaterialGeneratorNodeType_DistanceSqr,
        MaterialGeneratorNodeType_Dot,
        MaterialGeneratorNodeType_Floor,
        MaterialGeneratorNodeType_Frac,
        MaterialGeneratorNodeType_Lerp,
        MaterialGeneratorNodeType_Max,
        MaterialGeneratorNodeType_Min,
        MaterialGeneratorNodeType_OneMin,
        MaterialGeneratorNodeType_Normalize,
        MaterialGeneratorNodeType_Power,
        MaterialGeneratorNodeType_Reflect,
        MaterialGeneratorNodeType_Sign,
    MaterialGeneratorNodeType_MATH_MAX,

    // Texture
    MaterialGeneratorNodeType_TEXTURE = MaterialGeneratorNodeType_MATH_MAX,
    MaterialGeneratorNodeType_FlipBook = MaterialGeneratorNodeType_TEXTURE,
    MaterialGeneratorNodeType_Panner,
    MaterialGeneratorNodeType_Rotator,
    MaterialGeneratorNodeType_Texture2D,
    MaterialGeneratorNodeType_TextureCube,
    MaterialGeneratorNodeType_Texcoord,



    MaterialGeneratorNodeType_MAX,
    MaterialGeneratorNodeType_Unassigned = MaterialGeneratorNodeType_MAX
};

enum MaterialGeneratorNodeTypeSubdivion
{
    MaterialGeneratorNodeTypeSubdivion_ConstNodes,
    MaterialGeneratorNodeTypeSubdivion_MathNodes,
    MaterialGeneratorNodeTypeSubdivion_VectorNodes,
    MaterialGeneratorNodeTypeSubdivion_TextureNodes,

    MaterialGeneratorNodeTypeSubdivion_None,
    MaterialGeneratorNodeTypeSubdivion_MAX = MaterialGeneratorNodeTypeSubdivion_None
};

const char* materialGeneratorNodeTypeToString(const MaterialGeneratorNodeType type);
MaterialGeneratorNodeType materialGeneratorNodeTypeFromString(const char* str);

} //end namespace

#endif
