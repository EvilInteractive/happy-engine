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

#include "TestBullet.h"
#include "MathConstants.h"
#include "ObjLoader.h"
#include "VertexPNT.h"
#include "VertexLayout.h"
#include "TextureLoader.h"
#include "HappyEngine.h"
#include "HappyNew.h"
#include "PhysicsBoxShape.h"

namespace happytest {

TestBullet::TestBullet(const happyengine::game::EntityDesc& desc, 
                       const happyengine::math::Vector3& pos, const happyengine::math::Vector3& velocity): 
                        Entity(desc), m_pActor(nullptr), m_pMaterial(desc.physicsDesc.pMaterial)
{
    using namespace happyengine;

    ASSERT(desc.physicsDesc.usePhysics, "this class must have physics properties");

    m_pActor = NEW happyengine::physics::PhysicsDynamicActor(math::Matrix::createTranslation(pos),
        physics::shapes::IPhysicsShape::pointer(NEW physics::shapes::PhysicsBoxShape(math::Vector3(2, 2, 2))), desc.physicsDesc.density, m_pMaterial);

    m_pActor->setVelocity(velocity);
}


TestBullet::~TestBullet()
{
    delete m_pActor;
}

void TestBullet::tick(float /*dTime*/)
{
    setWorldMatrix(m_pActor->getPose());
}

} //end namespace