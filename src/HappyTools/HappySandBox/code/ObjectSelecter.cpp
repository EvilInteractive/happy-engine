//HappySandBox Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappySandBox.
//
//    HappySandBox is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappySandBox is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappySandBox.  If not, see <http://www.gnu.org/licenses/>.
// 
//Author:  Sebastiaan Sprengers
//Created: 22/01/2012

#include "ObjectSelecter.h"
#include "HappyPCH.h"
#include "GraphicsEngine.h"

namespace happysandbox {

/* CONSTRUCTOR - DESTRUCTOR */
ObjectSelecter::ObjectSelecter() :  m_pSelectedEntity(nullptr)
{
}

ObjectSelecter::~ObjectSelecter()
{
}

/* GENERAL */
void ObjectSelecter::deselect()
{
    m_pSelectedEntity = nullptr;
}

void ObjectSelecter::selectEntity(he::vec2 mousePos)
{
    //GRAPHICS->pick(
}

/* GETTERS */
he::game::Entity* ObjectSelecter::getSelectedEntity() const
{
    return nullptr;
}

} //end namespace