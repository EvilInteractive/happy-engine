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
//Created: 30/09/2011
//Changed: 04/11/2011
#include "HappyPCH.h" 

#include "Entity.h"
#include "HappyNew.h"
#include <algorithm>

namespace he {
namespace game {

Entity::Entity()
{
}


Entity::~Entity()
{
    std::for_each(m_Components.cbegin(), m_Components.cend(), [](IComponent* pComponent)
    {
        delete pComponent;
    });
}

mat44 Entity::getWorldMatrix() const
{
    return m_mtxWorld;
}

void Entity::setWorldMatrix(const mat44& mtxWorld)
{
    m_mtxWorld = mtxWorld;
}

void Entity::addComponent( IComponent* pComponent )
{
    m_Components.push_back(pComponent);
    pComponent->init(this);
}

} } //end namespace