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

#include "BufferLayout.h"

namespace he {
namespace gfx {

class ShaderLayoutElement
{
public:
    ShaderLayoutElement(): m_ElementIndex(UINT32_MAX), m_Usage(BufferElement::Usage_Other), m_NameInShader("ERROR") {}
    ShaderLayoutElement(const he::String& nameInShader, const BufferElement::Usage usage);
    ~ShaderLayoutElement() {}
    //default copy constructor and assignment operator are fine

    uint32 getElementIndex() const { return m_ElementIndex; }
    void setElementIndex(const uint32 index) { m_ElementIndex = index; }

    const he::String& getShaderVariableName() const { return m_NameInShader; }
    const BufferElement::Usage getUsage() const { return m_Usage; }

private:
    uint32 m_ElementIndex;
    BufferElement::Usage m_Usage;
    he::String m_NameInShader;
};

class ShaderLayout
{
public:
    ShaderLayout();
    ~ShaderLayout();
    
    typedef he::ObjectList<ShaderLayoutElement> layout;

    void addElement(const ShaderLayoutElement& element);

    const layout& getElements() const;

private:
    layout m_Layout;

    ShaderLayout(ShaderLayout&);
    ShaderLayout& operator=(const ShaderLayout&);
};

} } //end namespace

#endif
