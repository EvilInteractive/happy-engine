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
#include "InstancedModelComponent.h"
#include "LightComponent.h"
#include "ModelComponent.h"
#include "PickingComponent.h"
#include "SkinnedModelComponent.h"
#include "StaticPhysicsComponent.h"
#include "TriggerComponent.h"

namespace he {
namespace ge {

EntityComponent* EngineEntityComponentFactory::createEntityComponent( const EntityComponentID& type ) const
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
    else if (type == HEFS::strInstancedModelComponent)
    {
        result = NEW InstancedModelComponent();
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
    else if (type == HEFS::strSkinnedModelComponent)
    {
        result = NEW SkinnedModelComponent();
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

void EngineEntityComponentFactory::fillComponentDescList( he::ObjectList<EntityComponentDesc>& /*list*/ ) const
{
    //list.add(EntityComponentDesc(HEFS::strCharacterPhysicsComponent, CharacterPhysicsComponent::getPropertyDesc()));
    //list.add(EntityComponentDesc(HEFS::strDynamicPhysicsComponent, DynamicPhysicsComponent::getPropertyDesc()));
    //list.add(EntityComponentDesc(HEFS::strInstancedModelComponent, InstancedModelComponent::getPropertyDesc()));
    //list.add(EntityComponentDesc(HEFS::strPointLightComponent, PointLightComponent::getPropertyDesc()));
    //list.add(EntityComponentDesc(HEFS::strSpotLightComponent, SpotLightComponent::getPropertyDesc()));
    //list.add(EntityComponentDesc(HEFS::strModelComponent, ModelComponent::getPropertyDesc()));
    //list.add(EntityComponentDesc(HEFS::strPickingComponent, PickingComponent::getPropertyDesc()));
    //list.add(EntityComponentDesc(HEFS::strSkinnedModelComponent, SkinnedModelComponent::getPropertyDesc()));
    //list.add(EntityComponentDesc(HEFS::strStaticPhysicsComponent, StaticPhysicsComponent::getPropertyDesc()));
    //list.add(EntityComponentDesc(HEFS::strTriggerComponent, TriggerComponent::getPropertyDesc()));
}

} } //end namespace
