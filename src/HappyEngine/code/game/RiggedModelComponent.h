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

#ifndef _HE_RIGGED_MODEL_COMPONENT_H_
#define _HE_RIGGED_MODEL_COMPONENT_H_
#pragma once

#include "IComponent.h"
#include "DefaultSkinnedDrawable.h"

namespace he {
namespace game {
    
class RiggedModelComponent : public gfx::DefaultSkinnedDrawable, public IComponent
{
public:
    RiggedModelComponent();
    virtual ~RiggedModelComponent();

    struct BoneTransform
    {
        mat44 m_ToOrigTransform;
        mat44 m_FromOrigTransform;
        mat44* m_RealTransform;
    };

    //////////////////////////////////////////////////////////////////////////
    ///                         IComponent                                 ///
    //////////////////////////////////////////////////////////////////////////
    virtual void init(Entity* pParent);

    virtual void serialize(SerializerStream& stream);
    virtual void deserialize(const SerializerStream& stream);
    //////////////////////////////////////////////////////////////////////////
    

    //////////////////////////////////////////////////////////////////////////
    ///                         ISkinnedDrawable                           ///
    //////////////////////////////////////////////////////////////////////////
    virtual const gfx::Material& getMaterial() const;
    virtual const gfx::ModelMesh::pointer& getModelMesh() const; 
    virtual const std::vector<mat44>& getBoneTransforms() const;
    virtual mat44 getWorldMatrix() const;
    //////////////////////////////////////////////////////////////////////////
        
    void setLocalTransform(const mat44& mtxWorld);
    const mat44& getLocalTransform() const;
   
    void setModelMesh(const gfx::ModelMesh::pointer& pModel);

    void setMaterial(const gfx::Material& material);

    BoneTransform getBone(const std::string& name) const;

protected:
    virtual void onModelLoaded() {}

private:
    void modelLoadedCallback(const gfx::ModelMesh::pointer& pMesh);

    gfx::ModelMesh::pointer m_pModel;
    gfx::Material m_Material;

    std::vector<mat44> m_BoneTransform;
    std::map<std::string, BoneTransform> m_Bones;

    mat44 m_mtxLocalTransform;
    
    Entity* m_pParent;
    

    //Disable default copy constructor and default assignment operator
    RiggedModelComponent(const RiggedModelComponent&);
    RiggedModelComponent& operator=(const RiggedModelComponent&);
};

} } //end namespace

#endif
