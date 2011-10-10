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
//
//Author: Bastian Damman
//Created: 01/10/2011

#ifndef _HE_ShaderLayout_H_
#define _HE_ShaderLayout_H_
#pragma once

#include <vector>
#include "HappyTypes.h"

namespace he {
namespace gfx {

class ShaderLayoutElement
{
public:
    ShaderLayoutElement(uint elementIndex, const std::string& nameInShader);
    virtual ~ShaderLayoutElement() {}
    //default copy constructor and assignment operator are fine

    uint getElementIndex() const;
    const std::string& getShaderVariableName() const;

private:
    uint m_ElementIndex;
    std::string m_NameInShader;
};

class ShaderLayout
{
public:
	ShaderLayout();
    virtual ~ShaderLayout();
    //default copy constructor and default assignment operator are OK
    
    typedef std::vector<ShaderLayoutElement> layout;

    void addElement(const ShaderLayoutElement& element);

    const layout& getElements() const;

private:
    layout m_Layout;
    uint m_VertexSize;
};

} } //end namespace

#endif
