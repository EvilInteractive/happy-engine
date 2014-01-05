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
#include "HappyPCH.h" 

#include "Model.h"
#include "ModelMesh.h"

namespace he {
namespace gfx {
    
Model::Model()
{
}

Model::~Model()
{
    std::for_each(cbegin(), cend(), [&](ModelMesh* pMesh)
    {
        pMesh->release();
    });
}

void Model::release() const
{
    Resource<Model>::release();
}
bool Model::canBeGarbageCollected() const
{
    // only GC if all meshes are referenced just by this instance
    bool block(std::any_of(cbegin(), cend(), [](ModelMesh* mesh)
    {
        return ResourceFactory<ModelMesh>::getInstance()->getRefCount(mesh->getHandle()) > 1;
    }));
    return !block;
}

void Model::addMesh(const ObjectHandle& handle)
{
    ResourceFactory<ModelMesh>::getInstance()->instantiate(handle);
    m_Meshes.add(ResourceFactory<ModelMesh>::getInstance()->get(handle));
}
size_t Model::getNumMeshes() const
{
    return m_Meshes.size();
}
    
    
ModelMesh* Model::getMesh(const uint32 index) const
{
    HE_ASSERT(index < m_Meshes.size(), "Model::getMesh: index out of range: %d", index);
    ModelMesh* mesh(m_Meshes[index]);
    return mesh;
}
    
ModelMesh* Model::instantiateMesh(uint32 index) const
{
    HE_ASSERT(index < m_Meshes.size(), "Model::instantiateMesh: index out of range: %d", index);
    ModelMesh* mesh(m_Meshes[index]);
    ResourceFactory<ModelMesh>::getInstance()->instantiate(mesh->getHandle());
    return mesh;
}

ModelMesh* Model::instantiateMesh( const he::String& name ) const
{
    ModelMesh* const value(tryInstantiateMesh(name));
    HE_CONDITIONAL_ERROR(value == nullptr, "Mesh in model (%s) not found with name %s", getName().c_str(), name.c_str());
    return value;
}

ModelMesh* Model::tryInstantiateMesh( const he::String& name ) const
{
    he::PrimitiveList<ModelMesh*>::const_iterator it(std::find_if(cbegin(), cend(), [&](ModelMesh* pMesh)
    {
        return pMesh->getName() == name;
    }));

    if (it != cend())
    {
        ModelMesh* mesh(*it);
        mesh->instantiate();
        return mesh;
    }
    return nullptr;
}

Model* Model::instantiateMeshesWithPrefix( const he::String& prefix ) const
{
    ObjectHandle modelHandle(ResourceFactory<Model>::getInstance()->create());
    Model* model(ResourceFactory<Model>::getInstance()->get(modelHandle));

    std::for_each(cbegin(), cend(), [&](ModelMesh* mesh)
    {
        if (mesh->getName().find(prefix) == 0)
        {
            model->addMesh(mesh->getHandle());
        }
    });
    return model;
}

he::PrimitiveList<ModelMesh*>::const_iterator Model::cbegin() const
{
    return m_Meshes.cbegin();
}
he::PrimitiveList<ModelMesh*>::const_iterator Model::cend() const
{
    return m_Meshes.cend();
}

} } //end namespace