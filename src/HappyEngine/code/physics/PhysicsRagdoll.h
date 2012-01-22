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
//Created: 20/01/2012

#ifndef _HE_PHYSICS_RAGDOLL_H_
#define _HE_PHYSICS_RAGDOLL_H_
#pragma once

namespace physx {
    class PxD6Joint;    
    class PxAggregate;
}

namespace he {
namespace px {

class PhysicsDynamicActor;
class PhysicsMaterial;

struct BodyPart
{
    mat44 bonePose;
    mat44 capsulePose; //capsules are created along x, center origin, this is the offset from bonePose
    float length;
    float radius;
    float mass;

    BodyPart(const mat44& p_BonePose, const mat44& p_CapsulePose, float p_Radius, float p_Length, float p_Mass):
    bonePose(p_BonePose), capsulePose(p_CapsulePose), length(p_Length), radius(p_Radius), mass(p_Mass)
    {}
    BodyPart() {}
};

struct RagdollDesc
{
    BodyPart armL1;
    BodyPart armL2;
    BodyPart handL;

    BodyPart armR1;
    BodyPart armR2;
    BodyPart handR;

    BodyPart head;
    BodyPart neck;

    BodyPart shoulderL;
    BodyPart shoulderR;

    BodyPart spine1;
    BodyPart spine2;
    BodyPart spine3;
    BodyPart pelvis;

    BodyPart legL1;
    BodyPart legL2;
    BodyPart footL;

    BodyPart legR1;
    BodyPart legR2;
    BodyPart footR;
};

class PhysicsRagdoll
{
public:
    PhysicsRagdoll();
    virtual ~PhysicsRagdoll();

    void initProperties(const vec3& position, const RagdollDesc& desc, const PhysicsMaterial& material);

    PhysicsDynamicActor* getArmL1() const;
    PhysicsDynamicActor* getArmL2() const;
    PhysicsDynamicActor* getHandL() const;

    PhysicsDynamicActor* getArmR1() const;
    PhysicsDynamicActor* getArmR2() const;
    PhysicsDynamicActor* getHandR() const;

    PhysicsDynamicActor* getHead() const;
    PhysicsDynamicActor* getNeck() const;

    PhysicsDynamicActor* getShoulderL() const;
    PhysicsDynamicActor* getShoulderR() const;

    PhysicsDynamicActor* getSpine1() const;
    PhysicsDynamicActor* getSpine2() const;
    PhysicsDynamicActor* getSpine3() const;
    PhysicsDynamicActor* getPelvis() const;

    PhysicsDynamicActor* getLegL1() const;
    PhysicsDynamicActor* getLegL2() const;
    PhysicsDynamicActor* getFootL() const;

    PhysicsDynamicActor* getLegR1() const;
    PhysicsDynamicActor* getLegR2() const;
    PhysicsDynamicActor* getFootR() const;

private:
    vec3 m_StartPosition;

    PhysicsDynamicActor* m_pArmL1;
    PhysicsDynamicActor* m_pArmL2;
    PhysicsDynamicActor* m_pHandL;

    PhysicsDynamicActor* m_pArmR1;
    PhysicsDynamicActor* m_pArmR2;
    PhysicsDynamicActor* m_pHandR;

    PhysicsDynamicActor* m_pHead;
    PhysicsDynamicActor* m_pNeck;

    PhysicsDynamicActor* m_pShoulderL;
    PhysicsDynamicActor* m_pShoulderR;

    PhysicsDynamicActor* m_pSpine1;
    PhysicsDynamicActor* m_pSpine2;
    PhysicsDynamicActor* m_pSpine3;
    PhysicsDynamicActor* m_pPelvis;

    PhysicsDynamicActor* m_pLegL1;
    PhysicsDynamicActor* m_pLegL2;
    PhysicsDynamicActor* m_pFootL;

    PhysicsDynamicActor* m_pLegR1;
    PhysicsDynamicActor* m_pLegR2;
    PhysicsDynamicActor* m_pFootR;

    physx::PxD6Joint* m_pPelvisSpine3Joint;
    physx::PxD6Joint* m_pSpine3Spine2Joint;
    physx::PxD6Joint* m_pSpine2Spine1Joint;
    physx::PxD6Joint* m_pSpine1ShoulderLJoint;
    physx::PxD6Joint* m_pSpine1ShoulderRJoint;
    physx::PxD6Joint* m_pSpine1NeckJoint;
    physx::PxD6Joint* m_pNeckHeadJoint;

    physx::PxD6Joint* m_pShouldersArmL1Joint;
    physx::PxD6Joint* m_pArmL1ArmL2Joint;
    physx::PxD6Joint* m_pArmL2HandJoint;

    physx::PxD6Joint* m_pShouldersArmR1Joint;
    physx::PxD6Joint* m_pArmR1ArmR2Joint;
    physx::PxD6Joint* m_pArmR2HandJoint;

    physx::PxD6Joint* m_pPelvisLegL1Joint;
    physx::PxD6Joint* m_pLegL1LegL2Joint;
    physx::PxD6Joint* m_pLegL2FootJoint;

    physx::PxD6Joint* m_pPelvisLegR1Joint;
    physx::PxD6Joint* m_pLegR1LegR2Joint;
    physx::PxD6Joint* m_pLegR2FootJoint;

    physx::PxAggregate* m_pAggregate;

    void addBodyPart(PhysicsDynamicActor** ppActor, const BodyPart& part, const PhysicsMaterial& material);

    //Disable default copy constructor and default assignment operator
    PhysicsRagdoll(const PhysicsRagdoll&);
    PhysicsRagdoll& operator=(const PhysicsRagdoll&);
};

} } //end namespace

#endif
