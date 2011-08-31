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

#include "TestObject.h"
#include "MathConstants.h"
#include "HappyEngine.h"
#include "VertexPNT.h"
#include "TextureLoader.h"
#include "FontLoader.h"
#include "HappyEngine.h"

#include <sstream>

namespace happytest {

TestObject::TestObject(): m_Rotation(0), m_Position(0, 0, 0)
{
}


TestObject::~TestObject()
{
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
}
void TestObject::tick(float dTime)
{
    using namespace happyengine;
    if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Left))
        m_Rotation += math::pi * dTime;
    if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Right))
        m_Rotation -= math::pi * dTime;
    if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Down))
        m_Position -= math::Vector3(cosf(m_Rotation), 0, -sinf(m_Rotation)) * dTime * 5;
    if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Up))
        m_Position += math::Vector3(cosf(m_Rotation), 0, -sinf(m_Rotation)) * dTime * 5;
}
void TestObject::draw(happyengine::graphics::I3DRenderer* pRenderer, DeferredPreEffect* m_pEffect, float /*dTime*/)
{
    using namespace happyengine;

    math::Matrix persp(math::Matrix::createPerspectiveLH(math::piOverFour, 1280, 720, 1, 1000));
    math::Matrix view(math::Matrix::createLookAtLH(math::Vector3(-5, 5, -4), math::Vector3(0, 0, 0), math::Vector3(0, 1, 0)));
    math::Matrix world(
        math::Matrix::createTranslation(m_Position) *
        math::Matrix::createRotation(math::Vector3(0.0f, 1.0f, 0.0f), m_Rotation) *
        math::Matrix::createScale(2.0f)
                      );

    m_pEffect->setWVP(persp * view * world);
    m_pEffect->setWorld(world);  
    m_pEffect->setDiffuseMap(m_pDiffuseMap);
    m_pEffect->setNormalMap(m_pNormalMap);
    m_pEffect->setSpecGlossIllMap(m_pSGIMap);

    pRenderer->draw(m_pModel);
}

} //end namespace