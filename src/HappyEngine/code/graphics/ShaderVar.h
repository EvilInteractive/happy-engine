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
//Author:  Bastian Damman
//Created: 30/09/2011

#ifndef _HE_SHADERVAR_H_
#define _HE_SHADERVAR_H_
#pragma once

#include "Shader.h"
#include "Texture2D.h"
#include "TextureCube.h"

namespace he {
namespace gfx {

enum EShaderVariableType
{
    eShaderVariableType_ObjectProperty,
    eShaderVariableType_GlobalProperty,
    eShaderVariableType_UserProperty,

    eShaderVariableType_Invalid
};

enum EShaderAttributePropertyType
{   // Only add to the back, or you will corrupt save files!
    eShaderAttributePropertyType_Position,
    eShaderAttributePropertyType_TextureCoordiante,
    eShaderAttributePropertyType_Normal,
    eShaderAttributePropertyType_Tangent,
    eShaderAttributePropertyType_Color,
    eShaderAttributePropertyType_BoneIndices,
    eShaderAttributePropertyType_BoneWeights
};

enum EShaderObjectPropertyType
{   // Only add to the back, or you will corrupt save files!
    eShaderObjectPropertyType_WorldViewProjection,
    eShaderObjectPropertyType_World,
    eShaderObjectPropertyType_WorldView,
    eShaderObjectPropertyType_WorldPosition,
    eShaderObjectPropertyType_BoneTransforms
};  

enum EShaderGlobalPropertyType
{   // Only add to the back, or you will corrupt save files!
    eShaderGlobalPropertyType_ViewProjection,
    eShaderGlobalPropertyType_View,
    eShaderGlobalPropertyType_AmbientColor,
    eShaderGlobalPropertyType_DirectionalColor,
    eShaderGlobalPropertyType_DirectionalDirection,
    eShaderGlobalPropertyType_NearFar
};

enum EShaderUserPropertyType
{   // Only add to the back, or you will corrupt save files!
    eShaderUserPropertyType_UserFloat,
    eShaderUserPropertyType_UserFloat2,
    eShaderUserPropertyType_UserFloat3,
    eShaderUserPropertyType_UserFloat4,
    eShaderUserPropertyType_Texture1D,
    eShaderUserPropertyType_Texture2D,
    eShaderUserPropertyType_TextureCube,
};

class ShaderVar
{
public:
    virtual ~ShaderVar() {}

    virtual ShaderVar* copy() = 0;
    virtual void apply(Shader* const shader) = 0;
    virtual EShaderVariableType getType() = 0;

    const uint32 getId() const { return m_Id; }
    const he::FixedString& getName() const { return m_Name; }

protected:
    explicit ShaderVar(const he::FixedString& name): m_Dirty(false), m_Id(UINT8_MAX), m_Name(name) {}

    bool m_Dirty;
    uint8 m_Id;
    he::FixedString m_Name;
};

class ShaderGlobalVar : public ShaderVar
{
public:
    ShaderGlobalVar(const he::FixedString& name, const EShaderGlobalPropertyType type): ShaderVar(name)
    {
    }
    ~ShaderGlobalVar() {}

    void apply(Shader* const shader)
    {
        if (m_Dirty)
        {

        }
    }

    ShaderVar* copy()
    {
        return NEW ShaderGlobalVar(m_Id, m_Name, m_Type);
    }
};

template<typename T>
class ShaderUserVar : public ShaderVar
{
public:
    ShaderUserVar(uint32 id, const he::FixedString& name, const T& data): ShaderVar(id, name, ShaderVarType_User), m_Data(data)
    {
    }
    ~ShaderUserVar() {}

    const T& getData() const { return m_Data; }
    void setData(const T& data) { m_Data = data; }

    void assignData(Shader* shader)
    {
        shader->setShaderVar(m_Id, m_Data);
    }

    ShaderVar* copy()
    {
        return NEW ShaderUserVar<T>(m_Id, m_Name, m_Data);
    }

private:
    T m_Data;
};
template<>
class ShaderUserVar<const Texture2D*> : public ShaderVar
{
public:
    ShaderUserVar(uint32 id, const he::FixedString& name, const Texture2D* data): ShaderVar(id, name, ShaderVarType_User), m_Data(data)
    {
        m_Data->instantiate();
    }
    ~ShaderUserVar()
    {
        m_Data->release();
    }

    const Texture2D* getData() const { return m_Data; }
    void setData(const Texture2D* data) 
    { 
        m_Data->release();
        m_Data = data; 
        m_Data->instantiate();
    }

    void assignData(Shader* shader)
    {
        shader->setShaderVar(m_Id, m_Data);
    }

    ShaderVar* copy()
    {
        return NEW ShaderUserVar<const Texture2D*>(m_Id, m_Name, m_Data);
    }

private:
    const Texture2D* m_Data;
};
template<>
class ShaderUserVar<const TextureCube*> : public ShaderVar
{
public:
    ShaderUserVar(uint32 id, const he::FixedString& name, const TextureCube* data): ShaderVar(id, name, ShaderVarType_User), m_Data(data)
    {
        m_Data->instantiate();
    }
    ~ShaderUserVar()
    {
        m_Data->release();
    }

    const TextureCube* getData() const { return m_Data; }
    void setData(const TextureCube* data) 
    { 
        m_Data->release();
        m_Data = data; 
        m_Data->instantiate();
    }

    void assignData(Shader* shader)
    {
        shader->setShaderVar(m_Id, m_Data);
    }

    ShaderVar* copy()
    {
        return NEW ShaderUserVar<const TextureCube*>(m_Id, m_Name, m_Data);
    }

private:
    const TextureCube* m_Data;
};

} } //end namespace

#endif
