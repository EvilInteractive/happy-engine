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
#include "Font.h"
#include "Entity.h"

#include "ITickable.h"
#pragma warning(disable:4505)
#include "PhysicsCar.h"
#pragma warning(default:4505)
#include "RiggedModelComponent.h"

#include "Text.h"
#include "Font.h"
#include "audio/Sound2D.h"

namespace happytest {

class TestObject : public he::game::Entity, public he::game::ITickable
{
public:
    TestObject();
    virtual ~TestObject();

    virtual void tick(float dTime);

private:   
    he::gfx::Font::pointer m_pFont;
    he::sfx::Sound2D* m_pCarEngineSfx;

    float m_WheelOrientation, m_WheelMax, m_WheelTurnSpeed, m_WheelSpeedRotation;
    float m_Rotation;
    he::vec3 m_Position;
    
    he::px::PhysicsCar* m_pCar;
    he::uint m_CameraShakeTL;

    he::game::RiggedModelComponent* m_pRiggedModelComponent;
    he::game::RiggedModelComponent::BoneTransform m_WheelFLBone, m_WheelFRBone, m_WheelBLBone, m_WheelBRBone;
    void onModelLoaded();

    //Disable default copy constructor and default assignment operator
    TestObject(const TestObject&);
    TestObject& operator=(const TestObject&);
};

} //end namespace

#endif
