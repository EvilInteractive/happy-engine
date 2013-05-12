//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 2013/03/05
#include "HappyPCH.h" 

#include "EngineEntityComponentFactory.h"

#include "CharacterPhysicsComponent.h"
#include "DynamicPhysicsComponent.h"
#include "LightComponent.h"
#include "ModelComponent.h"
#include "PickingComponent.h"
#include "StaticPhysicsComponent.h"
#include "TriggerComponent.h"

namespace he {
namespace ge {

EntityComponent* EngineEntityComponentFactory::createEntityComponent( const he::FixedString& type ) const
{
    EntityComponent* result(nullptr);
    if (type == HEFS::strCharacterPhysicsComponent)
    {
        result = NEW CharacterPhysicsComponent();
    }
    else if (type == HEFS::strDynamicPhysicsComponent)
    {
        result = NEW DynamicPhysicsComponent();
    }
    else if (type == HEFS::strPointLightComponent)
    {
        result = NEW PointLightComponent();
    }
    else if (type == HEFS::strSpotLightComponent)
    {
        result = NEW SpotLightComponent();
    }
    else if (type == HEFS::strModelComponent)
    {
        result = NEW ModelComponent();
    }
    else if (type == HEFS::strPickingComponent)
    {
        result = NEW PickingComponent();
    }
    else if (type == HEFS::strStaticPhysicsComponent)
    {
        result = NEW StaticPhysicsComponent();
    }
    else if (type == HEFS::strTriggerComponent)
    {
        result = NEW TriggerComponent();
    }
    return result;
}

#define FILL_COMPONENT_DESC(list, T) \
{\
    EntityComponentDesc* desc(NEW EntityComponentDesc()); \
    T::fillEntityComponentDesc(*desc); \
    list.add(desc);\
}

void EngineEntityComponentFactory::fillComponentDescList( he::PrimitiveList<EntityComponentDesc*>& list ) const
{
    //// Fully implemented ////
    FILL_COMPONENT_DESC(list, PointLightComponent);
    FILL_COMPONENT_DESC(list, SpotLightComponent);

    //// Do we need exposed and non exposed components? ///
    //FILL_COMPONENT_DESC(list, PickingComponent);

    //// Needs to be better with a ModelResource and a MaterialResource ////
    FILL_COMPONENT_DESC(list, ModelComponent);
        
    //// Need shape components for these - also these are mutually exclusive! ////
    //FILL_COMPONENT_DESC(list, CharacterPhysicsComponent);
    //FILL_COMPONENT_DESC(list, DynamicPhysicsComponent);
    //FILL_COMPONENT_DESC(list, StaticPhysicsComponent);
    //FILL_COMPONENT_DESC(list, TriggerComponent);
}

#undef FILL_COMPONENT_DESC

} } //end namespace
