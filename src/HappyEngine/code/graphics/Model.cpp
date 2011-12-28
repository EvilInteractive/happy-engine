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
#include "HappyPCH.h" 

#include "Model.h"

#include "GL/glew.h"
#include <algorithm>
#include "HeAssert.h"
#include "Color.h"
#include "ExternalError.h"

namespace he {
namespace gfx {
    
Model::Model(const BufferLayout& vertexLayout): m_Complete(false), m_VertexLayout(vertexLayout)
{
}


Model::~Model()
{
}

void Model::addMesh(const ModelMesh::pointer& pMesh)
{
    m_Meshes.push_back(pMesh);
}
uint Model::getNumMeshes() const
{
    return m_Meshes.size();
}
ModelMesh::pointer Model::getMesh(int index) const
{
    return m_Meshes[index];
}

ModelMesh::pointer Model::getMesh( const std::string& name ) const
{
    ModelMesh::pointer retMesh;
    std::for_each(cbegin(), cend(), [&](const ModelMesh::pointer& pMesh)
    {
        if (pMesh->getName() == name)
        {
            retMesh = pMesh;
            return;
        }
    });
    return retMesh;
}

std::vector<ModelMesh::pointer> Model::getMeshesWithPrefix( const std::string& prefix ) const
{
    std::vector<ModelMesh::pointer> meshes;
    std::for_each(cbegin(), cend(), [&](const ModelMesh::pointer& pMesh)
    {
        if (pMesh->getName().find(prefix) == 0)
        {
            meshes.push_back(pMesh);
        }
    });
    return meshes;
}

std::vector<ModelMesh::pointer>::const_iterator Model::cbegin() const
{
    return m_Meshes.cbegin();
}
std::vector<ModelMesh::pointer>::const_iterator Model::cend() const
{
    return m_Meshes.cend();
}

bool Model::isComplete() const
{
    return m_Complete;
}
void Model::setComplete()
{
    m_Complete = true;
}

const BufferLayout& Model::getVertexLayout() const
{
    return m_VertexLayout;
}

} } //end namespace