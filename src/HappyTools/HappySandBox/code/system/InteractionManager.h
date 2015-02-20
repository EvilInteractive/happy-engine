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

#ifndef _HT_InteractionManager_H_
#define _HT_InteractionManager_H_
#pragma once

namespace hs {

class IInteractionSet;

class InteractionManager
{
public:
    InteractionManager();
    ~InteractionManager();

    void addInteractionSet(IInteractionSet* interactionSet); // Takes ownership
    void removeInteractionSet(IInteractionSet* interactionSet);

    void requestActivate(IInteractionSet* interactionSet);

    void tick(const float dTime);

private:
    he::PrimitiveList<IInteractionSet*> m_InteractionSets;

    /* DEFAULT COPY & ASSIGNMENT */
    InteractionManager(const InteractionManager&);
    InteractionManager& operator=(const InteractionManager&);
};

} //end namespace

#endif
