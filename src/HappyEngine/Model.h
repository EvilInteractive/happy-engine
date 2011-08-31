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

#ifndef _MODEL_H_
#define _MODEL_H_
#pragma once

#include <vector>
#include "VertexLayout.h"
#include "boost/shared_ptr.hpp"
#include "HappyTypes.h"

namespace happyengine {
namespace graphics {

enum IndexStride
{
    IndexStride_Byte = sizeof(byte),
    IndexStride_UShort = sizeof(ushort),
    IndexStride_UInt = sizeof(uint)
};

class Model
{
public:
    typedef boost::shared_ptr<Model> pointer;

	Model();
    virtual ~Model();

    void init();
    void setVertices(const void* pVertices, uint num, const VertexLayout& vertexLayout);
    void setIndices(const void* pIndices, uint num, IndexStride type);

    uint getVertexArraysID() const;

    uint getNumVertices() const;
    uint getNumIndices() const;

    uint getIndexType() const;

    bool isComplete() const;

private:

    uint m_VaoID[1];
    uint m_VertexVboID[1];
    uint m_IndexVboID[1];

    uint m_NumVertices;
    uint m_NumIndices;

    uint m_IndexType;

    bool m_Complete;

    //Disable default copy constructor and default assignment operator
    Model(const Model&);
    Model& operator=(const Model&);
};

} } //end namespace

#endif
