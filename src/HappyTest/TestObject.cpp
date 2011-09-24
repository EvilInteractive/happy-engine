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

#include "HappyNew.h"
#include "TestObject.h"
#include "MathConstants.h"
#include "HappyEngine.h"
#include "VertexPNT.h"
#include "TextureLoader.h"
#include "FontLoader.h"
#include "HappyEngine.h"

#include <sstream>

namespace happytest {

TestObject::TestObject(): m_Rotation(0), m_Position(0, 0, 0), m_pActor(nullptr)
{
}


TestObject::~TestObject()
{
    delete m_pActor;
    delete m_pMaterial;
}


void TestObject::load()
{
    using namespace happyengine::graphics;
    using namespace happyengine;

    VertexLayout layout;
    layout.addElement(VertexElement(0, VertexElement::Type_Vector3, VertexElement::Usage_Position, sizeof(math::Vector3), 0, "inPosition"));
    layout.addElement(VertexElement(1, VertexElement::Type_Vector2, VertexElement::Usage_TextureCoordinate, sizeof(math::Vector2), 12, "inTexCoord"));
    layout.addElement(VertexElement(2, VertexElement::Type_Vector3, VertexElement::Usage_Normal, sizeof(math::Vector3), 20, "inNormal"));
    layout.addElement(VertexElement(3, VertexElement::Type_Vector3, VertexElement::Usage_Tangent, sizeof(math::Vector3), 32, "inTangent"));

    m_pModel = CONTENT->asyncLoadModel("../data/models/car.binobj", layout);
    
    m_pDiffuseMap = CONTENT->asyncLoadTexture("../data/textures/v8_vantage_color.png");
    m_pNormalMap = CONTENT->asyncLoadTexture("../data/textures/v8_vantage_normal.png");
    m_pSGIMap = CONTENT->asyncLoadTexture("../data/textures/v8_vantage_specGlossIll.png");

    happyengine::content::FontLoader fontLoader;
    fontLoader.load("../data/fonts/Ubuntu-Regular.ttf", 14, m_pFont);
    
    m_pMaterial = NEW happyengine::physics::PhysicsMaterial(0.7f, 0.5f, 0.2f);
    m_pActor = NEW happyengine::physics::PhysicsDynamicActor(math::Matrix::createTranslation(m_Position),
        CONTENT->loadPhysicsShape("../data/physics/car.bphys"), 2.0f, m_pMaterial);
    m_pActor->setKeyframed(true);
}
void TestObject::tick(float dTime)
{
    using namespace happyengine;
    if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Left))
    {
        m_Rotation += math::pi * dTime;
    }
    if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Right))
    {
        m_Rotation -= math::pi * dTime;
    }
    if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Down))
    {
        m_Position -= math::Vector3(cosf(m_Rotation), 0, -sinf(m_Rotation)) * dTime * 5;
    }
    if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Up))
    {
        m_Position += math::Vector3(cosf(m_Rotation), 0, -sinf(m_Rotation)) * dTime * 5;
    }
    m_pActor->keyframedSetPose(m_Position, math::Vector3(0, 1, 0), m_Rotation);

    m_matWorld = m_pActor->getPose();
}
void TestObject::draw(happyengine::graphics::I3DRenderer* pRenderer, DeferredPreEffect* m_pEffect, float /*dTime*/, const happyengine::graphics::Camera* pCamera)
{
    using namespace happyengine;

	m_pEffect->setWVP(pCamera->getViewProjection() * m_matWorld);
    m_pEffect->setWorld(m_matWorld);  
    m_pEffect->setDiffuseMap(m_pDiffuseMap);
    m_pEffect->setNormalMap(m_pNormalMap);
    m_pEffect->setSpecGlossIllMap(m_pSGIMap);

    pRenderer->draw(m_pModel);
}

} //end namespace