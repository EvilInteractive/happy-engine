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

#ifndef _HE_RIGGED_MODEL_COMPONENT_H_
#define _HE_RIGGED_MODEL_COMPONENT_H_
#pragma once

#include "EntityComponent.h"
#include "DefaultSkinnedDrawable.h"

namespace he {
namespace ge {
   
struct EntityComponentDesc; 
class HAPPY_ENTRY SkinnedModelComponent : public gfx::DefaultSkinnedDrawable, public EntityComponent
{
    IMPLEMENT_IOBJECT3D_FROM(gfx::DefaultSkinnedDrawable)
public:
    SkinnedModelComponent();
    virtual ~SkinnedModelComponent();

    struct BoneTransform
    {
        mat44 m_ToOrigTransform;
        mat44 m_FromOrigTransform;
        mat44* m_RealTransform;
    };

    //////////////////////////////////////////////////////////////////////////
    ///                         EntityComponent                                 ///
    //////////////////////////////////////////////////////////////////////////
    virtual void init(Entity* pParent);
    virtual void visit(he::io::BinaryVisitor* const /*visitor*/) {}

    virtual const he::FixedString& getComponentID() const { return HEFS::strSkinnedModelComponent; }

    //// Editor //////////////////////////////////////////////////////////////
    static void fillEntityComponentDesc(EntityComponentDesc& desc);
    virtual bool setProperty(const Property* const inProperty);
    virtual bool getProperty(Property* const inOutProperty);
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    

    //////////////////////////////////////////////////////////////////////////
    ///                         ISkinnedDrawable                           ///
    //////////////////////////////////////////////////////////////////////////
    virtual const gfx::Material* getMaterial() const;
    virtual const gfx::ModelMesh* getModelMesh() const; 
    virtual const he::PrimitiveList<mat44>& getBoneTransforms() const;
    //////////////////////////////////////////////////////////////////////////
           
    void setModelMesh(const ObjectHandle& modelHandle);
    void setMaterial(const ObjectHandle& material);

    BoneTransform getBone(const he::String& name) const;

protected:
    virtual void onModelLoaded() {}

private:
    void modelLoadedCallback();

    const gfx::ModelMesh* m_ModelMesh;
    const gfx::Material* m_Material;

    he::PrimitiveList<mat44> m_BoneTransform;
    std::map<he::String, BoneTransform> m_Bones;
        
    Entity* m_Parent;
    

    //Disable default copy constructor and default assignment operator
    SkinnedModelComponent(const SkinnedModelComponent&);
    SkinnedModelComponent& operator=(const SkinnedModelComponent&);
};

} } //end namespace

#endif
