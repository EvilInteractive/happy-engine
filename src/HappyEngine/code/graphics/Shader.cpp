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
#include "HappyPCH.h" 

#include "Shader.h"

#include "HeAssert.h"

#include "FileReader.h"
#include "FileNotFoundException.h"
#include "ShaderPreProcessor.h"

#include <iostream>
#include <algorithm>

#include "OpenGL.h"

namespace he {
namespace gfx {

uint Shader::s_CurrentBoundShader = 0;
uint UniformBuffer::s_UniformBufferCount = 0;

Shader::Shader() : m_Id(0), m_VsId(0), m_FsId(0)
{
}

Shader::~Shader()
{
    glDetachShader(m_Id, m_VsId);
    glDetachShader(m_Id, m_FsId);

    glDeleteShader(m_VsId);
    glDeleteShader(m_FsId);
    glDeleteProgram(m_Id);
}
bool validateShader(GLuint shaderID, const std::string& file)
{
    GLint compiled;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE)
    {
        const uint BUFFER_SIZE = 512;
        char buffer[BUFFER_SIZE];
        GLsizei length = 0;

        glGetShaderInfoLog(shaderID, BUFFER_SIZE, &length, buffer);

        char sId[5];
        sprintf(sId, "%d", shaderID);
        HE_ERROR("Shader " + std::string(sId) + " (" + file + ") compile err: ");
        HE_ERROR(std::string(buffer));
    }
    return compiled == GL_TRUE;
}
bool validateProgram(GLuint programID)
{
    bool succes = true;

    GLint linkStatus;
    glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == GL_FALSE)
    {
        const uint BUFFER_SIZE = 512;
        char buffer[BUFFER_SIZE];
        GLsizei length = 0;
    
        glGetProgramInfoLog(programID, BUFFER_SIZE, &length, buffer);
        if (length > 0)
        {
            char sId[5];
            sprintf(sId, "%d", (int)programID);
            HE_ERROR("Program " + std::string(sId) + "link err:");
            HE_ERROR(std::string(buffer));
        }
        succes = false;
    }

    glValidateProgram(programID);
    GLint validateStatus;
    glGetProgramiv(programID, GL_VALIDATE_STATUS, &validateStatus);
    if (validateStatus == GL_FALSE)
    {
        char sId[5];
        sprintf(sId, "%d", (int)programID);
        HE_ERROR("Error validating shader " + std::string(sId));
        succes = false;
    }

    return succes;
}
bool Shader::initFromFile(const std::string& vsPath, const std::string& fsPath, const ShaderLayout& shaderLayout)
{
    return initFromFile(vsPath, fsPath, shaderLayout, std::set<std::string>(), std::vector<std::string>());
}
bool Shader::initFromFile(const std::string& vsPath, const std::string& fsPath, const ShaderLayout& shaderLayout, const std::vector<std::string>& outputs)
{
    return initFromFile(vsPath, fsPath, shaderLayout, std::set<std::string>(), outputs);
}
bool Shader::initFromFile(const std::string& vsPath, const std::string& fsPath, const ShaderLayout& shaderLayout, const std::set<std::string>& defines, const std::vector<std::string>& outputs)
{
    ASSERT(m_Id != -1, "no need to init twice");
    
    // Read VS and FS files --------------------------->
    io::FileReader reader;

    std::string strVS;
    std::string strFS;
    try 
    {
        reader.open(vsPath, io::FileReader::OpenType_ASCII);
        strVS = reader.readToEnd();
        reader.close();

        reader.open(fsPath, io::FileReader::OpenType_ASCII);
        strFS = reader.readToEnd();
        reader.close();
    }
    catch (const err::FileNotFoundException& e)
    { std::wcout << e.getMsg(); return false; }
    // <-----------------------------------------------

    return initFromMem(strVS, strFS, shaderLayout, vsPath, fsPath, defines, outputs);
}

bool Shader::initFromMem( const std::string& vs, const std::string& fs, const ShaderLayout& shaderLayout, const std::string& debugVertName, const std::string& debugFragName)
{
    return initFromMem(vs, fs, shaderLayout, debugVertName, debugFragName, std::set<std::string>(), std::vector<std::string>());
}
bool Shader::initFromMem( const std::string& vs, const std::string& fs, const ShaderLayout& shaderLayout, const std::string& debugVertName, const std::string& debugFragName , const std::vector<std::string>& outputs)
{
    return initFromMem(vs, fs, shaderLayout, debugVertName, debugFragName, std::set<std::string>(), outputs);
}
bool Shader::initFromMem( const std::string& vs, const std::string& fs, const ShaderLayout& shaderLayout, const std::string& debugVertName, const std::string& debugFragName, const std::set<std::string>& defines, const std::vector<std::string>& outputs)
{
    bool succes = true;

    m_VertShaderName = debugVertName;
    m_FragShaderName = debugFragName;

    std::string vsPost = ct::details::ShaderPreProcessor::process(vs, defines);       
    std::string fsPost = ct::details::ShaderPreProcessor::process(fs, defines);

    m_VsId = glCreateShader(GL_VERTEX_SHADER);
    m_FsId = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vsBuff(vsPost.c_str());
    glShaderSource(m_VsId, 1, &vsBuff, 0);
    glCompileShader(m_VsId);
    succes = succes && validateShader(m_VsId, m_VertShaderName);

    const char* fsBuff(fsPost.c_str());
    glShaderSource(m_FsId, 1, &fsBuff, 0);
    glCompileShader(m_FsId);
    succes = succes && validateShader(m_FsId, m_FragShaderName);

    m_Id = glCreateProgram();
    glAttachShader(m_Id, m_VsId);
    glAttachShader(m_Id, m_FsId);

    const ShaderLayout::layout& layout(shaderLayout.getElements());
    std::for_each(layout.cbegin(), layout.cend(), [&](const ShaderLayoutElement& e)
    {
        glBindAttribLocation(m_Id, e.getElementIndex(), e.getShaderVariableName().c_str());
    });

    for (uint i = 0; i < outputs.size(); ++i)
    {
        glBindFragDataLocation(m_Id, i, outputs[i].c_str());
    }

    glLinkProgram(m_Id);

    succes = succes && validateProgram(m_Id);

    return succes;
}

void Shader::bind()
{
    if (s_CurrentBoundShader != m_Id)
    {
        glUseProgram(m_Id);
        s_CurrentBoundShader = m_Id;
    }
}

uint Shader::getBufferId( const std::string& name ) const
{
    uint loc(glGetUniformBlockIndex(m_Id, name.c_str()));
    if (loc == -1)
    {
        HE_ERROR("Uniform buffer: '" + name + "' not found!");
        HE_ERROR("In shader: " + m_FragShaderName);
    }
    return loc;
}

uint Shader::getShaderVarId(const std::string& name) const
{
    uint loc(glGetUniformLocation(m_Id, name.c_str()));
    if (loc == -1)
    {
        HE_ERROR("Shader var: '" + name + "' not found!");
        HE_ERROR("In shader: " + m_FragShaderName);
    }
    return loc;
}
uint Shader::getShaderSamplerId(const std::string& name)
{
    std::map<std::string, uint>::const_iterator loc(m_SamplerLocationMap.find(name));
    if (loc != m_SamplerLocationMap.cend())
    {
        return loc->second;
    }
    else
    {
        uint texLoc(getShaderVarId(name));
        uint samplerIndex(m_SamplerLocationMap.size());
        bind();
        glUniform1i(texLoc, samplerIndex);
        m_SamplerLocationMap[name] = samplerIndex;
        return samplerIndex;
    }
}

void Shader::setShaderVar(uint id, int value) const
{
    ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    glUniform1i(id, value);
}
void Shader::setShaderVar(uint id, uint value) const
{
    ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    glUniform1ui(id, value);
}
void Shader::setShaderVar(uint id, float value) const
{
    ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    glUniform1f(id, value);
}
void Shader::setShaderVar(uint id, const vec2& vec) const
{
    ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    glUniform2f(id, vec.x, vec.y);
}
void Shader::setShaderVar(uint id, const vec3& vec) const
{
    ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    glUniform3f(id, vec.x, vec.y, vec.z);
}
void Shader::setShaderVar(uint id, const vec4& vec) const
{
    ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    glUniform4f(id, vec.x, vec.y, vec.z, vec.w);
}
void Shader::setShaderVar(uint id, const mat44& matrix) const
{
    ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    glUniformMatrix4fv(id, 1, GL_FALSE, matrix.toFloatArray());
}
void Shader::setShaderVar(uint id, const std::vector<mat44>& matrixArray) const
{
    ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    ASSERT(matrixArray.size() > 0, "there must be at least one matrix in the array");
    glUniformMatrix4fv(id, matrixArray.size(), GL_FALSE, matrixArray[0].toFloatArray());
}
void Shader::setShaderVar(uint id, const gfx::Texture2D::pointer& tex2D) const
{
    ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    GL::heBindTexture2D(id, tex2D->getID());
}
void Shader::setShaderVar( uint id, const gfx::Texture2D* pTex2D ) const
{
    ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    GL::heBindTexture2D(id, pTex2D->getID());
}


void Shader::setBuffer( uint id, const UniformBuffer::pointer& pBuffer )
{
    glUniformBlockBinding(m_Id, id, pBuffer->m_BufferId);
}

UniformBuffer::pointer Shader::setBuffer( uint id )
{
    UniformBuffer::pointer buffer(NEW UniformBuffer(m_Id, id));
    glUniformBlockBinding(m_Id, id, buffer->m_BufferId);
    m_UniformBufferMap[buffer->m_BufferId] = buffer;
    return buffer;
}

} } //end namespace
