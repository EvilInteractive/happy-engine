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
//Created: 28/11/2012

#ifndef _HE_CHARACTER_PHYSICS_COMPONENT_H_
#define _HE_CHARACTER_PHYSICS_COMPONENT_H_
#pragma once

#include "EntityComponent.h"
#include "ITickable.h"

namespace he {
namespace px {
    class PhysicsCharacterController;
}
namespace ge {
struct EntityComponentDesc;
class HAPPY_ENTRY CharacterPhysicsComponent : public EntityComponent, public ITickable
{
public:
    CharacterPhysicsComponent();
    ~CharacterPhysicsComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         EntityComponent                            ///
    //////////////////////////////////////////////////////////////////////////
    void visit(he::io::BinaryVisitor* const /*visitor*/) {}  // override, final

    void activate(); // override, final
    void deactivate(); // override, final

    const he::FixedString& getComponentID() const { return HEFS::strCharacterPhysicsComponent; } // override, final

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
    

protected:
    void init(Entity* parent); // override, final

    px::PhysicsCharacterController* getCharacterController() const { return m_CharacterController; }

private:
    px::PhysicsCharacterController* m_CharacterController;
    
    Entity* m_Parent;
    
    //Disable default copy constructor and default assignment operator
    CharacterPhysicsComponent(const CharacterPhysicsComponent&);
    CharacterPhysicsComponent& operator=(const CharacterPhysicsComponent&);
};
} } //end namespace

#endif
