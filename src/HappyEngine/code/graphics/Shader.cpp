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

#include "FileReader.h"
#include "ShaderPreProcessor.h"

#include "Texture2D.h"
#include "TextureCube.h"
#include "ExternalError.h"

namespace he {
namespace gfx {

uint32 Shader::s_CurrentBoundShader = 0;
uint32 UniformBuffer::s_UniformBufferCount = 0;

Shader::Shader() : m_Id(0), m_VsId(0), m_FsId(0)
{
}

Shader::~Shader()
{
    std::for_each(m_UniformBufferMap.cbegin(), m_UniformBufferMap.cend(), [](const std::pair<uint32, UniformBuffer*>& p)
    {
        delete p.second;
    });

    glDetachShader(m_Id, m_VsId);
    glDetachShader(m_Id, m_FsId);

    glDeleteShader(m_VsId);
    glDeleteShader(m_FsId);
    glDeleteProgram(m_Id);
}
bool validateShader(GLuint shaderID, const he::String& file)
{
    GLint compiled;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE)
    {
        const uint32 BUFFER_SIZE = 512;
        char buffer[BUFFER_SIZE];
        GLsizei length = 0;

        glGetShaderInfoLog(shaderID, BUFFER_SIZE, &length, buffer);

        HE_ERROR("Shader %d(%s) compile err:", shaderID, file.c_str());
        HE_ERROR(buffer);
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
        const uint32 BUFFER_SIZE = 512;
        char buffer[BUFFER_SIZE];
        GLsizei length = 0;

        glGetProgramInfoLog(programID, BUFFER_SIZE, &length, buffer);
        if (length > 0)
        {
            HE_ERROR("Program %d link err:", (int)programID);
            HE_ERROR(buffer);
        }
        succes = false;
    }

    glValidateProgram(programID);
    GLint validateStatus;
    glGetProgramiv(programID, GL_VALIDATE_STATUS, &validateStatus);
    if (validateStatus == GL_FALSE)
    {
        HE_ERROR("Error validating shader %d", programID);
        succes = false;
    }

    return succes;
}
bool Shader::initFromFile(const he::String& vsPath, const he::String& fsPath, const ShaderLayout& shaderLayout)
{
    return initFromFile(vsPath, fsPath, shaderLayout, std::set<he::String>(), he::ObjectList<he::String>());
}
bool Shader::initFromFile(const he::String& vsPath, const he::String& fsPath, const ShaderLayout& shaderLayout, const he::ObjectList<he::String>& outputs)
{
    return initFromFile(vsPath, fsPath, shaderLayout, std::set<he::String>(), outputs);
}
bool Shader::initFromFile(const he::String& vsPath, const he::String& fsPath, const ShaderLayout& shaderLayout, const std::set<he::String>& defines, const he::ObjectList<he::String>& outputs)
{
    HE_ASSERT(m_Id != -1, "no need to init twice");

    // Read VS and FS files --------------------------->
    io::FileReader reader;

    he::String strVS;
    he::String strFS;
    if (reader.open(vsPath, io::FileReader::OpenType_ASCII))
    {
        strVS = reader.readToEnd();
        reader.close();
    }
    else
    {
        HE_ERROR("Error reading: %s", vsPath.c_str());
        return false;
    }
    if (reader.open(fsPath, io::FileReader::OpenType_ASCII))
    {
        strFS = reader.readToEnd();
        reader.close();
    }
    else
    {
        HE_ERROR("Error reading: %s", fsPath.c_str());
        return false;
    }
    // <-----------------------------------------------

    return initFromMem(strVS, strFS, shaderLayout, vsPath, fsPath, defines, outputs);
}

bool Shader::initFromMem( const he::String& vs, const he::String& fs, const ShaderLayout& shaderLayout, const he::String& debugVertName, const he::String& debugFragName)
{
    return initFromMem(vs, fs, shaderLayout, debugVertName, debugFragName, std::set<he::String>(), he::ObjectList<he::String>());
}
bool Shader::initFromMem( const he::String& vs, const he::String& fs, const ShaderLayout& shaderLayout, const he::String& debugVertName, const he::String& debugFragName , const he::ObjectList<he::String>& outputs)
{
    return initFromMem(vs, fs, shaderLayout, debugVertName, debugFragName, std::set<he::String>(), outputs);
}
bool Shader::initFromMem( const he::String& vs, const he::String& fs, const ShaderLayout& shaderLayout, const he::String& debugVertName, const he::String& debugFragName, const std::set<he::String>& defines, const he::ObjectList<he::String>& outputs)
{
    bool succes = true;

    setName(debugFragName);

    m_VertShaderName = debugVertName;
    m_FragShaderName = debugFragName;

    he::String vsPost = ct::details::ShaderPreProcessor::process(vs, defines);
    he::String fsPost = ct::details::ShaderPreProcessor::process(fs, defines);

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

    for (uint32 i = 0; i < outputs.size(); ++i)
    {
        glBindFragDataLocation(m_Id, i, outputs[i].c_str());
    }

    glLinkProgram(m_Id);

    succes = succes && validateProgram(m_Id);

    const ShaderLayout::layout& layout(shaderLayout.getElements());
    std::for_each(layout.cbegin(), layout.cend(), [&](const ShaderLayoutElement& e)
    {
        const GLint loc(glGetAttribLocation(m_Id, e.getShaderVariableName().c_str()));
        if (loc != -1)
        {
            ShaderLayoutElement el(e);
            el.setElementIndex(checked_numcast<uint32>(loc));
            m_Layout.addElement(el);
        }
        else
        {
            LOG(LogType_ProgrammerAssert, "Could not bind shader attribute: %s to shader: %s", e.getShaderVariableName().c_str(), debugVertName.c_str());
        }
    });

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

uint32 Shader::getBufferId( const he::FixedString& id ) const
{
    uint32 loc(glGetUniformBlockIndex(m_Id, id.c_str()));
    if (loc == -1)
    {
        HE_ERROR("Uniform buffer: '%s' not found!",  id.c_str());
        HE_ERROR("In shader: %s", m_FragShaderName.c_str());
    }
    return loc;
}

uint32 Shader::getShaderVarId(const he::FixedString& id) const
{
    uint32 loc(glGetUniformLocation(m_Id, id.c_str()));
    if (loc == -1)
    {
        HE_ERROR("Shader var: '%s' not found!", id.c_str());
        HE_ERROR("In shader: %s", m_FragShaderName.c_str());
    }
    return loc;
}
uint32 Shader::getShaderSamplerId(const he::FixedString& id)
{
    he::FixedStringMap<uint32>::const_iterator loc(m_SamplerLocationMap.find(id));
    if (loc != m_SamplerLocationMap.cend())
    {
        return loc->second;
    }
    else
    {
        const uint32 texLoc(getShaderVarId(id));
        const uint32 samplerIndex(static_cast<uint32>(m_SamplerLocationMap.size()));
        bind();
        glUniform1i(texLoc, samplerIndex);
        m_SamplerLocationMap[id] = samplerIndex;
        return samplerIndex;
    }
}

void Shader::setShaderVar(uint32 id, const int value) const
{
    HE_ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    glUniform1i(id, value);
}
void Shader::setShaderVar(uint32 id, const uint32 value) const
{
    HE_ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    glUniform1ui(id, value);
}
void Shader::setShaderVar(uint32 id, const float value) const
{
    HE_ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    glUniform1f(id, value);
}
void Shader::setShaderVar(uint32 id, const vec2& vec) const
{
    HE_ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    glUniform2f(id, vec.x, vec.y);
}
void Shader::setShaderVar(uint32 id, const vec3& vec) const
{
    HE_ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    glUniform3f(id, vec.x, vec.y, vec.z);
}
void Shader::setShaderVar(uint32 id, const vec4& vec) const
{
    HE_ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    glUniform4f(id, vec.x, vec.y, vec.z, vec.w);
}
void Shader::setShaderVar(uint32 id, const mat44& matrix) const
{
    HE_ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    glUniformMatrix4fv(id, 1, GL_FALSE, matrix.toFloatArray());
}
void Shader::setShaderVar(uint32 id, const he::PrimitiveList<mat44>& matrixArray) const
{
    HE_ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    HE_ASSERT(matrixArray.size() > 0, "there must be at least one matrix in the array");
    glUniformMatrix4fv(id, static_cast<uint32>(matrixArray.size()), GL_FALSE, matrixArray[0].toFloatArray());
}
void Shader::setShaderVar(uint32 id, const gfx::Texture2D* tex2D) const
{
    HE_ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    GL::heBindTexture2D(id, tex2D->getID());
}
void Shader::setShaderVar( uint32 id, const gfx::TextureCube* texCube ) const
{
    HE_ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    GL::heBindTextureCube(id, texCube->getID());
}

void Shader::setBuffer( uint32 id, UniformBuffer* buffer )
{
    glUniformBlockBinding(m_Id, id, buffer->m_BufferId);
}

UniformBuffer* Shader::setBuffer( uint32 id )
{
    int blockSize;
    glGetActiveUniformBlockiv(m_Id, id, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
    UniformBuffer* buffer(NEW UniformBuffer(blockSize));
    glUniformBlockBinding(m_Id, id, buffer->m_BufferId);
    m_UniformBufferMap[buffer->m_BufferId] = buffer;
    return buffer;
}

} } //end namespace
