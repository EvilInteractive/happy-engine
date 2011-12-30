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
#include "HappyPCH.h" 

#include "PhysicsCar.h"
#include "HappyEngine.h"
#include "HeAssert.h"
#include "PhysicsEngine.h"

#include "PhysicsConvexShape.h"
#include "ControlsManager.h"
#include "PhysicsCarManager.h"


namespace he {
namespace px {

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



PhysicsCar::PhysicsCar(): m_Vehicle()
{  
}
PhysicsCar::~PhysicsCar()
{  
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
    chassisMOI *= chassisMass * 0.08f;
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

    simpleSetup.mEngineMaxOmega = 800;
}

void PhysicsCar::init(const ChassiDesc& chassisDesc, const PhysicsMaterial& chassiMaterial,
                      const std::vector<TyreDesc>& tyreDescs, const PhysicsMaterial& tyreMaterial)
{
    PHYSICS->lock();
    physx::PxVehicle4WSimpleSetup simpleSetup;
    vec3 wheelOffset[4];
    float wheelWidth[4];
    float wheelR[4];
    for (int i(0); i < 4; ++i)
    {
        wheelOffset[i] = tyreDescs[i].m_Offset;
        wheelWidth[i] = tyreDescs[i].m_Width;
        wheelR[i] = tyreDescs[i].m_Radius;
    }
    setVehicleGeometricData(chassisDesc.m_Aabb, wheelOffset, wheelWidth, wheelR, simpleSetup);

    vec3 moi(computeChassisMOI(chassisDesc.m_Aabb, chassisDesc.m_Mass));
    setVehicleMassData(chassisDesc.m_Mass, moi, simpleSetup);

    setOptionalVehicleData(simpleSetup);

    const physx::PxVehicle4WSimulationData vehSimulationData(PxCreateVehicle4WSimulationData(simpleSetup));

    //Create Vehicle
    
    //We need a rigid body actor for the vehicle.
    //Don't forget to add the actor the scene after setting up the associated vehicle.
    m_pActor = PHYSICS->getSDK()->createRigidDynamic(physx::PxTransform::createIdentity());

    //We need to add wheel collision shapes, a material for the wheels, and a simulation filter for the wheels.
    ASSERT(tyreDescs.size() == 4, "car needs 4 wheels!");
    physx::PxConvexMeshGeometry frontLeftWheelGeom(tyreDescs[Tyre_FrontLeft].m_Shape.getInternalMesh());
    physx::PxConvexMeshGeometry frontRightWheelGeom(tyreDescs[Tyre_FrontRight].m_Shape.getInternalMesh());
    physx::PxConvexMeshGeometry rearLeftWheelGeom(tyreDescs[Tyre_RearLeft].m_Shape.getInternalMesh());
    physx::PxConvexMeshGeometry rearRightWheelGeom(tyreDescs[Tyre_RearRight].m_Shape.getInternalMesh());
    physx::PxMaterial& wheelMaterial(*tyreMaterial.getInternalMaterial());
    physx::PxFilterData wheelCollFilterData;
    wheelCollFilterData.word0 = COLLISION_FLAG_WHEEL;
    wheelCollFilterData.word1 = COLLISION_FLAG_WHEEL_AGAINST;

    //We need to add chassis collision shapes, their local poses, a material for the chassis, and a simulation filter for the chassis.
    std::vector<const physx::PxGeometry*> chassispGeoms;
    chassispGeoms.reserve(chassisDesc.m_ChassiShapes.size());
    std::vector<physx::PxTransform> chassisLocalPoses;
    chassisLocalPoses.reserve(chassispGeoms.size());
    std::for_each(chassisDesc.m_ChassiShapes.cbegin(), chassisDesc.m_ChassiShapes.cend(), [&](const px::PhysicsConvexShape& pShape)
    {
        chassispGeoms.push_back(NEW physx::PxConvexMeshGeometry(pShape.getInternalMesh()));
        //We need to specify the local poses of the chassis composite shapes.
        chassisLocalPoses.push_back(physx::PxTransform::createIdentity());
    });


    physx::PxMaterial& chassisMaterial(*chassiMaterial.getInternalMaterial());
    physx::PxFilterData chassisCollFilterData;
    chassisCollFilterData.word0 = COLLISION_FLAG_CHASSIS;
    chassisCollFilterData.word1 = COLLISION_FLAG_CHASSIS_AGAINST;

    //Create a query filter data for the car to ensure that cars
    //do not attempt to drive on themselves.
    static physx::PxU32 vehIndex(1);
    physx::PxFilterData vehQryFilterData;
    physx::PxSetupVehicleShapeQueryFilterData(vehIndex++, &vehQryFilterData);

    //Create a car.
    physx::PxVehicle4WSetup(
            vehSimulationData,
            vehQryFilterData, m_pActor,
            frontLeftWheelGeom, frontRightWheelGeom, rearLeftWheelGeom, rearRightWheelGeom, &wheelMaterial, wheelCollFilterData,
            &chassispGeoms[0], &chassisLocalPoses[0], chassispGeoms.size(), &chassisMaterial, chassisCollFilterData,
            PHYSICS->getSDK(),
            &m_Vehicle);

    physx::PxVehicle4WSetUseAutoGears(true, m_Vehicle);

    //Don't forget to add the actor to the scene.
    PHYSICS->getScene()->addActor(*m_pActor);
    
    reset();

    std::for_each(chassispGeoms.cbegin(), chassispGeoms.cend(), [&](const physx::PxGeometry* pShape)
    {
        delete pShape;
    });

    m_TyreShape[Tyre_FrontLeft] = physx::PxVehicle4WGetFrontLeftWheelShape(m_Vehicle);
    m_TyreShape[Tyre_FrontRight] = physx::PxVehicle4WGetFrontRightWheelShape(m_Vehicle);
    m_TyreShape[Tyre_RearLeft] = physx::PxVehicle4WGetRearLeftWheelShape(m_Vehicle);
    m_TyreShape[Tyre_RearRight] = physx::PxVehicle4WGetRearRightWheelShape(m_Vehicle);

    PHYSICS->unlock();

    PHYSICS->getCarManager()->startCarSimulation(this);
}



void PhysicsCar::tick( float dTime )
{
    physx::PxVehicle4WSmoothDigitalRawInputsAndSetAnalogInputs(s_KeySmoothingData, s_SteerVsForwardSpeedTable, m_InputData, dTime, m_Vehicle);
}

void PhysicsCar::reset()
{
    physx::PxVehicle4WSetToRestState(m_Vehicle);
    physx::PxVehicle4WForceGearChange(physx::PxVehicleGearsData::eNEUTRAL, m_Vehicle);
}

//////////////////////////////////////////////////////////////////////////
/// SETTERS                                                            ///
//////////////////////////////////////////////////////////////////////////
void PhysicsCar::shiftGear( Gear gear )
{
    physx::PxVehicle4WForceGearChange(gear, m_Vehicle);
}

void PhysicsCar::setAutoTransmission( bool automatic )
{
    physx::PxVehicle4WSetUseAutoGears(automatic, m_Vehicle);
}

void PhysicsCar::inputAccel(bool accel)
{
    m_InputData.setDigitalAccel(accel);
}
void PhysicsCar::inputAccel( float val )
{
    m_InputData.setAnalogAccel(val);
}

void PhysicsCar::inputBrake(bool brake)
{
    m_InputData.setDigitalBrake(brake);
}
void PhysicsCar::inputBrake( float val )
{
    m_InputData.setAnalogBrake(val);
}

void PhysicsCar::inputTurnLeft(bool turn)
{
    m_InputData.setDigitalSteerRight(turn);
}
void PhysicsCar::inputTurnRight(bool turn)
{
    m_InputData.setDigitalSteerLeft(turn);
}
void PhysicsCar::inputTurn( float val )
{
    m_InputData.setAnalogSteer(val);
}

void PhysicsCar::inputGearUp(bool up)
{
    m_InputData.setGearUp(up);
}
void PhysicsCar::inputGearDown(bool down)
{
    m_InputData.setGearDown(down);
}
void PhysicsCar::inputHandBrake(bool brake)
{
    m_InputData.setDigitalHandbrake(brake);
}

void PhysicsCar::setPose( const mat44& pose )
{
    m_pActor->setGlobalPose(physx::PxTransform(pose.getPhyicsMatrix()));
}
void PhysicsCar::setAngularVelocity( const vec3& vel )
{
    m_pActor->setAngularVelocity(physx::PxVec3(vel.x, vel.y, vel.z));
}
void PhysicsCar::addForce( const vec3& force )
{
    m_pActor->addForce(physx::PxVec3(force.x, force.y, force.z));
}


//////////////////////////////////////////////////////////////////////////
///  GETTERS                                                           ///
//////////////////////////////////////////////////////////////////////////

he::mat44 PhysicsCar::getChassiPose() const
{
    return mat44(physx::PxMat44(physx::PxMat33(m_pActor->getGlobalPose().q), m_pActor->getGlobalPose().p));
}
he::mat44 PhysicsCar::getPose() const
{
    return mat44(physx::PxMat44(physx::PxMat33(m_pActor->getGlobalPose().q), m_pActor->getGlobalPose().p));
}
he::vec3 PhysicsCar::getPosition() const
{
    return vec3(m_pActor->getGlobalPose().p);
}

he::mat44 PhysicsCar::getTyrePose( Tyre tyre ) const
{
    return mat44(physx::PxMat44(physx::PxMat33(m_TyreShape[tyre]->getLocalPose().q), m_TyreShape[tyre]->getLocalPose().p));
}

float PhysicsCar::getSpeed() const
{
    return m_pActor->getLinearVelocity().magnitude();
}

PhysicsCar::Gear PhysicsCar::getGear() const
{
    return static_cast<Gear>(physx::PxVehicle4WGetCurrentGear(m_Vehicle));
}

bool PhysicsCar::isInAir() const
{
    return physx::PxVehicle4WIsInAir(m_Vehicle);
}

bool PhysicsCar::isDrifting() const
{
    for (uint tyre(0); tyre < MAX_TYRES; ++tyre)
    {
        if (physx::PxVehicle4WGetTyreLongSlip(m_Vehicle, tyre) > 0.9f)
            return true;
    }
    return false;
}
float PhysicsCar::getTyreLatSlip( Tyre tyre )
{
    return physx::PxVehicle4WGetTyreLatSlip(m_Vehicle, tyre);
}
float PhysicsCar::getTyreLongSlip( Tyre tyre )
{
    return physx::PxVehicle4WGetTyreLongSlip(m_Vehicle, tyre);
}

float PhysicsCar::getEngineRPM() const
{
    return physx::PxVehicle4WGetEngineRotationSpeed(m_Vehicle);
}

float PhysicsCar::getSteer() const
{
    return physx::PxVehicle4WGetAppliedSteer(m_Vehicle);
}

physx::PxRigidActor* PhysicsCar::getInternalActor() const
{
    return m_pActor;
}






} } //end namespace