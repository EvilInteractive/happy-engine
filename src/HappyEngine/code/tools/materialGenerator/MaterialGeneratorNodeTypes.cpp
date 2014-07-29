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
//Created: 14/11/2012
#include "HappyPCH.h" 

#include "MaterialGeneratorNodeTypes.h"

namespace he {
namespace tools {
    
const char* materialGeneratorVariableTypeToString( const MaterialGeneratorVariableType type)
{
    const char* result("Unknown Type");
    switch (type)
    {
    case MaterialGeneratorVariableType_Float:
        {
            result = "Float";
        } break;
    case MaterialGeneratorVariableType_Float2:
        {
            result = "Float2";
        } break;
    case MaterialGeneratorVariableType_Float3:
        {
            result = "Float3";
        } break;
    case MaterialGeneratorVariableType_Float4:
        {
            result = "Float4";
        } break;
    case MaterialGeneratorVariableType_Texture2D:
        {
            result = "Texture2D";
        } break;
    case MaterialGeneratorVariableType_TextureCube:
        {
            result = "TextureCube";
        } break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown MaterialGeneratorVariableType: %d", type);
        break;
    }
    return result;
}

const char* materialGeneratorNodeTypeToString( const MaterialGeneratorNodeType type )
{
    const char* result("Unknown Type");
    switch (type)
    {
        case MaterialGeneratorNodeType_RootNormalDraw: result = "Root Normal Draw"; break;

        // Const
        case MaterialGeneratorNodeType_Float1: result = "Float1"; break;
        case MaterialGeneratorNodeType_Float2: result = "Float2"; break;
        case MaterialGeneratorNodeType_Float3: result = "Float3"; break;
        case MaterialGeneratorNodeType_Float4: result = "Float4"; break;

        // Globals
        case MaterialGeneratorNodeType_WorldViewPosition: result = "World View Position"; break;
        case MaterialGeneratorNodeType_WorldViewNormal: result = "World View Normal"; break;

        // Vector
        case MaterialGeneratorNodeType_Swizzle: result = "Swizzle"; break;
        case MaterialGeneratorNodeType_ComposeVector: result = "ComposeVector"; break;

        // Math
        // Basic
        case MaterialGeneratorNodeType_Abs: result = "Abs"; break;
        case MaterialGeneratorNodeType_Add: result = "Add"; break;
        case MaterialGeneratorNodeType_Cos: result = "Cos"; break;
        case MaterialGeneratorNodeType_Divide: result = "Divide"; break;
        case MaterialGeneratorNodeType_Multiply: result = "Multiply"; break;
        case MaterialGeneratorNodeType_Sin: result = "Sin"; break;
        case MaterialGeneratorNodeType_Subtract: result = "Subtract"; break;
        // Func
        case MaterialGeneratorNodeType_Ceil: result = "Ceil"; break;
        case MaterialGeneratorNodeType_Clamp: result = "Clamp"; break;
        case MaterialGeneratorNodeType_Cross: result = "Cross"; break;
        case MaterialGeneratorNodeType_Distance: result = "Distance"; break;
        case MaterialGeneratorNodeType_DistanceSqr: result = "Distance Squared"; break;
        case MaterialGeneratorNodeType_Dot: result = "Dot"; break;
        case MaterialGeneratorNodeType_Floor: result = "Floor"; break;
        case MaterialGeneratorNodeType_Frac: result = "Frac"; break;
        case MaterialGeneratorNodeType_Lerp: result = "Lerp"; break;
        case MaterialGeneratorNodeType_Max: result = "Max"; break;
        case MaterialGeneratorNodeType_Min: result = "Min"; break;
        case MaterialGeneratorNodeType_OneMin: result = "One Minus"; break;
        case MaterialGeneratorNodeType_Normalize: result = "Normalize"; break;
        case MaterialGeneratorNodeType_Power: result = "Power"; break;
        case MaterialGeneratorNodeType_Reflect: result = "Reflect"; break;
        case MaterialGeneratorNodeType_Sign: result = "Sign"; break;

        // Texture
        case MaterialGeneratorNodeType_FlipBook: result = "Flip Book"; break;
        case MaterialGeneratorNodeType_Panner: result = "Panner"; break;
        case MaterialGeneratorNodeType_Rotator: result = "Rotator"; break;
        case MaterialGeneratorNodeType_Texture2D: result = "Texture2D"; break;
        case MaterialGeneratorNodeType_TextureCube: result = "TextureCube"; break;
        case MaterialGeneratorNodeType_Texcoord: result = "Texture Coordinate"; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown MaterialGeneratorNodeType: %d", type);
        break;
    }

    return result;
}

he::tools::MaterialGeneratorNodeType materialGeneratorNodeTypeFromString( const char* str )
{
    MaterialGeneratorNodeType result(MaterialGeneratorNodeType_Unassigned);

    if ( strcmp(str, "Root Normal Draw") == 0 ) result = MaterialGeneratorNodeType_RootNormalDraw;

    // Const
    else if ( strcmp(str, "Float1") == 0 ) result = MaterialGeneratorNodeType_Float1;
    else if ( strcmp(str, "Float2") == 0 ) result = MaterialGeneratorNodeType_Float2;
    else if ( strcmp(str, "Float3") == 0 ) result = MaterialGeneratorNodeType_Float3;
    else if ( strcmp(str, "Float4") == 0 ) result = MaterialGeneratorNodeType_Float4;

    // Globals
    else if ( strcmp(str, "World View Position") == 0 ) result = MaterialGeneratorNodeType_WorldViewPosition;
    else if ( strcmp(str, "World View Normal") == 0 ) result = MaterialGeneratorNodeType_WorldViewNormal;

    // Vector
    else if ( strcmp(str, "Swizzle") == 0 ) result = MaterialGeneratorNodeType_Swizzle;
    else if ( strcmp(str, "ComposeVector") == 0 ) result = MaterialGeneratorNodeType_ComposeVector;

    // Math
    // Basic
    else if ( strcmp(str, "Abs") == 0 ) result = MaterialGeneratorNodeType_Abs;
    else if ( strcmp(str, "Add") == 0 ) result = MaterialGeneratorNodeType_Add;
    else if ( strcmp(str, "Cos") == 0 ) result = MaterialGeneratorNodeType_Cos;
    else if ( strcmp(str, "Divide") == 0 ) result = MaterialGeneratorNodeType_Divide;
    else if ( strcmp(str, "Multiply") == 0 ) result = MaterialGeneratorNodeType_Multiply;
    else if ( strcmp(str, "Sin") == 0 ) result = MaterialGeneratorNodeType_Sin;
    else if ( strcmp(str, "Subtract") == 0 ) result = MaterialGeneratorNodeType_Subtract;
    // Func
    else if ( strcmp(str, "Ceil") == 0 ) result = MaterialGeneratorNodeType_Ceil;
    else if ( strcmp(str, "Clamp") == 0 ) result = MaterialGeneratorNodeType_Clamp;
    else if ( strcmp(str, "Cross") == 0 ) result = MaterialGeneratorNodeType_Cross;
    else if ( strcmp(str, "Distance") == 0 ) result = MaterialGeneratorNodeType_Distance;
    else if ( strcmp(str, "Distance Squared") == 0 ) result = MaterialGeneratorNodeType_DistanceSqr;
    else if ( strcmp(str, "Dot") == 0 ) result = MaterialGeneratorNodeType_Dot;
    else if ( strcmp(str, "Floor") == 0 ) result = MaterialGeneratorNodeType_Floor;
    else if ( strcmp(str, "Frac") == 0 ) result = MaterialGeneratorNodeType_Frac;
    else if ( strcmp(str, "Lerp") == 0 ) result = MaterialGeneratorNodeType_Lerp;
    else if ( strcmp(str, "Max") == 0 ) result = MaterialGeneratorNodeType_Max;
    else if ( strcmp(str, "Min") == 0 ) result = MaterialGeneratorNodeType_Min;
    else if ( strcmp(str, "One Minus") == 0 ) result = MaterialGeneratorNodeType_OneMin;
    else if ( strcmp(str, "Normalize") == 0 ) result = MaterialGeneratorNodeType_Normalize;
    else if ( strcmp(str, "Power") == 0 ) result = MaterialGeneratorNodeType_Power;
    else if ( strcmp(str, "Reflect") == 0 ) result = MaterialGeneratorNodeType_Reflect;
    else if ( strcmp(str, "Sign") == 0 ) result = MaterialGeneratorNodeType_Sign;

    // Texture
    else if ( strcmp(str, "Flip Book") == 0 ) result = MaterialGeneratorNodeType_FlipBook;
    else if ( strcmp(str, "Panner") == 0 ) result = MaterialGeneratorNodeType_Panner;
    else if ( strcmp(str, "Rotator") == 0 ) result = MaterialGeneratorNodeType_Rotator;
    else if ( strcmp(str, "Texture2D") == 0 ) result = MaterialGeneratorNodeType_Texture2D;
    else if ( strcmp(str, "TextureCube") == 0 ) result = MaterialGeneratorNodeType_TextureCube;
    else if ( strcmp(str, "Texture Coordinate") == 0 ) result = MaterialGeneratorNodeType_Texcoord;

    else { LOG(LogType_ProgrammerAssert, "Unknown MaterialGeneratorNodeType string: %s", str); }

    return result;
}

he::tools::MaterialGeneratorNodeTypeSubdivion getMaterialGeneratorNodeTypeSubdivision( const MaterialGeneratorNodeType type )
{
    MaterialGeneratorNodeTypeSubdivion result(MaterialGeneratorNodeTypeSubdivion_None);
    switch (type)
    {
    case MaterialGeneratorNodeType_RootNormalDraw: result = MaterialGeneratorNodeTypeSubdivion_None; break;

    // Const
    case MaterialGeneratorNodeType_Float1: result = MaterialGeneratorNodeTypeSubdivion_ConstNodes; break;
    case MaterialGeneratorNodeType_Float2: result = MaterialGeneratorNodeTypeSubdivion_ConstNodes; break;
    case MaterialGeneratorNodeType_Float3: result = MaterialGeneratorNodeTypeSubdivion_ConstNodes; break;
    case MaterialGeneratorNodeType_Float4: result = MaterialGeneratorNodeTypeSubdivion_ConstNodes; break;

    // Globals
    case MaterialGeneratorNodeType_WorldViewPosition: result = MaterialGeneratorNodeTypeSubdivion_None; break;
    case MaterialGeneratorNodeType_WorldViewNormal: result = MaterialGeneratorNodeTypeSubdivion_None; break;

    // Vector
    case MaterialGeneratorNodeType_Swizzle: result = MaterialGeneratorNodeTypeSubdivion_VectorNodes; break;
    case MaterialGeneratorNodeType_ComposeVector: result = MaterialGeneratorNodeTypeSubdivion_VectorNodes; break;

    // Math
    // Basic
    case MaterialGeneratorNodeType_Abs: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Add: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Cos: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Divide: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Multiply: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Sin: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Subtract: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    // Func
    case MaterialGeneratorNodeType_Ceil: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Clamp: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Cross: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Distance: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_DistanceSqr: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Dot: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Floor: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Frac: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Lerp: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Max: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Min: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_OneMin: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Normalize: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Power: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Reflect: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;
    case MaterialGeneratorNodeType_Sign: result = MaterialGeneratorNodeTypeSubdivion_MathNodes; break;

            // Texture
    case MaterialGeneratorNodeType_FlipBook: result = MaterialGeneratorNodeTypeSubdivion_TextureNodes; break;
    case MaterialGeneratorNodeType_Panner: result = MaterialGeneratorNodeTypeSubdivion_TextureNodes; break;
    case MaterialGeneratorNodeType_Rotator: result = MaterialGeneratorNodeTypeSubdivion_TextureNodes; break;
    case MaterialGeneratorNodeType_Texture2D: result = MaterialGeneratorNodeTypeSubdivion_TextureNodes; break;
    case MaterialGeneratorNodeType_TextureCube: result = MaterialGeneratorNodeTypeSubdivion_TextureNodes; break;
    case MaterialGeneratorNodeType_Texcoord: result = MaterialGeneratorNodeTypeSubdivion_TextureNodes; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown MaterialGeneratorNodeType: %s", materialGeneratorNodeTypeToString(type));
        break;
    }

    return result;
}

const char* materialGeneratorNodeTypeSubdivionToString( const MaterialGeneratorNodeTypeSubdivion type )
{
    const char* result("Unknown Type");
    switch (type)
    {
    case MaterialGeneratorNodeTypeSubdivion_ConstNodes: result = "Const Nodes"; break;
    case MaterialGeneratorNodeTypeSubdivion_MathNodes: result = "Math Nodes"; break;
    case MaterialGeneratorNodeTypeSubdivion_VectorNodes: result = "Vector Nodes"; break;
    case MaterialGeneratorNodeTypeSubdivion_TextureNodes: result = "Texture Nodes"; break;
    case MaterialGeneratorNodeTypeSubdivion_None: result = "None"; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown MaterialGeneratorNodeTypeSubdivion: %d", type);
        break;
    }

    return result;
}

} } //end namespace
