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
#include "PhysicsMaterial.h"
#include "ModelComponent.h"
#include "ContentManager.h"

namespace happytest {

TestBullet::TestBullet(const he::vec3& pos, const he::vec3& velocity): 
                       m_pActor(nullptr), m_pMaterial(NEW he::px::PhysicsMaterial(0.8f, 0.5f, 0.2f))
{
    using namespace he;
    
    m_pActor = NEW px::PhysicsDynamicActor(mat44::createTranslation(pos),
        px::shapes::IPhysicsShape::pointer(NEW px::shapes::PhysicsBoxShape(vec3(2, 2, 2))), 2.0f, m_pMaterial);

    m_pActor->setVelocity(velocity);

    game::ModelComponent* pModelComp(NEW game::ModelComponent());
    pModelComp->setMaterial(CONTENT->loadMaterial("bullet.material"));
    pModelComp->setModel(CONTENT->asyncLoadModelMesh("cube.binobj", "M_Cube", pModelComp->getMaterial().getCompatibleVertexLayout()));
    addComponent(pModelComp);
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