//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Author:  Bastian Damman
//Created: 2013/09/28

#ifndef _HE_ShaderUniformBuffer_H_
#define _HE_ShaderUniformBuffer_H_
#pragma once

namespace he {
namespace gfx {

#define UNIFORM_STRUCT(name) struct name {
#define UNIFORM_VEC3(var) vec3 var; float __##pad_##var;
#define UNIFORM_MAT4(var) mat44 var;
#define UNIFORM_VEC4(var) vec4 var;
#define UNIFORM_VEC2_VEC2(var1, var2) vec2 var1; vec2 var2;
#define UNIFORM_VEC2_FLOAT_FLOAT(var1, var2, var3) vec2 var1; float var2; float var3;
#define UNIFORM_FLOAT_FLOAT_FLOAT_FLOAT(var1, var2, var3, var4) vec2 var1; float var2; float var3; float var4;
#define UNIFORM_STRUCT_END(name) }; HE_COMPILE_ASSERT(sizeof(name) % 16 == 0, "Uniform struct '" STR(name) "' is not 16byte aligned!");
// EVAL(CONCAT(CONCAT("Uniform struct ", STR(name)), " is not 16byte aligned!"))

template<typename T>
class SharedUniformBuffer
{
public:
    SharedUniformBuffer(): m_BufferId(UINT32_MAX), m_GlBuffer(0)
    {
        HE_COMPILE_ASSERT(sizeof(T) % 16 == 0, "Uniform struct is not 16byte aligned!");
    }
    ~SharedUniformBuffer()
    {
        if (m_BufferId != UINT32_MAX)
        {
            glDeleteBuffers(1, &m_GlBuffer);
        }
    }

    void init(const size_t id, const T& defaultValues)
    {
        m_BufferId = id;
        glGenBuffers(1, &m_GlBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, m_GlBuffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(T), &defaultValues, GL_STREAM_DRAW);
    }

    void updateData(const T& data)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_GlBuffer);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &data);
    }

    uint32 getBufferID() const
    {
        return m_BufferId;
    }

    void bindBuffer() const
    {
        GL::heBindUniformBuffer(m_GlBuffer, m_BufferId);
    }

private:
    uint32 m_GlBuffer;
    uint32 m_BufferId;
};

} } //end namespace

#endif
