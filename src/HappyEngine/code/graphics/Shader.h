//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#ifndef _HE_SHADER_H_
#define _HE_SHADER_H_
#pragma once

#include <string>
#include <map>
#include <vector>
#include <set>
#include "ShaderLayout.h"

#include "mat44.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "Texture2D.h"
#include "HappyTypes.h"

#include "OpenGL.h"

#include "boost/shared_ptr.hpp"

namespace he {
namespace gfx {

class UniformBuffer;
class Shader;

class ShaderVariableBase
{
friend class Shader;
friend class UniformBuffer;

protected:
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

    ShaderVariable& operator=(const T& data)
    {
        set(data);
        return *this;
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
public:
    typedef boost::shared_ptr<UniformBuffer> pointer;
    ~UniformBuffer()
    {
        glDeleteBuffers(1, &m_GlBuffer);
    }

    void getShaderVar(const std::string& name, ShaderVariableBase& outVar) const
    {
        uint index;
        const char* c_name(name.c_str());
        glGetUniformIndices(m_ProgramId, 1, &c_name, &index);

        glGetActiveUniformsiv(m_ProgramId, 1, &index, GL_UNIFORM_OFFSET, &outVar.m_Offset);
    }
    void setShaderVar(ShaderVariableBase& var)
    {
        if (var.m_Update == true)
        {
            GL::heBindUniformBuffer(m_BufferId, m_GlBuffer);
            glBufferSubData(GL_UNIFORM_BUFFER, var.m_Offset, var.m_Size, var.data());
            var.m_Update = false;
        }
    }

private:
    UniformBuffer(uint programId, uint bufferPos): m_ProgramId(programId), m_BufferId(s_UniformBufferCount++)
    {
        int blockSize;
        glGetActiveUniformBlockiv(programId, bufferPos, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

        void* m_Buffer = malloc(blockSize);
        memset(m_Buffer, 0, blockSize);

        glGenBuffers(1, &m_GlBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, m_GlBuffer);
        glBufferData(GL_UNIFORM_BUFFER, blockSize, m_Buffer, GL_DYNAMIC_DRAW);

        free(m_Buffer);
    }

    static uint s_UniformBufferCount;

    uint m_GlBuffer;

    uint m_ProgramId;
    uint m_BufferId;
};

class Shader
{
public:
    Shader();
    virtual ~Shader();

    bool init(const std::string& vsPath, const std::string& fsPath, const ShaderLayout& shaderLayout);
    bool init(const std::string& vsPath, const std::string& fsPath, const ShaderLayout& shaderLayout, const std::vector<std::string>& outputs);
    bool init(const std::string& vsPath, const std::string& fsPath, const ShaderLayout& shaderLayout, const std::set<std::string>& defines, const std::vector<std::string>& outputs = std::vector<std::string>());

    void bind();

    uint getShaderVarId(const std::string& name) const;
    uint getBufferId(const std::string& name) const;
    uint getBufferVarId(uint bufferId, const std::string& name) const;
    uint getShaderSamplerId(const std::string& name);

    UniformBuffer::pointer setBuffer(uint id); //create new buffer
    void setBuffer(uint id, const UniformBuffer::pointer& pBuffer); //used to share buffer

    void setShaderVar(uint id, int value) const;
    void setShaderVar(uint id, uint value) const;
    void setShaderVar(uint id, float value) const;
    void setShaderVar(uint id, const vec2& vec) const;
    void setShaderVar(uint id, const vec3& vec) const;
    void setShaderVar(uint id, const vec4& vec) const;
    void setShaderVar(uint id, const mat44& matrix) const;
    void setShaderVar(uint id, const std::vector<mat44>& matrixArray) const;
    void setShaderVar(uint id, const gfx::Texture2D::pointer& tex2D) const;

    typedef boost::shared_ptr<Shader> pointer;

private:
    uint m_Id;
    uint m_VsId;
    uint m_FsId;

    std::map<std::string, uint> m_SamplerLocationMap;
    std::map<uint, UniformBuffer::pointer> m_UniformBufferMap;

    std::string m_FragShaderName;

    static uint s_CurrentBoundShader;

    //Disable default copy constructor and default assignment operator
    Shader(const Shader&);
    Shader& operator=(const Shader&);
};

} } //end namespace

#endif
