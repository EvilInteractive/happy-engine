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

namespace happytest {

TestBullet::TestBullet(const happyengine::math::Vector3& pos, const happyengine::math::Vector3& velocity): m_pActor(nullptr), m_pMaterial(nullptr)
{
    using namespace happyengine;
    m_pMaterial = NEW physics::PhysicsMaterial(0.5f, 0.8f, 0.4f);
    m_pActor = NEW physics::PhysicsDynamicActor(math::Matrix::createTranslation(pos), 
        physics::shapes::IPhysicsShape::pointer(NEW physics::shapes::PhysicsBoxShape(math::Vector3(2, 2, 2))), 1.5f, m_pMaterial);
    m_pActor->setVelocity(velocity);
}


TestBullet::~TestBullet()
{
    delete m_pActor;
    delete m_pMaterial;
}

void TestBullet::load()
{
    using namespace happyengine;
    using namespace graphics;
    
    m_pModel = CONTENT->asyncLoadModel("../data/models/cube.binobj", DeferredPreEffect::getVertexLayout());
    
    m_pDiffuseMap = CONTENT->asyncLoadTexture("../data/textures/testTex.png");
    m_pNormalMap = CONTENT->asyncLoadTexture("../data/textures/gravel_normal.png");
    m_pSGIMap = CONTENT->asyncLoadTexture("../data/textures/gravel_sgi.png");
}

void TestBullet::tick(float /*dTime*/)
{
    m_mtxWorld = m_pActor->getPose();
}
void TestBullet::draw(happyengine::graphics::I3DRenderer* pRenderer, DeferredPreEffect* m_pEffect, const happyengine::graphics::Camera* pCamera)
{
    using namespace happyengine;
    
    m_pEffect->setWVP(pCamera->getViewProjection() * m_mtxWorld);
    m_pEffect->setWorld(m_mtxWorld);  
    m_pEffect->setDiffuseMap(m_pDiffuseMap);
    m_pEffect->setNormalMap(m_pNormalMap);
    m_pEffect->setSpecGlossIllMap(m_pSGIMap);

    pRenderer->draw(m_pModel);
}

} //end namespace