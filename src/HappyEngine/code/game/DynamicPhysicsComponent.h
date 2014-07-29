//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

struct EntityComponentDesc;
class HAPPY_ENTRY DynamicPhysicsComponent : public EntityComponent, public ITickable
{
public:
    DynamicPhysicsComponent();
    ~DynamicPhysicsComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         EntityComponent                            ///
    //////////////////////////////////////////////////////////////////////////
    void visit(he::io::BinaryVisitor* const /*visitor*/) {} // override, final

    void activate(); // override, final
    void deactivate(); // override, final

    const he::FixedString& getComponentID() const { return HEFS::strDynamicPhysicsComponent; } // override, final

    //// Editor //////////////////////////////////////////////////////////////
    static void fillEntityComponentDesc(EntityComponentDesc& desc);
    bool setProperty(const Property* const inProperty); // override, final
    bool getProperty(Property* const inOutProperty); // override, final
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
     
    //////////////////////////////////////////////////////////////////////////
    ///                            ITickable                               ///
    //////////////////////////////////////////////////////////////////////////
    void tick(float dTime); // override, final
    //////////////////////////////////////////////////////////////////////////
    
    void addShape( const px::IPhysicsShape* shape, const px::PhysicsMaterial& material, float mass, 
        uint32 collisionGroup, uint32 collisionGroupAgainst, const mat44& localPose = mat44::Identity);

    px::PhysicsDynamicActor* getDynamicActor() const;

private:
    void init(Entity* parent); // override, final

    px::PhysicsDynamicActor* m_DynamicActor;
    
    Entity* m_Parent;
    
    //Disable default copy constructor and default assignment operator
    DynamicPhysicsComponent(const DynamicPhysicsComponent&);
    DynamicPhysicsComponent& operator=(const DynamicPhysicsComponent&);


};

} } //end namespace

#endif
