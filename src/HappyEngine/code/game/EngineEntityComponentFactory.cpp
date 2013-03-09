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

EntityComponent* EngineEntityComponentFactory::createEntityComponent( const EntityComponentType type )
{
    EntityComponent* result(nullptr);
    if (type == CharacterPhysicsComponent::s_ComponentType)
    {
        result = NEW CharacterPhysicsComponent();
    }
    else if (type == DynamicPhysicsComponent::s_ComponentType)
    {
        result = NEW DynamicPhysicsComponent();
    }
    else if (type == CharacterPhysicsComponent::s_ComponentType)
    {
        result = NEW CharacterPhysicsComponent();
    }
    else if (type == PointLightComponent::s_ComponentType)
    {
        result = NEW PointLightComponent();
    }
    else if (type == SpotLightComponent::s_ComponentType)
    {
        result = NEW SpotLightComponent();
    }
    else if (type == ModelComponent::s_ComponentType)
    {
        result = NEW ModelComponent();
    }
    else if (type == PickingComponent::s_ComponentType)
    {
        result = NEW PickingComponent();
    }
    else if (type == SkinnedModelComponent::s_ComponentType)
    {
        result = NEW SkinnedModelComponent();
    }
    else if (type == StaticPhysicsComponent::s_ComponentType)
    {
        result = NEW StaticPhysicsComponent();
    }
    else if (type == TriggerComponent::s_ComponentType)
    {
        result = NEW TriggerComponent();
    }
    return result;
}

} } //end namespace
