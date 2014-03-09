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
//Author: Bastian Damman
//Created: 01/10/2011

#ifndef _HE_ShaderLayout_H_
#define _HE_ShaderLayout_H_
#pragma once

#include "VertexLayout.h"
#include "ShaderEnums.h"

namespace he {
namespace gfx {

class ShaderLayoutAttribute
{
public:
    ShaderLayoutAttribute(): m_ElementIndex(UINT32_MAX), m_Type(eShaderAttribute_Invalid), m_Name(HEFS::strError) {}
    ShaderLayoutAttribute(const he::FixedString& nameInShader, const EShaderAttribute type, const uint32 index);
    ~ShaderLayoutAttribute() {}
    //default copy constructor and assignment operator are fine


    uint32 getElementIndex() const { return m_ElementIndex; }
    const he::FixedString& getName() const { return m_Name; }
    const EShaderAttribute getType() const { return m_Type; }

private:
    uint32 m_ElementIndex;
    EShaderAttribute m_Type;
    he::FixedString m_Name;
};

class ShaderLayoutUniform
{
public:
    ShaderLayoutUniform(): m_Usage(eShaderUniformUsage_Invalid), m_Name(HEFS::strError) {}
    ShaderLayoutUniform(const he::String& nameInShader, const EShaderUniformUsage usage);
    ~ShaderLayoutUniform() {}
    //default copy constructor and assignment operator are fine
    
    const he::FixedString& getName() const { return m_Name; }
    const EShaderUniformUsage getUsage() const { return m_Usage; }

private:
    uint32 m_ElementIndex;
    EShaderUniformUsage m_Usage;
    he::FixedString m_Name;
};

class ShaderLayout
{
public:
    ShaderLayout();
    ~ShaderLayout();
    
    typedef he::ObjectList<ShaderLayoutAttribute> AttributeLayoutList;
    typedef he::ObjectList<ShaderLayoutUniform> UniformLayoutList;

    void addAttribute(const ShaderLayoutAttribute& element);
    void addUniform(const ShaderLayoutUniform& element);

    const AttributeLayoutList& getAttributes() const { return m_Attributes; }
    const UniformLayoutList& getUniforms() const { return m_Uniforms; }

private:
    AttributeLayoutList m_Attributes;
    UniformLayoutList m_Uniforms;

    ShaderLayout(ShaderLayout&);
    ShaderLayout& operator=(const ShaderLayout&);
};

} } //end namespace

#endif
