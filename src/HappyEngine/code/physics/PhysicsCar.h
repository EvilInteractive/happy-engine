////HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
////Created: 11/11/2011
//
//#ifndef _HE_PHYSICS_CAR_ACTOR_H_
//#define _HE_PHYSICS_CAR_ACTOR_H_
//#pragma once
//
//#include "PxRigidDynamic.h"
//#include "vec3.h"
//#include "mat44.h"
//#include "IPhysicsShape.h"
//#include "PhysicsMaterial.h"
//#include <vector>
//#include "vehicle/PxVehicleDrive.h"
//#include "PhysicsConvexShape.h"
//#include "PhysicsEngine.h"
//#include "IPhysicsActor.h"
//
//namespace he {
//namespace px {
//
//class PhysicsCarManager;
//
//struct TyreDesc
//{
//    vec3 m_Offset;
//    float m_Radius;
//    float m_Width;
//    float m_Mass;
//    PhysicsConvexShape m_Shape;
//
//    TyreDesc(const vec3& offset, float radius, float width, float mass, const PhysicsConvexShape& shape):
//        m_Offset(offset), m_Radius(radius), m_Width(width), m_Mass(mass), m_Shape(shape)
//    {}
//    TyreDesc():
//        m_Offset(), m_Radius(0), m_Width(0), m_Mass(0), m_Shape()
//    {}
//};
//struct ChassiDesc
//{
//    vec3 m_Aabb;
//    float m_Mass;
//    std::vector<PhysicsConvexShape> m_ChassiShapes;
//};
//class PhysicsCar : public IPhysicsActor
//{
//friend class PhysicsCarManager;
//
//public:
//    enum Tyre
//    {
//        Tyre_FrontLeft = 0,
//        Tyre_FrontRight,
//        Tyre_RearLeft,
//        Tyre_RearRight,
//        MAX_TYRES
//    };
//    enum Gear
//    {
//        Gear_R = 0,
//        Gear_N,
//        Gear_1,
//        Gear_2,
//        Gear_3,
//        Gear_4,
//        Gear_5,
//        Gear_6,
//    };
//
//
//    void init(const ChassiDesc& chassisDesc, const PhysicsMaterial& chassiMaterial,
//              const std::vector<TyreDesc>& tyreDescs, const PhysicsMaterial& tyreMaterial);
//
//
//    void tick(float dTime);
//
//    void reset();
//
//    //////////////////////////////////////////////////////////////////////////
//    /// SETTERS
//    //////////////////////////////////////////////////////////////////////////
//    void shiftGear(Gear gear);
//    void setAutoTransmission(bool automatic);
//
//    void inputAccel(bool accel);
//    void inputBrake(bool brake);
//    void inputTurnLeft(bool turn);
//    void inputTurnRight(bool turn);
//    void inputGearUp(bool up);
//    void inputGearDown(bool down);
//    void inputHandBrake(bool brake);
//
//    void inputAccel(float val);
//    void inputBrake(float val);
//    void inputTurn(float val);
//
//    void setPose(const mat44& pose);
//    void setAngularVelocity(const vec3& vel);
//    void addForce(const vec3& force);
//
//    //////////////////////////////////////////////////////////////////////////
//    /// GETTERS
//    //////////////////////////////////////////////////////////////////////////
//    mat44 getTyrePose(Tyre tyre) const;
//    mat44 getChassiPose() const;
//
//    vec3 getPosition() const;
//    mat44 getPose() const;
//    virtual physx::PxRigidActor* getInternalActor() const;
//
//    float getSpeed() const;
//    Gear getGear() const;
//    bool isInAir() const;
//
//    float getEngineRPM() const;
//    float getSteer() const;
//
//    bool isDrifting() const;
//    float getTyreLatSlip(Tyre tyre);
//    float getTyreLongSlip(Tyre tyre);
//
//private:
//    PhysicsCar();
//    virtual ~PhysicsCar();
//
//    physx::PxRigidDynamic* m_pActor;
//    physx::PxVehicleDrive4W m_Vehicle;
//
//    physx::PxShape* m_TyreShape[MAX_TYRES];
//
//    physx::PxVehicleDrive4WRawInputData m_InputData;
//
//    //STATIC
//    static physx::PxVehicleKeySmoothingData s_KeySmoothingData;
//    static physx::PxVehiclePadSmoothingData s_PadSmoothingData;
//    static float s_SteerVsForwardSpeedData[2*8];
//    static physx::PxFixedSizeLookupTable<8> s_SteerVsForwardSpeedTable;
//
//
//    //Disable default copy constructor and default assignment operator
//    PhysicsCar(const PhysicsCar&);
//    PhysicsCar& operator=(const PhysicsCar&);
//};
//
//} } //end namespace
//
//#endif
