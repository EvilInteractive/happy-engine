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

#ifndef _CHARACTER_H_
#define _CHARACTER_H_
#pragma once

#include "Entity.h"
#include "ITickable.h"
#include "RiggedModelComponent.h"
#include "PhysicsRagdoll.h"
#include "PhysicsFixedJoint.h"

namespace happytest {

class Character : public he::game::Entity, public he::game::ITickable
{
public:
    Character();
    virtual ~Character();

    virtual void tick(float dTime);

private:

    void initRagDoll();

    he::game::RiggedModelComponent* m_pRiggedModelComponent;
    he::game::RiggedModelComponent::BoneTransform m_bPelvis, m_bSpine1, m_bSpine2, m_bSpine3, m_bShoulderL, 
                                                  m_bShoulderR, m_bNeck, m_bHead, m_bArmL1, m_bArmL2, m_bHandL, 
                                                  m_bArmR1, m_bArmR2, m_bHandR, m_bLegL1, m_bLegL2, m_bFootL, 
                                                  m_bLegR1, m_bLegR2, m_bFootR;

    he::px::PhysicsFixedJoint* m_pCharacterFixJoint;

    he::px::PhysicsRagdoll* m_pDoll;

    //Disable default copy constructor and default assignment operator
    Character(const Character&);
    Character& operator=(const Character&);
};

} //end namespace

#endif
