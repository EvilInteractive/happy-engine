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

TestBullet::TestBullet(const he::game::EntityDesc& desc, 
                       const he::vec3& pos, const he::vec3& velocity): 
                        Entity(desc), m_pActor(nullptr), m_pMaterial(desc.physicsDesc.pMaterial)
{
    using namespace he;

    ASSERT(desc.physicsDesc.usePhysics, "this class must have px properties");

    m_pActor = NEW he::px::PhysicsDynamicActor(mat44::createTranslation(pos),
        px::shapes::IPhysicsShape::pointer(NEW px::shapes::PhysicsBoxShape(vec3(2, 2, 2))), desc.physicsDesc.density, m_pMaterial);

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