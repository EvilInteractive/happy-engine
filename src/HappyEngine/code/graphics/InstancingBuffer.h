//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
    InstancingBuffer();
    InstancingBuffer(uint32 itemSize, uint32 maxItems);
    virtual ~InstancingBuffer();
    InstancingBuffer& operator=(const InstancingBuffer&);

    void resize(uint32 newSize);

    char* addItem();
    void reset();

    char* getBuffer() const;

    uint32 getSize() const;
    uint32 getSizeCapacity() const;
    uint32 getCountCapacity() const;
    uint32 getCount() const;

private:

    char* m_Buffer;
    char* m_End;

    uint32 m_Size;
    uint32 m_ItemSize;
    uint32 m_Count;

    //Disable default copy constructor
    InstancingBuffer(const InstancingBuffer&);
};

} } } //end namespace

#endif
