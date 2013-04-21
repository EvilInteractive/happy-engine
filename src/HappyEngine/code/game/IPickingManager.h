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
//Created: 13/04/2013

#ifndef _HE_IPickingManager_H_
#define _HE_IPickingManager_H_
#pragma once

namespace he {
class Ray;
namespace ge {
class PickResult;
class Pickable;

class IPickingManager
{
public:
    virtual ~IPickingManager() {}

    virtual bool pick(const Ray& ray, PickResult& result) = 0;

    virtual void addToPickList(Pickable* const object) = 0;
    virtual void removeFromPickList(Pickable* const object) = 0;
};

} } //end namespace

#endif
