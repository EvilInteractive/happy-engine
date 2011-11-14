////HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
////
////This file is part of HappyEngine.
////
////    HappyEngine is free software: you can redistribute it and/or modify
////    it under the terms of the GNU Lesser General Public License as published by
////    the Free Software Foundation, either version 3 of the License, or
////    (at your option) any later version.
////
////    HappyEngine is distributed in the hope that it will be useful,
////    but WITHOUT ANY WARRANTY; without even the implied warranty of
////    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
////    GNU Lesser General Public License for more details.
////
////    You should have received a copy of the GNU Lesser General Public License
////    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.
////
////Author:  Bastian Damman
////Created: 22/08/2011
////Updated: 14/09/2011 -added addForce/velocity and kinematic methods
//#include "StdAfx.h" 
//
//#include "PhysicsCar.h"
//#include "HappyEngine.h"
//#include "HeAssert.h"
//#include "PhysicsEngine.h"
//
//#include "PhysicsConvexShape.h"
//
//#include "vehicle/PxVehicle.h"
//#include "vehicle/PxVehicleUtils.h"
//
//namespace he {
//namespace px {
//
//PhysicsCar::PhysicsCar()
//{  
//}
//
//void setVehicleGeometricData (const vec3& chassisDims, const vec3* const wheelCentreOffsets, 
//                              const float* const wheelWidths, const float* const wheelRadii, 
//                              physx::PxVehicle4WSimpleSetup& simpleSetup)
//{
//    //Chassis descriptor.
//    simpleSetup.mChassisDims = physx::PxVec3(chassisDims.x, chassisDims.y, chassisDims.z);
//    simpleSetup.mChassisCMOffset = physx::PxVec3(0.0f, -chassisDims.y * 0.5f + 0.65f, 0.25f);
//
//    //Wheels descriptor.
//    for(uint i(0); i < physx::PxVehicle4WSimulationData::eNUM_WHEELS; ++i)
//    {
//        simpleSetup.mWheelCentreCMOffsets[i] =  physx::PxVec3(wheelCentreOffsets[i].x, wheelCentreOffsets[i].y, wheelCentreOffsets[i].z) - simpleSetup.mChassisCMOffset;
//    }
//    simpleSetup.mFrontWheelWidth = wheelWidths[0];
//    simpleSetup.mFrontWheelRadius = wheelRadii[0];
//    simpleSetup.mRearWheelWidth = wheelWidths[2];
//    simpleSetup.mRearWheelRadius = wheelRadii[2];
//
//    //Suspension descriptor.
//    simpleSetup.mFrontSuspensionTravelDir = physx::PxVec3(0.0f, -1.0f, 0.0f);
//    simpleSetup.mRearSuspensionTravelDir = physx::PxVec3(0.0f, -1.0f, 0.0f);
//}
//
//void setVehicleMassData(const float chassisMass, const vec3& chassisMOI, physx::PxVehicle4WSimpleSetup& simpleSetup)
//{
//    simpleSetup.mChassisMass = chassisMass;
//    simpleSetup.mChassisMOI = physx::PxVec3(chassisMOI.x, chassisMOI.y, chassisMOI.z);
//    simpleSetup.mFrontWheelMass = 20.0f;
//    simpleSetup.mRearWheelMass = 20.0f;
//}
//
//vec3 computeChassisMOI(const vec3& chassisDims, const float chassisMass)
//{
//    //We can approximately work out the chassis moment of inertia from the aabb.
//    vec3 chassisMOI(chassisDims.y*chassisDims.y + chassisDims.z*chassisDims.z,
//                    chassisDims.x*chassisDims.x + chassisDims.z*chassisDims.z,
//                    chassisDims.x*chassisDims.x + chassisDims.y*chassisDims.y);
//    chassisMOI *= chassisMass * 0.0833f;
//    //Well, the AABB moi gives rather sluggish behaviour so lets let the car turn a bit quicker.
//    chassisMOI.y *= 0.8f;//
//    return chassisMOI;
//}
//
//void setOptionalVehicleData(physx::PxVehicle4WSimpleSetup& simpleSetup)
//{
//    //Tyres descriptor.
//    simpleSetup.mFrontTyreType = 0;
//    simpleSetup.mRearTyreType = 0;
//
//    //Diff descriptor.
//    simpleSetup.mDiffType = physx::PxVehicleDifferential4WData::eDIFF_TYPE_LS_REARWD;
//}
//
//physx::PxVehicle4W* createVehicle(const physx::PxVehicle4WSimulationData& vehSimulationData,
//                                  const PhysicsConvexShape* wheelConvexMeshes, 
//                                  const PhysicsConvexShape& chassisConvexMesh,
//                                  const PhysicsMaterial& material)
//{
//    //We need a rigid body actor for the vehicle.
//    //Don't forget to add the actor the scene after setting up the associated vehicle.
//    physx::PxRigidDynamic* vehActor = PHYSICS->getSDK()->createRigidDynamic(physx::PxTransform::createIdentity());
//
//    //We need to add wheel collision shapes, a material for the wheels, and a simulation filter for the wheels.
//    physx::PxConvexMeshGeometry frontLeftWheelGeom(wheelConvexMeshes[0].getInternalMesh());
//    physx::PxConvexMeshGeometry frontRightWheelGeom(wheelConvexMeshes[1].getInternalMesh());
//    physx::PxConvexMeshGeometry rearLeftWheelGeom(wheelConvexMeshes[2].getInternalMesh());
//    physx::PxConvexMeshGeometry rearRightWheelGeom(wheelConvexMeshes[3].getInternalMesh());
//    physx::PxMaterial& wheelMaterial(*material.getInternalMaterial());
//    physx::PxFilterData wheelCollFilterData;
//    wheelCollFilterData.word0 = PxFilter_CarWheel;
//    wheelCollFilterData.word1 = ~PxFilter_CarChassi;
//
//    //We need to add chassis collision shapes, their local poses, a material for the chassis, and a simulation filter for the chassis.
//    physx::PxConvexMeshGeometry chassisConvexGeom(chassisConvexMesh.getInternalMesh());
//    physx::PxGeometry* chassisGeoms[1] = { &chassisConvexGeom };
//
//    //We need to specify the local poses of the chassis composite shapes.
//    physx::PxTransform chassisLocalPoses[1] = { physx::PxTransform::createIdentity() };
//
//    physx::PxMaterial& chassisMaterial(*material.getInternalMaterial());
//    physx::PxFilterData chassisCollFilterData;
//    chassisCollFilterData.word0 = PxFilter_CarChassi;
//    chassisCollFilterData.word1 = ~PxFilter_CarWheel;
//
//    //Create a query filter data for the car to ensure that cars
//    //do not attempt to drive on themselves.
//    static physx::PxU32 vehIndex(0);
//    physx::PxFilterData vehQryFilterData;
//    physx::PxSetupVehicleShapeQueryFilterData(vehIndex++, &vehQryFilterData);
//
//    //Create a car.
//    physx::PxVehicle4W* car = NEW physx::PxVehicle4W();
//    physx::PxVehicle4WSetup(
//            vehSimulationData,
//            vehQryFilterData, vehActor,
//            frontLeftWheelGeom, frontRightWheelGeom, rearLeftWheelGeom, rearRightWheelGeom, &wheelMaterial, wheelCollFilterData,
//            chassisGeoms, chassisLocalPoses, 1, &chassisMaterial, chassisCollFilterData,
//            PHYSICS->getSDK(),
//            car);
//
//    //Don't forget to add the actor to the scene.
//    PHYSICS->getScene()->addActor(*vehActor);
//
//    //That's us finished.
//    return car;
//}
//
//void PhysicsCar::init()
//{
//    physx::PxVehicle4WSimpleSetup simpleSetup;
//    vec3 wheelOffset[4];
//    wheelOffset[0] = vec3(-0.687f, 0.339f, 1.346f);
//    wheelOffset[1] = vec3(0.697f, 0.339f, 1.346f);
//    wheelOffset[2] = vec3(-0.687f, 0.339f, -1.204f);
//    wheelOffset[3] = vec3(0.697f, 0.339f, -1.204f);
//    float wheelWidth[4];
//    wheelWidth[0] = 0.281f;
//    wheelWidth[1] = 0.281f;
//    wheelWidth[2] = 0.281f;
//    wheelWidth[3] = 0.281f;
//    float wheelR[4];
//    wheelR[0] = 0.335f;
//    wheelR[1] = 0.335f;
//    wheelR[2] = 0.335f;
//    wheelR[3] = 0.335f;
//    vec3 chassiDim(2.025f, 1.323f, 4.445f);
//    setVehicleGeometricData(chassiDim, wheelOffset, wheelWidth, wheelR, simpleSetup);
//
//    float chassiMass(1000);
//    vec3 moi(computeChassisMOI(chassiDim, chassiMass));
//    setVehicleMassData(chassiMass, moi, simpleSetup);
//
//    setOptionalVehicleData(simpleSetup);
//
//    const physx::PxVehicle4WSimulationData vehSimulationData(PxCreateVehicle4WSimulationData(simpleSetup));
//
//    //createVehicle(vehSimulationData,  
//
//
//    //physx::PxVehicleDrivableSurfaceType mVehicleDrivableSurfaceTypes[1];
//
//    ////Create a new material.
//    //PhysicsMaterial standardMaterial(0.9f, 0.7f, 0.1f);
//
//    ////Set up the drivable surface type that will be used for the new material.
//    //mVehicleDrivableSurfaceTypes[0].mType = 0;
//
//    ////Set the material user data to be the drivable surface data.
//    //mStandardMaterials[i]->userData = &mVehicleDrivableSurfaceTypes[i];
//
//}
//
//
//void suspensionRaycasts()
//{
//    //Create a scene query if we haven't already done so.
//    if(NULL == mSqWheelRaycastSceneQuery)
//    {
//        mSqWheelRaycastSceneQuery = physx::PxVehicle4WSetUpSceneQuery(PHYSICS->getScene(), mSqData);
//    }
//
//    //Raycasts.
//    physx::PxVehicle4WSuspensionRaycasts(mSqWheelRaycastSceneQuery,mNumVehicles,mSqData.mSqResults,mVehicles);
//}
//
//
//} } //end namespace