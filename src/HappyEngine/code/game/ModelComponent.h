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
//Created: 29/10/2011

#ifndef _HE_MODEL_COMPONENT_H_
#define _HE_MODEL_COMPONENT_H_
#pragma once

#include "IComponent.h"
#include "DefaultSingleDrawable.h"
#include "IPickable.h"

namespace he {
namespace ge {

class ModelComponent;

class ModelComponent : public gfx::DefaultSingleDrawable, public IComponent
{
public:
    ModelComponent();
    virtual ~ModelComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         IComponent                                 ///
    //////////////////////////////////////////////////////////////////////////
    virtual void init(Entity* pParent);

    virtual void serialize(SerializerStream& stream);
    virtual void deserialize(const SerializerStream& stream);
    //////////////////////////////////////////////////////////////////////////
    

    //////////////////////////////////////////////////////////////////////////
    ///                         IDrawable                                  ///
    //////////////////////////////////////////////////////////////////////////
    virtual const gfx::Material* getMaterial() const;
    virtual const gfx::ModelMesh* getModelMesh() const;   
    virtual mat44 getWorldMatrix() const;
    //////////////////////////////////////////////////////////////////////////
    
    void setLocalTransform(const mat44& mtxWorld);
    const mat44& getLocalTransform() const;
   
    void setModelMesh(const ObjectHandle& modelHandle, bool isPickable = true);

    void setMaterial(const ObjectHandle& material);

    virtual bool isDynamic() const = 0;
    virtual bool isSleeping() const = 0; 

protected:
    Entity* m_Parent;

private:
    const gfx::ModelMesh* m_Model;
    const gfx::Material* m_Material;

    mat44 m_mtxLocalTransform;   

    //Disable default copy constructor and default assignment operator
    ModelComponent(const ModelComponent&);
    ModelComponent& operator=(const ModelComponent&);
};

class StaticModelComponent : public ModelComponent
{
    virtual bool isDynamic() const { return false; };
    virtual bool isSleeping() const { return true; }; 
};
class DynamicModelComponent : public ModelComponent
{
    virtual bool isDynamic() const { return true; };
    virtual bool isSleeping() const; 
};

} } //end namespace

#endif
