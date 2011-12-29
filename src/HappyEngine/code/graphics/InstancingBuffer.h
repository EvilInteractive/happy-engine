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
//Author:  Bastian Damman
//Created: 28/12/2011

#ifndef _HE_InstancingBuffer_H_
#define _HE_InstancingBuffer_H_
#pragma once

namespace he {
namespace gfx {
namespace details {

class InstancingBuffer
{
public:
    InstancingBuffer(uint itemSize, uint maxItems);
    virtual ~InstancingBuffer();

    void resize(uint newSize);

    char* addItem();
    void reset();

    char* getBuffer() const;

    uint getSize() const;
    uint getSizeCapacity() const;
    uint getCountCapacity() const;
    uint getCount() const;

private:

    char* m_Buffer;
    char* m_End;

    uint m_Size;
    uint m_ItemSize;
    uint m_Count;

    //Disable default copy constructor and default assignment operator
    InstancingBuffer(const InstancingBuffer&);
    InstancingBuffer& operator=(const InstancingBuffer&);
};

} } } //end namespace

#endif
