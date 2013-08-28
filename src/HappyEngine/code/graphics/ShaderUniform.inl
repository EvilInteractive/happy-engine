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

template<>
class ShaderUniform<const he::gfx::Texture2D*, eShaderUniformType_Texture2D> : public IShaderUniform
{
public:
    ShaderUniform(const he::FixedString& name, const uint32 id, const he::gfx::Texture2D* defaultValue)
        : m_Dirty(true)
        , m_Value(defaultValue)
        , m_Name(name)
        , m_ID(id)
    {
        if (nullptr != m_Value)
        {
            defaultValue->instantiate();
        }
    }
    ~ShaderUniform()
    {
        if (nullptr != m_Value)
        {
            m_Value->release();
        }
    }

    void push(Shader* const shader)
    {
        if (m_Dirty)
        {
            shader->setShaderVar(m_ID, m_Value);
            m_Dirty = false;
        }
    }
    void set(const he::gfx::Texture2D* const value)
    {
        if (m_Value != value)
        {
            if (nullptr != m_Value)
            {
                m_Value->release();
            }
            m_Value = value;
            m_Value->instantiate();
        }
    }

    const he::FixedString& getName() const
    {
        return m_Name;
    }

    EShaderUniformType getType() const { return eShaderUniformType_Texture2D; }

private:
    const he::gfx::Texture2D* m_Value;
    bool m_Dirty;
    uint32 m_ID;
    he::FixedString m_Name;
};

template<>
class ShaderUniform<const he::gfx::TextureCube*, eShaderUniformType_TextureCube> : public IShaderUniform
{
public:
    ShaderUniform(const he::FixedString& name, const uint32 id, const he::gfx::TextureCube* defaultValue)
        : m_Dirty(true)
        , m_Value(defaultValue)
        , m_Name(name)
        , m_ID(id)
    {
        if (nullptr != m_Value)
        {
            defaultValue->instantiate();
        }
    }
    ~ShaderUniform()
    {
        if (nullptr != m_Value)
        {
            m_Value->release();
        }
    }

    void push(Shader* const shader)
    {
        if (m_Dirty)
        {
            shader->setShaderVar(m_ID, m_Value);
            m_Dirty = false;
        }
    }
    void set(const he::gfx::TextureCube* const value)
    {
        if (m_Value != value)
        {
            if (nullptr != m_Value)
            {
                m_Value->release();
            }
            m_Value = value;
            m_Value->instantiate();
        }
    }

    const he::FixedString& getName() const
    {
        return m_Name;
    }

    EShaderUniformType getType() const { return eShaderUniformType_TextureCube; }

private:
    const he::gfx::TextureCube* m_Value;
    bool m_Dirty;
    uint32 m_ID;
    he::FixedString m_Name;
};

} } }
