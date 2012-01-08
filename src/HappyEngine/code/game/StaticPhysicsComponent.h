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

#include "IComponent.h"
#include "mat44.h"
#include "ITickable.h"
#include "IPhysicsShape.h"
#include "PhysicsStaticActor.h"

namespace he {
namespace game {

class StaticPhysicsComponent : public IComponent
{
public:
	StaticPhysicsComponent();
    virtual ~StaticPhysicsComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         IComponent                                 ///
    //////////////////////////////////////////////////////////////////////////
    virtual void init(Entity* pParent);

    virtual void serialize(SerializerStream& stream);
    virtual void deserialize(const SerializerStream& stream);
    //////////////////////////////////////////////////////////////////////////
         
    void addShape( const px::IPhysicsShape* pShape, const px::PhysicsMaterial& material );

    px::PhysicsStaticActor* getStaticActor() const;

private:

    px::PhysicsStaticActor* m_pStaticActor;
    
    Entity* m_pParent;
    

    //Disable default copy constructor and default assignment operator
    StaticPhysicsComponent(const StaticPhysicsComponent&);
    StaticPhysicsComponent& operator=(const StaticPhysicsComponent&);
};

} } //end namespace

#endif
