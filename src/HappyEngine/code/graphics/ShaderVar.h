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

namespace he {
namespace gfx {
    
enum ShaderVarType
{
    ShaderVarType_WorldViewProjection,
    ShaderVarType_ViewProjection,
    ShaderVarType_View,
    ShaderVarType_World,
    ShaderVarType_WorldView,
    ShaderVarType_WorldPosition,
    ShaderVarType_BoneTransforms,

    ShaderVarType_AmbientColor,
    ShaderVarType_DirectionalColor,
    ShaderVarType_DirectionalDirection,

    ShaderVarType_ShadowCascadeMatrix0,
    ShaderVarType_ShadowCascadeMatrix1,
    ShaderVarType_ShadowCascadeMatrix2,
    ShaderVarType_ShadowCascadeMatrix3,

    ShaderVarType_ShadowCascade0,
    ShaderVarType_ShadowCascade1,
    ShaderVarType_ShadowCascade2,
    ShaderVarType_ShadowCascade3,

    ShaderVarType_User
};
class ShaderVar
{
public:
    ShaderVar(uint id, const std::string& name, ShaderVarType type): m_Id(id), m_Type(type), m_Name(name) {}

    virtual void assignData(const Shader::pointer& pShader) = 0;

    virtual ShaderVarType getType() { return m_Type; }
    uint getId() const { return m_Id; }
    const std::string& getName() const { return m_Name; }

    typedef boost::shared_ptr<ShaderVar> pointer;

protected:
    uint m_Id;
    std::string m_Name;
    ShaderVarType m_Type;
};
class ShaderGlobalVar : public ShaderVar
{
public:
    ShaderGlobalVar(uint id, const std::string& name, ShaderVarType type): ShaderVar(id ,name, type)
    {
    }

    virtual void assignData(const Shader::pointer& /*pShader*/)
    {
    }
};
template<typename T>
class ShaderUserVar : public ShaderVar
{
public:
    ShaderUserVar(uint id, const std::string& name, const T& data): ShaderVar(id, name, ShaderVarType_User), m_Data(data)
    {
    }

    const T& getData() const { return m_Data; }
    void setData(const T& data) { m_Data = data; }

    virtual void assignData(const Shader::pointer& pShader)
    {
        pShader->setShaderVar(m_Id, m_Data);
    }

    typedef boost::shared_ptr<ShaderUserVar<T>> pointer;
private:
    T m_Data;
};
template<>
class ShaderUserVar<const Texture2D*> : public ShaderVar
{
public:
    ShaderUserVar(uint id, const std::string& name, const Texture2D* data): ShaderVar(id, name, ShaderVarType_User), m_Data(data)
    {
        ResourceFactory<Texture2D>::getInstance()->instantiate(m_Data->getHandle());
    }
    virtual ~ShaderUserVar()
    {
        m_Data->release();
    }

    const Texture2D* getData() const { return m_Data; }
    void setData(const Texture2D* data) 
    { 
        m_Data->release();
        m_Data = data; 
        ResourceFactory<Texture2D>::getInstance()->instantiate(m_Data->getHandle());
    }

    virtual void assignData(const Shader::pointer& pShader)
    {
        pShader->setShaderVar(m_Id, m_Data);
    }

    typedef boost::shared_ptr<ShaderUserVar<const Texture2D*>> pointer;
private:
    const Texture2D* m_Data;
};

} } //end namespace

#endif
