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

namespace he {
namespace px {

class PhysicsDynamicActor;
class PhysicsMaterial;

struct BodyPart
{
    mat44 pose;
    float length;
    float thickness;
    float mass;
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

    BodyPart shoulders;
    BodyPart spine1;
    BodyPart spine2;
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

    void initProperties(const RagdollDesc& desc, const PhysicsMaterial& material);

private:


    PhysicsDynamicActor* m_pArmL1;
    PhysicsDynamicActor* m_pArmL2;
    PhysicsDynamicActor* m_pHandL;

    PhysicsDynamicActor* m_pArmR1;
    PhysicsDynamicActor* m_pArmR2;
    PhysicsDynamicActor* m_pHandR;

    PhysicsDynamicActor* m_pHead;
    PhysicsDynamicActor* m_pNeck;

    PhysicsDynamicActor* m_pShoulders;
    PhysicsDynamicActor* m_pSpine1;
    PhysicsDynamicActor* m_pSpine2;
    PhysicsDynamicActor* m_pPelvis;

    PhysicsDynamicActor* m_pLegL1;
    PhysicsDynamicActor* m_pLegL2;
    PhysicsDynamicActor* m_FootL;

    PhysicsDynamicActor* m_pLegR1;
    PhysicsDynamicActor* m_pLegR2;
    PhysicsDynamicActor* m_FootR;

    //Disable default copy constructor and default assignment operator
    PhysicsRagdoll(const PhysicsRagdoll&);
    PhysicsRagdoll& operator=(const PhysicsRagdoll&);
};

} } //end namespace

#endif
