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
//Created: 28/12/2011

#ifndef _HE_DYNAMIC_BUFFER_H_
#define _HE_DYNAMIC_BUFFER_H_
#pragma once

#include "VertexLayout.h"

namespace he {
namespace gfx {

class DynamicBuffer
{
public:
    DynamicBuffer(const VertexLayout& bufferLayout);
    virtual ~DynamicBuffer();

    void setValue(uint32 element, float value);
    void setValue(uint32 element, int value);
    void setValue(uint32 element, uint32 value);

    void setValue(uint32 element, const vec2& value);
    void setValue(uint32 element, const vec3& value);
    void setValue(uint32 element, const vec4& value);

    void setValue(uint32 element, const mat44& value);

    const void* getBuffer() const;
    void setBuffer(char* pBuffer);

private:

    char* m_Buffer;
    VertexLayout m_Layout;

    //Disable default copy constructor and default assignment operator
    DynamicBuffer(const DynamicBuffer&);
    DynamicBuffer& operator=(const DynamicBuffer&);
};

} } //end namespace

#endif
