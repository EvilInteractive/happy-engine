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
//Created: 2015/02/15
#include "HappySandBoxPCH.h" 

#include "InteractionManager.h"
#include "IInteractionSet.h"

namespace hs {

InteractionManager::InteractionManager()
{
}

InteractionManager::~InteractionManager()
{
    m_InteractionSets.forEach([](IInteractionSet* set)
    {
        set->deactivate();
        HEDelete(set);
    });
}

void InteractionManager::addInteractionSet( IInteractionSet* interactionSet )
{
    interactionSet->init(this);
    m_InteractionSets.add(interactionSet);
}

void InteractionManager::removeInteractionSet( IInteractionSet* interactionSet )
{
    interactionSet->deactivate();
    m_InteractionSets.remove(interactionSet);
}

void InteractionManager::requestActivate( IInteractionSet* interactionSet )
{
    m_InteractionSets.forEach([interactionSet](IInteractionSet* set)
    {
        if (set != interactionSet)
            set->deactivate();
    });
    interactionSet->activate();
}

void InteractionManager::tick( const float dTime )
{
    m_InteractionSets.forEach([dTime](IInteractionSet* set)
    {
        set->tick(dTime);
    });
}

} //end namespace