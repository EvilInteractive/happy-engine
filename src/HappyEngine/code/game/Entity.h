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

#ifndef _HE_ENTITY_H_
#define _HE_ENTITY_H_
#pragma once

#include "EntityComponent.h"

namespace he {
namespace gfx {
    class Scene;
}
namespace ge {

class HAPPY_ENTRY Entity : public EntityComponent
{
public:
    DECLARE_RTTI(RTTI::Entity)

    Entity();
    virtual ~Entity();
    
    void addComponent(EntityComponent* const component);
    void removeComponent(EntityComponent* const component);
    void removeComponentAt(const size_t index);
    EntityComponent* getComponent(const he::FixedString& id);
    size_t getComponentCount() const { return m_Components.size(); }
    EntityComponent* getComponentAt(const size_t index) const { return m_Components[index]; }

    const he::FixedString& getComponentID() const { return HEFS::strEntity; }

    void activate();
    void deactivate();
    bool isActive() const { return m_IsActive; }

    void setScene(gfx::Scene* const scene) { m_Scene = scene; }
    gfx::Scene* getScene() const { return m_Scene; }
     
    //////////////////////////////////////////////////////////////////////////
    /// EntityComponent
    //////////////////////////////////////////////////////////////////////////    
    virtual void visit(he::io::BinaryVisitor* const /*visitor*/) {}

private:
    virtual void init(Entity* parent);

    // Made these methods private, use addComponent
    virtual void attach(IObject3D* child) { Object3D::attach(child); }
    virtual void detach(IObject3D* child) { Object3D::detach(child); }

    he::PrimitiveList<EntityComponent*> m_Components;
    Entity* m_Parent;
    gfx::Scene* m_Scene;
    bool m_IsActive;

    //Disable default copy constructor and default assignment operator
    Entity(const Entity&);
    Entity& operator=(const Entity&);
};

} } //end namespace

#endif
