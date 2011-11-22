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
//Created: 20/11/2011

#ifndef _HE_PHYSICS_CAR_MANAGER_H_
#define _HE_PHYSICS_CAR_MANAGER_H_
#pragma once

#include <vector>
#include "vehicle/PxVehicle.h"
#include "vehicle/PxVehicleUtils.h"
#include "PhysicsMaterial.h"
#include "HappyTypes.h"

namespace he {
namespace px {

class PhysicsCar;

class DriveableSurfaceTyreFrictionTable
{
public:
    DriveableSurfaceTyreFrictionTable(byte drivableSurfaces, byte tyreTypes);
    ~DriveableSurfaceTyreFrictionTable();

    void setMaterial(byte id, const PhysicsMaterial& material);
    void setTyreType(byte id, float globalFrictionMult);
    void setTyreMaterialMult(byte tyreId, byte materialId, float mult);

    float* getFrictionPairs() const;
    byte getNumSurfaces() const;
    byte getNumTyreTypes() const;

    const PhysicsMaterial& getMaterial(byte materialId) const;

private:

    float*                                  m_TyreFrictionPair; //16 byte aligned

    physx::PxVehicleDrivableSurfaceType*	m_VehicleDrivableSurfaceTypes; //16 byte aligned

    std::vector<physx::PxMaterial*>			m_PxDrivableMaterials;
    std::vector<px::PhysicsMaterial>		m_DrivableMaterials;

    byte m_DrivableSurfaces, m_TyreTypes;

};

class PhysicsCarManager
{
public:
    PhysicsCarManager();
    virtual ~PhysicsCarManager();

    void init(const DriveableSurfaceTyreFrictionTable* pFrictionTable);

    const DriveableSurfaceTyreFrictionTable* getFrictionTable() const;

    PhysicsCar* createCar();
    void removeCar(PhysicsCar* pCar);

    void tick(float dTime);

private:

    std::vector<PhysicsCar*> m_CarList;
    std::vector<physx::PxVehicle4W*> m_InternalCarList;

    static const int MAX_VEHICLES = 8;
    physx::PxVehicle4WSceneQueryData<MAX_VEHICLES> m_SqData;
    physx::PxBatchQuery* m_SqWheelRaycastSceneQuery;

    const DriveableSurfaceTyreFrictionTable* m_pSurfaceTyreFrictionTable;

    //Disable default copy constructor and default assignment operator
    PhysicsCarManager(const PhysicsCarManager&);
    PhysicsCarManager& operator=(const PhysicsCarManager&);
};

} } //end namespace

#endif
