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
//Created: 29/10/2011

#ifndef _HE_MODEL_COMPONENT_H_
#define _HE_MODEL_COMPONENT_H_
#pragma once

#include "IComponent.h"
#include "IDrawable.h"

namespace he {
namespace game {
    
class ModelComponent : public IComponent, public gfx::IDrawable
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
    virtual const gfx::Material& getMaterial() const;
    virtual const gfx::ModelMesh::pointer& getModel() const;   
    virtual mat44 getWorldMatrix() const;

    virtual bool getCastsShadow() const;
    virtual void setCastsShadow(bool casts);
    //////////////////////////////////////////////////////////////////////////
    
    void setLocalTransform(const mat44& mtxWorld);
    const mat44& getLocalTransform() const;
   
    void setModel(const gfx::ModelMesh::pointer& pModel);

    void setMaterial(const gfx::Material& material);

private:
    gfx::ModelMesh::pointer m_pModel;
    gfx::Material m_Material;

    mat44 m_mtxLocalTransform;

    bool m_CastShadow;

    Entity* m_pParent;
    

    //Disable default copy constructor and default assignment operator
    ModelComponent(const ModelComponent&);
    ModelComponent& operator=(const ModelComponent&);
};

} } //end namespace

#endif
