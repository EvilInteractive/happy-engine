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
    
Model::Model(): m_IsLoaded(false)
{
}


Model::~Model()
{
    std::for_each(cbegin(), cend(), [&](ModelMesh* pMesh)
    {
        pMesh->release();
    });
}

void Model::release()
{
    Resource<Model>::release();
}
bool Model::canBeGarbageCollected()
{
    // only GC if all meshes are referenced just by this instance
    bool block(std::any_of(cbegin(), cend(), [](ModelMesh* mesh)
    {
        return ResourceFactory<ModelMesh>::getInstance()->getRefCount(mesh->getHandle()) == 1;
    }));
    return !block;
}

void Model::addMesh(const ObjectHandle& handle)
{
    ResourceFactory<ModelMesh>::getInstance()->instantiate(handle);
    m_Meshes.push_back(ResourceFactory<ModelMesh>::getInstance()->get(handle));
}
uint Model::getNumMeshes() const
{
    return m_Meshes.size();
}
ModelMesh* Model::instantiateMesh(uint index) const
{
    HE_ASSERT(index < m_Meshes.size(), "Model::instantiateMesh: index out of range: %d", index);
    ModelMesh* mesh(m_Meshes[index]);
    ResourceFactory<ModelMesh>::getInstance()->instantiate(mesh->getHandle());
    return mesh;
}

ModelMesh* Model::instantiateMesh( const std::string& name ) const
{
    std::vector<ModelMesh*>::const_iterator it(std::find_if(cbegin(), cend(), [&](ModelMesh* pMesh)
    {
        return pMesh->getName() == name;
    }));

    if (it != cend())
    {
        ModelMesh* mesh(*it);
        ResourceFactory<ModelMesh>::getInstance()->instantiate(mesh->getHandle());
        return mesh;
    }
    HE_ERROR("Mesh in model (%s) not found with name %s", getName().c_str(), name.c_str());
    return nullptr;
}

Model* Model::instantiateMeshesWithPrefix( const std::string& prefix ) const
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

std::vector<ModelMesh*>::const_iterator Model::cbegin() const
{
    return m_Meshes.cbegin();
}
std::vector<ModelMesh*>::const_iterator Model::cend() const
{
    return m_Meshes.cend();
}

bool Model::isLoaded() const
{
    return m_IsLoaded;
}
void Model::setLoaded()
{
    m_IsLoaded = true;
    m_LoadedMutex.lock();
    m_LoadedCallback();
    m_LoadedCallback.clear();
    m_LoadedMutex.unlock();
}

void Model::callbackOnceIfLoaded( const boost::function<void()>& callback )
{
    m_LoadedMutex.lock();
    if (isLoaded())
    {
        m_LoadedMutex.unlock();
        callback();
    }
    else
    {
        m_LoadedCallback += callback;
        m_LoadedMutex.unlock();
    }
}

} } //end namespace