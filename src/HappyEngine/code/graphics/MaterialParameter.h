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
//Created: 2013/09/02

#ifndef _HE_MaterialParameter_H_
#define _HE_MaterialParameter_H_
#pragma once

#include "ShaderEnums.h"
#include "MaterialEnums.h"

namespace he {
namespace gfx {
class Texture2D;
class TextureCube;
class MaterialParameter
{
public:
    enum EType
    {
        eType_Invalid,
        eType_Float,
        eType_Float2,
        eType_Float3,
        eType_Float4,
        eType_Float44,
        eType_Int,
        eType_Texture2D,
        eType_TextureCube,
    };
    const char* typeToString(const EType type) const;
    
    MaterialParameter();
    ~MaterialParameter();
    MaterialParameter& operator=(const MaterialParameter& other);
    MaterialParameter& operator=(MaterialParameter&& other);
    
    void init(const EType type);
    void setShaderUniformID(const ShaderUniformID id, const he::gfx::EShaderPassType pass, const he::gfx::EShaderRenderType rtype);
    
    ShaderUniformID getID(const he::gfx::EShaderPassType pass, const he::gfx::EShaderRenderType rtype) const { return m_ID[pass][rtype]; }
    EType getType() const { return m_Type; }

    float getFloat() const;
    const vec2& getFloat2() const;
    const vec3& getFloat3() const;
    const vec4& getFloat4() const;
    const mat44& getFloat44() const;
    int32 getInt() const;
    ObjectHandle getTexture2D() const;
    ObjectHandle getTextureCube() const;
    
    void setFloat(const float data);
    void setFloat2(const vec2& data);
    void setFloat3(const vec3& data);
    void setFloat4(const vec4& data);
    void setFloat44(const mat44& data);
    void setInt32(const int32 data);
    void setTexture2D(const Texture2D* const data);
    void setTextureCube(const TextureCube* const data);

private:
    ShaderUniformID m_ID[eShaderPassType_MAX][eShaderRenderType_MAX];
    EType m_Type;
    union Data
    {
        float m_Float[4];
        float* m_Matrix;
        int32 m_Int;
        uint32 m_Handle;
    } m_Data;
    
    void instantiate();
    void release();
    
    HE_COMPILE_ASSERT(sizeof(vec2) == sizeof(float) * 2, "Vec2 does not have the same size as a float*2!");
    HE_COMPILE_ASSERT(sizeof(vec3) == sizeof(float) * 3, "Vec3 does not have the same size as a float*3!");
    HE_COMPILE_ASSERT(sizeof(vec4) == sizeof(float) * 4, "Vec4 does not have the same size as a float*4!");
    HE_COMPILE_ASSERT(sizeof(mat44) == sizeof(float) * 4 * 4, "Mat44 does not have the same size as a float*4*4!");
    
    // disabled
    MaterialParameter(const MaterialParameter&);
};

MaterialParameter::EType ShaderUniformTypeToMaterialType(const EShaderUniformType type);

} } //end namespace

#endif
