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
//Author:  Sebastiaan Sprengers
//Created: 18/01/2012

#ifndef _HE_MODELMESH2D_H_
#define _HE_MODELMESH2D_H_
#pragma once

#include "HeAssert.h"
#undef assert
#define assert ASSERT

#include "BufferLayout.h"
#include "boost/shared_ptr.hpp"
#include "HappyTypes.h"
#include "ModelMesh.h"

namespace he {
namespace gfx {

class ModelMesh2D
{
public:
  
    /* CONSTRUCTOR - DESTRUCTOR */
    explicit ModelMesh2D();
    virtual ~ModelMesh2D();

    /* SETTERS */
    void setVertices(const void* pVertices, uint num, const BufferLayout& vertexLayout, bool bStatic = false);
    void setIndices(const void* pIndices, uint num, IndexStride type, bool bStatic = false);

    /* GETTERS */
    uint getVertexArraysID() const;
    uint getVBOID() const;
    uint getVBOIndexID() const;

    uint getNumVertices() const;
    uint getNumIndices() const;

    uint getIndexType() const;

    /* EXTRA */
    typedef boost::shared_ptr<ModelMesh2D> pointer;

private:

    uint m_VaoID[1];
    uint m_VertexVboID[1];
    uint m_IndexVboID[1];

    uint m_NumVertices;
    uint m_NumIndices;

    uint m_IndexType;

    /* DEFAULT COPY & ASSIGNMENT */
    ModelMesh2D(const ModelMesh2D&);
    ModelMesh2D& operator=(const ModelMesh2D&);
};

} } //end namespace

#endif
