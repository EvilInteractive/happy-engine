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
#include "HappyPCH.h" 

#include "PhysicsMaterial.h"
#include "HappyEngine.h"
#include "HeAssert.h"
#include "PhysicsEngine.h"

namespace he {
namespace px {

PhysicsMaterial::PhysicsMaterial(float staticFriction, float dynamicFriction, float restitution): m_pInternalMaterial(nullptr)
{    
    m_pInternalMaterial = PHYSICS->createMaterial(staticFriction, dynamicFriction, restitution);
    ASSERT(m_pInternalMaterial != nullptr, "px material creation failed");
}

PhysicsMaterial::PhysicsMaterial(): m_pInternalMaterial(nullptr)
{

}

PhysicsMaterial::PhysicsMaterial( physx::PxMaterial* pMaterial ): m_pInternalMaterial(pMaterial)
{

}

PhysicsMaterial::~PhysicsMaterial()
{
}

physx::PxMaterial* PhysicsMaterial::getInternalMaterial() const
{
    return m_pInternalMaterial;
}

} } //end namespace