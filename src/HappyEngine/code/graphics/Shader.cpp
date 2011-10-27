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
#include "StdAfx.h" 

#include "Shader.h"

#include "Assert.h"

#include "FileReader.h"
#include "FileNotFoundException.h"
#include "ShaderPreProcessor.h"

#include <iostream>
#include <algorithm>

#include "OpenGL.h"

namespace he {
namespace gfx {

uint Shader::s_CurrentBoundShader = 0;

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
    const uint BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    GLsizei length = 0;

    glGetShaderInfoLog(shaderID, BUFFER_SIZE, &length, buffer);

    if (length > 0)
    {
        std::cout << "Shader " << shaderID << " (" << file << ") compile err: \n" <<
            buffer << "\n";
    }
    return true;
}
bool validateProgram(GLuint programID)
{
    bool succes = true;

    const uint BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    GLsizei length = 0;

    glGetProgramInfoLog(programID, BUFFER_SIZE, &length, buffer);
    if (length > 0)
    {
        std::cout << "Program " << programID << "link err: \n" <<
            buffer << "\n";
    }

    glValidateProgram(programID);
    GLint status;
    glGetProgramiv(programID, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE)
    {
        std::cout << "Error validating shader " << programID << "\n";
        succes = false;
    }

    return succes;
}
bool Shader::init(const std::string& vsPath, const std::string& fsPath, const ShaderLayout& shaderLayout)
{
    return init(vsPath, fsPath, shaderLayout, std::set<std::string>(), std::vector<std::string>());
}
bool Shader::init(const std::string& vsPath, const std::string& fsPath, const ShaderLayout& shaderLayout, const std::vector<std::string>& outputs)
{
    return init(vsPath, fsPath, shaderLayout, std::set<std::string>(), outputs);
}
bool Shader::init(const std::string& vsPath, const std::string& fsPath, const ShaderLayout& shaderLayout, const std::set<std::string>& defines, const std::vector<std::string>& outputs)
{
    ASSERT(m_Id != -1, "no need to init twice");
    bool succes = true;
    
    m_FragShaderName = fsPath;

    // Read VS and FS files --------------------------->
    io::FileReader reader;
    std::string strVS;
    std::string strFS;
    try 
    {
        strVS = ct::details::ShaderPreProcessor::process(vsPath, defines);       
        strFS = ct::details::ShaderPreProcessor::process(fsPath, defines);
    }
    catch (const err::FileNotFoundException& e)
    { std::wcout << e.getMsg(); return false; }
    // <-----------------------------------------------

    m_VsId = glCreateShader(GL_VERTEX_SHADER);
    m_FsId = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vsBuff(strVS.c_str());
    glShaderSource(m_VsId, 1, &vsBuff, 0);
    glCompileShader(m_VsId);
    succes = succes && validateShader(m_VsId, vsPath);

    const char* fsBuff(strFS.c_str());
    glShaderSource(m_FsId, 1, &fsBuff, 0);
    glCompileShader(m_FsId);
    succes = succes && validateShader(m_FsId, fsPath);

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

uint Shader::getShaderVarId(const std::string& name) const
{
    uint loc(glGetUniformLocation(m_Id, name.c_str()));
    if (loc == -1)
        std::cout << "shader var: '" << name << "' not found!\n" << "in shader: "<< m_FragShaderName << "\n";
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
void Shader::setShaderVar(uint id, const gfx::Texture2D::pointer& tex2D) const
{
    ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    GL::heBindTexture2D(id, tex2D->getID());
}

} } //end namespace
