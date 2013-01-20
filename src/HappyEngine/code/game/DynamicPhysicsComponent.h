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

#ifndef _HE_DYNAMIC_PHYSICS_COMPONENT_H_
#define _HE_DYNAMIC_PHYSICS_COMPONENT_H_
#pragma once

#include "EntityComponent.h"
#include "ITickable.h"

namespace he {
namespace px {
    class PhysicsDynamicActor;
    class PhysicsMaterial;
    class IPhysicsShape;
}
namespace ge {

class DynamicPhysicsComponent : public EntityComponent, public Object3D, public ITickable
{
public:
    DynamicPhysicsComponent();
    virtual ~DynamicPhysicsComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         EntityComponent                                 ///
    //////////////////////////////////////////////////////////////////////////
    virtual void serialize(SerializerStream& stream);
    virtual void deserialize(const SerializerStream& stream);

    virtual void activate();
    virtual void deactivate();
    //////////////////////////////////////////////////////////////////////////
     
    //////////////////////////////////////////////////////////////////////////
    ///                            ITickable                               ///
    //////////////////////////////////////////////////////////////////////////
    virtual void tick(float dTime);
    //////////////////////////////////////////////////////////////////////////
    
    void addShape( const px::IPhysicsShape* shape, const px::PhysicsMaterial& material, float mass, 
        uint32 collisionGroup, uint32 collisionGroupAgainst, const mat44& localPose = mat44::Identity);

    px::PhysicsDynamicActor* getDynamicActor() const;

protected:
    virtual void init(Entity* parent);

private:
    px::PhysicsDynamicActor* m_DynamicActor;
    
    Entity* m_Parent;
    
    //Disable default copy constructor and default assignment operator
    DynamicPhysicsComponent(const DynamicPhysicsComponent&);
    DynamicPhysicsComponent& operator=(const DynamicPhysicsComponent&);


    //////////////////////////////////////////////////////////////////////////
    /// Object3D
    //////////////////////////////////////////////////////////////////////////
public:
    virtual void setLocalTranslate(const vec3& translate)  { Object3D::setLocalTranslate(translate); } 
    virtual void setLocalRotate(const mat33& rotate) { Object3D::setLocalRotate(rotate); } 
    virtual void setLocalScale(const vec3& scale) { Object3D::setLocalScale(scale); } 

    virtual const vec3&  getLocalTranslate() const { return Object3D::getLocalTranslate(); } 
    virtual const mat33& getLocalRotate() const { return Object3D::getLocalRotate(); } 
    virtual const vec3&  getLocalScale() const { return Object3D::getLocalScale(); } 

    virtual const mat44& getLocalMatrix() const { return Object3D::getLocalMatrix(); } 
    virtual const mat44& getWorldMatrix() const { return Object3D::getWorldMatrix(); } 

    virtual void attach(IObject3D* child) { Object3D::attach(child); }
    virtual void detach(IObject3D* child) { Object3D::detach(child); }

protected:
    virtual IObject3D* getParent() const { return Object3D::getParent(); } 
    virtual void setParent(IObject3D* parent) { Object3D::setParent(parent); } 

    virtual void setWorldMatrixDirty(uint8 cause) { Object3D::setWorldMatrixDirty(cause); } 
    virtual void setLocalMatrixDirty(uint8 cause) { Object3D::setLocalMatrixDirty(cause); } 

    virtual void calculateWorldMatrix();
};

} } //end namespace

#endif
