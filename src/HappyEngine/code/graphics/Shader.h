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
public:
    ~UniformBuffer()
    {
        glDeleteBuffers(1, &m_GlBuffer);
    }

    void getShaderVar(const std::string& name, ShaderVariableBase& outVar) const
    {
        uint32 index;
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
    UniformBuffer(uint32 programId, uint32 bufferPos): m_ProgramId(programId), m_BufferId(s_UniformBufferCount++)
    {
        int blockSize;
        glGetActiveUniformBlockiv(programId, bufferPos, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

        void* m_Buffer = he_malloc(blockSize);
        he_memset(m_Buffer, 0, blockSize);

        glGenBuffers(1, &m_GlBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, m_GlBuffer);
        glBufferData(GL_UNIFORM_BUFFER, blockSize, m_Buffer, GL_DYNAMIC_DRAW);

        he_free(m_Buffer);
    }

    static uint32 s_UniformBufferCount;

    uint32 m_GlBuffer;

    uint32 m_ProgramId;
    uint32 m_BufferId;
};

class Shader : public Resource<Shader>
{
public:
    Shader();
    virtual ~Shader();

    bool initFromFile(const std::string& vsPath, const std::string& fsPath, const ShaderLayout& shaderLayout);
    bool initFromFile(const std::string& vsPath, const std::string& fsPath, const ShaderLayout& shaderLayout, const he::ObjectList<std::string>& outputs);
    bool initFromFile(const std::string& vsPath, const std::string& fsPath, const ShaderLayout& shaderLayout, const std::set<std::string>& defines, const he::ObjectList<std::string>& outputs = he::ObjectList<std::string>());
    
    bool initFromMem(const std::string& vs, const std::string& fs, const ShaderLayout& shaderLayout, const std::string& debugVertName, const std::string& debugFragName);
    bool initFromMem(const std::string& vs, const std::string& fs, const ShaderLayout& shaderLayout, const std::string& debugVertName, const std::string& debugFragName, const he::ObjectList<std::string>& outputs);
    bool initFromMem(const std::string& vs, const std::string& fs, const ShaderLayout& shaderLayout, const std::string& debugVertName, const std::string& debugFragName, const std::set<std::string>& defines, const he::ObjectList<std::string>& outputs = he::ObjectList<std::string>());

    void bind();

    uint32 getShaderVarId(const std::string& name) const;
    uint32 getBufferId(const std::string& name) const;
    uint32 getBufferVarId(uint32 bufferId, const std::string& name) const;
    uint32 getShaderSamplerId(const std::string& name);

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

    std::map<std::string, uint32> m_SamplerLocationMap;
    std::map<uint32, UniformBuffer*> m_UniformBufferMap;

    std::string m_FragShaderName;
    std::string m_VertShaderName;

    static uint32 s_CurrentBoundShader;

    //Disable default copy constructor and default assignment operator
    Shader(const Shader&);
    Shader& operator=(const Shader&);
};

} } //end namespace

#endif
