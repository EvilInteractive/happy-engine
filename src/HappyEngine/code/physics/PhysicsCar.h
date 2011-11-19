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
//Created: 11/11/2011

#ifndef _HE_PHYSICS_CAR_ACTOR_H_
#define _HE_PHYSICS_CAR_ACTOR_H_
#pragma once

#include "PxRigidDynamic.h"
#include "vec3.h"
#include "mat44.h"
#include "IPhysicsShape.h"
#include "PhysicsMaterial.h"
#include <vector>
#include "vehicle/PxVehicle.h"
#include "vehicle/PxVehicleUtils.h"
#include "PhysicsConvexShape.h"
#include "PhysicsEngine.h"

namespace he {
namespace px {


class PhysicsCar
{
public:
    enum Tyre
    {
        Tyre_FrontLeft = 0,
        Tyre_FrontRight,
        Tyre_BackLeft,
        Tyre_BackRight,
        MAX_TYRES
    };
    enum TyreType
    {
        TyreType_Slicks,
        TyreType_Wets,
        TyreType_MAX
    };

    PhysicsCar();
    virtual ~PhysicsCar();

    void init(const std::vector<PhysicsConvexShape>& wheelConvexMeshes, 
              const std::vector<PhysicsConvexShape>& chassisConvexMeshes,
              const PhysicsMaterial& material);

    vec3 getPosition() const;
    mat44 getPose() const;

    void tick(float dTime);

    mat44 getTyrePose(Tyre tyre) const;
    mat44 getChassiPose() const;

private:

    physx::PxRigidDynamic* m_pActor;
    physx::PxVehicle4W* m_pVehicle;

    static const int MAX_TYRE_TYPES = TyreType_MAX;
    

    static const int MAX_VEHICLES = 1;
    physx::PxVehicle4WSceneQueryData<MAX_VEHICLES> m_SqData;
    physx::PxBatchQuery* m_SqWheelRaycastSceneQuery;

    //STATIC
    static float PX_ALIGN(16, s_TyreFrictionMultipliers[MAX_DRIVABLE_SURFACES][MAX_TYRE_TYPES]);
    static physx::PxVehicleKeySmoothingData s_KeySmoothingData;
    static physx::PxVehiclePadSmoothingData s_PadSmoothingData;
    static float s_SteerVsForwardSpeedData[2*8];
    static physx::PxFixedSizeLookupTable<8> s_SteerVsForwardSpeedTable;

    physx::PxShape* m_TyreShape[MAX_TYRES];
    physx::PxShape* m_pChassiShape; //random chassi shape to get pose



    //Disable default copy constructor and default assignment operator
    PhysicsCar(const PhysicsCar&);
    PhysicsCar& operator=(const PhysicsCar&);
};

} } //end namespace

#endif
