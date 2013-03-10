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
// Author: Bastian Damman

#ifndef _HE_SHADER_H_
#define _HE_SHADER_H_
#pragma once

#include "ShaderLayout.h"
#include "Resource.h"

namespace he {
class mat44;
namespace gfx {

class UniformBuffer;
class Shader;
class Texture2D;
class TextureCube;

class ShaderVariableBase
{
friend class Shader;
friend class UniformBuffer;

protected:
    ShaderVariableBase(): m_Update(false), m_Size(0), m_Offset(0) {}
    bool m_Update;
    int m_Size;

private:
    virtual void* data() = 0;
    virtual const void* data() const = 0;

    int m_Offset;
};

template<typename T>
class ShaderVariable : public ShaderVariableBase
{
public:
    ShaderVariable() { m_Size = sizeof(T); }

    void set(const T& data)
    {
        if (m_Data != data)
        {
            m_Data = data;
            m_Update = true;
        }
    }
    const T& get() const
    {
        return m_Data;
    }

    virtual void* data()
    {
        return &m_Data;
    }
    virtual const void* data() const
    {
        return &m_Data;
    }

private:
    T m_Data;

    ShaderVariable(const ShaderVariable&);
    ShaderVariable& operator=(const ShaderVariable&);
};

class UniformBuffer
{
friend class Shader;
private:
    static uint32 s_UniformBufferCount;

public:
    UniformBuffer(size_t sizeInBytes): m_BufferId(s_UniformBufferCount++), m_BufferSize(sizeInBytes)
    {
        void* m_Buffer = he_malloc(sizeInBytes);
        he_memset(m_Buffer, 0, sizeInBytes);

        glGenBuffers(1, &m_GlBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, m_GlBuffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeInBytes, m_Buffer, GL_STREAM_DRAW);

        he_free(m_Buffer);
    }
    ~UniformBuffer()
    {
        glDeleteBuffers(1, &m_GlBuffer);
    }

    void uploadData(void* data, size_t sizeInBytes, size_t offsetInBytes = 0)
    {
        HE_ASSERT(sizeInBytes + offsetInBytes <= m_BufferSize, "Uniformbuffer going out of bounds!");
        GL::heBindUniformBuffer(m_BufferId, m_GlBuffer);
        glBufferSubData(GL_UNIFORM_BUFFER, offsetInBytes, sizeInBytes, data);
    }

private:
    uint32 m_GlBuffer;
    uint32 m_BufferId;
    size_t m_BufferSize;
};

class Shader : public Resource<Shader>
{
public:
    Shader();
    virtual ~Shader();

    bool initFromFile(const he::String& vsPath, const he::String& fsPath, const ShaderLayout& shaderLayout);
    bool initFromFile(const he::String& vsPath, const he::String& fsPath, const ShaderLayout& shaderLayout, const he::ObjectList<he::String>& outputs);
    bool initFromFile(const he::String& vsPath, const he::String& fsPath, const ShaderLayout& shaderLayout, const std::set<he::String>& defines, const he::ObjectList<he::String>& outputs = he::ObjectList<he::String>());
    
    bool initFromMem(const he::String& vs, const he::String& fs, const ShaderLayout& shaderLayout, const he::String& debugVertName, const he::String& debugFragName);
    bool initFromMem(const he::String& vs, const he::String& fs, const ShaderLayout& shaderLayout, const he::String& debugVertName, const he::String& debugFragName, const he::ObjectList<he::String>& outputs);
    bool initFromMem(const he::String& vs, const he::String& fs, const ShaderLayout& shaderLayout, const he::String& debugVertName, const he::String& debugFragName, const std::set<he::String>& defines, const he::ObjectList<he::String>& outputs = he::ObjectList<he::String>());

    void bind();

    uint32 getShaderVarId(const he::String& name) const;
    uint32 getBufferId(const he::String& name) const;
    uint32 getBufferVarId(uint32 bufferId, const he::String& name) const;
    uint32 getShaderSamplerId(const he::String& name);

    UniformBuffer* setBuffer(uint32 id); //create new buffer
    void setBuffer(uint32 id, UniformBuffer* pBuffer); //used to share buffer

    void setShaderVar(uint32 id, int value) const;
    void setShaderVar(uint32 id, uint32 value) const;
    void setShaderVar(uint32 id, float value) const;
    void setShaderVar(uint32 id, const vec2& vec) const;
    void setShaderVar(uint32 id, const vec3& vec) const;
    void setShaderVar(uint32 id, const vec4& vec) const;
    void setShaderVar(uint32 id, const mat44& matrix) const;
    void setShaderVar(uint32 id, const he::PrimitiveList<mat44>& matrixArray) const;
    void setShaderVar(uint32 id, const gfx::Texture2D* tex2D) const;
    void setShaderVar(uint32 id, const gfx::TextureCube* texCube) const;
    
private:
    uint32 m_Id;
    uint32 m_VsId;
    uint32 m_FsId;

    std::map<he::String, uint32> m_SamplerLocationMap;
    std::map<uint32, UniformBuffer*> m_UniformBufferMap;

    he::String m_FragShaderName;
    he::String m_VertShaderName;

    static uint32 s_CurrentBoundShader;

    //Disable default copy constructor and default assignment operator
    Shader(const Shader&);
    Shader& operator=(const Shader&);
};

} } //end namespace

#endif
