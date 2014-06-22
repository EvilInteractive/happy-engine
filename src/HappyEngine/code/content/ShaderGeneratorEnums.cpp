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
#include "HappyPCH.h" 

#include "ShaderGeneratorEnums.h"

namespace he {
namespace ct {
    
const char* getGlobalInputVariableName( const ShaderGeneratorGlobalInputVariableType type )
{
    const char* result("ERROR_UNKNOW_INPUT_VAR");
    switch (type)
    {
    case ShaderGeneratorGlobalInputVariableType_Position:   result = "inPosition";   break;
    case ShaderGeneratorGlobalInputVariableType_TexCoord:   result = "inTexCoord";   break;
    case ShaderGeneratorGlobalInputVariableType_Normal:     result = "inNormal";     break;
    case ShaderGeneratorGlobalInputVariableType_Tangent:    result = "inTangent";    break;
    case ShaderGeneratorGlobalInputVariableType_World:      result = "inWorld";      break;
    case ShaderGeneratorGlobalInputVariableType_BoneId:     result = "inBoneId";     break;
    case ShaderGeneratorGlobalInputVariableType_BoneWeight: result = "inBoneWeight"; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown ShaderGeneratorGlobalInputVariableType: %d", type);
    }
    return result;
}

const char* getGlobalFragmentVariableName( const ShaderGeneratorGlobalFragmentVariableType type )
{
    const char* result("ERROR_UNKNOW_FRAGMENT_VAR");
    switch (type)
    {
    case ShaderGeneratorGlobalFragmentVariableType_ViewPosition: result = "passPosition"; break;
    case ShaderGeneratorGlobalFragmentVariableType_ViewNormal: result = "passNormal"; break;
    case ShaderGeneratorGlobalFragmentVariableType_ViewTangent: result = "passTangent"; break;
    case ShaderGeneratorGlobalFragmentVariableType_Depth: result = "passDepth"; break;
    case ShaderGeneratorGlobalFragmentVariableType_TexCoord: result = "passTexCoord"; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown ShaderGeneratorGlobalFragmentVariableType: %d", type);
    }
    return result;
}

const char* getGlobalCodeVariableName( const ShaderGeneratorGlobalCodeVariableType type )
{
    const char* result("ERROR_UNKNOW_CODE_VAR");
    switch (type)
    {
    case ShaderGeneratorGlobalCodeVariableType_CameraViewDirection: result = "camViewDir"; break;
    case ShaderGeneratorGlobalCodeVariableType_CameraViewPosition: result = "camViewPos"; break;
    case ShaderGeneratorGlobalCodeVariableType_CameraNearFar: result = "camNearFar"; break;

    case ShaderGeneratorGlobalCodeVariableType_Time: result = "time"; break;

    case ShaderGeneratorGlobalCodeVariableType_ScreenPosition: result = "screenPos"; break;
    case ShaderGeneratorGlobalCodeVariableType_ScreenSize: result = "screenSize"; break;

    case ShaderGeneratorGlobalCodeVariableType_World: result = "matWorld"; break;
    case ShaderGeneratorGlobalCodeVariableType_WorldView: result = "matWorldView"; break;
    case ShaderGeneratorGlobalCodeVariableType_WorldViewProjection: result = "matWVP"; break;
    case ShaderGeneratorGlobalCodeVariableType_ViewProjection: result = "matViewProj"; break;
    case ShaderGeneratorGlobalCodeVariableType_View: result = "matView"; break;
    case ShaderGeneratorGlobalCodeVariableType_Projection: result = "matProj"; break;

    case ShaderGeneratorGlobalSamplerVariableType_ColorMap: result = "colorMap"; break;
    case ShaderGeneratorGlobalSamplerVariableType_DepthMap: result = "depthMap"; break;
    case ShaderGeneratorGlobalSamplerVariableType_Skybox: result = "skybox"; break;

    default:
        LOG(LogType_ProgrammerAssert, "Unknown ShaderGeneratorGlobalCodeVariableType: %d", type);
    }
    return result;
}

const char* getOutVariableName( const ShaderGeneratorOutVariableType type )
{
    const char* result("ERROR_UNKNOW_OUT_VAR");
    switch (type)
    {
    case ShaderGeneratorOutVariableType_GLPosition: result = "gl_Position"; break;
    case ShaderGeneratorOutVariableType_OutColor: result = "outColor"; break;
    case ShaderGeneratorOutVariableType_OutNormalDepth: result = "outNormalDepth"; break;
    case ShaderGeneratorOutVariableType_OutSG: result = "outSG"; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown ShaderGeneratorOutVariableType: %d", type);
    }

    return result;
}


ShaderGeneratorVariableType getGlobalInputVariableType( const ShaderGeneratorGlobalInputVariableType type )
{
    ShaderGeneratorVariableType result(ShaderGeneratorVariableType_Unknown);
    switch (type)
    {
    case ShaderGeneratorGlobalInputVariableType_Position:   result = ShaderGeneratorVariableType_Float3; break;
    case ShaderGeneratorGlobalInputVariableType_TexCoord:   result = ShaderGeneratorVariableType_Float2; break;
    case ShaderGeneratorGlobalInputVariableType_Normal:     result = ShaderGeneratorVariableType_Float3; break;
    case ShaderGeneratorGlobalInputVariableType_Tangent:    result = ShaderGeneratorVariableType_Float3; break;
    case ShaderGeneratorGlobalInputVariableType_World:      result = ShaderGeneratorVariableType_Mat44; break;
    case ShaderGeneratorGlobalInputVariableType_BoneId:     result = ShaderGeneratorVariableType_Float4; break;
    case ShaderGeneratorGlobalInputVariableType_BoneWeight: result = ShaderGeneratorVariableType_Float4; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown ShaderGeneratorGlobalInputVariableType: %d", type);
    }
    return result;
}

ShaderGeneratorVariableType getGlobalFragmentVariableType( const ShaderGeneratorGlobalFragmentVariableType type )
{
    ShaderGeneratorVariableType result(ShaderGeneratorVariableType_Unknown);
    switch (type)
    {
    case ShaderGeneratorGlobalFragmentVariableType_ViewPosition: result = ShaderGeneratorVariableType_Float3; break;
    case ShaderGeneratorGlobalFragmentVariableType_ViewNormal:   result = ShaderGeneratorVariableType_Float3; break;
    case ShaderGeneratorGlobalFragmentVariableType_ViewTangent:  result = ShaderGeneratorVariableType_Float3; break;
    case ShaderGeneratorGlobalFragmentVariableType_Depth:        result = ShaderGeneratorVariableType_Float; break;
    case ShaderGeneratorGlobalFragmentVariableType_TexCoord:     result = ShaderGeneratorVariableType_Float2; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown ShaderGeneratorGlobalFragmentVariableType: %d", type);
    }
    return result;
}

ShaderGeneratorVariableType getGlobalCodeVariableType( const ShaderGeneratorGlobalCodeVariableType type )
{
    ShaderGeneratorVariableType result(ShaderGeneratorVariableType_Unknown);
    switch (type)
    {
    case ShaderGeneratorGlobalCodeVariableType_CameraViewDirection: result = ShaderGeneratorVariableType_Float3; break;
    case ShaderGeneratorGlobalCodeVariableType_CameraViewPosition:  result = ShaderGeneratorVariableType_Float3; break;
    case ShaderGeneratorGlobalCodeVariableType_CameraNearFar:       result = ShaderGeneratorVariableType_Float2; break;

    case ShaderGeneratorGlobalCodeVariableType_Time:                result = ShaderGeneratorVariableType_Float; break;

    case ShaderGeneratorGlobalCodeVariableType_ScreenPosition:      result = ShaderGeneratorVariableType_Float2; break;
    case ShaderGeneratorGlobalCodeVariableType_ScreenSize:          result = ShaderGeneratorVariableType_Float2; break;

    case ShaderGeneratorGlobalCodeVariableType_World:               result = ShaderGeneratorVariableType_Mat44; break;
    case ShaderGeneratorGlobalCodeVariableType_WorldView:           result = ShaderGeneratorVariableType_Mat44; break;
    case ShaderGeneratorGlobalCodeVariableType_WorldViewProjection: result = ShaderGeneratorVariableType_Mat44; break;
    case ShaderGeneratorGlobalCodeVariableType_ViewProjection:      result = ShaderGeneratorVariableType_Mat44; break;
    case ShaderGeneratorGlobalCodeVariableType_View:                result = ShaderGeneratorVariableType_Mat44; break;
    case ShaderGeneratorGlobalCodeVariableType_Projection:          result = ShaderGeneratorVariableType_Mat44; break;

    case ShaderGeneratorGlobalSamplerVariableType_ColorMap:         result = ShaderGeneratorVariableType_Texture2D; break;
    case ShaderGeneratorGlobalSamplerVariableType_DepthMap:         result = ShaderGeneratorVariableType_Texture2D; break;
    case ShaderGeneratorGlobalSamplerVariableType_Skybox:           result = ShaderGeneratorVariableType_TextureCube; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown ShaderGeneratorGlobalCodeVariableType: %d", type);
    }
    return result;
}

he::ct::ShaderGeneratorVariableType getOutVariableType( const ShaderGeneratorOutVariableType type )
{
    ShaderGeneratorVariableType result(ShaderGeneratorVariableType_Unknown);
    switch (type)
    {
    case ShaderGeneratorOutVariableType_GLPosition: result = ShaderGeneratorVariableType_Float4; break;
    case ShaderGeneratorOutVariableType_OutColor: result = ShaderGeneratorVariableType_Float4; break;
    case ShaderGeneratorOutVariableType_OutNormalDepth: result = ShaderGeneratorVariableType_Float3; break;
    case ShaderGeneratorOutVariableType_OutSG: result = ShaderGeneratorVariableType_Float4; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown ShaderGeneratorVariableType: %d", type);
    }
    return result;
}

const char* shaderGeneratorSwizzleMaskToString( const ShaderGeneratorSwizzleMask mask )
{
    const char* result("");
    switch (mask)
    {
    case ShaderGeneratorSwizzleMask_X: result = "x"; break;
    case ShaderGeneratorSwizzleMask_Y: result = "y"; break;
    case ShaderGeneratorSwizzleMask_Z: result = "z"; break;
    case ShaderGeneratorSwizzleMask_W: result = "w"; break;
    }
    return result;
}


} } //end namespace
