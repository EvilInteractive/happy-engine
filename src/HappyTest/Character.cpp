//HappyTest Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
//
//This file is part of HappyTest.
//
//    HappyTest is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyTest is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyTest.  If not, see <http://www.gnu.org/licenses/>.

#include "HappyNew.h"
#include "MathConstants.h"
#include "Character.h"

#include "HappyEngine.h"
#include "ContentManager.h"
#include "Game.h"
#include "PhysicsEngine.h"
#include "PhysicsData.h"
#include "PhysicsCarManager.h"

#include "PhysicsDynamicActor.h"

using namespace he;

namespace happytest {


Character::Character(): m_pDoll(NEW px::PhysicsRagdoll()), 
                        m_pRiggedModelComponent(NEW game::RiggedModelComponent())
{
    gfx::Material material(CONTENT->loadMaterial("char/dikken.material"));
    m_pRiggedModelComponent->setModelMesh(CONTENT->asyncLoadModelMesh("char/dikken.binobj", "fattyUnwrapped", material.getCompatibleVertexLayout()));
    m_pRiggedModelComponent->setMaterial(material);
    addComponent(m_pRiggedModelComponent);
    m_pRiggedModelComponent->getModelMesh()->callbackIfLoaded([&]()
    {
        m_bPelvis = m_pRiggedModelComponent->getBone("dikken_pelvis");
        m_bSpine3 = m_pRiggedModelComponent->getBone("dikken_torso1"); 
        m_bSpine2 = m_pRiggedModelComponent->getBone("dikken_torso2"); 
        m_bSpine1 = m_pRiggedModelComponent->getBone("dikken_torso3"); 

        m_bShoulderL = m_pRiggedModelComponent->getBone("dikken_clavicle_L"); 
        m_bShoulderR = m_pRiggedModelComponent->getBone("dikken_clavicle_R");

        m_bNeck = m_pRiggedModelComponent->getBone("dikken_neck"); 
        m_bHead = m_pRiggedModelComponent->getBone("dikken_head");

        m_bArmL1 = m_pRiggedModelComponent->getBone("dikken_arm1_L"); 
        m_bArmL2 = m_pRiggedModelComponent->getBone("dikken_arm2_L"); 
        m_bHandL = m_pRiggedModelComponent->getBone("dikken_hand_L"); 
        m_bArmR1 = m_pRiggedModelComponent->getBone("dikken_arm1_R"); 
        m_bArmR2 = m_pRiggedModelComponent->getBone("dikken_arm2_R"); 
        m_bHandR = m_pRiggedModelComponent->getBone("dikken_arm_R");

        m_bLegL1 = m_pRiggedModelComponent->getBone("dikken_leg1_L"); 
        m_bLegL2 = m_pRiggedModelComponent->getBone("dikken_leg2_L"); 
        m_bFootL = m_pRiggedModelComponent->getBone("dikken_foot_L"); 
        m_bLegR1 = m_pRiggedModelComponent->getBone("dikken_leg1_R"); 
        m_bLegR2 = m_pRiggedModelComponent->getBone("dikken_leg2_R"); 
        m_bFootR = m_pRiggedModelComponent->getBone("dikken_foot_R");

        initRagDoll();
    });

    GAME->addToTickList(this);
}

void Character::initRagDoll()
{
    px::RagdollDesc desc;

    desc.pelvis = px::BodyPart(m_bPelvis.m_FromOrigTransform,
        mat44::Identity, 
        0.1f, 0.05f, 2);
    desc.spine3 = px::BodyPart(m_bSpine3.m_FromOrigTransform, 
        mat44::Identity, 
        0.1f, 0.05f, 2);
    desc.spine2 = px::BodyPart(m_bSpine2.m_FromOrigTransform, 
        mat44::Identity, 
        0.1f, 0.05f, 2);
    desc.spine1 = px::BodyPart(m_bSpine1.m_FromOrigTransform, 
        mat44::Identity, 
        0.1f, 0.05f, 2);

    desc.neck = px::BodyPart(m_bNeck.m_FromOrigTransform, 
        mat44::Identity, 
        0.1f, 0.05f, 2);
    desc.head = px::BodyPart(m_bHead.m_FromOrigTransform,
        mat44::Identity, 
        0.1f, 0.05f, 2);

    desc.shoulderL = px::BodyPart(m_bShoulderL.m_FromOrigTransform, 
        mat44::Identity, 
        0.1f, 0.05f, 2);
    desc.shoulderR = px::BodyPart(m_bShoulderR.m_FromOrigTransform, 
        mat44::Identity, 
        0.1f, 0.05f, 2);

    desc.armR1 = px::BodyPart(m_bArmR1.m_FromOrigTransform, 
        mat44::Identity, 
        0.1f, 0.05f, 8);
    desc.armL1 = px::BodyPart(m_bArmL1.m_FromOrigTransform, 
        mat44::Identity, 
        0.1f, 0.05f, 8);

    desc.armR2 = px::BodyPart(m_bArmR2.m_FromOrigTransform, 
        mat44::Identity, 
        0.1f, 0.05f, 5);
    desc.armL2 = px::BodyPart(m_bArmL2.m_FromOrigTransform, 
        mat44::Identity, 
        0.1f, 0.05f, 5);

    desc.handR = px::BodyPart(m_bHandR.m_FromOrigTransform, 
        mat44::Identity, 
        0.1f, 0.05f, 2);
    desc.handL = px::BodyPart(m_bHandL.m_FromOrigTransform, 
        mat44::Identity, 
        0.1f, 0.05f, 2);

    //legs
    desc.legL1 = px::BodyPart(m_bLegL1.m_FromOrigTransform, 
        mat44::Identity, 
        0.1f, 0.05f, 20);
    desc.legL2 = px::BodyPart(m_bLegL2.m_FromOrigTransform, 
        mat44::Identity, 
        0.1f, 0.05f, 10);
    desc.footL = px::BodyPart(m_bFootL.m_FromOrigTransform, 
        mat44::Identity, 
        0.1f, 0.05f, 1);

    desc.legR1 = px::BodyPart(m_bLegR1.m_FromOrigTransform, 
        mat44::Identity, 
        0.1f, 0.05f, 20);
    desc.legR2 = px::BodyPart(m_bLegR2.m_FromOrigTransform, 
        mat44::Identity, 
        0.1f, 0.05f, 10);
    desc.footR = px::BodyPart(m_bFootR.m_FromOrigTransform, 
        mat44::Identity, 
        0.1f, 0.05f, 1);

    m_pDoll->initProperties(vec3(0, 2, 0), desc, PHYSICS->getCarManager()->getFrictionTable()->getMaterial(DM_Concrete));

    m_pCharacterFixJoint = NEW px::PhysicsFixedJoint(m_pDoll->getHandL(), vec3::zero, 
                                                    nullptr, m_pDoll->getHandL()->getPosition());
}


Character::~Character()
{
    delete m_pCharacterFixJoint;
    delete m_pDoll;
}

void Character::tick( float /*dTime*/ )
{
    if (m_pRiggedModelComponent->isVisible())
    {
        (*m_bPelvis.m_RealTransform) = m_pDoll->getPelvis()->getPose() * m_bPelvis.m_ToOrigTransform;
        (*m_bSpine3.m_RealTransform) = m_pDoll->getSpine3()->getPose() * m_bSpine3.m_ToOrigTransform; 
        (*m_bSpine2.m_RealTransform) = m_pDoll->getSpine2()->getPose() * m_bSpine2.m_ToOrigTransform;
        (*m_bSpine1.m_RealTransform) = m_pDoll->getSpine1()->getPose() * m_bSpine1.m_ToOrigTransform; 

        (*m_bShoulderL.m_RealTransform) = m_pDoll->getShoulderL()->getPose() * m_bShoulderL.m_ToOrigTransform;
        (*m_bShoulderR.m_RealTransform) = m_pDoll->getShoulderR()->getPose() * m_bShoulderR.m_ToOrigTransform;

        (*m_bNeck.m_RealTransform) = m_pDoll->getNeck()->getPose() * m_bNeck.m_ToOrigTransform; 
        (*m_bHead.m_RealTransform) = m_pDoll->getHead()->getPose() * m_bHead.m_ToOrigTransform;

        (*m_bArmL1.m_RealTransform) = m_pDoll->getArmL1()->getPose() * m_bArmL1.m_ToOrigTransform; 
        (*m_bArmL2.m_RealTransform) = m_pDoll->getArmL2()->getPose() * m_bArmL2.m_ToOrigTransform; 
        (*m_bArmR1.m_RealTransform) = m_pDoll->getArmR1()->getPose() * m_bArmR1.m_ToOrigTransform; 
        (*m_bArmR2.m_RealTransform) = m_pDoll->getArmR2()->getPose() * m_bArmR2.m_ToOrigTransform; 
        (*m_bHandL.m_RealTransform) = m_pDoll->getHandL()->getPose() * m_bHandL.m_ToOrigTransform; 
        (*m_bHandR.m_RealTransform) = m_pDoll->getHandR()->getPose() * m_bHandR.m_ToOrigTransform; 

        (*m_bLegL1.m_RealTransform) = m_pDoll->getLegL1()->getPose() * m_bLegL1.m_ToOrigTransform; 
        (*m_bLegL2.m_RealTransform) = m_pDoll->getLegL2()->getPose() * m_bLegL2.m_ToOrigTransform; 
        (*m_bFootL.m_RealTransform) = m_pDoll->getFootL()->getPose() * m_bFootL.m_ToOrigTransform; 
        (*m_bLegR1.m_RealTransform) = m_pDoll->getLegR1()->getPose() * m_bLegR1.m_ToOrigTransform; 
        (*m_bLegR2.m_RealTransform) = m_pDoll->getLegR2()->getPose() * m_bLegR2.m_ToOrigTransform; 
        (*m_bFootR.m_RealTransform) = m_pDoll->getFootR()->getPose() * m_bFootR.m_ToOrigTransform;
    }
}

} //end namespace