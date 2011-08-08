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
#include "Shader.h"
#include "Vector3.h"
#include "Matrix.h"
#include "I3DRenderer.h"

namespace happytest {

class TestObject
{
public:
	TestObject();
    virtual ~TestObject();

    void load();
    void tick(float dTime);
    void draw(happyengine::graphics::I3DRenderer* pRenderer);

private:
    happyengine::graphics::Model::pointer m_pModel;
    happyengine::graphics::Shader* m_pShader;

    float m_Rotation;
    happyengine::math::Vector3 m_Position;

    //Disable default copy constructor and default assignment operator
    TestObject(const TestObject&);
    TestObject& operator=(const TestObject&);
};

} //end namespace

#endif
