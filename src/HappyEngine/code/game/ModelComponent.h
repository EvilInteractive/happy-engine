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

#include "EntityComponent.h"
#include "DefaultSingleDrawable.h"

namespace he {
namespace ge {

class ModelComponent;

class ModelComponent : public gfx::DefaultSingleDrawable, public EntityComponent
{
    IMPLEMENT_IOBJECT3D_FROM(gfx::DefaultSingleDrawable)
    DECLARE_ENTITY_COMPONENT_TYPE()
public:
    ModelComponent();
    virtual ~ModelComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         EntityComponent                            ///
    //////////////////////////////////////////////////////////////////////////
    virtual void init(Entity* parent);

    virtual void visit(he::io::BinaryVisitor* const /*visitor*/) {}

    virtual void activate();
    virtual void deactivate();
    //////////////////////////////////////////////////////////////////////////
    

    //////////////////////////////////////////////////////////////////////////
    ///                         IDrawable                                  ///
    //////////////////////////////////////////////////////////////////////////
    virtual const gfx::Material* getMaterial() const;
    virtual const gfx::ModelMesh* getModelMesh() const;
    //////////////////////////////////////////////////////////////////////////
       
    void setModelMeshAndMaterial(const std::string& materialAsset, const std::string& modelAsset, const std::string& meshName = "");

    he::event0<void> OnModelMeshLoaded;

protected:
    Entity* m_Parent;

private:
    bool m_IsAttached;

    gfx::ModelMesh* m_ModelMesh;
    const gfx::Material* m_Material;

    //Disable default copy constructor and default assignment operator
    ModelComponent(const ModelComponent&);
    ModelComponent& operator=(const ModelComponent&);
};

} } //end namespace

#endif
