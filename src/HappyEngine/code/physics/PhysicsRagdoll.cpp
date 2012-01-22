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
#include "HappyPCH.h" 

#include "PhysicsRagdoll.h"

#include "PhysicsDynamicActor.h"
#include "PhysicsCapsuleShape.h"

#include "PhysicsEngine.h"

namespace he {
namespace px {

PhysicsRagdoll::PhysicsRagdoll():
m_pArmL1(nullptr), m_pArmL2(nullptr), m_pHandL(nullptr), m_pArmR1(nullptr), m_pArmR2(nullptr), 
m_pHandR(nullptr), m_pHead(nullptr), m_pNeck(nullptr), m_pShoulderL(nullptr), m_pShoulderR(nullptr), m_pSpine1(nullptr), 
m_pSpine2(nullptr), m_pSpine3(nullptr), m_pPelvis(nullptr), m_pLegL1(nullptr), m_pLegL2(nullptr), m_pFootL(nullptr), 
m_pLegR1(nullptr), m_pLegR2(nullptr), m_pFootR(nullptr), 
m_pPelvisSpine3Joint(nullptr), m_pSpine3Spine2Joint(nullptr), m_pSpine2Spine1Joint(nullptr), 
m_pSpine1ShoulderLJoint(nullptr), m_pSpine1ShoulderRJoint(nullptr), 
m_pSpine1NeckJoint(nullptr), m_pNeckHeadJoint(nullptr), m_pShouldersArmL1Joint(nullptr), 
m_pArmL1ArmL2Joint(nullptr), m_pArmL2HandJoint(nullptr), m_pShouldersArmR1Joint(nullptr), 
m_pArmR1ArmR2Joint(nullptr), m_pArmR2HandJoint(nullptr), m_pPelvisLegL1Joint(nullptr), 
m_pLegL1LegL2Joint(nullptr), m_pLegL2FootJoint(nullptr), m_pPelvisLegR1Joint(nullptr), 
m_pLegR1LegR2Joint(nullptr), m_pLegR2FootJoint(nullptr)
{
}


PhysicsRagdoll::~PhysicsRagdoll()
{
    delete m_pArmL1;
    delete m_pArmL2;
    delete m_pHandL;

    delete m_pArmR1;
    delete m_pArmR2;
    delete m_pHandR;

    delete m_pHead;
    delete m_pNeck;

    delete m_pShoulderL;
    delete m_pShoulderR;

    delete m_pSpine1;
    delete m_pSpine2;
    delete m_pSpine3;
    delete m_pPelvis;

    delete m_pLegL1;
    delete m_pLegL2;
    delete m_pFootL;

    delete m_pLegR1;
    delete m_pLegR2;
    delete m_pFootR;

    m_pPelvisSpine3Joint->release();
    m_pSpine3Spine2Joint->release();
    m_pSpine2Spine1Joint->release();
    m_pSpine1ShoulderLJoint->release();
    m_pSpine1ShoulderRJoint->release();
    m_pSpine1NeckJoint->release();
    m_pNeckHeadJoint->release();

    m_pShouldersArmL1Joint->release();
    m_pArmL1ArmL2Joint->release();
    m_pArmL2HandJoint->release();

    m_pShouldersArmR1Joint->release();
    m_pArmR1ArmR2Joint->release();
    m_pArmR2HandJoint->release();

    m_pPelvisLegL1Joint->release();
    m_pLegL1LegL2Joint->release();
    m_pLegL2FootJoint->release();

    m_pPelvisLegR1Joint->release();
    m_pLegR1LegR2Joint->release();
    m_pLegR2FootJoint->release();

    m_pAggregate->release();
}

void PhysicsRagdoll::addBodyPart(PhysicsDynamicActor** ppActor, const BodyPart& part, const PhysicsMaterial& material)
{
    (*ppActor) = new PhysicsDynamicActor(mat44::createTranslation(m_StartPosition) * part.bonePose);
    PHYSICS->getScene()->removeActor(*(*ppActor)->getInternalActor());
    m_pAggregate->addActor(*(*ppActor)->getInternalActor());
    (*ppActor)->getInternalActor()->setSolverIterationCounts(8, 2);
    PhysicsCapsuleShape pelvisShape(part.radius / 2.0f, part.length);
    (*ppActor)->addShape(&pelvisShape, material, part.mass, part.capsulePose);
}

void addJoint(physx::PxD6Joint** ppJoint, 
              const PhysicsDynamicActor* pActor1, const PhysicsDynamicActor* pActor2,
              const BodyPart& part1, const BodyPart& part2,
              physx::PxD6Motion::Enum lockTwist, physx::PxD6Motion::Enum lockSwing1, physx::PxD6Motion::Enum lockSwing2,
              float twistLimitMin, float twistLimitMax,
              float swingLimit1, float swingLimit2)
{
    (*ppJoint) = physx::PxD6JointCreate(*PHYSICS->getSDK(), 
        pActor1->getInternalActor(), physx::PxTransform(
        (part1.bonePose.inverse() * part2.bonePose).getPhyicsMatrix()),
        pActor2->getInternalActor(), physx::PxTransform::createIdentity());
    (*ppJoint)->setMotion(physx::PxD6Axis::eTWIST, lockTwist);
    (*ppJoint)->setMotion(physx::PxD6Axis::eSWING1, lockSwing1);
    (*ppJoint)->setMotion(physx::PxD6Axis::eSWING2, lockSwing2);

    if (lockSwing1 == physx::PxD6Motion::eLIMITED || lockSwing2 == physx::PxD6Motion::eLIMITED)
    {
        physx::PxJointLimitCone limit(swingLimit1, swingLimit2, 0.01f);
        //limit.spring = 1000000;
        //limit.damping = 1000000;
        (*ppJoint)->setSwingLimit(limit);
    }
    if (lockTwist == physx::PxD6Motion::eLIMITED)
        (*ppJoint)->setTwistLimit(physx::PxJointLimitPair(twistLimitMin, twistLimitMax, 0.01f));
}

void PhysicsRagdoll::initProperties(const vec3& position, const RagdollDesc& desc, const PhysicsMaterial& material )
{
    m_StartPosition = position;
    m_pAggregate = PHYSICS->getSDK()->createAggregate(20, false);

    addBodyPart(&m_pArmL1, desc.armL1, material);
    addBodyPart(&m_pArmL2, desc.armL2, material);
    addBodyPart(&m_pHandL, desc.handL, material);

    addBodyPart(&m_pArmR1, desc.armR1, material);
    addBodyPart(&m_pArmR2, desc.armR2, material);
    addBodyPart(&m_pHandR, desc.handR, material);

    addBodyPart(&m_pHead, desc.head, material);
    addBodyPart(&m_pNeck, desc.neck, material);

    addBodyPart(&m_pShoulderL, desc.shoulderL, material);
    addBodyPart(&m_pShoulderR, desc.shoulderR, material);

    addBodyPart(&m_pSpine1, desc.spine1, material);
    addBodyPart(&m_pSpine2, desc.spine2, material);
    addBodyPart(&m_pSpine3, desc.spine3, material);
    addBodyPart(&m_pPelvis, desc.pelvis, material);

    addBodyPart(&m_pLegL1, desc.legL1, material);
    addBodyPart(&m_pLegL2, desc.legL2, material);
    addBodyPart(&m_pFootL, desc.footL, material);

    addBodyPart(&m_pLegR1, desc.legR1, material);
    addBodyPart(&m_pLegR2, desc.legR2, material);
    addBodyPart(&m_pFootR, desc.footR, material);


    //Spine
    addJoint(&m_pPelvisSpine3Joint, m_pPelvis, m_pSpine3, desc.pelvis, desc.spine3,  
              physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED,
              -pi / 16.0f, pi / 16.0f, piOverFour, piOverFour);
    addJoint(&m_pSpine3Spine2Joint, m_pSpine3, m_pSpine2, desc.spine3, desc.spine2,  
        physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED,
        -pi / 8.0f, pi / 8.0f, piOverFour, piOverFour);
    addJoint(&m_pSpine2Spine1Joint, m_pSpine2, m_pSpine1, desc.spine2, desc.spine1,  
        physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED,
        -pi / 4.0f, pi / 4.0f, piOverFour, piOverFour);

    //Shoulders
    addJoint(&m_pSpine1ShoulderLJoint, m_pSpine1, m_pShoulderL, desc.spine1, desc.shoulderL,  
        physx::PxD6Motion::eLOCKED, physx::PxD6Motion::eLOCKED, physx::PxD6Motion::eLOCKED,
        0, 0, pi / 6.0f, pi / 9.0f);
    addJoint(&m_pSpine1ShoulderRJoint, m_pSpine1, m_pShoulderR, desc.spine1, desc.shoulderR,  
        physx::PxD6Motion::eLOCKED, physx::PxD6Motion::eLOCKED, physx::PxD6Motion::eLOCKED,
        0, 0, pi / 6.0f, pi / 9.0f);

    //Neck/Head
    addJoint(&m_pSpine1NeckJoint, m_pSpine1, m_pNeck, desc.spine1, desc.neck,
        physx::PxD6Motion::eLOCKED, physx::PxD6Motion::eLOCKED, physx::PxD6Motion::eLOCKED,
        0, 0, pi/6, pi/6);
    addJoint(&m_pNeckHeadJoint, m_pNeck, m_pHead, desc.neck, desc.head,
        physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED,
        -piOverTwo, piOverTwo, pi/6, pi/6);
    
    //Arms
    addJoint(&m_pShouldersArmL1Joint, m_pShoulderL, m_pArmL1, desc.shoulderL, desc.armL1,  
        physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED,
        -pi / 6.0f, pi / 6.0f, piOverTwo, piOverTwo);
    addJoint(&m_pShouldersArmR1Joint, m_pShoulderR, m_pArmR1, desc.shoulderR, desc.armR1,  
        physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED,
        -pi / 6.0f, pi / 6.0f, piOverTwo, piOverTwo);

    addJoint(&m_pArmL1ArmL2Joint, m_pArmL1, m_pArmL2, desc.armL1, desc.armL2,  
        physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLOCKED,
        -pi / 8.0f, pi / 8.0f, pi / 3, pi / 3);
    addJoint(&m_pArmR1ArmR2Joint, m_pArmR1, m_pArmR2, desc.armR1, desc.armR2,  
        physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLOCKED,
        -pi / 8.0f, pi / 8.0f, pi / 3, pi / 3);

    addJoint(&m_pArmL2HandJoint, m_pArmL2, m_pHandL, desc.armL2, desc.handL,  
        physx::PxD6Motion::eLOCKED, physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED,
        0, 0, piOverFour, piOverTwo);
    addJoint(&m_pArmR2HandJoint, m_pArmR2, m_pHandR, desc.armR2, desc.handR,  
        physx::PxD6Motion::eLOCKED, physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED,
        0, 0, piOverFour, piOverTwo);

    //Legs
    addJoint(&m_pPelvisLegL1Joint, m_pPelvis, m_pLegL1, desc.pelvis, desc.legL1,  
        physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED,
        -pi / 6.0f, pi / 6.0f, piOverTwo, piOverTwo);
    addJoint(&m_pPelvisLegR1Joint, m_pPelvis, m_pLegR1, desc.pelvis, desc.legR1,  
        physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED,
        -pi / 6.0f, pi / 6.0f, piOverTwo, piOverTwo);

    addJoint(&m_pLegL1LegL2Joint, m_pLegL1, m_pLegL2, desc.legL1, desc.legL2,  
        physx::PxD6Motion::eLOCKED, physx::PxD6Motion::eLOCKED, physx::PxD6Motion::eLIMITED,
        0, 0, pi / 3, pi / 3);
    addJoint(&m_pLegR1LegR2Joint, m_pLegR1, m_pLegR2, desc.legR1, desc.legR2,  
        physx::PxD6Motion::eLOCKED, physx::PxD6Motion::eLOCKED, physx::PxD6Motion::eLIMITED,
        0, 0, pi / 3, pi / 3);

    addJoint(&m_pLegL2FootJoint, m_pLegL2, m_pFootL, desc.legL2, desc.footL,  
        physx::PxD6Motion::eLOCKED, physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED,
        0, 0, piOverFour, piOverFour);
    addJoint(&m_pLegR2FootJoint, m_pLegR2, m_pFootR, desc.legR2, desc.footR,  
        physx::PxD6Motion::eLOCKED, physx::PxD6Motion::eLIMITED, physx::PxD6Motion::eLIMITED,
        0, 0, piOverFour, piOverFour);
    
    PHYSICS->getScene()->addAggregate(*m_pAggregate);

}


PhysicsDynamicActor* PhysicsRagdoll::getArmL1() const { return m_pArmL1; }
PhysicsDynamicActor* PhysicsRagdoll::getArmL2() const { return m_pArmL2; }
PhysicsDynamicActor* PhysicsRagdoll::getHandL() const { return m_pHandL; }

PhysicsDynamicActor* PhysicsRagdoll::getArmR1() const { return m_pArmR1; }
PhysicsDynamicActor* PhysicsRagdoll::getArmR2() const { return m_pArmR2; }
PhysicsDynamicActor* PhysicsRagdoll::getHandR() const { return m_pHandR; }

PhysicsDynamicActor* PhysicsRagdoll::getHead() const { return m_pHead; }
PhysicsDynamicActor* PhysicsRagdoll::getNeck() const { return m_pNeck; }

PhysicsDynamicActor* PhysicsRagdoll::getShoulderL() const { return m_pShoulderL; }
PhysicsDynamicActor* PhysicsRagdoll::getShoulderR() const { return m_pShoulderR; }

PhysicsDynamicActor* PhysicsRagdoll::getSpine1() const { return m_pSpine1; }
PhysicsDynamicActor* PhysicsRagdoll::getSpine2() const { return m_pSpine2; }
PhysicsDynamicActor* PhysicsRagdoll::getSpine3() const { return m_pSpine3; }
PhysicsDynamicActor* PhysicsRagdoll::getPelvis() const { return m_pPelvis; }

PhysicsDynamicActor* PhysicsRagdoll::getLegL1() const { return m_pLegL1; }
PhysicsDynamicActor* PhysicsRagdoll::getLegL2() const { return m_pLegL2; }
PhysicsDynamicActor* PhysicsRagdoll::getFootL() const { return m_pFootL; }

PhysicsDynamicActor* PhysicsRagdoll::getLegR1() const { return m_pLegR1; }
PhysicsDynamicActor* PhysicsRagdoll::getLegR2() const { return m_pLegR2; }
PhysicsDynamicActor* PhysicsRagdoll::getFootR() const { return m_pFootR; }

} } //end namespace