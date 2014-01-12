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
//Created: 2013/08/21

#include "Texture2D.h"
#include "TextureCube.h"

namespace he {
namespace gfx {
namespace details {
    
template<typename T, EShaderUniformType TType>
ShaderUniform<T, TType>::ShaderUniform(const he::FixedString& name, const uint32 id, const T& defaultValue)
: m_Value(id)
, m_ID(id)
, m_Name(name)
{
}

template<typename T, EShaderUniformType TType>
void ShaderUniform<T, TType>::init(Shader* const shader)
{
    shader->setShaderVar(m_ID, m_Value);
}
    
template<typename T, EShaderUniformType TType>
void ShaderUniform<T, TType>::set(Shader* const shader, const T& value)
{
    if (m_Value != value)
    {
        m_Value = value;
        shader->setShaderVar(m_ID, value);
    }
}
    
template<>
class ShaderUniform<he::PrimitiveList<mat44>, eShaderUniformType_Mat44Array> : public IShaderUniform
{
public:
    ShaderUniform(const he::FixedString& name, const uint32 id)
        : m_ID(id)
        , m_Name(name)
    {
    }
    
    void init(Shader* const shader)
    {
        shader->setShaderVar(m_ID, m_Value);
    }
    
    void set(Shader* const shader, const he::PrimitiveList<mat44>& value)
    {
        const size_t size(value.size());
        bool dirty(false);
        if (m_Value.size() == size)
        {
            for (size_t i(0); i < size; ++i)
            {
                if (m_Value[i] != value[i])
                {
                    dirty = true;
                    break;
                }
            }
        }
        if (dirty == true)
        {
            m_Value.clear();
            m_Value.append(value);
            shader->setShaderVar(m_ID, m_Value);
        }
    }
    
    const he::PrimitiveList<mat44>& getValue() const { return m_Value; }

    const he::FixedString& getName() const
    {
        return m_Name;
    }

    EShaderUniformType getType() const { return eShaderUniformType_Mat44Array; }

private:
    he::PrimitiveList<mat44> m_Value;
    uint32 m_ID;
    he::FixedString m_Name;
};

template<>
class ShaderUniform<uint32, eShaderUniformType_Texture2D> : public IShaderUniform
{
public:
    ShaderUniform(const he::FixedString& name, const uint32 id, const he::gfx::Texture2D* defaultValue)
        : m_Value(0)
        , m_ID(id)
        , m_Name(name)
    {
        if (defaultValue != nullptr)
        {
            m_Value = defaultValue->getID();
        }
    }
    
    ~ShaderUniform()
    {
    }
    
    void init(Shader* const shader)
    {
        shader->setSampler2D(m_ID, m_Value);
    }
    
    void set(Shader* const shader, const he::ObjectHandle handle)
    {
        if (handle != ObjectHandle::unassigned)
        {
            set(shader, TextureFactory::getInstance()->get(handle));
        }
        else
        {
            set(shader, nullptr);
        }
    }
    void set(Shader* const shader, const he::gfx::Texture2D* const value)
    {
        if (value != nullptr)
        {
            if (m_Value != value->getID())
            {
                m_Value = value->getID();
                shader->setSampler2D(m_ID, m_Value);
            }
        }
        else
        {
            m_Value = 0; // Fallback needed?
            shader->setSampler2D(m_ID, m_Value);
        }
    }
    
    const uint32& getValue() const { return m_Value; }

    const he::FixedString& getName() const
    {
        return m_Name;
    }

    EShaderUniformType getType() const { return eShaderUniformType_Texture2D; }

private:
    uint32 m_Value;
    uint32 m_ID;
    he::FixedString m_Name;
};

template<>
class ShaderUniform<uint32, eShaderUniformType_TextureCube> : public IShaderUniform
{
public:
    ShaderUniform(const he::FixedString& name, const uint32 id, const he::gfx::TextureCube* defaultValue)
        : m_Value(0)
        , m_ID(id)
        , m_Name(name)
    {
        if (defaultValue != nullptr)
        {
            m_Value = defaultValue->getID();
        }
    }
    ~ShaderUniform()
    {
    }
    
    void init(Shader* const shader)
    {
        shader->setSamplerCube(m_ID, m_Value);
    }
    
    void set(Shader* const shader, const he::ObjectHandle handle)
    {
        if (handle != ObjectHandle::unassigned)
        {
            set(shader, TextureCubeFactory::getInstance()->get(handle));
        }
        else
        {
            set(shader, nullptr);
        }
    }
    void set(Shader* const shader, const he::gfx::TextureCube* const value)
    {
        if (value != nullptr)
        {
            if (m_Value != value->getID())
            {
                m_Value = value->getID();
                shader->setSamplerCube(m_ID, m_Value);
            }
        }
        else
        {
            m_Value = 0; // Fallback needed?
            shader->setSamplerCube(m_ID, m_Value);
        }
    }
    
    const uint32& getValue() const { return m_Value; }
    
    const he::FixedString& getName() const
    {
        return m_Name;
    }

    EShaderUniformType getType() const { return eShaderUniformType_TextureCube; }

private:
    uint32 m_Value;
    uint32 m_ID;
    he::FixedString m_Name;
};

} } }
