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
//Author:  Sebastiaan Sprengers
//Created: 11/11/2011

#ifndef _HE_PickingManager_H_
#define _HE_PickingManager_H_
#pragma once

#include "Singleton.h"

namespace he {
namespace ge {
class Ray;
class PickResult;
class Pickable;

class PickingManager : public Singleton<PickingManager>
{
    friend class Singleton<PickingManager>;
public:
    virtual ~PickingManager();

    bool pick(const Ray& ray, PickResult& result);

    void addToPickList(Pickable* const object);
    void removeFromPickList(Pickable* const object);

private:
    PickingManager();
    
    PrimitiveList<Pickable*> m_PickList;

    /* DEFAULT COPY & ASSIGNMENT */
    PickingManager(const PickingManager&);
    PickingManager& operator=(const PickingManager&);
};

} } //end namespace

#endif
