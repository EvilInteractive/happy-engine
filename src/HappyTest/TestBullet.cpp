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

TestBullet::TestBullet(): m_pActor(nullptr), m_pShape(nullptr), m_pMaterial(nullptr)
{
}


TestBullet::~TestBullet()
{
    delete m_pActor;
    delete m_pShape;
    delete m_pMaterial;
}

void TestBullet::load()
{
    using namespace happyengine;
    using namespace graphics;
    
    VertexLayout layout;
    layout.addElement(VertexElement(0, VertexElement::Type_Vector3, VertexElement::Usage_Position, sizeof(math::Vector3), 0, "inPosition"));
    layout.addElement(VertexElement(1, VertexElement::Type_Vector2, VertexElement::Usage_TextureCoordinate, sizeof(math::Vector2), 12, "inTexCoord"));
    layout.addElement(VertexElement(2, VertexElement::Type_Vector3, VertexElement::Usage_Normal, sizeof(math::Vector3), 20, "inNormal"));

    m_pModel = CONTENT->asyncLoadModel("../data/models/cube.obj", layout);
    
    m_pDiffuseMap = CONTENT->asyncLoadTexture("../data/textures/testTex.png");

    m_pMaterial = NEW physics::PhysicsMaterial(0.5f, 0.8f, 0.4f);
    m_pShape = NEW physics::shapes::PhysicsBoxShape(math::Vector3(2, 2, 2));
    m_pActor = NEW physics::PhysicsDynamicActor(math::Vector3(0, 10, 0), *m_pShape, m_pMaterial);
}

void TestBullet::tick(float /*dTime*/)
{
    m_mtxWorld = m_pActor->getPose();
}
void TestBullet::draw(happyengine::graphics::I3DRenderer* pRenderer, DeferredPreEffect* m_pEffect, float /*dTime*/)
{
    using namespace happyengine;

    math::Matrix persp(math::Matrix::createPerspectiveLH(math::piOverFour, 1280, 720, 1, 1000));
    math::Matrix view(math::Matrix::createLookAtLH(math::Vector3(-5, 5, -4), math::Vector3(0, 0, 0), math::Vector3(0, 1, 0)));

    m_pEffect->setWVP(persp * view * m_mtxWorld);
    m_pEffect->setWorld(m_mtxWorld);  
    m_pEffect->setDiffuseMap(m_pDiffuseMap);

    pRenderer->draw(m_pModel);
}

} //end namespace