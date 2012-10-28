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
#include "HappyPCH.h" 

#include "Entity.h"
#include "Scene.h"
#include "GraphicsEngine.h"

namespace he {
namespace ge {

Entity::Entity():
    m_SleepEvaluaters([](bool& inoutA, const bool& outB) -> bool
    { 
        inoutA = outB; 
        return inoutA;
    }, false)
{
}


Entity::~Entity()
{
    std::for_each(m_Components.cbegin(), m_Components.cend(), [](EntityComponent* component)
    {
        delete component;
    });
}

void Entity::init( gfx::Scene* scene )
{
    m_Scene = scene;
}

void Entity::init( Entity* parent )
{
    m_Parent = parent;
    m_Scene = m_Parent->getScene();
}

void Entity::addComponent( EntityComponent* component )
{
    m_Components.push_back(component);
    attach(component);
    component->init(this);
}

void Entity::removeComponent( EntityComponent* component )
{
    detach(component);
    std::vector<EntityComponent*>::iterator it(std::find(m_Components.begin(), m_Components.end(), component));
    HE_IF_ASSERT(it != m_Components.cend(), "Component not attached to Entity")
    {
        *it = m_Components.back();
        m_Components.pop_back();
    }
}

bool Entity::isSleeping() const
{
    return m_SleepEvaluaters();
}

} } //end namespace
