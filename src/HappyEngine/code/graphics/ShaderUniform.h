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

#ifndef _HE_ShaderUniform_H_
#define _HE_ShaderUniform_H_
#pragma once

#include "ShaderEnums.h"
#include "Shader.h"

namespace he {
namespace gfx {

class IShaderUniform
{
public:
    virtual ~IShaderUniform() {}
    virtual void push(Shader* const shader) = 0;
    virtual const he::FixedString& getName() const = 0;
    virtual EShaderUniformType getType() const = 0;
};

namespace details {
template<typename T, EShaderUniformType TType>
class ShaderUniform;
}

typedef details::ShaderUniform<int32, eShaderUniformType_Int> ShaderUniformInt;
typedef details::ShaderUniform<uint32, eShaderUniformType_UInt> ShaderUniformUint;
typedef details::ShaderUniform<float, eShaderUniformType_Float> ShaderUniformFloat;
typedef details::ShaderUniform<vec2, eShaderUniformType_Float2> ShaderUniformVec2;
typedef details::ShaderUniform<vec3, eShaderUniformType_Float3> ShaderUniformVec3;
typedef details::ShaderUniform<vec4, eShaderUniformType_Float4> ShaderUniformVec4;
typedef details::ShaderUniform<mat44, eShaderUniformType_Mat44> ShaderUniformMat44;
typedef details::ShaderUniform<he::PrimitiveList<mat44>, eShaderUniformType_Mat44Array> ShaderUniformMat44Array;
typedef details::ShaderUniform<const Texture2D*, eShaderUniformType_Texture2D> ShaderUniformTexture2D;
typedef details::ShaderUniform<const TextureCube*, eShaderUniformType_TextureCube> ShaderUniformTextureCube;

namespace details {

template<typename T, EShaderUniformType TType>
class ShaderUniform : public IShaderUniform
{
public:
    ShaderUniform(const he::FixedString& name, const uint32 id, const T& defaultValue)
        : m_Dirty(true)
        , m_Name(name)
        , m_Value(defaultValue)
        , m_ID(id)
    {
        
    }

    void push(Shader* const shader)
    {
        if (m_Dirty)
        {
            shader->setShaderVar(m_ID, m_Value);
            m_Dirty = false;
        }
    }
    void set(const T& value)
    {
        if (m_Value != value)
        {
            m_Value = value;
            m_Dirty = true;
        }
    }

    const he::FixedString& getName() const
    {
        return m_Name;
    }

    EShaderUniformType getType() const { return TType; }
    
private:
    T m_Value;
    bool m_Dirty;
    uint32 m_ID;
    he::FixedString m_Name;
};

template<>
class ShaderUniform<he::PrimitiveList<mat44>, eShaderUniformType_Mat44Array> : public IShaderUniform
{
public:
    ShaderUniform(const he::FixedString& name, const uint32 id)
        : m_Dirty(true)
        , m_Name(name)
        , m_ID(id)
    {

    }

    void push(Shader* const shader)
    {
        if (m_Dirty)
        {
            shader->setShaderVar(m_ID, m_Value);
            m_Dirty = false;
        }
    }
    void set(const he::PrimitiveList<mat44>& value)
    {
        const size_t size(value.size());
        if (m_Dirty == false)
        {
            if (m_Value.size() == size)
            {
                for (size_t i(0); i < size; ++i)
                {
                    if (m_Value[i] != value[i])
                    {
                        m_Dirty = true;
                        break;
                    }
                }
            }
        }
        if (m_Dirty == true)
        {
            m_Value.clear();
            m_Value.append(value);
        }
    }

    const he::FixedString& getName() const
    {
        return m_Name;
    }

    EShaderUniformType getType() const { return eShaderUniformType_Mat44Array; }

private:
    he::PrimitiveList<mat44> m_Value;
    bool m_Dirty;
    uint32 m_ID;
    he::FixedString m_Name;
};

}

} } //end namespace

#endif
