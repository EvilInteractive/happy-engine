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

#include "IComponent.h"
#include "mat44.h"
#include "ITickable.h"
#include "IPhysicsShape.h"
#include "PhysicsDynamicActor.h"

namespace he {
namespace ge {

class DynamicPhysicsComponent : public IComponent, public ITickable
{
public:
	DynamicPhysicsComponent();
    virtual ~DynamicPhysicsComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         IComponent                                 ///
    //////////////////////////////////////////////////////////////////////////
    virtual void init(Entity* pParent);

    virtual void serialize(SerializerStream& stream);
    virtual void deserialize(const SerializerStream& stream);
    //////////////////////////////////////////////////////////////////////////
     
    //////////////////////////////////////////////////////////////////////////
    ///                            ITickable                               ///
    //////////////////////////////////////////////////////////////////////////
    virtual void tick(float dTime);
    //////////////////////////////////////////////////////////////////////////
    
    void addShape( const px::IPhysicsShape* pShape, const px::PhysicsMaterial& material, float mass, 
        uint32 collisionGroup, uint32 collisionGroupAgainst, const mat44& localPose = mat44::Identity);

    px::PhysicsDynamicActor* getDynamicActor() const;

private:

    px::PhysicsDynamicActor* m_pDynamicActor;
    
    Entity* m_pParent;
    

    //Disable default copy constructor and default assignment operator
    DynamicPhysicsComponent(const DynamicPhysicsComponent&);
    DynamicPhysicsComponent& operator=(const DynamicPhysicsComponent&);
};

} } //end namespace

#endif
