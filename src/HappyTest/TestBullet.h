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

#ifndef _TEST_BULLET_H_
#define _TEST_BULLET_H_
#pragma once

#include "Assert.h"
#define assert ASSERT

#include "PhysicsDynamicActor.h"
#include "PhysicsMaterial.h"
#include "PhysicsBoxShape.h"
#include "Model.h"
#include "Matrix.h"
#include "Texture2D.h"

#include "I3DRenderer.h"
#include "DeferredPreEffect.h"

namespace happytest {

class TestBullet
{
public:
	TestBullet();
    virtual ~TestBullet();
    
    void load();
    void tick(float dTime);
    void draw(happyengine::graphics::I3DRenderer* pRenderer, DeferredPreEffect* m_pEffect, float dTime);

private:

    happyengine::physics::PhysicsDynamicActor* m_pActor;
    happyengine::physics::PhysicsMaterial* m_pMaterial;
    happyengine::physics::shapes::PhysicsBoxShape* m_pShape;

    happyengine::graphics::Model::pointer m_pModel;
    happyengine::graphics::Texture2D::pointer m_pDiffuseMap;

    happyengine::math::Matrix m_mtxWorld;

    //Disable default copy constructor and default assignment operator
    TestBullet(const TestBullet&);
    TestBullet& operator=(const TestBullet&);
};

} //end namespace

#endif
