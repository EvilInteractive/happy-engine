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
//Created: 13/07/2011
//Changed: 15/09/2011 -Model class became ModelMesh

#ifndef _HE_MODEL_H_
#define _HE_MODEL_H_
#pragma once

#include "Assert.h"
#undef assert
#define assert ASSERT

#include <vector>
#include "VertexLayout.h"
#include "boost/shared_ptr.hpp"
#include "HappyTypes.h"
#include "ModelMesh.h"
#include "VertexLayout.h"

namespace he {
namespace gfx {

class Model
{
public:
    typedef boost::shared_ptr<Model> pointer;

	Model(const VertexLayout& vertexLayout);
    virtual ~Model();
    
    void addMesh(const ModelMesh::pointer& pMesh);
    uint getNumMeshes() const;
    ModelMesh::pointer getMesh(int index) const;

    std::vector<ModelMesh::pointer>::const_iterator cbegin() const;
    std::vector<ModelMesh::pointer>::const_iterator cend() const;

    bool isComplete() const;
    void setComplete();

    const VertexLayout& getVertexLayout() const;

private:

    VertexLayout m_VertexLayout;
    std::vector<ModelMesh::pointer> m_Meshes;
    bool m_Complete;

    //Disable default copy constructor and default assignment operator
    Model(const Model&);
    Model& operator=(const Model&);
};

} } //end namespace

#endif
