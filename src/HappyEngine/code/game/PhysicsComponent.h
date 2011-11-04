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

#ifndef _HE_PHYSICS_COMPONENT_H_
#define _HE_PHYSICS_COMPONENT_H_
#pragma once

#include "IComponent.h"
#include "mat44.h"
#include "ITickable.h"

namespace he {
namespace game {
    
class PhysicsComponent : public IComponent, public ITickable
{
public:
	PhysicsComponent();
    virtual ~PhysicsComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         IComponent                                 ///
    //////////////////////////////////////////////////////////////////////////
    virtual void tick(float dTime);
    virtual void init(Entity* pParent);

    virtual void serialize(const SerializerStream& stream);
    virtual void deserialize(const SerializerStream& stream);
    //////////////////////////////////////////////////////////////////////////
     
    void setLocalTransform(const mat44& mtxWorld);
    const mat44& getLocalTransform() const;

private:

    mat44 m_mtxLocalTransform;

    Entity* m_pParent;
    

    //Disable default copy constructor and default assignment operator
    PhysicsComponent(const PhysicsComponent&);
    PhysicsComponent& operator=(const PhysicsComponent&);
};

} } //end namespace

#endif
