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
    
const char* getGlobalInputVariableName( const EShaderGeneratorGlobalInputVariableType type )
{
    const char* result("ERROR_UNKNOW_INPUT_VAR");
    switch (type)
    {
    case eShaderGeneratorGlobalInputVariableType_Position:   result = "Position";   break;
    case eShaderGeneratorGlobalInputVariableType_TexCoord:   result = "TexCoord";   break;
    case eShaderGeneratorGlobalInputVariableType_Normal:     result = "Normal";     break;
    case eShaderGeneratorGlobalInputVariableType_Tangent:    result = "Tangent";    break;
    case eShaderGeneratorGlobalInputVariableType_World:      result = "World";      break;
    case eShaderGeneratorGlobalInputVariableType_BoneId:     result = "BoneId";     break;
    case eShaderGeneratorGlobalInputVariableType_BoneWeight: result = "BoneWeight"; break;
    case eShaderGeneratorGlobalInputVariableType_Depth:      result = "Depth"; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown ShaderGeneratorGlobalInputVariableType: %d", type);
    }
    return result;
}

const char* getGlobalFrameVariableName(const EShaderGeneratorGlobalFrameVariableType type)
{
    const char* result("ERROR_UNKNOW_FRAME_VAR");
    switch (type)
    {
    case eShaderGeneratorGlobalFrameVariableType_Time: result = "perFrameUniformBuffer.time"; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown EShaderGeneratorGlobalFrameVariableType: %d", type);
    }
    return result;
}
const char* getGlobalSceneVariableName(const EShaderGeneratorGlobalSceneVariableType type)
{
    const char* result("ERROR_UNKNOW_SCENE_VAR");
    switch (type)
    {
    case eShaderGeneratorGlobalSceneVariableType_AmbientLightColor: result = "perSceneUniformBuffer.ambLightColor"; break;
    case eShaderGeneratorGlobalSceneVariableType_DirectionalLightColor: result = "perSceneUniformBuffer.dirLightColor"; break;
    case eShaderGeneratorGlobalSceneVariableType_DirectionalLightDirection: result = "perSceneUniformBuffer.dirLightDirection"; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown EShaderGeneratorGlobalSceneVariableType: %d", type);
    }
    return result;
}
const char* getGlobalCameraVariableName(const EShaderGeneratorGlobalCameraVariableType type)
{
    const char* result("ERROR_UNKNOW_CAMERA_VAR");
    switch (type)
    {
    case eShaderGeneratorGlobalCameraVariableType_CameraNearFar: result = "perCameraUniformBuffer.cameraNearFar"; break;
    case eShaderGeneratorGlobalCameraVariableType_CameraPostion: result = "perCameraUniformBuffer.cameraPosition"; break;
    case eShaderGeneratorGlobalCameraVariableType_ViewportSize: result = "perCameraUniformBuffer.cameraViewSize"; break;
    case eShaderGeneratorGlobalCameraVariableType_ViewProjection: result = "perCameraUniformBuffer.viewProjMatrix"; break;
    case eShaderGeneratorGlobalCameraVariableType_View: result = "perCameraUniformBuffer.viewMatrix"; break;
    case eShaderGeneratorGlobalCameraVariableType_Projection: result = "perCameraUniformBuffer.projMatrix"; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown EShaderGeneratorGlobalFrameVariableType: %d", type);
    }
    return result;
}
const char* getGlobalSamplerVariableName(const EShaderGeneratorGlobalSamplerVariableType type)
{
    const char* result("ERROR_UNKNOW_TEXTURE_VAR");
    switch (type)
    {
    case eShaderGeneratorGlobalSamplerVariableType_DepthMap: result = "depthMap"; break;
    case eShaderGeneratorGlobalSamplerVariableType_ColorMap: result = "colorMap"; break;
    case eShaderGeneratorGlobalSamplerVariableType_Skybox: result = "skybox"; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown EShaderGeneratorGlobalTextureVariableType: %d", type);
    }
    return result;
}

const char* getOutVariableName( const EShaderGeneratorGlobalOutVariableType type )
{
    const char* result("ERROR_UNKNOW_OUT_VAR");
    switch (type)
    {
    case eShaderGeneratorGlobalOutVariableType_GLPosition: result = "gl_Position"; break;
    case eShaderGeneratorGlobalOutVariableType_OutColor: result = "outColor"; break;
    case eShaderGeneratorGlobalOutVariableType_OutNormalDepth: result = "outNormalDepth"; break;
    case eShaderGeneratorGlobalOutVariableType_OutSG: result = "outSG"; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown ShaderGeneratorOutVariableType: %d", type);
    }

    return result;
}


EShaderGeneratorVariableType getGlobalInputVariableType( const EShaderGeneratorGlobalInputVariableType type )
{
    EShaderGeneratorVariableType result(eShaderGeneratorVariableType_Unknown);
    switch (type)
    {
    case eShaderGeneratorGlobalInputVariableType_Position:   result = eShaderGeneratorVariableType_Float3; break;
    case eShaderGeneratorGlobalInputVariableType_TexCoord:   result = eShaderGeneratorVariableType_Float2; break;
    case eShaderGeneratorGlobalInputVariableType_Normal:     result = eShaderGeneratorVariableType_Float3; break;
    case eShaderGeneratorGlobalInputVariableType_Tangent:    result = eShaderGeneratorVariableType_Float3; break;
    case eShaderGeneratorGlobalInputVariableType_World:      result = eShaderGeneratorVariableType_Mat44; break;
    case eShaderGeneratorGlobalInputVariableType_BoneId:     result = eShaderGeneratorVariableType_Float4; break;
    case eShaderGeneratorGlobalInputVariableType_BoneWeight: result = eShaderGeneratorVariableType_Float4; break;
    case eShaderGeneratorGlobalInputVariableType_Depth:      result = eShaderGeneratorVariableType_Float; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown ShaderGeneratorGlobalInputVariableType: %d", type);
    }
    return result;
}

EShaderGeneratorVariableType getGlobalFrameVariableType(const EShaderGeneratorGlobalFrameVariableType type)
{
    EShaderGeneratorVariableType result(eShaderGeneratorVariableType_Unknown);
    switch (type)
    {
    case eShaderGeneratorGlobalFrameVariableType_Time: result = eShaderGeneratorVariableType_Uint; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown EShaderGeneratorGlobalFrameVariableType: %d", type);
    }
    return result;
}
EShaderGeneratorVariableType getGlobalSceneVariableType(const EShaderGeneratorGlobalSceneVariableType type)
{
    EShaderGeneratorVariableType result(eShaderGeneratorVariableType_Unknown);
    switch (type)
    {
    case eShaderGeneratorGlobalSceneVariableType_AmbientLightColor: result = eShaderGeneratorVariableType_Float4; break;
    case eShaderGeneratorGlobalSceneVariableType_DirectionalLightColor: result = eShaderGeneratorVariableType_Float4; break;
    case eShaderGeneratorGlobalSceneVariableType_DirectionalLightDirection: result = eShaderGeneratorVariableType_Float3; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown EShaderGeneratorGlobalSceneVariableType: %d", type);
    }
    return result;
}
EShaderGeneratorVariableType getGlobalCameraVariableType(const EShaderGeneratorGlobalCameraVariableType type)
{
    EShaderGeneratorVariableType result(eShaderGeneratorVariableType_Unknown);
    switch (type)
    {
    case eShaderGeneratorGlobalCameraVariableType_CameraNearFar: result = eShaderGeneratorVariableType_Float2; break;
    case eShaderGeneratorGlobalCameraVariableType_CameraPostion: result = eShaderGeneratorVariableType_Float3; break;
    case eShaderGeneratorGlobalCameraVariableType_ViewportSize: result = eShaderGeneratorVariableType_Float2; break;
    case eShaderGeneratorGlobalCameraVariableType_ViewProjection: result = eShaderGeneratorVariableType_Mat44; break;
    case eShaderGeneratorGlobalCameraVariableType_View: result = eShaderGeneratorVariableType_Mat44; break;
    case eShaderGeneratorGlobalCameraVariableType_Projection: result = eShaderGeneratorVariableType_Mat44; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown EShaderGeneratorGlobalFrameVariableType: %d", type);
    }
    return result;
}
EShaderGeneratorVariableType getGlobalSamplerVariableType(const EShaderGeneratorGlobalSamplerVariableType type)
{
    EShaderGeneratorVariableType result(eShaderGeneratorVariableType_Unknown);
    switch (type)
    {
    case eShaderGeneratorGlobalSamplerVariableType_DepthMap: result = eShaderGeneratorVariableType_Texture2D; break;
    case eShaderGeneratorGlobalSamplerVariableType_ColorMap: result = eShaderGeneratorVariableType_Texture2D; break;
    case eShaderGeneratorGlobalSamplerVariableType_Skybox: result = eShaderGeneratorVariableType_TextureCube; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown EShaderGeneratorGlobalTextureVariableType: %d", type);
    }
    return result;
}


he::ct::EShaderGeneratorVariableType getOutVariableType( const EShaderGeneratorGlobalOutVariableType type )
{
    EShaderGeneratorVariableType result(eShaderGeneratorVariableType_Unknown);
    switch (type)
    {
    case eShaderGeneratorGlobalOutVariableType_GLPosition: result = eShaderGeneratorVariableType_Float4; break;
    case eShaderGeneratorGlobalOutVariableType_OutColor: result = eShaderGeneratorVariableType_Float4; break;
    case eShaderGeneratorGlobalOutVariableType_OutNormalDepth: result = eShaderGeneratorVariableType_Float3; break;
    case eShaderGeneratorGlobalOutVariableType_OutSG: result = eShaderGeneratorVariableType_Float4; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown ShaderGeneratorVariableType: %d", type);
    }
    return result;
}

const char* shaderGeneratorSwizzleMaskToString( const EShaderGeneratorSwizzleMask mask )
{
    const char* result("");
    switch (mask)
    {
    case eShaderGeneratorSwizzleMask_X: result = "x"; break;
    case eShaderGeneratorSwizzleMask_Y: result = "y"; break;
    case eShaderGeneratorSwizzleMask_Z: result = "z"; break;
    case eShaderGeneratorSwizzleMask_W: result = "w"; break;
    case eShaderGeneratorSwizzleMask_None: result = "none"; break;
    }
    return result;
}

he::ct::EShaderGeneratorSwizzleMask shaderGeneratorSwizzleMaskFromString( const char* str )
{
    if (strcmp(str, "x") == 0)
        return eShaderGeneratorSwizzleMask_X;
    else if (strcmp(str, "y") == 0)
        return eShaderGeneratorSwizzleMask_Y;
    else if (strcmp(str, "z") == 0)
        return eShaderGeneratorSwizzleMask_Z;
    else if (strcmp(str, "w") == 0)
        return eShaderGeneratorSwizzleMask_W;
    else
        return eShaderGeneratorSwizzleMask_None;
}

const char* getGlobalIncludeFile( const EShaderGeneratorVariableScope scope )
{
    const char* result(nullptr);
    switch (scope)
    {
    case eShaderGeneratorVariableScope_GlobalCamera: result = "shared/perCameraUniformBuffer.frag"; break;
    case eShaderGeneratorVariableScope_GlobalScene: result = "shared/perSceneUniformBuffer.frag"; break;
    case eShaderGeneratorVariableScope_GlobalFrame: result = "shared/perFrameUniformBuffer.frag"; break;
    }
    return result;
}


} } //end namespace
