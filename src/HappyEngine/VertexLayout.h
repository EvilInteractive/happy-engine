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

#ifndef _VERTEXLAYOUT_H_
#define _VERTEXLAYOUT_H_
#pragma once

#include <string>
#include <vector>

#include "HappyTypes.h"

namespace happyengine {
namespace graphics {

class VertexElement
{
public:
    enum Type
    {
        Type_Vector2,
        Type_Vector3,
        Type_Vector4,
        Type_Float,
        Type_Double,
        Type_Int,
        Type_UInt,
        Type_Short,
        Type_UShort,
        Type_Byte,
        Type_UByte,
    };
    enum Usage
    {
        Usage_Position,
        Usage_TextureCoordinate,
        Usage_Normal,
        Usage_Other
    };
    VertexElement(uint elementIndex, Type type, Usage usage, 
        uint size, uint byteOffset, const std::string& nameInShader);
    virtual ~VertexElement() {}
    //default copy constructor and assignment operator are fine

    uint getElementIndex() const;
    uint getSize() const; 
    uint getByteOffset() const; 
    Type getType() const;
    Usage getUsage() const;
    const std::string& getShaderVariableName() const;

private:
    uint m_ElementIndex;
    Type m_Type;
    Usage m_Usage;
    uint m_Size;
    uint m_ByteOffset;
    std::string m_NameInShader;
};

class VertexLayout
{
public:
    typedef std::vector<VertexElement> layout;

	VertexLayout();
    virtual ~VertexLayout();
    //default copy constructor and assignment operator are fine

    void addElement(const VertexElement& element);

    const layout& getElements() const;
    uint getVertexSize() const;


private:
    layout m_Layout;
    uint m_VertexSize;
};

} } //end namespace

#endif
