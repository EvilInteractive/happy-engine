//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
//Author: Bastian Damman
//Created: 2013/06/16

#ifndef _HE_ShaderEnums_H_
#define _HE_ShaderEnums_H_
#pragma once

#include "BufferLayout.h"

namespace he {
namespace gfx {
    
struct ShaderUniformID
{
    const static size_t Unassigned;
    size_t m_ID;
    
    ShaderUniformID(): m_ID(Unassigned) {}
};
    
enum EShaderUniformUsage
{   // Only add to the back, or you will corrupt save files!
    eShaderUniformUsage_Invalid,
    eShaderUniformUsage_ObjectProperty,
    eShaderUniformUsage_GlobalProperty,
    eShaderUniformUsage_UserProperty,

};

enum EShaderAttributePropertyUsage
{   // Only add to the back, or you will corrupt save files!
    eShaderAttributePropertyUsage_Invalid,
    eShaderAttributePropertyUsage_Position,
    eShaderAttributePropertyUsage_TextureCoordiante,
    eShaderAttributePropertyUsage_Normal,
    eShaderAttributePropertyUsage_Tangent,
    eShaderAttributePropertyUsage_Color,
    eShaderAttributePropertyUsage_BoneIndices,
    eShaderAttributePropertyUsage_BoneWeights
};
EShaderAttributePropertyUsage bufferElementUsageToShaderAttribUsage(const BufferElement::Usage usage);

enum EShaderObjectPropertyUsage
{   // Only add to the back, or you will corrupt save files!
    eShaderObjectPropertyUsage_Invalid,
    eShaderObjectPropertyUsage_WorldViewProjection,
    eShaderObjectPropertyUsage_World,
    eShaderObjectPropertyUsage_WorldView,
    eShaderObjectPropertyUsage_WorldPosition,
    eShaderObjectPropertyUsage_BoneTransforms
};  

enum EShaderGlobalPropertyUsage
{   // Only add to the back, or you will corrupt save files!
    eShaderGlobalPropertyUsage_Invalid,
    eShaderGlobalPropertyUsage_ViewProjection,
    eShaderGlobalPropertyUsage_View,
    eShaderGlobalPropertyUsage_AmbientColor,
    eShaderGlobalPropertyUsage_DirectionalColor,
    eShaderGlobalPropertyUsage_DirectionalDirection,
    eShaderGlobalPropertyUsage_NearFar
};

ENUM(EShaderUniformType, uint8)
{   // Only add to the back, or you will corrupt save files!
    eShaderUniformType_Invalid,
    eShaderUniformType_Int,
    eShaderUniformType_UInt,
    eShaderUniformType_Float,
    eShaderUniformType_Float2,
    eShaderUniformType_Float3,
    eShaderUniformType_Float4,
    eShaderUniformType_Mat44,
    eShaderUniformType_Mat44Array,
    eShaderUniformType_Texture1D,
    eShaderUniformType_Texture2D,
    eShaderUniformType_TextureCube,
};

} } //end namespace

#endif
