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

namespace he {
namespace gfx {
   
struct ShaderUniformID
{
    const static ShaderUniformID Unassigned;
    uint32 m_ID;
    
    ShaderUniformID(): m_ID(UINT32_MAX) {}
    explicit ShaderUniformID(const uint32 value): m_ID(value) {}

    bool operator==(const ShaderUniformID other) const { return m_ID == other.m_ID; }
    bool operator!=(const ShaderUniformID other) const { return m_ID != other.m_ID; }
    
    HE_FORCEINLINE uint32 getValue() const { return m_ID; }
    HE_FORCEINLINE void setValue(const uint32 value) { m_ID = value; }
};
    
enum EShaderUniformUsage
{   // Only add to the back, or you will corrupt save files!
    eShaderUniformUsage_Invalid,
    eShaderUniformUsage_ObjectProperty,
    eShaderUniformUsage_GlobalProperty,
    eShaderUniformUsage_UserProperty,

};

enum EShaderAttributeType
{
    eShaderAttributeType_Float = GL_FLOAT,
    eShaderAttributeType_Int32 = GL_INT,
    eShaderAttributeType_UInt32 = GL_UNSIGNED_INT
};

enum EShaderAttributeTypeComponents
{
    eShaderAttributeTypeComponents_1 = 1,
    eShaderAttributeTypeComponents_2 = 2,
    eShaderAttributeTypeComponents_3 = 3,
    eShaderAttributeTypeComponents_4 = 4
};

size_t getShaderAttributeSize(const EShaderAttributeType type, const EShaderAttributeTypeComponents components);

enum EShaderAttribute
{
    eShaderAttribute_Invalid = -1,
    eShaderAttribute_Position,
    eShaderAttribute_TextureCoordiante,
    eShaderAttribute_Normal,
    eShaderAttribute_Tangent,
    eShaderAttribute_Color,
    eShaderAttribute_BoneIndices,
    eShaderAttribute_BoneWeights
};

enum EShaderObjectPropertyUsage
{   // Only add to the back, or you will corrupt save files!
    eShaderObjectPropertyUsage_Invalid = -1,
    eShaderObjectPropertyUsage_WorldViewProjection,
    eShaderObjectPropertyUsage_World,
    eShaderObjectPropertyUsage_WorldView,
    eShaderObjectPropertyUsage_WorldPosition,
    eShaderObjectPropertyUsage_BoneTransforms
};  

enum EShaderGlobalPropertyUsage
{   // Only add to the back, or you will corrupt save files!
    eShaderGlobalPropertyUsage_Invalid = -1,
    eShaderGlobalPropertyUsage_ViewProjection,
    eShaderGlobalPropertyUsage_View,
    eShaderGlobalPropertyUsage_AmbientColor,
    eShaderGlobalPropertyUsage_DirectionalColor,
    eShaderGlobalPropertyUsage_DirectionalDirection,
    eShaderGlobalPropertyUsage_NearFar
};

ENUM(EShaderUniformType, uint8)
{
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
