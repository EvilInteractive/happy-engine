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

#ifndef _HE_STATIC_PHYSICS_COMPONENT_H_
#define _HE_STATIC_PHYSICS_COMPONENT_H_
#pragma once

#include "EntityComponent.h"
#include "ITickable.h"


namespace he {
namespace px {
    class PhysicsStaticActor;
    class PhysicsMaterial;
    class IPhysicsShape;
}
namespace ge {

class StaticPhysicsComponent : public EntityComponent, public Object3D
{
public:
    StaticPhysicsComponent();
    virtual ~StaticPhysicsComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         EntityComponent                            ///
    //////////////////////////////////////////////////////////////////////////
    virtual void serialize(SerializerStream& stream);
    virtual void deserialize(const SerializerStream& stream);
    //////////////////////////////////////////////////////////////////////////
         
    void addShape( const px::IPhysicsShape* shape, const px::PhysicsMaterial& material, uint32 collisionGroup = 0xffffffff, const mat44& localPose = mat44::Identity );

    px::PhysicsStaticActor* getStaticActor() const;

protected:
    virtual void init(Entity* parent);

private:
    px::PhysicsStaticActor* m_StaticActor;    
    Entity* m_Parent;
    

    //Disable default copy constructor and default assignment operator
    StaticPhysicsComponent(const StaticPhysicsComponent&);
    StaticPhysicsComponent& operator=(const StaticPhysicsComponent&);

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

    virtual void setWorldMatrixDirty(byte cause) { Object3D::setWorldMatrixDirty(cause); } 
    virtual void setLocalMatrixDirty(byte cause) { Object3D::setLocalMatrixDirty(cause); } 

    virtual void calculateWorldMatrix();
};

} } //end namespace

#endif
