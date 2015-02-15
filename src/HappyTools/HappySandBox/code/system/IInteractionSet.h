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
//Created: 2015/02/10

#ifndef _HT_IInteractionSet_H_
#define _HT_IInteractionSet_H_
#pragma once

namespace hs {

class InteractionManager;

class IInteractionSet
{
public:
    virtual ~IInteractionSet() {}

    virtual void init(InteractionManager* const interactionManager) = 0;

    virtual void activate() = 0;
    virtual void deactivate() = 0;
};

} //end namespace

#endif
