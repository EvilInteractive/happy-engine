//HappyTest Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#ifndef _TEST_OBJECT_H_
#define _TEST_OBJECT_H_
#pragma once

#include "vec3.h"
#include "mat44.h"
#include "PhysicsDynamicActor.h"
#include "Font.h"
#include "Entity.h"
#include "RiggedModelComponent.h"

namespace happytest {

class TestObject : public he::game::Entity
{
public:
	TestObject();
    virtual ~TestObject();

    virtual void tick(float dTime);

private:   
    he::gfx::Font::pointer m_pFont;

    float m_WheelOrientation, m_WheelMax, m_WheelTurnSpeed, m_WheelSpeedRotation;
    float m_Rotation;
    he::vec3 m_Position;
    
    he::px::PhysicsDynamicActor* m_pActor;
    he::px::PhysicsMaterial::pointer m_pMaterial;
    
    he::game::RiggedModelComponent* m_pRiggedModelComponent;
    he::game::RiggedModelComponent::BoneTransform m_LeftWheelBone, m_RightWheelBone;
    void onModelLoaded();

    //Disable default copy constructor and default assignment operator
    TestObject(const TestObject&);
    TestObject& operator=(const TestObject&);
};

} //end namespace

#endif
