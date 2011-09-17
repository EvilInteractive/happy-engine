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

#define BOOST_DISABLE_ASSERTS
#include "Model.h"
#include "Vector3.h"
#include "Matrix.h"
#include "I3DRenderer.h"
#include "Texture2D.h"
#include "Camera.h"

#include "DeferredPreEffect.h" 

#include "PhysicsDynamicActor.h"

#include "Font.h"

namespace happytest {

class TestObject
{
public:
	TestObject();
    virtual ~TestObject();

    void load();
    void tick(float dTime);
    void draw(	happyengine::graphics::I3DRenderer* pRenderer,
				DeferredPreEffect* m_pEffect,
				float dTime,
				const happyengine::graphics::Camera* pCamera);

private:
    happyengine::graphics::Model::pointer m_pModel;
    happyengine::graphics::Texture2D::pointer m_pDiffuseMap;
    happyengine::graphics::Texture2D::pointer m_pNormalMap;
    happyengine::graphics::Texture2D::pointer m_pSGIMap;
    
    happyengine::graphics::Font::pointer m_pFont;

    float m_Rotation;
    happyengine::math::Vector3 m_Position;

	happyengine::math::Matrix m_matWorld;

    happyengine::physics::PhysicsDynamicActor* m_pActor;
    happyengine::physics::PhysicsMaterial* m_pMaterial;

    //Disable default copy constructor and default assignment operator
    TestObject(const TestObject&);
    TestObject& operator=(const TestObject&);
};

} //end namespace

#endif
