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
//Created: 20/08/2011

#ifndef _HE_PHYSICS_MATERIAL_H_
#define _HE_PHYSICS_MATERIAL_H_
#pragma once

#include "PxMaterial.h"
#include "boost/shared_ptr.hpp"

namespace he {
namespace px {

class PhysicsMaterial
{
public:
	PhysicsMaterial(float staticFriction, float dynamicFriction, float restitution);
    virtual ~PhysicsMaterial();

    physx::PxMaterial* getInternalMaterial() const;

private:

    physx::PxMaterial* m_pInternalMaterial;

    //Disable default copy constructor and default assignment operator
    PhysicsMaterial(const PhysicsMaterial&);
    PhysicsMaterial& operator=(const PhysicsMaterial&);
};

} } //end namespace

#endif
