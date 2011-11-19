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
//Created: 22/08/2011
//Updated: 14/09/2011 -added addForce/velocity and kinematic methods
#include "StdAfx.h" 

#include "PhysicsCar.h"
#include "HappyEngine.h"
#include "HeAssert.h"
#include "PhysicsEngine.h"

#include "PhysicsConvexShape.h"
#include "ControlsManager.h"


namespace he {
namespace px {

float PhysicsCar::s_TyreFrictionMultipliers[MAX_DRIVABLE_SURFACES][PhysicsCar::MAX_TYRE_TYPES] =
{
    { 1.0f,  1.1f }, //Grass
    { 1.0f,  1.1f }, //Sand
    { 1.05f,  1.15f }, //Concrete
    { 1.0f,  1.1f }  //Ice
};
physx::PxVehicleKeySmoothingData PhysicsCar::s_KeySmoothingData =
{
    3.0f,   //rise rate PX_VEHICLE_ANALOG_INPUT_ACCEL
    3.0f,   //rise rate PX_VEHICLE_ANALOG_INPUT_BRAKE
    10.0f,  //rise rate PX_VEHICLE_ANALOG_INPUT_HANDBRAKE
    2.5f,   //rise rate PX_VEHICLE_ANALOG_INPUT_STEER_LEFT
    2.5f,   //rise rate PX_VEHICLE_ANALOG_INPUT_STEER_RIGHT
    5.0f,   //fall rate PX_VEHICLE_ANALOG_INPUT_ACCEL
    5.0f,   //fall rate PX_VEHICLE_ANALOG_INPUT_BRAKE
    10.0f,  //fall rate PX_VEHICLE_ANALOG_INPUT_HANDBRAKE
    5.0f,   //fall rate PX_VEHICLE_ANALOG_INPUT_STEER_LEFT
    5.0f    //fall rate PX_VEHICLE_ANALOG_INPUT_STEER_RIGHT
};

physx::PxVehiclePadSmoothingData PhysicsCar::s_PadSmoothingData=
{
    6.0f,   //rise rate PX_VEHICLE_ANALOG_INPUT_ACCEL
    6.0f,   //rise rate PX_VEHICLE_ANALOG_INPUT_BRAKE
    12.0f,  //rise rate PX_VEHICLE_ANALOG_INPUT_HANDBRAKE
    2.5f,   //rise rate PX_VEHICLE_ANALOG_INPUT_STEER_LEFT
    0.0f,   //rise rate PX_VEHICLE_ANALOG_INPUT_STEER_RIGHT
    10.0f,  //fall rate PX_VEHICLE_ANALOG_INPUT_ACCEL
    10.0f,  //fall rate PX_VEHICLE_ANALOG_INPUT_BRAKE
    12.0f,  //fall rate PX_VEHICLE_ANALOG_INPUT_HANDBRAKE
    5.0f,   //fall rate PX_VEHICLE_ANALOG_INPUT_STEER_LEFT
    0.0f    //fall rate PX_VEHICLE_ANALOG_INPUT_STEER_RIGHT
};
float PhysicsCar::s_SteerVsForwardSpeedData[2*8]=
{
    0.0f,           0.75f,
    5.0f,           0.75f,
    30.0f,          0.125f,
    120.0f,         0.1f,
    PX_MAX_F32, PX_MAX_F32,
    PX_MAX_F32, PX_MAX_F32,
    PX_MAX_F32, PX_MAX_F32,
    PX_MAX_F32, PX_MAX_F32
};
physx::PxFixedSizeLookupTable<8> PhysicsCar::s_SteerVsForwardSpeedTable(s_SteerVsForwardSpeedData, 4);



PhysicsCar::PhysicsCar(): m_SqWheelRaycastSceneQuery(nullptr)
{  
}
PhysicsCar::~PhysicsCar()
{  
    delete m_pVehicle;
}

void setVehicleGeometricData (const vec3& chassisDims, const vec3* const wheelCentreOffsets, 
                              const float* const wheelWidths, const float* const wheelRadii, 
                              physx::PxVehicle4WSimpleSetup& simpleSetup)
{
    //Chassis descriptor.
    simpleSetup.mChassisDims = physx::PxVec3(chassisDims.x, chassisDims.y, chassisDims.z);
    simpleSetup.mChassisCMOffset = physx::PxVec3(0.0f, -chassisDims.y * 0.5f + 0.65f, 0.25f);

    //Wheels descriptor.
    for(uint i(0); i < physx::PxVehicle4WSimulationData::eNUM_WHEELS; ++i)
    {
        simpleSetup.mWheelCentreCMOffsets[i] =  physx::PxVec3(wheelCentreOffsets[i].x, wheelCentreOffsets[i].y, wheelCentreOffsets[i].z) - simpleSetup.mChassisCMOffset;
    }
    simpleSetup.mFrontWheelWidth = wheelWidths[0];
    simpleSetup.mFrontWheelRadius = wheelRadii[0];
    simpleSetup.mRearWheelWidth = wheelWidths[2];
    simpleSetup.mRearWheelRadius = wheelRadii[2];

    //Suspension descriptor.
    simpleSetup.mFrontSuspensionTravelDir = physx::PxVec3(0.0f, -1.0f, 0.0f);
    simpleSetup.mRearSuspensionTravelDir = physx::PxVec3(0.0f, -1.0f, 0.0f);
    //simpleSetup.mFrontSuspensionMaxCompression = 0.6f;
    //simpleSetup.mFrontSuspensionSpringStrength = 75000;
    //simpleSetup.mRearSuspensionMaxCompression = 0.6f;
    //simpleSetup.mRearSuspensionSpringStrength = 75000;
}

void setVehicleMassData(const float chassisMass, const vec3& chassisMOI, physx::PxVehicle4WSimpleSetup& simpleSetup)
{
    simpleSetup.mChassisMass = chassisMass;
    simpleSetup.mChassisMOI = physx::PxVec3(chassisMOI.x, chassisMOI.y, chassisMOI.z);
    simpleSetup.mFrontWheelMass = 20.0f;
    simpleSetup.mRearWheelMass = 20.0f;
}

vec3 computeChassisMOI(const vec3& chassisDims, const float chassisMass)
{
    //We can approximately work out the chassis moment of inertia from the aabb.
    vec3 chassisMOI(chassisDims.y*chassisDims.y + chassisDims.z*chassisDims.z,
                    chassisDims.x*chassisDims.x + chassisDims.z*chassisDims.z,
                    chassisDims.x*chassisDims.x + chassisDims.y*chassisDims.y);
    chassisMOI *= chassisMass * 0.0833f;
    //Well, the AABB moi gives rather sluggish behaviour so lets let the car turn a bit quicker.
    chassisMOI.y *= 0.8f;//
    return chassisMOI;
}

void setOptionalVehicleData(physx::PxVehicle4WSimpleSetup& simpleSetup)
{
    //Tyres descriptor.
    simpleSetup.mFrontTyreType = 0;
    simpleSetup.mRearTyreType = 0;

    //Diff descriptor.
    simpleSetup.mDiffType = physx::PxVehicleDifferential4WData::eDIFF_TYPE_LS_4WD;
}

void PhysicsCar::init(
          const std::vector<PhysicsConvexShape>& wheelConvexMeshes, 
          const std::vector<PhysicsConvexShape>& chassisConvexMeshes,
          const PhysicsMaterial& material)
{
    physx::PxVehicle4WSimpleSetup simpleSetup;
    vec3 wheelOffset[4];
    wheelOffset[0] = vec3(0.691f, -0.314f*2.5f, 1.348f);
    wheelOffset[1] = vec3(-0.691f, -0.314f*2.5f, 1.348f);
    wheelOffset[2] = vec3(0.691f, -0.314f*2.5f, -1.203f);
    wheelOffset[3] = vec3(-0.691f, -0.314f*2.5f, -1.203f);
    float wheelWidth[4];
    wheelWidth[0] = 0.281f;
    wheelWidth[1] = 0.281f;
    wheelWidth[2] = 0.281f;
    wheelWidth[3] = 0.281f;
    float wheelR[4];
    wheelR[0] = 0.335f;
    wheelR[1] = 0.335f;
    wheelR[2] = 0.335f;
    wheelR[3] = 0.335f;
    vec3 chassiDim(2.025f, 1.323f, 4.445f);
    setVehicleGeometricData(chassiDim, wheelOffset, wheelWidth, wheelR, simpleSetup);

    float chassiMass(2000);
    vec3 moi(computeChassisMOI(chassiDim, chassiMass));
    setVehicleMassData(chassiMass, moi, simpleSetup);

    setOptionalVehicleData(simpleSetup);

    const physx::PxVehicle4WSimulationData vehSimulationData(PxCreateVehicle4WSimulationData(simpleSetup));

    //Create Vehicle
    
    //We need a rigid body actor for the vehicle.
    //Don't forget to add the actor the scene after setting up the associated vehicle.
    m_pActor = PHYSICS->getSDK()->createRigidDynamic(physx::PxTransform::createIdentity());

    //We need to add wheel collision shapes, a material for the wheels, and a simulation filter for the wheels.
    ASSERT(wheelConvexMeshes.size() == 4, "car needs 4 wheels!");
    physx::PxConvexMeshGeometry frontLeftWheelGeom(wheelConvexMeshes[0].getInternalMesh());
    physx::PxConvexMeshGeometry frontRightWheelGeom(wheelConvexMeshes[1].getInternalMesh());
    physx::PxConvexMeshGeometry rearLeftWheelGeom(wheelConvexMeshes[2].getInternalMesh());
    physx::PxConvexMeshGeometry rearRightWheelGeom(wheelConvexMeshes[3].getInternalMesh());
    physx::PxMaterial& wheelMaterial(*material.getInternalMaterial());
    physx::PxFilterData wheelCollFilterData;
    wheelCollFilterData.word0 = COLLISION_FLAG_WHEEL;
    wheelCollFilterData.word1 = COLLISION_FLAG_WHEEL_AGAINST;

    //We need to add chassis collision shapes, their local poses, a material for the chassis, and a simulation filter for the chassis.
    std::vector<const physx::PxGeometry*> chassispGeoms;
    chassispGeoms.reserve(chassisConvexMeshes.size());
    std::vector<physx::PxTransform> chassisLocalPoses;
    chassisLocalPoses.reserve(chassispGeoms.size());
    std::for_each(chassisConvexMeshes.cbegin(), chassisConvexMeshes.cend(), [&](const px::PhysicsConvexShape& pShape)
    {
        chassispGeoms.push_back(NEW physx::PxConvexMeshGeometry(pShape.getInternalMesh()));
        //We need to specify the local poses of the chassis composite shapes.
        chassisLocalPoses.push_back(physx::PxTransform::createIdentity());
    });


    physx::PxMaterial& chassisMaterial(*material.getInternalMaterial());
    physx::PxFilterData chassisCollFilterData;
    chassisCollFilterData.word0 = COLLISION_FLAG_CHASSIS;
    chassisCollFilterData.word1 = COLLISION_FLAG_CHASSIS_AGAINST;

    //Create a query filter data for the car to ensure that cars
    //do not attempt to drive on themselves.
    static physx::PxU32 vehIndex(1);
    physx::PxFilterData vehQryFilterData;
    physx::PxSetupVehicleShapeQueryFilterData(vehIndex++, &vehQryFilterData);

    //Create a car.
    m_pVehicle = NEW physx::PxVehicle4W();
    physx::PxVehicle4WSetup(
            vehSimulationData,
            vehQryFilterData, m_pActor,
            frontLeftWheelGeom, frontRightWheelGeom, rearLeftWheelGeom, rearRightWheelGeom, &wheelMaterial, wheelCollFilterData,
            &chassispGeoms[0], &chassisLocalPoses[0], chassispGeoms.size(), &chassisMaterial, chassisCollFilterData,
            PHYSICS->getSDK(),
            m_pVehicle);
    physx::PxVehicle4WSetUseAutoGears(true, *m_pVehicle);

    //Don't forget to add the actor to the scene.
    PHYSICS->getScene()->addActor(*m_pActor);

    m_pActor->setGlobalPose(physx::PxTransform(physx::PxVec3(-10, 25, 0)));

    physx::PxVehicle4WSetToRestState(*m_pVehicle);
    physx::PxVehicle4WForceGearChange(physx::PxVehicleGearsData::eFIRST, *m_pVehicle);


    

    std::for_each(chassispGeoms.cbegin(), chassispGeoms.cend(), [&](const physx::PxGeometry* pShape)
    {
        delete pShape;
    });

    m_TyreShape[Tyre_FrontLeft] = physx::PxVehicle4WGetFrontLeftWheelShape(*m_pVehicle);
    m_TyreShape[Tyre_FrontRight] = physx::PxVehicle4WGetFrontRightWheelShape(*m_pVehicle);
    m_TyreShape[Tyre_BackLeft] = physx::PxVehicle4WGetRearLeftWheelShape(*m_pVehicle);
    m_TyreShape[Tyre_BackRight] = physx::PxVehicle4WGetRearRightWheelShape(*m_pVehicle);

    physx::PxVehicle4WGetChassisShapes(*m_pVehicle, &m_pChassiShape, 1);
}

void PhysicsCar::tick( float dTime )
{
    if (CONTROLS->getKeyboard()->isKeyPressed(io::Key_R))
    {
        m_pActor->setGlobalPose(physx::PxTransform(m_pActor->getGlobalPose().p + physx::PxVec3(0, 2, 0)));

        physx::PxVehicle4WSetToRestState(*m_pVehicle);
        physx::PxVehicle4WForceGearChange(physx::PxVehicleGearsData::eFIRST, *m_pVehicle);
    }
    //Controls
    physx::PxVehicleRawInputData rawInputData;
    rawInputData.setDigitalAccel(CONTROLS->getKeyboard()->isKeyDown(io::Key_Up));
    rawInputData.setDigitalBrake(CONTROLS->getKeyboard()->isKeyDown(io::Key_Down));
    rawInputData.setDigitalHandbrake(CONTROLS->getKeyboard()->isKeyDown(io::Key_Lctrl));
    rawInputData.setDigitalSteerLeft(CONTROLS->getKeyboard()->isKeyDown(io::Key_Right));
    rawInputData.setDigitalSteerRight(CONTROLS->getKeyboard()->isKeyDown(io::Key_Left));
    rawInputData.setGearUp(CONTROLS->getKeyboard()->isKeyPressed(io::Key_PageUp));
    rawInputData.setGearDown(CONTROLS->getKeyboard()->isKeyPressed(io::Key_PageDown));

    physx::PxVehicle4WSmoothDigitalRawInputsAndSetAnalogInputs(s_KeySmoothingData, s_SteerVsForwardSpeedTable, rawInputData, dTime, *m_pVehicle);
    //RayCast

    //Create a scene query if we haven't already done so.
    if(m_SqWheelRaycastSceneQuery == nullptr)
    {
        m_SqWheelRaycastSceneQuery = physx::PxVehicle4WSetUpSceneQuery(PHYSICS->getScene(), m_SqData);
    }

    //Raycasts.
    physx::PxVehicle4WSuspensionRaycasts(m_SqWheelRaycastSceneQuery, 1, m_SqData.mSqResults, &m_pVehicle);


    physx::PxVehicleDrivableSurfaceToTyreFrictionPairs surfaceTyrePairs;
    surfaceTyrePairs.mPairs= &s_TyreFrictionMultipliers[0][0];
    surfaceTyrePairs.mNumSurfaceTypes= MAX_DRIVABLE_SURFACES;
    surfaceTyrePairs.mNumTyreTypes= MAX_TYRE_TYPES;
    PxVehicle4WUpdate(dTime, physx::PxVec3(0.0f, -9.81f, 0.0f), surfaceTyrePairs, 1, &m_pVehicle);
}

he::mat44 PhysicsCar::getChassiPose() const
{
    return mat44(physx::PxMat44(physx::PxMat33(m_pActor->getGlobalPose().q), m_pActor->getGlobalPose().p));
}

he::mat44 PhysicsCar::getTyrePose( Tyre tyre ) const
{
    return mat44(physx::PxMat44(physx::PxMat33(m_TyreShape[tyre]->getLocalPose().q), m_TyreShape[tyre]->getLocalPose().p));
}



} } //end namespace