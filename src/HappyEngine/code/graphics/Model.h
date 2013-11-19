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

#include "Resource.h"

namespace he {
namespace gfx {
class ModelMesh;
class Model : public Resource<Model>
{
public:
    Model();
    virtual ~Model();

    //////////////////////////////////////////////////////////////////////////
    /// Resource
    virtual void release() const;
    virtual bool canBeGarbageCollected() const;

    //////////////////////////////////////////////////////////////////////////
    /// Model
    void addMesh(const ObjectHandle& handle);
    size_t getNumMeshes() const;
    ModelMesh* instantiateMesh(uint32 index) const;
    ModelMesh* instantiateMesh(const he::String& name) const;
    Model* instantiateMeshesWithPrefix(const he::String& prefix) const;

    he::PrimitiveList<ModelMesh*>::const_iterator cbegin() const;
    he::PrimitiveList<ModelMesh*>::const_iterator cend() const;

    void callbackOnceIfLoaded(const boost::function<void()>& callback);

    bool isLoaded() const;
    void setLoaded();

private:

    event0<void> m_LoadedCallback;
    he::Mutex m_LoadedMutex;

    he::PrimitiveList<ModelMesh*> m_Meshes;
    bool m_IsLoaded;

    //Disable default copy constructor and default assignment operator
    Model(const Model&);
    Model& operator=(const Model&);
};

} } //end namespace

#endif
