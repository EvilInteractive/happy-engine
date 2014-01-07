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
 
struct EntityComponentDesc;   
class HAPPY_ENTRY ModelComponent : public gfx::DefaultSingleDrawable, public EntityComponent
{
    IMPLEMENT_IOBJECT3D_FROM(gfx::DefaultSingleDrawable)
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

    virtual const he::FixedString& getComponentID() const { return HEFS::strModelComponent; }

    //// Editor //////////////////////////////////////////////////////////////
    static void fillEntityComponentDesc(EntityComponentDesc& desc);
    virtual bool setProperty(const Property* const inProperty);
    virtual bool getProperty(Property* const inOutProperty);
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    

    //////////////////////////////////////////////////////////////////////////
    ///                         IDrawable                                  ///
    //////////////////////////////////////////////////////////////////////////
    virtual const gfx::Material* getMaterial() const;
    virtual const gfx::ModelMesh* getModelMesh() const;
    //////////////////////////////////////////////////////////////////////////
       
    void loadModelMeshAndMaterial(const he::String& materialAsset, const he::String& modelAsset, const he::String& meshName);
    void unloadModelMeshAndMaterial();

    he::event0<void> OnModelMeshLoaded;

protected:
    Entity* m_Parent;

private:
    bool m_IsAttached;

    gfx::ModelMesh* m_ModelMesh;
    gfx::ModelMesh* m_NewModelMesh; // Will be fixed with the NewMaterialSystem
    const gfx::Material* m_Material;

    he::String m_MaterialAsset;
    he::String m_ModelAsset;

    //Disable default copy constructor and default assignment operator
    ModelComponent(const ModelComponent&);
    ModelComponent& operator=(const ModelComponent&);
};

} } //end namespace

#endif
