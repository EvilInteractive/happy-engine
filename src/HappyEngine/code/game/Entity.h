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
//Created: 30/09/2011
//Changed: 04/11/2011
//added networking: 26/06/2012

#ifndef _HE_ENTITY_H_
#define _HE_ENTITY_H_
#pragma once

#include "EntityComponent.h"

namespace he {
namespace gfx {
    class Scene;
}
namespace ge {

class Entity : public EntityComponent, public Object3D
{
public:
    DECLARE_RTTI(RTTI::Entity)

    Entity();
    virtual ~Entity();
    
    void addComponent(EntityComponent* component);      // Gives ownership to Entity
    void removeComponent(EntityComponent* component);   // Returns ownership to caller

    virtual void init(gfx::Scene* scene);
    gfx::Scene* getScene() const { return m_Scene; }
     
    //////////////////////////////////////////////////////////////////////////
    /// EntityComponent
    //////////////////////////////////////////////////////////////////////////
    virtual void serialize(SerializerStream& /*stream*/) {};
    virtual void deserialize(const SerializerStream& /*stream*/) {};

    //////////////////////////////////////////////////////////////////////////
    /// Object3D (resolve ambiguity)
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
           
protected:
    virtual IObject3D* getParent() const { return Object3D::getParent(); } 
    virtual void setParent(IObject3D* parent) { Object3D::setParent(parent); } 

    virtual void setWorldMatrixDirty(uint8 cause) { Object3D::setWorldMatrixDirty(cause); } 
    virtual void setLocalMatrixDirty(uint8 cause) { Object3D::setLocalMatrixDirty(cause); } 

    virtual void calculateWorldMatrix() { Object3D::calculateWorldMatrix(); }
    
private:
    virtual void init(Entity* parent);

    // Made these methods private, use addComponent
    virtual void attach(IObject3D* child) { Object3D::attach(child); }
    virtual void detach(IObject3D* child) { Object3D::detach(child); }

    he::PrimitiveList<EntityComponent*> m_Components;
    Entity* m_Parent;
    gfx::Scene* m_Scene;

    //Disable default copy constructor and default assignment operator
    Entity(const Entity&);
    Entity& operator=(const Entity&);
};

} } //end namespace

#endif
