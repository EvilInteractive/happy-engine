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

#include "PhysicsDynamicActor.h"
#include "PhysicsMaterial.h"
#include "Entity.h"

namespace happytest {

class TestBullet : public he::game::Entity
{
public:
	TestBullet(const he::vec3& pos, const he::vec3& velocity);
    virtual ~TestBullet();
    
    virtual void tick(float dTime);

private:

    he::px::PhysicsDynamicActor* m_pActor;
    he::px::PhysicsMaterial::pointer m_pMaterial;
    
    //Disable default copy constructor and default assignment operator
    TestBullet(const TestBullet&);
    TestBullet& operator=(const TestBullet&);
};

} //end namespace

#endif
