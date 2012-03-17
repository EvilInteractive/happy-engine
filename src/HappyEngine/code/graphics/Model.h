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
//Created: 13/07/2011
//Changed: 15/09/2011 -Model class became ModelMesh

#ifndef _HE_MODEL_H_
#define _HE_MODEL_H_
#pragma once

#include "HeAssert.h"
#undef assert
#define assert HE_ASSERT

#include <vector>
#include "BufferLayout.h"
#include "boost/shared_ptr.hpp"
#include "HappyTypes.h"
#include "ModelMesh.h"
#include "BufferLayout.h"

#include "Sphere.h"

namespace he {
namespace gfx {

class Model
{
public:
    typedef boost::shared_ptr<Model> pointer;

	Model(const BufferLayout& vertexLayout);
    virtual ~Model();
    
    void addMesh(const ModelMesh::pointer& pMesh);
    uint getNumMeshes() const;
    ModelMesh::pointer getMesh(int index) const;
    ModelMesh::pointer getMesh(const std::string& name) const;
    std::vector<ModelMesh::pointer> getMeshesWithPrefix(const std::string& prefix) const;

    std::vector<ModelMesh::pointer>::const_iterator cbegin() const;
    std::vector<ModelMesh::pointer>::const_iterator cend() const;

    bool isComplete() const; //used as mutex
    void setComplete();

    const BufferLayout& getVertexLayout() const;

private:

    BufferLayout m_VertexLayout;
    //std::vector<std::vector<ModelMesh::pointer>> m_MeshesPerLod;
    std::vector<ModelMesh::pointer> m_Meshes;
    bool m_Complete;

    shapes::Sphere m_BoundingSphere;

    //Disable default copy constructor and default assignment operator
    Model(const Model&);
    Model& operator=(const Model&);
};

} } //end namespace

#endif
