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

namespace happyengine {
namespace graphics {

enum IndexType
{
    IndexType_Byte = sizeof(unsigned char),
    IndexType_UShort = sizeof(unsigned short),
    IndexType_UInt = sizeof(unsigned int)
};

class Model
{
public:
    typedef boost::shared_ptr<Model> pointer;

	Model();
    virtual ~Model();

    void setVertices(const void* pVertices, unsigned int num, const VertexLayout& vertexLayout);
    void setIndices(const void* pIndices, unsigned int num, IndexType type);

    unsigned int getVertexArraysID() const;

    unsigned int getNumVertices() const;
    unsigned int getNumIndices() const;

    unsigned int getIndexType() const;

private:

    unsigned int m_VaoID[1];
    unsigned int m_VertexVboID[1];
    unsigned int m_IndexVboID[1];

    unsigned int m_NumVertices;
    unsigned int m_NumIndices;

    unsigned int m_IndexType;

    //Disable default copy constructor and default assignment operator
    Model(const Model&);
    Model& operator=(const Model&);
};

} } //end namespace

#endif
