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
//Created: 2014/03/07

#ifndef _HE_VERTEXLAYOUT_H_
#define _HE_VERTEXLAYOUT_H_
#pragma once

#include "ShaderEnums.h"

namespace he {
namespace gfx {

class HAPPY_ENTRY VertexElement
{
public:
    VertexElement();
    /**
     * Create a VertexElement
     * @param attrib  Attribute to link the element with
     * @param type  The type of the element
     * @param components  The amount of components of type the element has (float3: type = Float, components = 3)
     * @param byteOffset  The offset between two VertexElements
    */
    VertexElement(const EShaderAttribute attrib, const EShaderAttributeType type, const EShaderAttributeTypeComponents components, const uint32 byteOffset);
    ~VertexElement() {}
    //default copy constructor and assignment operator are fine

    uint32 getSize() const { return getShaderAttributeSize(m_Type, m_Components); }
    uint32 getByteOffset() const { return m_ByteOffset; }
    EShaderAttributeType getType() const { return m_Type; } 
    EShaderAttribute getAttribute() const { return m_Attribute; }
    EShaderAttributeTypeComponents getComponents() const { return m_Components; }

private:
    EShaderAttributeType m_Type;
    EShaderAttributeTypeComponents m_Components;
    EShaderAttribute m_Attribute;
    uint32 m_ByteOffset;
};

class HAPPY_ENTRY VertexLayout
{
public:
    typedef he::ObjectList<VertexElement> layout;

    VertexLayout();
    VertexLayout(const VertexLayout& other);
    VertexLayout& operator=(const VertexLayout& other);
    ~VertexLayout();

    void addElement(const VertexElement& element);

    const layout& getElements() const;
    uint32 getSize() const;

private:
    layout m_Layout;
    uint32 m_Size;
};

} } //end namespace

#endif
