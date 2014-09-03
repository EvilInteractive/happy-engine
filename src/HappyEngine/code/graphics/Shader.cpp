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
#include "HappyPCH.h"

#include "Shader.h"

#include "FileReader.h"
#include "GraphicsEngine.h"
#include "ExternalError.h"
#include "GlobalStringTable.h"

#include "ShaderPreProcessor.h"
#include "ShaderUniform.h"
#include "ShaderUniformFactory.h"
#include "ShaderUniformBufferManager.h"


namespace 
{
    bool validateShader(GLuint shaderID, const he::String& file)
    {
        GLint compiled;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_FALSE)
        {
            const he::uint32 BUFFER_SIZE = 512;
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
            const he::uint32 BUFFER_SIZE = 512;
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
            const he::uint32 BUFFER_SIZE(512);
            char buffer[BUFFER_SIZE];
            he_memset(buffer, 0, BUFFER_SIZE);
            GLsizei length(0);
        
            glGetProgramInfoLog(programID, BUFFER_SIZE - 1, &length, buffer);   
        
            HE_ERROR("Error validating shader %d\n%s", programID, buffer);
            succes = false;
        }

        return succes;
    }
}

namespace he {
namespace gfx {

uint32 Shader::s_CurrentBoundShader = 0;

Shader::Shader() : m_Id(0), m_VsId(0), m_FsId(0)
{
}

Shader::~Shader()
{
    size_t unitformCount(m_Uniforms.size());
    for (size_t i(0); i < unitformCount; ++i)
    {
        ShaderUniformFactory::destroy(m_Uniforms[i]);
    }
    
    glDetachShader(m_Id, m_VsId);
    glDetachShader(m_Id, m_FsId);

    glDeleteShader(m_VsId);
    glDeleteShader(m_FsId);
    glDeleteProgram(m_Id);
}

bool Shader::initFromFile(const he::String& vsPath, const he::String& fsPath, const he::ObjectList<he::String>* const defines, const he::ObjectList<he::String>* const outputLayout /*= nullptr*/)
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

    return initFromMem(strVS, strFS, vsPath, fsPath, defines, outputLayout);
}

bool Shader::initFromMem( const he::String& vs, const he::String& fs, const he::String& debugVertName, const he::String& debugFragName, const he::ObjectList<he::String>* const defines /*= nullptr*/, const he::ObjectList<he::String>* const outputLayout /*= nullptr*/)
{
    bool succes = true;

    setName(debugVertName+"/"+debugFragName);

    m_VertShaderName = debugVertName;
    m_FragShaderName = debugFragName;

    he::ObjectList<he::String> shaderDefines;

    if (defines)
        shaderDefines.append(*defines);
    he::String vsPost = ct::details::ShaderPreProcessor::process(vs, shaderDefines);

    shaderDefines.clear();

    if (defines)
        shaderDefines.append(*defines);
    he::String fsPost = ct::details::ShaderPreProcessor::process(fs, shaderDefines);

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
    
    if (outputLayout)
    {
        GLuint index(0);
        outputLayout->forEach([this, &index](const he::String& output)
        {
            glBindFragDataLocation(m_Id, GL_COLOR_ATTACHMENT0 + index++, output.c_str());
        });
    }

    glLinkProgram(m_Id);

    succes &= validateProgram(m_Id);

    bind();

    GlobalStringTable* const stringTable(GlobalStringTable::getInstance());
    
    GLint attribCount(-1);
    glGetProgramiv( m_Id, GL_ACTIVE_ATTRIBUTES, &attribCount );
    for (GLint i(0); i < attribCount; ++i)
    {
        GLint nameLen(-1);
        GLint size(0);
        GLenum type(GL_ZERO);
        GLchar name[100];
        glGetActiveAttrib(m_Id, i, 99, &nameLen, &size, &type, name);
        name[nameLen] = '\0';
        const GLuint location(glGetAttribLocation( m_Id, name ));
        if (location != -1)
        {
            const FixedString fixedName(stringTable->add(name, nameLen));

            he::toLower(name);

            EShaderAttribute usage(eShaderAttribute_Invalid);
            if (strstr(name, "pos") != 0)
                usage = eShaderAttribute_Position;
            else if (strstr(name, "tex") != 0)
                usage = eShaderAttribute_TextureCoordiante;
            else if (strstr(name, "nor") != 0)
                usage = eShaderAttribute_Normal;
            else if (strstr(name, "tan") != 0)
                usage = eShaderAttribute_Tangent;
            else if (strstr(name, "col") != 0)
                usage = eShaderAttribute_Color;
            else if (strstr(name, "id") != 0)
                usage = eShaderAttribute_BoneIndices;
            else if (strstr(name, "wei") != 0)
                usage = eShaderAttribute_BoneWeights;
            else
            {
                LOG(he::LogType_ProgrammerAssert, "Could not deduce shader attribute usage from name '%s'", fixedName.c_str());
                continue;
            }

            const ShaderLayoutAttribute el(fixedName, usage, location);
            m_Layout.addAttribute(el);
        }
        else
        {
            LOG(he::LogType_ProgrammerAssert, "Could not bind shader attribute: %s to shader: %s", name, debugVertName.c_str());
        }
    }

    GLint uniformBlockCount(-1);
    glGetProgramiv( m_Id, GL_ACTIVE_UNIFORM_BLOCKS, &uniformBlockCount );
    for(GLint i(0); i < uniformBlockCount; ++i)  
    {
        GLchar buffer[100];
        GLsizei size(0);
        glGetActiveUniformBlockName(m_Id, i, 99, &size, buffer);
        const he::FixedString uboName(he::GlobalStringTable::getInstance()->add(buffer, size));

        const GLuint location(glGetUniformBlockIndex(m_Id, uboName.c_str()));
        ShaderUniformBufferManager* uboMan(GRAPHICS->getShaderUniformBufferManager());
        uint32 uboLink(uboMan->findLink(uboName));
        if (uboLink != UINT32_MAX)
        {
            glUniformBlockBinding(m_Id, location, uboLink);
        }
    }

    GLint uniformCount(-1);
    glGetProgramiv( m_Id, GL_ACTIVE_UNIFORMS, &uniformCount ); 
    size_t samplers(0);
    for(GLint i(0); i < uniformCount; ++i)  
    {
        GLint nameLen(-1);
        GLint num(1);
        GLenum type(GL_ZERO);
        GLchar name[100];
        glGetActiveUniform( m_Id, i, 99, &nameLen, &num, &type, name );
        name[nameLen] = '\0'; // Add null terminator
        const GLuint location(glGetUniformLocation( m_Id, name ));
        if (location != -1) // Can be -1 if it is inside an UBO!
        {
            const he::FixedString fsName(he::GlobalStringTable::getInstance()->add(name, nameLen));
            IShaderUniform* uniform(nullptr);
            if (num == 1)
            {
                switch (type)
                {
                case GL_FLOAT: uniform = ShaderUniformFactory::create(eShaderUniformType_Float, fsName, location); break;
                case GL_FLOAT_VEC2: uniform = ShaderUniformFactory::create(eShaderUniformType_Float2, fsName, location); break;
                case GL_FLOAT_VEC3: uniform = ShaderUniformFactory::create(eShaderUniformType_Float3, fsName, location); break;
                case GL_FLOAT_VEC4: uniform = ShaderUniformFactory::create(eShaderUniformType_Float4, fsName, location); break;
                case GL_INT: uniform = ShaderUniformFactory::create(eShaderUniformType_Int, fsName, location); break;
                case GL_UNSIGNED_INT: uniform = ShaderUniformFactory::create(eShaderUniformType_UInt, fsName, location); break;
                case GL_FLOAT_MAT4: uniform = ShaderUniformFactory::create(eShaderUniformType_Mat44, fsName, location); break;
                case GL_SAMPLER_2D: 
                {
                    glUniform1i(location, samplers);
                    uniform = ShaderUniformFactory::create(eShaderUniformType_Texture2D, fsName, samplers++);
                } break;
                case GL_SAMPLER_CUBE: 
                {
                    glUniform1i(location, samplers);
                    uniform = ShaderUniformFactory::create(eShaderUniformType_TextureCube, fsName, samplers++); 
                } break;
                default: LOG(LogType_ProgrammerAssert, "Unsupported shader uniform type %d (%s)", type, fsName.c_str());
                }
            }
            else
            {
                HE_ASSERT(num > 1, "GL returned a uniform with an array size of 0?");
                switch (type)
                {
                case GL_FLOAT_MAT4: uniform = ShaderUniformFactory::create(eShaderUniformType_Mat44Array, fsName, location); break;
                default: LOG(LogType_ProgrammerAssert, "Unsupported shader uniform array type %d", type);
                }
            }
            if (nullptr != uniform)
            {
                uniform->init(this);
                m_Uniforms.add(uniform);
            }
        }
    }

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
void Shader::setSampler2D(uint32 id, const uint32 sampler) const
{
    HE_ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    // HE_ASSERT(sampler != UINT32_MAX, "Binding unassigned texture!");
    GL::heBindTexture2D(id, sampler);
}
void Shader::setSamplerCube( uint32 id, const uint32 sampler ) const
{
    HE_ASSERT(s_CurrentBoundShader == m_Id, "shader must be bound before using setShaderVar(...)");
    GL::heBindTextureCube(id, sampler);
}

ShaderUniformID Shader::getUniformID( const he::FixedString& name ) const
{
    ShaderUniformID id;
    size_t unitformCount(m_Uniforms.size());
    for (size_t i(0); i < unitformCount; ++i)
    {
        if (m_Uniforms[i]->getName() == name)
        {
            id.m_ID = i;
            break;
        }
    }
    return id;
}

IShaderUniform* Shader::getUniform( const ShaderUniformID id ) const
{
    return m_Uniforms[id.m_ID];
}

he::int8 Shader::getOutputIndex( const he::FixedString& outputName ) const
{
    return checked_numcast<int8>(glGetFragDataLocation(m_Id, outputName.c_str()));
}

} } //end namespace
