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

#include "Shader.h"

#include "GL/glew.h"
#include "FileReader.h"
#include "FileOpenException.h"
#include <iostream>
#include <algorithm>
#include "Assert.h"

namespace happyengine {
namespace graphics {

Shader::Shader() : m_Id(0), m_VsId(0), m_FsId(0), m_Bound(false)
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
    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    GLsizei length = 0;

    glGetShaderInfoLog(shaderID, BUFFER_SIZE, &length, buffer);

    if (length > 0)
    {
        std::cout << "Shader " << shaderID << " (" << file << ") compile error: \n" <<
            buffer << "\n";
        return false;
    }
    return true;
}
bool validateProgram(GLuint programID)
{
    bool succes = true;

    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    GLsizei length = 0;

    glGetProgramInfoLog(programID, BUFFER_SIZE, &length, buffer);
    if (length > 0)
    {
        std::cout << "Program " << programID << "link error: \n" <<
            buffer << "\n";
        succes = false;
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
bool Shader::init(const std::string& vsPath, const std::string& fsPath, const VertexLayout& vertexlayout)
{
    bool succes = true;
    //ASSERT(m_Id != -1, "no need to init twice");

    // Read VS and FS files --------------------------->
    io::FileReader reader;
    std::string strVS;
    std::string strFS;
    try 
    {
        reader.open(vsPath, io::FileReader::OpenType_ASCII);
        strVS = reader.readToEnd();
        reader.close();

        reader.open(fsPath,  io::FileReader::OpenType_ASCII);
        strFS = reader.readToEnd();
        reader.close();
    }
    catch (const error::FileOpenException& /*e*/)
    { /*PANIC(e.getMsg());*/ return false; }
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

    const VertexLayout::layout& layout(vertexlayout.getElements());
    std::for_each(layout.cbegin(), layout.cend(), [&](VertexElement e)
    {
        glBindAttribLocation(m_Id, e.getElementIndex(), e.getShaderVariableName().c_str());
    });

    glLinkProgram(m_Id);
    
    succes = succes && validateProgram(m_Id);

    return succes;
}

void Shader::begin()
{
    glUseProgram(m_Id);
    m_Bound = true;
}
void Shader::end()
{
    glUseProgram(0);
    m_Bound = false;
}

unsigned int Shader::getShaderVarId(const std::string& name) const
{
    return glGetUniformLocation(m_Id, name.c_str());
}

void Shader::setShaderVar(unsigned int id, const math::Matrix& matrix) const
{
    ASSERT(m_Bound, "shader must be bound before using setShaderVar(...)");
    float fArr[16];
    matrix.toFloatArray(fArr);
    glUniformMatrix4fv(id, 1, GL_FALSE, fArr);
}
void Shader::setShaderVar(unsigned int id, const math::Vector3& vec) const
{
    ASSERT(m_Bound, "shader must be bound before using setShaderVar(...)");
    glUniform3f(id, vec.x, vec.y, vec.z);
}

} } //end namespace
