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
//Created: 13/11/2012

#ifndef _HE_MaterialGeneratorNodeTypes_H_
#define _HE_MaterialGeneratorNodeTypes_H_
#pragma once

namespace he {
namespace tools {
    
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
    MaterialGeneratorNodeType_Float1,
    MaterialGeneratorNodeType_Float2,
    MaterialGeneratorNodeType_Float3,
    MaterialGeneratorNodeType_Float4,

    // Globals
    MaterialGeneratorNodeType_Position,
    MaterialGeneratorNodeType_Normal,

    // Vector
    MaterialGeneratorNodeType_ComponentMask,
    MaterialGeneratorNodeType_ComposeVector,

    // Math
        // Basic
        MaterialGeneratorNodeType_Abs,
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

    // Texture
    MaterialGeneratorNodeType_FlipBook,
    MaterialGeneratorNodeType_Panner,
    MaterialGeneratorNodeType_Rotator,
    MaterialGeneratorNodeType_Texture2D,
    MaterialGeneratorNodeType_TextureCube,
    MaterialGeneratorNodeType_Texcoord,



    MaterialGeneratorNodeType_MAX,
    MaterialGeneratorNodeType_Unassigned = MaterialGeneratorNodeType_MAX
};


} } //end namespace

#endif
